/********************************** (C) COPYRIGHT *******************************
* File Name          : usbdisk_app.h
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : usbdisk_app.h
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#ifndef _USBDISK_APP_H_
#define _USBDISK_APP_H_


#include "debug.h"
#include "ch32v30x_usbhs_host.h"
#include "string.h"
#include "CHRV3UFI.h"
#include <rtthread.h>
#include <rthw.h>


void usb_task_init(void);


#endif /* _USBDISK_APP_H_ */
