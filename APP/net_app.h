/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : net_app.h
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#ifndef _NET_APP_H_
#define _NET_APP_H_

#include "debug.h"
#include "WCHNET.h"
#include "eth_driver.h"

void WCHNET_CreateTcpSocketListen(void);
void WCHNET_CreatTCPSocket(void);
void WCHNET_CreatUDPSocket(void);
u8 WCHNET_DHCPCallBack(u8 status, void *arg);
void WCHNET_DNSCallBack(const char *name, u8 *ipaddr, void *callback_arg);
void WCHNET_UdpServerRecv(struct _SCOK_INF *socinf,u32 ipaddr,u16 port,u8 *buf,u32 len);

void Transport_Open(void);
void Transport_Close(void);
void Transport_SendPacket(u8 *buf, u32 len);
void MQTT_Connect(char *username, char *password);
void MQTT_Subscribe( char *topic,int req_qos);
void MQTT_Unsubscribe(char *topic);
void MQTT_Publish(char *topic, int qos, char *payload);
void MQTT_Pingreq(void);
void MQTT_Disconnect(void);
void msgDeal(char *msg, int len);

void net_task_init(void);


#endif /* _NET_APP_H_ */
