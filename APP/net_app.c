/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2020/04/30
 * Description        : net_app.c
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/
#include "net_app.h"
#include <rtthread.h>
#include <rthw.h>
#include "string.h"
#include "stdlib.h"
#include "MQTTPacket.h"
#include <agile_modbus.h>
#include "modbus_app.h"
#include "eth_driver.h"
#include "net_config.h"
#include "flashdb.h"
#include "io_app.h"
#define MODE_TCPSERVER 0
#define MODE_TCPCLIENT 1
/********************************************************************************************************/
ALIGN(RT_ALIGN_SIZE)
static char net_task_stack[2048];
static struct rt_thread net_task_thread;

extern SOCK_INF SocketInf[];

/*********************************************************************************************************/
#define KEEPLIVE_ENABLE /*ʹ��keeplive����*/

/* for net */
u8 MACAddr[6];                            /*Mac��ַ*/
 u8 IPAddr[4] = {192, 168, 1, 10};         /*IP��ַ*/
 u8 GWIPAddr[4] = {192, 168, 1, 1};        /*����*/
 u8 IPMask[4] = {255, 255, 255, 0};        /*��������*/
u8 DESIP[4] = {192, 168, 1, 232};         /*Ŀ��IP��ַ*/
u8 BroadcastIP[4] = {255, 255, 255, 255}; /* �㲥IP */
u8 DNSServerIP[4] = {192, 168, 1, 1};     /* DNS������ */
u16 DnsPort = 53;
__attribute__((__aligned__(4))) u8 RemoteIp[4];

/* for socket */
u8 SocketId_TCP = 0; /*socket id��*/
u8 SocketIdForListen = 0;
u8 SocketId_UDP = 0;
u8 socketId_MQTT = 0;
u8 SocketRecvBuf[WCHNET_MAX_SOCKET_NUM][RECE_BUF_LEN]; /*socket������*/
u8 MyBuf[RECE_BUF_LEN];
u8 MyBuf1[RECE_BUF_LEN];
u16 desport = 2000; /*Ŀ�Ķ˿ں�*/
u16 srcport = 1000; /*Դ�˿ں�*/
u16 server_port = 8080;
u8 socket[WCHNET_MAX_SOCKET_NUM];

/* for MQTT */
char *username = "user1";    /* �豸����ÿ���豸Ψһ������"/"���ּ�  */
char *password = "user1";    /* ��������¼���� */
char *sub_topic = "topic/1"; /* ���ĵĻỰ����,Ϊ���Է����գ�Ӧ���뷢���ĻỰ������ͬ  */
char *pub_topic = "topic/1"; /* �����ĻỰ */
int pub_qos = 0;
int sub_qos = 0;
char *payload = "WCHNET MQTT TSET";

u8 MQTT_DESIP[4] = {192, 168, 1, 232};
u16 desport_mqtt = 1883;
u16 srcport_mqtt = 4200;

u8 con_flag = 0;  /* ������MQTT��������־λ */
u8 pub_flag = 0;  /* �ѷ����ػ���Ϣ��־λ */
u8 sub_flag = 0;  /* �Ѷ��ĻỰ��־λ */
u8 tout_flag = 0; /* ��ʱ��־ */
u8 packetid = 5;  /* ��ID */

extern const agile_modbus_slave_util_t slave_util;

/*********************************************************************
 * @fn      mStopIfError
 *
 * @brief   check if error.
 *
 * @return  none
 */
void mStopIfError(u8 iError)
{
    if (iError == WCHNET_ERR_SUCCESS)
        return;                                 /* �����ɹ� */
    rt_kprintf("Error: %02X\r\n", (u16)iError); /* ��ʾ���� */
}

void wchnet_modbus(uint8_t id)
{
    u8 i;
    uint32_t len;
    uint32_t endAddr = SocketInf[id].RecvStartPoint + SocketInf[id].RecvBufLen;
}
/*********************************************************************
 * @fn      WCHNET_HandleSockInt
 *
 * @brief   Socket Interrupt Handle
 *
 * @return  none
 */
