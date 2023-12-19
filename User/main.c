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
 * @brief   mainֻ��һ���߳�֮һ,������main�����û��̳߳�ʼ����ֱ�ӷ��أ�δ��פ
 *
 * @note    ��������tshell��net��idle��USB
 *          �㲥����DNS��MQTT_PUB�Ȼ�ȡ����ֱ�Ӵ�shell�б�����
 *
 *          net����DHCP�ɹ���ʹ��DHCP��IP���г�ʼ��������ʹ��Ĭ��IP��net�½�
 *                                 �� һ��TCP��һ��UDP��һ��MQTT��һ��DHCP��һ��DNS������DHCP�ڽ���
 *                                 �ɹ���ʱ���ͷ�socket��DNSΪһֱ��ȡ����δ�ͷţ�ռ��һ��UDP socket
 *          MQTT�������Ӻ�������topic/1,pub��sub������ͬ��pubͬʱ�����յ����ݡ�
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
 * @brief   ����ʹ�������ӿڲ���I/O��
 *
 * @return  none
 */

//MSH_CMD_EXPORT(led, led sample by using I / O drivers);
