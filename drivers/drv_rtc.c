/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-09     WCH        the first version
 */

#include <board.h>
#include <rtthread.h>
#include <rtdevice.h>


#if defined(BSP_USING_ONCHIP_RTC)
#include "stdlib.h"
#include "drv_rtc.h"

//#define DRV_DEBUG
#define LOG_TAG              "drv.rtc"
#include <drv_log.h>

#define BKUP_REG_DATA 0xA5A5

const rt_uint8_t table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
const rt_uint8_t mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

RTC_HandleTypeDef RTC_Handler;

static uint8_t RTC_IsLeapYear(uint16_t nYear)
{
  if ((nYear % 4U) != 0U)
  {
    return 0U;
  }

  if ((nYear % 100U) != 0U)
  {
    return 1U;
  }

  if ((nYear % 400U) == 0U)
  {
    return 1U;
  }
  else
  {
    return 0U;
  }
}



/*********************************************************************
 * @fn      rt_rtc_set_time
 *
 * @brief   Set Time.
 *
 * @param   Struct of RTC_DateTypeDef
 *
 * @return  rt_err_t
 *
 */
static rt_err_t rt_rtc_set_time(RTC_DateTypeDef *date)
{
    rt_uint16_t t=0;
    rt_uint32_t seccount=0;
    if(date->year <1970 || date->year>2099)
    {
        return RT_ERROR;
    }

    for(t = 1970; t < date->year; t++)
    {
        if(RTC_IsLeapYear(t))
            seccount += 31622400;
        else
            seccount += 31536000;
    }
    date->month -=1;

    for(t = 0; t < date->month; t++)
    {
        seccount += (u32)mon_table[t] * 86400;
        if(RTC_IsLeapYear(date->year) && t == 1)
            seccount += 86400;
    }
    seccount += (u32)(date->day - 1) * 86400;
    seccount += (u32)date->hour * 3600;
    seccount += (u32)date->min * 60;
    seccount += date->sec;

    PWR_BackupAccessCmd(ENABLE);
    RTC_SetCounter(seccount);
    RTC_WaitForLastTask();
    return RT_EOK;
}

/*********************************************************************
 * @fn      rt_rtc_set_alarm_time
 *
 * @brief   Set Alarm Time.
 *
 * @param   Struct of RTC_DateTypeDef
 *
 * @return  rt_err_t
 *
 */
static rt_err_t rt_rtc_set_alarm_time(RTC_DateTypeDef *alarm)
{
    rt_uint16_t t=0;
    rt_uint32_t seccount = 0;

    if( alarm->year < 1970 || alarm->year > 2099)
        return RT_ERROR;
    for(t = 1970; t < alarm->year; t++)
    {
        if(RTC_IsLeapYear(t))
            seccount += 31622400;
        else
            seccount += 31536000;
    }
    alarm->month -= 1;
    for(t = 0; t < alarm->month ; t++)
    {
        seccount += (u32)mon_table[t] * 86400;
        if(RTC_IsLeapYear(alarm->year) && t == 1)
            seccount += 86400;
    }
    seccount += (u32)(alarm->day - 1) * 86400;
    seccount += (u32)alarm->hour * 3600;
    seccount += (u32)alarm->min * 60;
    seccount += alarm->sec;


    PWR_BackupAccessCmd(ENABLE);
    RTC_SetAlarm(seccount);
    RTC_WaitForLastTask();

    return RT_EOK;
}

/*********************************************************************
 * @fn      RTC_Get_Week
 *
 * @brief   Get the current day of the week.
 *
 * @param   year/month/day
 *
 * @return  week
 */
rt_uint8_t rt_rtc_get_week(rt_uint16_t year, rt_uint8_t month, rt_uint8_t day)
{
    rt_uint16_t temp2;
    rt_uint8_t  yearH, yearL;

    yearH = year / 100;
    yearL = year % 100;
    if(yearH > 19)
        yearL += 100;
    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + table_week[month - 1];
    if(yearL % 4 == 0 && month < 3)
        temp2--;
    return (temp2 % 7);
}

/*********************************************************************
 * @fn      rt_rtc_get_time
 *
 * @brief   Get current time.
 *
 * @return  1 - error
 *          0 - success
 */
