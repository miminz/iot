/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2020/04/30
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/
#include "ch32v30x.h"
#include <rtthread.h>
#include <rthw.h>
#include "drivers/pin.h"
#include "net_app.h"
#include "usbdisk_app.h"
#include "rtc_app.h"
#include "board.h"
#include "modbus_app.h"
#include "fal.h"
#include "sfud_def.h"
#include "io_app.h"
#include "eth_driver.h"
/* Global typedef */

/* Global define */

ALIGN(RT_ALIGN_SIZE)
static char led_task_stack[1024];
static struct rt_thread led_task_thread;

int led_task_entry(void)
{
    rt_uint8_t count;
    GPIO_InitTypeDef  GPIO={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO.GPIO_Pin = GPIO_Pin_12;
    GPIO.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO);
    rt_kprintf("led_SP:%08x\r\n", __get_SP());
    while (1)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
        rt_thread_mdelay(500);
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        rt_thread_mdelay(500);
    }
    return 0;
}

void led_task_init(void)
{
    rt_thread_init(&led_task_thread,
                   "led",
                   led_task_entry,
                   RT_NULL,
                   &led_task_stack[0],
                   sizeof(led_task_stack),
                   14, 20);

    rt_thread_startup(&led_task_thread);
}


/*********************************************************************
 * @fn      main
 *
 * @brief   main只是一个线程之一,此例程main调用用户线程初始化后直接返回，未常驻
 *
 * @note    其他任务：tshell、net、idle、USB
 *          广播包、DNS、MQTT_PUB等获取可以直接从shell中被调用
 *
 *          net任务DHCP成功则使用DHCP的IP进行初始化，否则使用默认IP。net新建
 *                                 了 一个TCP，一个UDP、一个MQTT、一个DHCP、一个DNS，其中DHCP在将在
 *                                 成功或超时后释放socket，DNS为一直获取，并未释放，占用一个UDP socket
 *          MQTT建立连接后订阅主题topic/1,pub和sub主题相同，pub同时可以收到数据。
 *
 * @return  none
 */


int main(void)
{

    rt_kprintf("\r\n MCU: CH32V307\r\n");
    rt_kprintf(" SysClk: %dHz\r\n", SystemCoreClock);
    rt_kprintf(" www.wch.cn\r\n");
   // fal_init();
    net_task_init();
    // usb_task_init();
    rtc_task_init();
   rtu_task_init();
   init_io();

   led_task_init();
    return 0;
}

/*********************************************************************
 * @fn      led
 *
 * @brief   测试使用驱动接口操作I/O口
 *
 * @return  none
 */

//MSH_CMD_EXPORT(led, led sample by using I / O drivers);
