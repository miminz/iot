/********************************** (C) COPYRIGHT *******************************
* File Name          : rtc_app.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : rtc_app.c
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "drv_rtc.h"
#include "rtc_app.h"

ALIGN(RT_ALIGN_SIZE)
static char rtc_task_stack[256];
static struct rt_thread rtc_task_thread;





/*********************************************************************
 * @fn      rtc_task_entry
 *
 * @brief   rtc task thread entry
 *
 * @return  none
 */
void rtc_task_entry(void *parameter)
{

    while(1)
    {
        rt_rtc_get_time(&RTC_Handler.Date);
        rt_thread_mdelay(500);
    }//end of while
}


/*********************************************************************
 * @fn      rtc_task_entry
 *
 * @brief   rtc task thread initial
 *
 * @return  none
 */
void rtc_task_init(void)
{
    rt_thread_init(&rtc_task_thread,
                   "rtc",
                   rtc_task_entry,
                   RT_NULL,
                   &rtc_task_stack[0],
                   sizeof(rtc_task_stack),
                   14, 20);//没必要执行很长时间
    rt_thread_startup(&rtc_task_thread);

}