void WCHNET_HandleSockInt(u8 socketid, u8 initstat, agile_modbus_t *ctx)
{
    u32 len, toltallen;
    u8 *p = MyBuf;
    u8 i;

    int qos, payloadlen;
    MQTTString topicName;
    unsigned short packetid;
    unsigned char retained, dup;
    unsigned char *payload = NULL;

    if (initstat & SINT_STAT_RECV) /* socket�����ж�*/
    {
        len = WCHNET_SocketRecvLen(socketid, NULL); /* ��ȡsocket���������ݳ���  */
            if (SocketInf[socketid].SourPort == server_port)
            {
                    ctx->read_bufsz = len;
                    WCHNET_SocketRecv(socketid, ctx->read_buf, &len);
                    int send_len = 0;
                    send_len = agile_modbus_slave_handle(ctx, len, 0, agile_modbus_slave_util_callback, &slave_util, NULL);
                    toltallen = send_len;
                    if (send_len > 0)
                    {
                        while (1)
                        {
                            len = toltallen;
                            WCHNET_SocketSend(socketid, ctx->send_buf, &len);
                            toltallen -= len;
                            p += len;
                            if (toltallen)
                                continue;
                            break;
                        }

                    }
                    // WCHNET_SocketClose(socketid, TCP_CLOSE_NORMAL);
        }

        if (socketid == SocketId_TCP) // �ͻ���
        {
            len = WCHNET_SocketRecvLen(socketid, NULL); /* ��ȡsocket���������ݳ���  */
                                                        //          rt_kprintf("rec len: %d \r\n",len);                                   /* ʵ�ʷ��͵ĳ���,���ͺ�������ֵ */
            toltallen = len;
            WCHNET_SocketRecv(socketid, MyBuf, &len);
            while (1)
            {
                len = toltallen;
                WCHNET_SocketSend(socketid, p, &len);
                toltallen -= len;
                p += len;
                if (toltallen)
                    continue;
                break;
            }
        }

#if 1
        /* MQTT process */
        if (socketid == socketId_MQTT)
        {
            len = WCHNET_SocketRecvLen(socketid, NULL);
            WCHNET_SocketRecv(socketid, MyBuf1, &len); /* �����ջ����������ݶ���MyBuf��*/
            switch (MyBuf1[0] >> 4)
            {
            case CONNACK:
                rt_kprintf("CONNACK\r\n");
                con_flag = 1;
                MQTT_Subscribe(sub_topic, sub_qos);
                break;

            case PUBLISH:
                payload = rt_malloc(100);
                MQTTDeserialize_publish(&dup, &qos, &retained, &packetid, &topicName,
                                        &payload, &payloadlen, MyBuf1, len);
                msgDeal(payload, payloadlen);
                rt_free(payload);
                break;

            case SUBACK:
                sub_flag = 1;
                rt_kprintf("SUBACK\r\n");
                break;

            default:

                break;
            }
            memset(MyBuf1, 0, sizeof(MyBuf1));
        }
#endif
    }

    if (initstat & SINT_STAT_CONNECT) /* socket���ӳɹ��ж�*/
    {
        if (socketid == SocketId_TCP)
        {
#ifdef KEEPLIVE_ENABLE
            WCHNET_SocketSetKeepLive(socketid, ENABLE);
#endif
            rt_kprintf("TCP Connect Success\r\n");
            return;
        }
        if (socketid == socketId_MQTT)
        {
            WCHNET_ModifyRecvBuf(socketid, (u32)SocketRecvBuf, RECE_BUF_LEN);
            MQTT_Connect(username, password);
            rt_kprintf("MQTT Connect\r\n");
            return;
        }
#ifdef KEEPLIVE_ENABLE
        WCHNET_SocketSetKeepLive(socketid, ENABLE);
#endif
        WCHNET_ModifyRecvBuf(socketid, (u32)SocketRecvBuf[socketid],
                             RECE_BUF_LEN);
        for (i = 0; i < WCHNET_MAX_SOCKET_NUM; i++)
        {
            if (socket[i] == 0xff)
            { // save connected socket id
                socket[i] = socketid;
                break;
            }
        }
        printf("TCP Connect Success\r\n");
        printf("socket id: %d\r\n", socket[i]);
    }

    if (initstat & SINT_STAT_DISCONNECT) /* socket���ӶϿ��ж�*/
    {
        for (i = 0; i < WCHNET_MAX_SOCKET_NUM; i++)
        { // delete disconnected socket id
            if (socket[i] == socketid)
            {
                socket[i] = 0xff;
                break;
            }
        }
        
        if (socketid == SocketId_TCP)
        {
            printf("TCP Disconnect\r\n");
            rt_thread_mdelay(200);
            WCHNET_CreatTCPSocket(); /* TCP���� */
        }
        if (socketid == socketId_MQTT)
        {
            rt_kprintf("MQTT Disconnect\r\n");
            con_flag = 0;
        }
    }

    if (initstat & SINT_STAT_TIM_OUT) /* socket���ӳ�ʱ�ж�*/
    {
        if (socketid == SocketId_TCP)
        {
            rt_kprintf("TCP Timeout\r\n"); /* ��ʱ200ms������*/
            rt_thread_mdelay(200);
            WCHNET_CreatTCPSocket(); /* TCP���� */
            return;
        }
        if (socketid == socketId_MQTT)
        {
            con_flag = 0;
            rt_kprintf("MQTT TCP Timeout\r\n");
            Transport_Open();
            return;
        }
        for (i = 0; i < WCHNET_MAX_SOCKET_NUM; i++)
        { // delete disconnected socket id
            if (socket[i] == socketid)
            {
                socket[i] = 0xff;
                break;
            }
        }
    }
}

