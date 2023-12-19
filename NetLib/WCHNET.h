/********************************** (C) COPYRIGHT *******************************
* File Name        : WCHNET.H
* Author           : WCH
* Version          : V1.20
* Date             : 2022/05/07
* Description      : ��̫��Э��ջ��ͷ�ļ�
* Copyright (c) 2022 Nanjing Qinheng Microelectronics Co., Ltd.
*******************************************************************************/
#ifndef __WCHNET_H__
#define __WCHNET_H__

#include "stdint.h"
#ifndef NET_LIB
#include "net_config.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif

#define WCHNET_LIB_VER                  0x12 /* ��汾�� */
#define WCHNET_CFG_VALID                0x12345678 /* ����ֵ��Ч��־ */

/* LED״̬ @LED_STAT */
#define  LED_ON                         0
#define  LED_OFF                        1

/* PHY ״̬ @PHY_STAT */
#define PHY_LINK_SUCCESS                (1 << 2) /* PHY�������� */
#define PHY_AUTO_SUCCESS                (1 << 5) /* PHY�Զ�Э����� */

/* ���ʼ��״̬ @CFG_INIT_STAT */
#define  INIT_OK                        0x00
#define  INIT_ERR_RX_BUF_SIZE           0x01
#define  INIT_ERR_TCP_MSS               0x02
#define  INIT_ERR_HEAP_SIZE             0x03
#define  INIT_ERR_ARP_TABLE_NEM         0x04
#define  INIT_ERR_MISC_CONFIG0          0x05
#define  INIT_ERR_MISC_CONFIG1          0x06
#define  INIT_ERR_FUNC_SEND             0x09
#define  INIT_ERR_CHECK_VALID           0xFF

/* Socket ����ģʽ����,Э������ */
#define PROTO_TYPE_IP_RAW               0     /* IP��ԭʼ���� */
#define PROTO_TYPE_UDP                  2     /* UDPЭ������ */
#define PROTO_TYPE_TCP                  3     /* TCPЭ������ */

/* �ж�״̬ */
/* ����Ϊ GLOB_INT �������״̬ */
#define GINT_STAT_UNREACH               (1 << 0) /* ���ɴ��ж�*/
#define GINT_STAT_IP_CONFLI             (1 << 1) /* IP��ͻ*/
#define GINT_STAT_PHY_CHANGE            (1 << 2) /* PHY״̬�ı� */
#define GINT_STAT_SOCKET                (1 << 4) /* socket �����ж� */

/*����Ϊ Sn_INT �������״̬*/
#define SINT_STAT_RECV                  (1 << 2) /* socket�˿ڽ��յ����ݻ��߽��ջ�������Ϊ�� */
#define SINT_STAT_CONNECT               (1 << 3) /* ���ӳɹ�,TCPģʽ�²������ж� */
#define SINT_STAT_DISCONNECT            (1 << 4) /* ���ӶϿ�,TCPģʽ�²������ж� */
#define SINT_STAT_TIM_OUT               (1 << 6) /* ARP��TCPģʽ�»ᷢ�����ж� */


/* Definitions for error constants. @ERR_T */
#define ERR_T
#define WCHNET_ERR_SUCCESS              0x00  /* No error, everything OK */
#define WCHNET_ERR_BUSY                 0x10  /* busy */
#define WCHNET_ERR_MEM                  0x11  /* Out of memory error */
#define WCHNET_ERR_BUF                  0x12  /* Buffer error */
#define WCHNET_ERR_TIMEOUT              0x13  /* Timeout */
#define WCHNET_ERR_RTE                  0x14  /* Routing problem */
#define WCHNET_ERR_ABRT                 0x15  /* Connection aborted */
#define WCHNET_ERR_RST                  0x16  /* Connection reset */
#define WCHNET_ERR_CLSD                 0x17  /* Connection closed */
#define WCHNET_ERR_CONN                 0x18  /* Not connected */
#define WCHNET_ERR_VAL                  0x19  /* Illegal value */
#define WCHNET_ERR_ARG                  0x1a  /* Illegal argument */
#define WCHNET_ERR_USE                  0x1b  /* Address in use */
#define WCHNET_ERR_IF                   0x1c  /* Low-level netif error  */
#define WCHNET_ERR_ISCONN               0x1d  /* Already connected */
#define WCHNET_ERR_INPROGRESS           0x1e  /* Operation in progress */
#define WCHNET_ERR_SOCKET_MEM           0X20  /* Socket information error */
#define WCHNET_ERR_UNSUPPORT_PROTO      0X21  /* unsupported protocol type */
#define WCHNET_RET_ABORT                0x5F  /* command process fail */
#define WCHNET_ERR_UNKNOW               0xFA  /* unknow */

