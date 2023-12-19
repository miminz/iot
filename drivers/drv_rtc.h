/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-09     WCH        the first version
 */
#ifndef DRV_RTC_H__
#define DRV_RTC_H__
#include "debug.h"


typedef struct
{
  uint8_t hour;
  uint8_t min;
  uint8_t sec;

  uint16_t year;
  uint8_t  month;
  uint8_t  day;
  uint8_t  week;

} RTC_DateTypeDef;




typedef struct
{
    RTC_TypeDef                   *Instance;
    RTC_DateTypeDef                Date;
}RTC_HandleTypeDef;



extern  RTC_HandleTypeDef RTC_Handler;
rt_err_t rt_rtc_get_time(RTC_DateTypeDef *date);

#endif