/*********************************************************************
 * @fn      WCHNET_HandleGlobalInt
 *
 * @brief   Global Interrupt Handle
 *
 * @return  none
 */
void WCHNET_HandleGlobalInt(agile_modbus_t *ctx)
{
    u8 initstat;
    u16 i;
    u8 socketinit;

    initstat = WCHNET_GetGlobalInt(); /* ��ȡȫ���жϱ�־*/
    if (initstat & GINT_STAT_UNREACH) /* ���ɴ��ж� */
    {
        rt_kprintf("GINT_STAT_UNREACH\r\n");
    }
    if (initstat & GINT_STAT_IP_CONFLI) /* IP��ͻ�ж� */
    {
        rt_kprintf("GINT_STAT_IP_CONFLI\r\n");
    }
    if (initstat & GINT_STAT_PHY_CHANGE) /* PHY״̬�仯�ж� */
    {
        rt_kprintf("PHY Status change\r\n");
        i = WCHNET_GetPHYStatus(); /* ��ȡPHY����״̬*/
        if (i & PHY_Linked_Status)
            rt_kprintf("PHY Link Success\r\n");
    }
    if (initstat & GINT_STAT_SOCKET)
    {
        for (i = 0; i < WCHNET_MAX_SOCKET_NUM; i++)
        {
            socketinit = WCHNET_GetSocketInt(i);
            if (socketinit)
                WCHNET_HandleSockInt(i, socketinit, ctx);
        }
    }
}
/*********************************************************************
 * @fn      WCHNET_CreatTCPSocket������
 *
 * @brief   Creat TCP Socket
 *
 * @return  none
 */
void WCHNET_CreateTcpSocketListen(void)
{
    u8 i;
    SOCK_INF TmpSocketInf;

    memset((void *) &TmpSocketInf, 0, sizeof(SOCK_INF));
    TmpSocketInf.SourPort = server_port;
    TmpSocketInf.ProtoType = PROTO_TYPE_TCP;
    i = WCHNET_SocketCreat(&SocketIdForListen, &TmpSocketInf);
    printf("SocketIdForListen %d\r\n", SocketIdForListen);
    mStopIfError(i);
    i = WCHNET_SocketListen(SocketIdForListen);
    mStopIfError(i);
}
/*********************************************************************
 * @fn      WCHNET_CreatTCPSocket�ͻ���
 *
 * @brief   Creat TCP Socket
 *
 * @return  none
 */
