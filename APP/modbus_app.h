/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : net_app.h
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#ifndef _MODBUS_APP_H_
#define _MODBUS_APP_H_

#include "debug.h"
#include "WCHNET.h"
#include "agile_modbus.h"
#include "agile_modbus_slave_util.h"



int rs485_init(void);
int rs485_send(uint8_t *buf, int len);
int rs485_receive(uint8_t *buf, int bufsz, int timeout, int bytes_timeout);
void rtu_task_init(void);
void rtu_task_entry(void *parameter);



#endif /* _NET_APP_H_ */