rt_err_t rt_rtc_get_time(RTC_DateTypeDef *date)
{
    static u16 daycnt = 0;
    u32        timecount = 0;
    u32        temp = 0;
    u16        temp1 = 0;
    timecount = RTC_GetCounter();
    temp = timecount / 86400;
    if(daycnt != temp)
    {
        daycnt = temp;
        temp1 = 1970;
        while(temp >= 365)
        {
            if(RTC_IsLeapYear(temp1))
            {
                if(temp >= 366)
                    temp -= 366;
                else
                {
                    temp1++;
                    break;
                }
            }
            else
                temp -= 365;
            temp1++;
        }
        date->year = temp1;

        temp1 = 0;
        while(temp >= 28)
        {
            if(RTC_IsLeapYear(date->year) && temp1 == 1)
            {
                if(temp >= 29)
                    temp -= 29;
                else
                    break;
            }
            else
            {
                if(temp >= mon_table[temp1])
                    temp -= mon_table[temp1];
                else
                    break;
            }
            temp1++;
        }
        date->month = temp1 + 1;
        date->day = temp + 1;
    }
    temp = timecount % 86400;
    date->hour = temp / 3600;
    date->min = (temp % 3600) / 60;
    date->sec = (temp % 3600) % 60;
    date->week = rt_rtc_get_week(date->year, date->month, date->day);
    return RT_EOK;
}


static void rt_rtc_init(void)
{
    rt_uint8_t temp = 0;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    if(BKP_ReadBackupRegister(BKP_DR1) != BKUP_REG_DATA)
    {

        RCC_LSEConfig(RCC_LSE_ON);
        do {
            temp++;
            rt_thread_mdelay(20);
        } while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && temp < 250);

        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        RCC_RTCCLKCmd(ENABLE);
        RTC_WaitForLastTask();
        RTC_WaitForSynchro();

        RTC_WaitForLastTask();
        RTC_EnterConfigMode();
        RTC_SetPrescaler(32767);
        RTC_WaitForLastTask();

        RTC_ExitConfigMode();
        BKP_WriteBackupRegister(BKP_DR1, BKUP_REG_DATA);
        rt_kprintf("rtc init\r\n");
    }
    else
    {
        rt_kprintf("rtc needn't init\r\n");
    }
}




int rt_hw_rtc_init(void)
{
    rt_rtc_init();
    return RT_EOK;
}
INIT_APP_EXPORT(rt_hw_rtc_init);



#if defined(RT_USING_FINSH) && defined(FINSH_USING_MSH)
static void date(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
      rt_kprintf("year-month-day,hour:min:sec\r\n");
      rt_kprintf("%d-%d-%d,%d:%d:%d\r\n",RTC_Handler.Date.year,RTC_Handler.Date.month,RTC_Handler.Date.day,RTC_Handler.Date.hour,RTC_Handler.Date.min,RTC_Handler.Date.sec);
    }
    else if (argc >= 7)
    {
        /* set time and date */
        RTC_DateTypeDef set={0};

        set.year = atoi(argv[1]);
        set.month = atoi(argv[2]);
        set.day = atoi(argv[3]);
        set.hour = atoi(argv[4]);
        set.min = atoi(argv[5]);
        set.sec = atoi(argv[6]);
        if (set.year > 2099 || set.year < 1970)
        {
            rt_kprintf("year is out of range [2000-2099]\n");
            return;
        }
        if (set.month == 0 || set.month > 12)
        {
            rt_kprintf("month is out of range [1-12]\n");
            return;
        }
        if (set.day == 0 || set.day > 31)
        {
            rt_kprintf("day is out of range [1-31]\n");
            return;
        }
        if (set.hour > 23)
        {
            rt_kprintf("hour is out of range [0-23]\n");
            return;
        }
        if (set.min > 59)
        {
            rt_kprintf("minute is out of range [0-59]\n");
            return;
        }
        if (set.sec > 59)
        {
            rt_kprintf("second is out of range [0-59]\n");
            return;
        }
        rt_rtc_set_time(&set);
    }
    else
    {
        rt_kprintf("please input: date [year month day hour min sec] or date\n");
        rt_kprintf("e.g: date 2022 01 01 23 59 59 or date\n");
    }
}
MSH_CMD_EXPORT(date, get date and time or set [year month day hour min sec]);
#endif /* defined(RT_USING_FINSH) && defined(FINSH_USING_MSH) */


#endif /* BSP_USING_RTC */