void WCHNET_CreatTCPSocket(void)
{
    u8 i;
    SOCK_INF TmpSocketInf;                                                                                       /* ������ʱsocket���� */
                                                                                                                 /* ����һ���ͻ���*/
    memset((void *)&TmpSocketInf, 0, sizeof(SOCK_INF)); /* ���ڲ��Ὣ�˱������ƣ�������ý���ʱ������ȫ������ */ /* ����Ŀ�Ķ˿� */
    memcpy((void *)TmpSocketInf.IPAddr, DESIP, 4);                                                               /* ����Ŀ��IP��ַ */
    TmpSocketInf.DesPort = desport;                                                                              /* ����Ŀ�Ķ˿� */
    TmpSocketInf.SourPort = srcport;                                                                             /* ����Դ�˿� */
    TmpSocketInf.ProtoType = PROTO_TYPE_TCP;                                                                     /* ����socekt���� */
    TmpSocketInf.RecvStartPoint = (u32)SocketRecvBuf[0];                                                         /* ���ý��ջ������Ľ��ջ����� */
    TmpSocketInf.RecvBufLen = RECE_BUF_LEN;                                                                      /* ���ý��ջ������Ľ��ճ��� */
    i = WCHNET_SocketCreat(&SocketId_TCP, &TmpSocketInf);                                                        /* ����socket�������ص�socket����������SocketId�� �����㿪ʼ����*/
    rt_kprintf("WCHNET_SocketCreatTCP %d\r\n", SocketId_TCP);
    mStopIfError(i);                        /* ������ */
    i = WCHNET_SocketConnect(SocketId_TCP); /* TCP���� */
    mStopIfError(i);                        /* ������ */
}

/*********************************************************************
 * @fn      WCHNET_UdpServerRecv
 *
 * @brief   Udp Recv function
 *
 * @return  none
 */
void WCHNET_UdpServerRecv(struct _SCOK_INF *socinf, u32 ipaddr, u16 port, u8 *buf, u32 len)
{
    u8 ip_addr[4], i;

    //  rt_kprintf("Remote IP:");
    for (i = 0; i < 4; i++)
    {
        ip_addr[i] = ipaddr & 0xff;
        //    rt_kprintf("%d ",ip_addr[i]);
        ipaddr = ipaddr >> 8;
    }

    //    rt_kprintf("srcport=%d len=%d socketid=%d\r\n",port,len,socinf->SockIndex);

    WCHNET_SocketUdpSendTo(socinf->SockIndex, buf, &len, ip_addr, port);
}

/*********************************************************************
 * @fn      WCHNET_CreatUDPSocket
 *
 * @brief   Creat UDP Socket
 *
 * @return  none
 */
void WCHNET_CreatUDPSocket(void)
{
    u8 i;
    SOCK_INF TmpSocketInf;                              /* ������ʱsocket���� */
                                                        /* ����һ���ͻ���*/
    memset((void *)&TmpSocketInf, 0, sizeof(SOCK_INF)); /* ���ڲ��Ὣ�˱������ƣ�������ý���ʱ������ȫ������ */
    memcpy((void *)TmpSocketInf.IPAddr, BroadcastIP, 4);
    TmpSocketInf.SourPort = srcport;                      /* ����Դ�˿� */
    TmpSocketInf.ProtoType = PROTO_TYPE_UDP;              /* ����socekt���� */
    TmpSocketInf.RecvStartPoint = (u32)SocketRecvBuf[1];  /* ���ý��ջ������Ľ��ջ����� */
    TmpSocketInf.RecvBufLen = RECE_BUF_LEN;               /* ���ý��ջ������Ľ��ճ��� */
    TmpSocketInf.AppCallBack = WCHNET_UdpServerRecv;      /* Udp���ջص����� ,Udp�Ľ����ڻص������д���*/
    i = WCHNET_SocketCreat(&SocketId_UDP, &TmpSocketInf); /* ����socket�������ص�socket����������SocketId�� */
    rt_kprintf("WCHNET_SocketCreatUDP %d\r\n", SocketId_UDP);
    mStopIfError(i); /* ������ */
}

