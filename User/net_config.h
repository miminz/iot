/********************************** (C) COPYRIGHT *******************************
* File Name          : net_config.h
* Author             : WCH
* Version            : V1.10
* Date               : 2022/04/28
* Description        : config file
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#ifndef __NET_CONFIG_H__
#define __NET_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * socket���ã�IPRAW + UDP + TCP + TCP_LISTEN = socket ����
 */
#define WCHNET_NUM_IPRAW              0  /* IPRAW���ӵĸ��� */

#define WCHNET_NUM_UDP                2  /* UDP���ӵĸ��� */

#define WCHNET_NUM_TCP                4  /* TCP���ӵĸ��� */

#define WCHNET_NUM_TCP_LISTEN         1  /* TCP�������� */

/* Socket�ĸ��������31  */
#define WCHNET_MAX_SOCKET_NUM    (WCHNET_NUM_IPRAW+WCHNET_NUM_UDP+WCHNET_NUM_TCP+WCHNET_NUM_TCP_LISTEN)

#define WCHNET_TCP_MSS                800  /* TCP MSS�Ĵ�С*/

/*********************************************************************
 * MAC��������
 */
#define ETH_TXBUFNB                   2    /* MAC��������������  */

#define ETH_RXBUFNB                   4    /* MAC��������������  */

#ifndef ETH_MAX_PACKET_SIZE
#define ETH_RX_BUF_SZE                1520  /* MAC���ջ��������ȣ�Ϊ4�������� */
#define ETH_TX_BUF_SZE                1520  /* MAC���ͻ��������ȣ�Ϊ4�������� */
#else
#define ETH_RX_BUF_SZE                ETH_MAX_PACKET_SIZE
#define ETH_TX_BUF_SZE                ETH_MAX_PACKET_SIZE
#endif

/*********************************************************************
  *  ��������
 */
#define WCHNET_PING_ENABLE            1    /* PINGʹ�ܣ�Ĭ��PING���� */

#define TCP_RETRY_COUNT               20   /* TCP�ش�������Ĭ��ֵ20 */

#define TCP_RETRY_PERIOD              10   /* TCP�ش����ڣ�Ĭ��ֵ10 ����λ50ms */

#define SOCKET_SEND_RETRY             1    /* ����ʧ���������ã�1�����ã�0������ */

#define CFG0_TCP_SEND_COPY            1    /* TCP���ͻ��������ƣ�1�����ƣ�0�������� */

#define CFG0_TCP_RECV_COPY            1    /* TCP���ո����Ż����ڲ�����ʹ�� */

#define CFG0_TCP_OLD_DELETE           0    /* ɾ�������TCP���ӣ�1�����ã�0������ */

/*********************************************************************
  *  �ڴ��������
 */
#define WCHNET_NUM_PBUF               5    /* PBUF�ṹ�ĸ��� */

#define WCHNET_NUM_POOL_BUF           6    /* POOL BUF�ĸ��� */

#define WCHNET_NUM_TCP_SEG            10   /* tcp�εĸ���*/

#define WCHNET_NUM_IP_REASSDATA       5    /* IP�ֶεĸ���*/

#define WCHNET_MEM_HEAP_SIZE          4600 /* �ڴ�Ѵ�С */

#define WCHNET_NUM_ARP_TABLE          10   /* ARP�б����� */

#define WCHNET_MEM_ALIGNMENT          4    /* 4�ֽڶ��� */

#define CFG0_IP_REASS_PBUFS           (WCHNET_NUM_POOL_BUF - 1) /* IP��Ƭ��PBUF���� */

#define RECE_BUF_LEN                  WCHNET_TCP_MSS*2  /* socket���ջ����С */

#define WCHNETTIMERPERIOD             10   /* ��ʱ�����ڣ���λMs */

/* ���SOCKET�������� */
#if( !WCHNET_MAX_SOCKET_NUM || WCHNET_MAX_SOCKET_NUM > 31)
  #error "WCHNET_MAX_SOCKET_NUM Error)"
#endif
#if( WCHNET_NUM_TCP_LISTEN && !WCHNET_NUM_TCP )
  #error "WCHNET_MAX_SOCKET_NUM Error)"
#endif
/* ����ֽڶ������Ϊ4�������� */
#if((WCHNET_MEM_ALIGNMENT % 4) || (WCHNET_MEM_ALIGNMENT == 0))
  #error "WCHNET_MEM_ALIGNMENT Error,Please Config WCHNET_MEM_ALIGNMENT = 4 * N, N >=1"
#endif
/* TCP����Ķγ��� */
#if((WCHNET_TCP_MSS > 1460) || (WCHNET_TCP_MSS < 60))
  #error "WCHNET_TCP_MSS Error,Please Config WCHNET_TCP_MSS >= 60 && WCHNET_TCP_MSS <= 1460"
#endif
/* ARP��������� */
#if((WCHNET_NUM_ARP_TABLE > 0X7F) || (WCHNET_NUM_ARP_TABLE < 1))
  #error "WCHNET_NUM_ARP_TABLE Error,Please Config WCHNET_NUM_ARP_TABLE >= 1 && WCHNET_NUM_ARP_TABLE <= 0X7F"
#endif
/* ���POOL BUF���� */
#if(WCHNET_NUM_POOL_BUF < 1)
  #error "WCHNET_NUM_POOL_BUF Error,Please Config WCHNET_NUM_POOL_BUF >= 1"
#endif
/* ���PBUF�ṹ���� */
#if(WCHNET_NUM_PBUF < 1)
  #error "WCHNET_NUM_PBUF Error,Please Config WCHNET_NUM_PBUF >= 1"
#endif
/* ���IP�������� */
#if((WCHNET_NUM_IP_REASSDATA > 10) || (WCHNET_NUM_IP_REASSDATA < 1))
  #error "WCHNET_NUM_IP_REASSDATA Error,Please Config WCHNET_NUM_IP_REASSDATA < 10 && WCHNET_NUM_IP_REASSDATA >= 1 "
#endif
/* ���IP��Ƭ��С */
#if(WCHNET_IP_REASS_PBUFS > WCHNET_NUM_POOL_BUF)
  #error "WCHNET_IP_REASS_PBUFS Error,Please Config WCHNET_IP_REASS_PBUFS < WCHNET_NUM_POOL_BUF"
#endif

#if(WCHNETTIMERPERIOD > 50)
  #error "WCHNETTIMERPERIOD Error,Please Config WCHNETTIMERPERIOD < 50"
#endif

/* ����ֵ0 */
#define WCHNET_MISC_CONFIG0    (((CFG0_TCP_SEND_COPY) << 0) |\
                               ((CFG0_TCP_RECV_COPY)  << 1) |\
                               ((CFG0_TCP_OLD_DELETE) << 2) |\
                               ((CFG0_IP_REASS_PBUFS) << 3) )
/* ����ֵ1 */
#define WCHNET_MISC_CONFIG1    (((WCHNET_MAX_SOCKET_NUM)<<0)|\
                               ((WCHNET_PING_ENABLE) << 13) |\
                               ((TCP_RETRY_COUNT)    << 14) |\
                               ((TCP_RETRY_PERIOD)   << 19) |\
                               ((SOCKET_SEND_RETRY)  << 25) )

#ifdef __cplusplus
}
#endif
#endif