/* ���ɴ���� */
#define UNREACH_CODE_HOST               0     /* �������ɴ� */
#define UNREACH_CODE_NET                1     /* ���粻�ɴ� */
#define UNREACH_CODE_PROTOCOL           2     /* Э�鲻�ɴ� */
#define UNREACH_CODE_PROT               3     /* �˿ڲ��ɴ� */
/*����ֵ��ο�RFC792�ĵ�*/

/* TCP�رղ��� */
#define TCP_CLOSE_NORMAL                0     /* �����رգ�����4������ */
#define TCP_CLOSE_RST                   1     /* ��λ���ӣ����ر�  */
#define TCP_CLOSE_ABANDON               2     /* �ڲ��������ӣ����ᷢ���κ���ֹ���� */

/* socket״̬ */
#define SOCK_STAT_CLOSED                0X00  /* socket�ر� */
#define SOCK_STAT_OPEN                  0X05  /* socket�� */

/* TCP״̬ */
#define TCP_CLOSED                      0     /* TCP�ر� */
#define TCP_LISTEN                      1     /* TCP���� */
#define TCP_SYN_SENT                    2     /* SYN���ͣ��������� */
#define TCP_SYN_RCVD                    3     /* SYN���գ����յ��������� */
#define TCP_ESTABLISHED                 4     /* TCP���ӽ��� */
#define TCP_FIN_WAIT_1                  5     /* WAIT_1״̬ */
#define TCP_FIN_WAIT_2                  6     /* WAIT_2״̬ */
#define TCP_CLOSE_WAIT                  7     /* �ȴ��ر� */
#define TCP_CLOSING                     8     /* ���ڹر� */
#define TCP_LAST_ACK                    9     /* LAST_ACK*/
#define TCP_TIME_WAIT                   10    /* 2MSL�ȴ� */

/* ����ֵΪ�̶�ֵ�����Ը��� */
#define WCHNET_MEM_ALIGN_SIZE(size)    (((size) + WCHNET_MEM_ALIGNMENT - 1) & ~(WCHNET_MEM_ALIGNMENT - 1))
#define WCHNET_SIZE_IPRAW_PCB          0xFF   /* IPRAW PCB��С */
#define WCHNET_SIZE_UDP_PCB            0x30   /* UDP PCB��С */
#define WCHNET_SIZE_TCP_PCB            0xFF   /* TCP PCB��С */
#define WCHNET_SIZE_TCP_PCB_LISTEN     0x40   /* TCP LISTEN PCB��С */
#define WCHNET_SIZE_IP_REASSDATA       0x40   /* IP��Ƭ����  */
#define WCHNET_SIZE_PBUF               0x40   /* Packet Buf */
#define WCHNET_SIZE_TCP_SEG            0x60   /* TCP SEG�ṹ */
#define WCHNET_SIZE_MEM                0x0c   /* sizeof(struct mem) */
#define WCHNET_SIZE_ARP_TABLE          0x20   /* sizeof ARP table */