/*********************************************************************
 * @fn      WCHNET_DHCPCallBack
 *
 * @brief   DHCPCallBack
 *
 * @return  DHCP status
 */
u8 WCHNET_DHCPCallBack(u8 status, void *arg)
{
    u8 *p;

    if (!status)
    {
        p = arg;
        rt_kprintf("DHCP Success\r\n");
        memcpy(IPAddr, p, 4);
        memcpy(GWIPAddr, &p[4], 4);
        memcpy(IPMask, &p[8], 4);
        memcpy(DNSServerIP, &p[12], 4);

        rt_kprintf("IPAddr = %d.%d.%d.%d \r\n", (u16)IPAddr[0], (u16)IPAddr[1],
                   (u16)IPAddr[2], (u16)IPAddr[3]);
        rt_kprintf("GWIPAddr = %d.%d.%d.%d \r\n", (u16)GWIPAddr[0], (u16)GWIPAddr[1],
                   (u16)GWIPAddr[2], (u16)GWIPAddr[3]);
        rt_kprintf("IPAddr = %d.%d.%d.%d \r\n", (u16)IPMask[0], (u16)IPMask[1],
                   (u16)IPMask[2], (u16)IPMask[3]);
        rt_kprintf("DNS1: %d.%d.%d.%d \r\n", DNSServerIP[0], DNSServerIP[1], DNSServerIP[2], DNSServerIP[3]);
        rt_kprintf("DNS2: %d.%d.%d.%d \r\n", p[16], p[17], p[18], p[19]);
    }
    else
    {
        rt_kprintf("DHCP Fail %02x \r\n", status);
    }

    if (!status)
        rt_kprintf("Using DHCP ip create socket\r\n");
    else
        rt_kprintf("Using Default ip create scoket\r\n");

    WCHNET_DHCPStop();       /* release UDP socket */
    
   // WCHNET_CreatTCPSocket(); /* create TCP sockets */
    WCHNET_CreateTcpSocketListen();
    WCHNET_CreatUDPSocket(); /* create UDP sockets */
   // Transport_Open();        /* create TCP socket for MQTT */
    return status;
}

/*********************************************************************
 * @fn      WCHNET_DNSCallBack
 *
 * @brief   DNSCallBack
 *
 * @return  none
 */
void WCHNET_DNSCallBack(const char *name, u8 *ipaddr, void *callback_arg)
{
    if (ipaddr == NULL)
    {
        rt_kprintf("DNS Fail\r\n");
        return;
    }
    rt_kprintf("Host Name = %s\r\n", name);
    rt_kprintf("IP= %d.%d.%d.%d\r\n", ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);
    if (callback_arg != NULL)
    {
        rt_kprintf("callback_arg = %02x\r\n", (*(u8 *)callback_arg));
    }
}

/*********************************************************************
 * @fn      net_task_entry
 *
 * @brief   net task thread entry
 *
 * @return  none
 */
void net_task_entry(void *parameter)
{struct fdb_blob blob;
    rt_uint8_t i;
    // ��ʼ��modbustcp
    uint8_t ctx_send_buf[AGILE_MODBUS_MAX_ADU_LENGTH];
    uint8_t ctx_read_buf[AGILE_MODBUS_MAX_ADU_LENGTH];

    agile_modbus_tcp_t ctx_tcp;
    agile_modbus_t *ctx = &ctx_tcp._ctx;
    agile_modbus_tcp_init(&ctx_tcp, ctx_send_buf, sizeof(ctx_send_buf), ctx_read_buf, sizeof(ctx_read_buf));
    agile_modbus_set_slave(ctx, 1);

//    uint8_t *IPaddr = rt_malloc(4);
//    uint8_t *GWIPaddr = rt_malloc(4);
//    uint8_t *IPmask = rt_malloc(4);
//    uint8_t *MacAddr = rt_malloc(6);
    rt_kprintf("WCH NET Test\r\n");
    rt_kprintf("net version:%x\n", WCHNET_GetVer());
    if (WCHNET_LIB_VER != WCHNET_GetVer())
    {
        rt_kprintf("version error.\n");
    }
    WCHNET_GetMacAddr(MACAddr); /*��ȡоƬMac��ַ*/
    rt_kprintf("mac addr:");
    for (int i = 0; i < 6; i++)
        rt_kprintf("%x ", MACAddr[i]);
    rt_kprintf("\n");
    WCHNET_DHCPSetHostname("ch32v307");                 /*����HostName*/

    fdb_kv_get_blob(&kvdb, "IPAddrs", fdb_blob_make(&blob, &IPAddr, 4));
    fdb_kv_get_blob(&kvdb, "IPMask", fdb_blob_make(&blob, &IPMask, 4));
    fdb_kv_get_blob(&kvdb, "GWIPAddr", fdb_blob_make(&blob, &GWIPAddr, 4));

    
    i = ETH_LibInit(IPAddr, GWIPAddr, IPMask, MACAddr); /*��̫�����ʼ��*/
    mStopIfError(i);
    if (i == WCHNET_ERR_SUCCESS)
        rt_kprintf("WCHNET_LibInit Success\r\n");

//    rt_free(IPaddr);
//    rt_free(GWIPaddr);
//    rt_free(IPmask);
//    rt_free(MacAddr);


#ifdef KEEPLIVE_ENABLE
    { /*����keeplive����*/
        struct _KEEP_CFG cfg;

        cfg.KLIdle = 20000;
        cfg.KLIntvl = 15000;
        cfg.KLCount = 9;
        WCHNET_ConfigKeepLive(&cfg);
    }
#endif

    WCHNET_DHCPStart(WCHNET_DHCPCallBack); /*��ʼDHCP,���ؽ����WCHNET_DHCPCallBack������*/

    rt_memset(socket, 0xff, WCHNET_MAX_SOCKET_NUM);

    while (1)
    {
        WCHNET_MainTask();           /*��̫����������������Ҫѭ������*/
        if (WCHNET_QueryGlobalInt()) /*��ѯ��̫��ȫ���жϣ�������жϣ�����ȫ���жϴ�������*/
        {
            WCHNET_HandleGlobalInt(ctx);
        }
        rt_thread_delay(1);
    }
}

/*********************************************************************
 * @fn      net_task_init
 *
 * @brief   net task thread initial
 *
 * @return  none
 */
void net_task_init(void)
{
    rt_thread_init(&net_task_thread,
                   "net",
                   net_task_entry,
                   RT_NULL,
                   &net_task_stack[0],
                   sizeof(net_task_stack),
                   4, 200);
    rt_thread_startup(&net_task_thread);
}

/********************************************************************
 *@note: ����ΪMQTT���
 */

/*********************************************************************
 * @fn      Transport_Open
 *
 * @brief   create MQTT tcp socket
 *
 * @return  error code
 */
void Transport_Open(void)
{
    u8 i;
    SOCK_INF TmpSocketInf;                              /* ������ʱsocket���� */
    memset((void *)&TmpSocketInf, 0, sizeof(SOCK_INF)); /* ���ڲ��Ὣ�˱������ƣ�������ý���ʱ������ȫ������ */
    memcpy((void *)TmpSocketInf.IPAddr, MQTT_DESIP, 4); /* ����Ŀ��IP��ַ */
    TmpSocketInf.DesPort = desport_mqtt;                /* ����Ŀ�Ķ˿� */
    TmpSocketInf.SourPort = srcport_mqtt;               /* ����Դ�˿� */
    TmpSocketInf.ProtoType = PROTO_TYPE_TCP;            /* ����socket���� */
    TmpSocketInf.RecvStartPoint = (u32)SocketRecvBuf[2];
    TmpSocketInf.RecvBufLen = RECE_BUF_LEN;                /* ���ý��ջ������Ľ��ճ��� */
    i = WCHNET_SocketCreat(&socketId_MQTT, &TmpSocketInf); /* ����socket�������ص�socket����������SocketId�� */
    rt_kprintf("WCHNET_SocketCreatMQTT %d\r\n", socketId_MQTT);
    mStopIfError(i);                         /* ������ */
    i = WCHNET_SocketConnect(socketId_MQTT); /* TCP���� */
    mStopIfError(i);                         /* ������ */
}