#define WCHNET_SIZE_POOL_BUF           WCHNET_MEM_ALIGN_SIZE(WCHNET_TCP_MSS + 40 + 14) /* pbuf�ش�С */
#define WCHNET_MEMP_SIZE               ((WCHNET_MEM_ALIGNMENT - 1) +                                                 \
                          (WCHNET_NUM_IPRAW * WCHNET_MEM_ALIGN_SIZE(WCHNET_SIZE_IPRAW_PCB)) +                        \
                          (WCHNET_NUM_UDP * WCHNET_MEM_ALIGN_SIZE(WCHNET_SIZE_UDP_PCB)) +                            \
                          (WCHNET_NUM_TCP * WCHNET_MEM_ALIGN_SIZE(WCHNET_SIZE_TCP_PCB)) +                            \
                          (WCHNET_NUM_TCP_LISTEN * WCHNET_MEM_ALIGN_SIZE(WCHNET_SIZE_TCP_PCB_LISTEN)) +              \
                          (WCHNET_NUM_TCP_SEG * WCHNET_MEM_ALIGN_SIZE(WCHNET_SIZE_TCP_SEG)) +                        \
                          (WCHNET_NUM_IP_REASSDATA * WCHNET_MEM_ALIGN_SIZE(WCHNET_SIZE_IP_REASSDATA)) +              \
                          (WCHNET_NUM_PBUF * (WCHNET_MEM_ALIGN_SIZE(WCHNET_SIZE_PBUF) + WCHNET_MEM_ALIGN_SIZE(0))) + \
                          (WCHNET_NUM_POOL_BUF * (WCHNET_MEM_ALIGN_SIZE(WCHNET_SIZE_PBUF) + WCHNET_MEM_ALIGN_SIZE(WCHNET_SIZE_POOL_BUF))))

#define HEAP_MEM_ALIGN_SIZE          (WCHNET_MEM_ALIGN_SIZE(WCHNET_SIZE_MEM))
#define WCHNET_RAM_HEAP_SIZE         (WCHNET_MEM_HEAP_SIZE + (2 * HEAP_MEM_ALIGN_SIZE) + WCHNET_MEM_ALIGNMENT)
#define WCHNET_RAM_ARP_TABLE_SIZE    (WCHNET_SIZE_ARP_TABLE * WCHNET_NUM_ARP_TABLE)

typedef struct
{
    uint32_t length;
    uint32_t buffer;
}ETHFrameType;

/* LED callback type */
typedef void (*led_callback)( uint8_t setbit );

/* net send callback type */
typedef uint32_t (*eth_tx_set )( uint16_t len, uint32_t *pBuff );

/* net receive callback type */
typedef uint32_t (*eth_rx_set )( ETHFrameType *pkt );

/* DNS callback type */
typedef void (*dns_callback)( const char *name, uint8_t *ipaddr, void *callback_arg );

/* DHCP callback type */
typedef uint8_t (*dhcp_callback)( uint8_t status, void * );

/* socket receive callback type */
struct _SCOK_INF;
typedef void (*pSockRecv)( struct _SCOK_INF *, uint32_t, uint16_t, uint8_t *, uint32_t);

/* sokcet��Ϣ�� */
typedef struct _SCOK_INF
{
    uint32_t IntStatus;                       /* �ж�״̬ */
    uint32_t SockIndex;                       /* Socket����ֵ */
    uint32_t RecvStartPoint;                  /* ���ջ������Ŀ�ʼָ�� */
    uint32_t RecvBufLen;                      /* ���ջ��������� */
    uint32_t RecvCurPoint;                    /* ���ջ������ĵ�ǰָ�� */
    uint32_t RecvReadPoint;                   /* ���ջ������Ķ�ָ�� */
    uint32_t RecvRemLen;                      /* ���ջ�������ʣ�೤�� */
    uint32_t ProtoType;                       /* Э������ */
    uint32_t ScokStatus;                      /* ���ֽ�Socket״̬���ε��ֽ�ΪTCP״̬����TCPģʽ�������� */
    uint32_t DesPort;                         /* Ŀ�Ķ˿� */
    uint32_t SourPort;                        /* Դ�˿���IPRAWģʽ��ΪЭ������ */
    uint8_t  IPAddr[4];                       /* SocketĿ��IP��ַ 32bit*/
    void *Resv1;                              /* �������ڲ�ʹ�ã����ڱ������PCB */
    void *Resv2;                              /* �������ڲ�ʹ�ã�TCP Serverʹ�� */
    pSockRecv AppCallBack;                    /* ���ջص�����*/
} SOCK_INF;