/*********************************************************************
 * @fn      Transport_Close
 *
 * @brief   close MQTT tcp socket
 *
 * @return  error code
 */
void Transport_Close(void)
{
    u8 i;
    i = WCHNET_SocketClose(socketId_MQTT, TCP_CLOSE_NORMAL);
    mStopIfError(i);
}

/*********************************************************************
 * @fn      Transport_SendPacket
 *
 * @brief   send MQTT packet
 *
 * @param   buf: send data buffer point
 *          len: data length.
 *
 * @return  none
 */
void Transport_SendPacket(u8 *buf, u32 len)
{
    u32 totallen;
    u8 *p = buf;

    totallen = len;
    while (1)
    {
        len = totallen;
        WCHNET_SocketSend(socketId_MQTT, p, &len); /* ��MyBuf�е����ݷ��� */
        totallen -= len;                           /* ���ܳ��ȼ�ȥ�Լ�������ϵĳ��� */
        p += len;                                  /* ��������ָ��ƫ��*/
        if (totallen)
            continue; /* �������δ������ϣ����������*/
        break;        /* ������ϣ��˳� */
    }
}

/*********************************************************************
 * @fn      MQTT_Connect
 *
 * @brief   creat MQTT connect
 *
 * @param   username: device name
 *          password: password.
 *
 * @return  none
 */
void MQTT_Connect(char *username, char *password)
{
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    u32 len;
    u8 buf[200];

    data.clientID.cstring = "ch32v307";
    data.keepAliveInterval = 2000;
    data.cleansession = 1;
    data.username.cstring = username;
    data.password.cstring = password;

    len = MQTTSerialize_connect(buf, sizeof(buf), &data);
    Transport_SendPacket(buf, len); /*����MQTT����*/
}

/*********************************************************************
 * @fn      MQTT_Subscribe
 *
 * @brief   Subscribe a topic
 *
 * @param   topic: topic name
 *          req_qos: qos level.
 *
 * @return  none
 */
void MQTT_Subscribe(char *topic, int req_qos)
{
    MQTTString topicString = MQTTString_initializer;
    u32 len;
    u32 msgid = 1;
    u8 buf[200];

    topicString.cstring = topic;
    len = MQTTSerialize_subscribe(buf, sizeof(buf), 0, msgid, 1, &topicString, &req_qos);
    Transport_SendPacket(buf, len);
}

/*********************************************************************
 * @fn      MQTT_Unsubscribe
 *
 * @brief   Unsubscribe a topic
 *
 * @param   topic: topic name
 *
 * @return  none
 */
void MQTT_Unsubscribe(char *topic)
{
    MQTTString topicString = MQTTString_initializer;
    u32 len;
    u32 msgid = 1;
    u8 buf[200];

    topicString.cstring = topic;
    len = MQTTSerialize_unsubscribe(buf, sizeof(buf), 0, msgid, 1, &topicString);
    Transport_SendPacket(buf, len);
}

/*********************************************************************
 * @fn      MQTT_Publish
 *
 * @brief   Publish a topic
 *
 * @param   topic  : topic name
 *          qos    : qos level
 *          payload: data payload
 *
 * @return  none
 */
void MQTT_Publish(char *topic, int qos, char *payload)
{
    MQTTString topicString = MQTTString_initializer;
    u32 payloadlen;
    u32 len;
    u8 buf[200];

    topicString.cstring = topic;
    payloadlen = strlen(payload);
    len = MQTTSerialize_publish(buf, sizeof(buf), 0, qos, 0, packetid++, topicString, payload, payloadlen);
    Transport_SendPacket(buf, len);
}

/*********************************************************************
 * @fn      MQTT_Pingreq
 *
 * @brief   send MQTT keeplive packet
 *
 * @return  none
 */