struct _WCH_CFG
{
  uint32_t TxBufSize;   /* MAC���ͻ�������С������ʹ�� */
  uint32_t TCPMss;      /* TCP MSS��С */
  uint32_t HeapSize;    /* �ѷ����ڴ��С */
  uint32_t ARPTableNum; /* ARP�б���� */
  uint32_t MiscConfig0; /* ��������0 */
  /* λ0     TCP���ͻ��������� 1�����ƣ�0�������� */
  /* λ1     TCP���ո����Ż����ڲ�����ʹ�� */
  /* λ2     ɾ�������TCP���� 1�����ã�0������  */
  /* λ3-7   IP��Ƭ��PBUF����  */
  uint32_t MiscConfig1; /* ��������1 */
  /* λ0-7   Socket�ĸ��� */
  /* λ8-12  ���� */
  /* λ13    PINGʹ�ܣ�1������  0���ر�  */
  /* λ14-18 TCP�ش�����  */
  /* λ19-23 TCP�ش����ڣ���λΪ50����  */
  /* λ25    ����ʧ�����ԣ�1�����ã�0������ */
  /* λ26-31 ���� */
  led_callback led_link; /* PHY Link״ָ̬ʾ�� */
  led_callback led_data; /* ��̫��ͨ��ָʾ�� */
  eth_tx_set net_send; /* ��̫������ */
  eth_rx_set net_recv; /* ��̫������ */
  uint32_t   CheckValid;/* ����ֵ��Ч��־���̶�ֵ @WCHNET_CFG_VALID */
};

struct _NET_SYS
{
  uint8_t  IPAddr[4];        /* IP��ַ 32bit */
  uint8_t  GWIPAddr[4];      /* ���ص�ַ 32bit */
  uint8_t  MASKAddr[4];      /* �������� 32bit */
  uint8_t  MacAddr[8];       /* MAC��ַ 48bit */
  uint8_t  UnreachIPAddr[4]; /* ���ɵ���IP */
  uint32_t RetranCount;      /* ���Դ��� Ĭ��Ϊ10�� */
  uint32_t RetranPeriod;     /* ��������,��λMS,Ĭ��500MS */
  uint32_t PHYStat;          /* PHY״̬�� 8bit */
  uint32_t NetStat;          /* ��̫����״̬ �������Ƿ�򿪵� */
  uint32_t MackFilt;         /*  MAC���ˣ�Ĭ��Ϊ���չ㲥�����ձ���MAC 8bit */
  uint32_t GlobIntStatus;    /* ȫ���ж� */
  uint32_t UnreachCode;      /* ���ɴ� */
  uint32_t UnreachProto;     /* ���ɴ�Э�� */
  uint32_t UnreachPort;      /* ���ɵ���˿� */
  uint32_t SendFlag;
  uint32_t Flags;
};

/* KEEP LIVE���ýṹ�� */
struct _KEEP_CFG
{
  uint32_t KLIdle;  /* KEEPLIVE����ʱ�䣬��λms */
  uint32_t KLIntvl; /* KEEPLIVE���ڣ���λms */
  uint32_t KLCount; /* KEEPLIVE���� */
};

/**
 * @brief   ���ʼ��.
 *
 * @param   ip - IP��ַ
 * @param   gwip - ����
 * @param   mask - ��������
 * @param   macaddr - MAC��ַ
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_Init(const uint8_t *ip, const uint8_t *gwip, const uint8_t *mask, const uint8_t *macaddr);

/**
 * @brief   ��ȡ�汾��
 *
 * @param   None
 *
 * @return  ��汾
 */
uint8_t WCHNET_GetVer(void);

/**
 * @brief   ��ȡMAC��ַ
 *
 * @param(in)   macaddr - �ڴ��ַ
 *
 * @param(out)  mac��ַ
 *
 * @return  None
 */
void WCHNET_GetMacAddr(uint8_t *macaddr);

/**
 * @brief   ���������
 *
 * @param   cfg -���ò��� @_WCH_CFG
 *
 * @return  �����ó�ʼ��״̬ @CFG_INIT_STAT
 */
uint8_t WCHNET_ConfigLIB(struct _WCH_CFG *cfg);

/**
 * @brief   ��������������Ҫһֱ���ϵ���
 *
 * @param   None
 *
 * @return  None
 */
void WCHNET_MainTask(void);

/**
 * @brief   ʱ���жϷ�������������ʱ������
 *
 * @param   timperiod - ʱ�����ڣ���λms
 *
 * @return  None
 */