void MQTT_Pingreq(void)
{
    u32 len;
    u8 buf[200];

    len = MQTTSerialize_pingreq(buf, sizeof(buf));
    Transport_SendPacket(buf, len);
}

/*********************************************************************
 * @fn      MQTT_Disconnect
 *
 * @brief   MQTT disconnect
 *
 * @return  none
 */
void MQTT_Disconnect(void)
{
    u32 len;
    u8 buf[50];
    len = MQTTSerialize_disconnect(buf, sizeof(buf));
    Transport_SendPacket(buf, len);
}

/*********************************************************************
 * @fn      msgDeal
 *
 * @brief   process subscribe msg
 *
 * @param   msg : subscribe msg
 *          len : length
 *
 * @return  none
 */
void msgDeal(char *msg, int len)
{
    char *ptr = msg;
    rt_kprintf("payload len = %d\r\n", len);
    rt_kprintf("payload: ");
    for (u8 i = 0; i < len; i++)
    {
        rt_kprintf("%c ", (u16)*ptr);
        ptr++;
    }
    rt_kprintf("\r\n");
}

/*********************************************************************
 *@note ����Ϊ�����е�����������shell�������б��б��ظ�����
 */
#ifdef RT_USING_FINSH
#include <finsh.h>

/*
 * note: udp server socket should be created
 * */
int broadcast2port(int argc, char **argv)
{
    int port;
    uint32_t len;
    char buffer[] = "This is a broadcast packet!";
    if (argc < 2)
    {
        rt_kprintf("Usage  : broadcast2port port\r\n");
        rt_kprintf("Example: broadcast2port 2000\r\n");
        return 0;
    }

    port = strtoul(argv[1], 0, 10);
    len = sizeof(buffer);

    WCHNET_SocketUdpSendTo(SocketId_UDP, buffer, &len, BroadcastIP, port);
    return 0;
}
FINSH_FUNCTION_EXPORT(broadcast2port, send a broadcast packet);
MSH_CMD_EXPORT(broadcast2port, send a broadcast packet);

/*
 * note: DNS will occupy a udp socket
 */
int DNS_Query(int argc, char **argv)
{
    const char *url;
    uint8_t err;
    if (argc < 2)
    {
        rt_kprintf("Usage  : DNS_Query url\r\n");
        rt_kprintf("Example: DNS_Query www.baidu.com\r\n");
        return 0;
    }

    url = argv[1];

    /* will occupy a UDP socket
     * user can call "WCHNET_DNSStop" to kill the dns socket
     * */
    WCHNET_InitDNS(DNSServerIP, DnsPort);
    err = WCHNET_HostNameGetIp(url, RemoteIp, WCHNET_DNSCallBack, NULL);
    if (err == WCHNET_ERR_SUCCESS)
    {
        rt_kprintf("Host Name = %s\r\n", url);
        rt_kprintf("IP= %d.%d.%d.%d\r\n", RemoteIp[0], RemoteIp[1], RemoteIp[2], RemoteIp[3]);
    }
    else if (err == WCHNET_ERR_INPROGRESS)
    {
        rt_kprintf("Will Process in WCHNET_DNSCallBack\r\n");
    }

    return 0;
}

FINSH_FUNCTION_EXPORT(DNS_Query, DNS query);
MSH_CMD_EXPORT(DNS_Query, DNS query);

int MQTT_Pub(int argc, char **argv)
{
    char *topic;
    int qos;
    char *payload;

    if (argc < 4)
    {
        rt_kprintf("Usage  : MQTT_Pub topic qos payload\r\n");
        rt_kprintf("Example: MQTT_Pub topic/1 0  payload \r\n");
        return 0;
    }
    topic = argv[1];
    payload = argv[3];
    qos = strtoul(argv[2], 0, 10);
    MQTT_Publish(topic, qos, payload);

    return 0;
}
FINSH_FUNCTION_EXPORT(MQTT_Pub, mqtt publish);
MSH_CMD_EXPORT(MQTT_Pub, mqtt publish);

#endif