void WCHNET_TimeIsr( uint16_t timperiod);

/**
 * @brief   ��̫���жϷ�������������̫���жϺ����
 *
 * @param   None
 *
 * @return  None
 */
void WCHNET_ETHIsr(void);

/**
 * @brief   Get PHY status
 *
 * @param   None
 *
 * @return  PHY ״̬ @PHY_STAT
 */
uint8_t WCHNET_GetPHYStatus(void);

/**
 * @brief   ��ѯȫ���ж�
 *
 * @param   None
 *
 * @return  GLOB_INT
 */
uint8_t WCHNET_QueryGlobalInt(void);

/**
 * @brief   ��ȫ���жϲ���ȫ���ж�����
 *
 * @param   None
 *
 * @return  GLOB_INT
 */
uint8_t WCHNET_GetGlobalInt(void);

/**
 * @brief   ����socket
 *
 * @param(in)   *socketid - �ڴ��ַ
 * @param       socinf - ����socket�����ò��� @SOCK_INF
 *
 * @param(out)  *socketid - socketIDֵ
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketCreat( uint8_t *socketid, SOCK_INF *socinf);

/**
 * @brief   Socket��������
 *
 * @param       socketid - socketIDֵ
 * @param       *buf - ���������׵�ַ
 * @param(in)   *len - �ڴ��ַ���������͵����ݳ���
 *
 * @param(out)  *len - ʵ�ʷ��͵����ݳ���
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketSend( uint8_t socketid, uint8_t *buf, uint32_t *len);

/**
 * @brief   Socket��������
 *
 * @param       socketid - socketIDֵ
 * @param       *buf - ���������׵�ַ
 * @param(in)   *len - �ڴ��ַ��������ȡ�����ݳ���
 *
 * @param(out)  *buf - д���ȡ������������
 * @param(out)  *len - ʵ�ʶ�ȡ�����ݳ���
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketRecv( uint8_t socketid, uint8_t *buf, uint32_t *len);

/**
 * @brief   ��ȡsocket�жϲ�����
 *
 * @param   socketid - socketIDֵ
 *
 * @return  Sn_INT
 */
uint8_t WCHNET_GetSocketInt( uint8_t socketid );

/**
 * @brief   ��ȡsocket���ճ���
 *
 * @param       socketid - socketIDֵ
 * @param(in)   *bufaddr - �ڴ��ַ
 *
 * @param(out)  *bufaddr - socket���ݵ��׵�ַ
 *
 * @return  ���ݳ���
 */
uint32_t WCHNET_SocketRecvLen( uint8_t socketid, uint32_t *bufaddr);

/**
 * @brief   TCP����
 *
 * @param   socketid - socketIDֵ
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketConnect( uint8_t socketid);

/**
 * @brief   TCP����
 *
 * @param   socketid - socketIDֵ
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketListen( uint8_t socketid);

/**
 * @brief   �ر�socket
 *
 * @param   socketid - socketIDֵ
 * @param   mode - socketΪTCP���ӣ�����Ϊ�رյķ�ʽ @TCP�رղ���
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketClose( uint8_t socketid, uint8_t mode );

/**
 * @brief   �޸Ľ��ջ�����
 *
 * @param   socketid - socketIDֵ
 * @param   bufaddr - ���ջ�������ַ
 * @param   bufsize - ���ջ�������С
 *
 * @return  None
 */
void WCHNET_ModifyRecvBuf( uint8_t socketid, uint32_t bufaddr, uint32_t bufsize);

/**
 * @brief   UDP���ͣ�ָ��Ŀ��IP��Ŀ�Ķ˿�
 *
 * @param       socketid - socketIDֵ
 * @param       *buf - �������ݵĵ�ַ
 * @param(in)   *slen - �������͵ĳ��ȵ�ַ
 * @param       *sip - Ŀ��IP��ַ
 * @param       port - Ŀ�Ķ˿ں�
 *
 * @param(out)  *slen - ʵ�ʷ��͵ĳ���
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketUdpSendTo( uint8_t socketid, uint8_t *buf, uint32_t *slen, uint8_t *sip, uint16_t port);

/**
 * @brief   ASCII���ַת�����ַ
 *
 * @param       *cp - ��ת����ASCII���ַ
 * @param(in)   *addr - ת���������ַ��ŵ��ڴ��׵�ַ
 *
 * @param(out)  *addr - ת����������ַ
 *
 * @return  0:ת���ɹ�  1��ת��ʧ��
 */
uint8_t WCHNET_Aton(const char *cp, uint8_t *addr);

/**
 * @brief   �����ַתASCII��ַ
 *
 * @param   *ipaddr - socketIDֵ
 *
 * @return  ת�����ASCII��ַ
 */
uint8_t *WCHNET_Ntoa( uint8_t *ipaddr);

/**
 * @brief   ����socket��TTL
 *
 * @param   socketid - socketIDֵ
 * @param   ttl - TTLֵ
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SetSocketTTL( uint8_t socketid, uint8_t ttl);

/**
 * @brief   ��������TCP�ش�
 *
 * @param   socketid - socketIDֵ
 *
 * @return  None
 */
void WCHNET_RetrySendUnack( uint8_t socketid);

/**
 * @brief   ��ѯδ���ͳɹ������ݰ�
 *
 * @param       socketid - socketIDֵ
 * @param(in)   *addrlist - ��ŵ��ڴ��׵�ַ
 * @param       lislen - ��ŵ��ڴ��С
 *
 * @param(out)  *addrlist - δ���ͳɹ������ݰ���ַ�б�
 *
 * @return  δ���ͺ�δӦ������ݶεĸ���
 */
uint8_t WCHNET_QueryUnack( uint8_t socketid, uint32_t *addrlist, uint16_t lislen );

/**
 * @brief   DHCP����
 *
 * @param   dhcp - Ӧ�ò�ص�����
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_DHCPStart( dhcp_callback dhcp );

/**
 * @brief   DHCPֹͣ
 *
 * @param   None
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_DHCPStop( void );

/**
 * @brief   ����DHCP������
 *
 * @param   *name - DHCP�������׵�ַ
 *
 * @return  0�ɹ�������ʧ��
 */
uint8_t WCHNET_DHCPSetHostname(char *name);

/**
 * @brief   DNSֹͣ
 *
 * @param   None
 *
 * @return  None
 */
void WCHNET_InitDNS( uint8_t *dnsip, uint16_t port);

/**
 * @brief   Initialize the resolver: set up the UDP pcb and configure the default server
 *
 * @param   *dnsip - dns������IP��ַ
 * @param   port - dns�������˿ں�
 *
 * @return  None
 */
void WCHNET_DNSStop(void);

/**
 * Resolve a hostname (string) into an IP address.
 *
 * @param   hostname - the hostname that is to be queried
 * @param   addr     - pointer to a struct ip_addr where to store the address if it is already
 *                     cached in the dns_table (only valid if ERR_OK is returned!)
 * @param   found    - a callback function to be called on success, failure or timeout (only if
 *                     ERR_INPROGRESS is returned!)
 * @param   arg      - argument to pass to the callback function
 *
 * @return  @ERR_T
 *   WCHNET_ERR_SUCCESS if hostname is a valid IP address string or the host name is already in the local names table.
 *   ERR_INPROGRESS     enqueue a request to be sent to the DNS server for resolution if no errors are present.
 */
uint8_t WCHNET_HostNameGetIp( const char *hostname, uint8_t *addr, dns_callback found, void *arg );

/**
 * @brief   ���ÿ�KEEP LIVE����
 *
 * @param   *cfg - KEEPLIVE���ò���
 *
 * @return  None
 */
void WCHNET_ConfigKeepLive( struct _KEEP_CFG *cfg );

/**
 * @brief   ����socket KEEP LIVEʹ��
 *
 * @param   socketid - socketIDֵ
 * @param   enable - 1������  0���ر�
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketSetKeepLive( uint8_t socketid, uint8_t enable );

/**
 * @brief   PHY״̬�ı�����
 *
 * @param   phy_stat - PHY״ֵ̬
 *
 * @return  None
 */
void WCHNET_PhyStatus( uint32_t phy_stat );


#ifdef __cplusplus
}
#endif
#endif
