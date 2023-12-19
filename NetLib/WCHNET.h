/********************************** (C) COPYRIGHT *******************************
* File Name        : WCHNET.H
* Author           : WCH
* Version          : V1.20
* Date             : 2022/05/07
* Description      : 以太网协议栈库头文件
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

#define WCHNET_LIB_VER                  0x12 /* 库版本号 */
#define WCHNET_CFG_VALID                0x12345678 /* 配置值有效标志 */

/* LED状态 @LED_STAT */
#define  LED_ON                         0
#define  LED_OFF                        1

/* PHY 状态 @PHY_STAT */
#define PHY_LINK_SUCCESS                (1 << 2) /* PHY建立连接 */
#define PHY_AUTO_SUCCESS                (1 << 5) /* PHY自动协商完成 */

/* 库初始化状态 @CFG_INIT_STAT */
#define  INIT_OK                        0x00
#define  INIT_ERR_RX_BUF_SIZE           0x01
#define  INIT_ERR_TCP_MSS               0x02
#define  INIT_ERR_HEAP_SIZE             0x03
#define  INIT_ERR_ARP_TABLE_NEM         0x04
#define  INIT_ERR_MISC_CONFIG0          0x05
#define  INIT_ERR_MISC_CONFIG1          0x06
#define  INIT_ERR_FUNC_SEND             0x09
#define  INIT_ERR_CHECK_VALID           0xFF

/* Socket 工作模式定义,协议类型 */
#define PROTO_TYPE_IP_RAW               0     /* IP层原始数据 */
#define PROTO_TYPE_UDP                  2     /* UDP协议类型 */
#define PROTO_TYPE_TCP                  3     /* TCP协议类型 */

/* 中断状态 */
/* 以下为 GLOB_INT 会产生的状态 */
#define GINT_STAT_UNREACH               (1 << 0) /* 不可达中断*/
#define GINT_STAT_IP_CONFLI             (1 << 1) /* IP冲突*/
#define GINT_STAT_PHY_CHANGE            (1 << 2) /* PHY状态改变 */
#define GINT_STAT_SOCKET                (1 << 4) /* socket 产生中断 */

/*以下为 Sn_INT 会产生的状态*/
#define SINT_STAT_RECV                  (1 << 2) /* socket端口接收到数据或者接收缓冲区不为空 */
#define SINT_STAT_CONNECT               (1 << 3) /* 连接成功,TCP模式下产生此中断 */
#define SINT_STAT_DISCONNECT            (1 << 4) /* 连接断开,TCP模式下产生此中断 */
#define SINT_STAT_TIM_OUT               (1 << 6) /* ARP和TCP模式下会发生此中断 */


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

/* 不可达代码 */
#define UNREACH_CODE_HOST               0     /* 主机不可达 */
#define UNREACH_CODE_NET                1     /* 网络不可达 */
#define UNREACH_CODE_PROTOCOL           2     /* 协议不可达 */
#define UNREACH_CODE_PROT               3     /* 端口不可达 */
/*其他值请参考RFC792文档*/

/* TCP关闭参数 */
#define TCP_CLOSE_NORMAL                0     /* 正常关闭，进行4此握手 */
#define TCP_CLOSE_RST                   1     /* 复位连接，并关闭  */
#define TCP_CLOSE_ABANDON               2     /* 内部丢弃连接，不会发送任何终止报文 */

/* socket状态 */
#define SOCK_STAT_CLOSED                0X00  /* socket关闭 */
#define SOCK_STAT_OPEN                  0X05  /* socket打开 */

/* TCP状态 */
#define TCP_CLOSED                      0     /* TCP关闭 */
#define TCP_LISTEN                      1     /* TCP监听 */
#define TCP_SYN_SENT                    2     /* SYN发送，连接请求 */
#define TCP_SYN_RCVD                    3     /* SYN接收，接收到连接请求 */
#define TCP_ESTABLISHED                 4     /* TCP连接建立 */
#define TCP_FIN_WAIT_1                  5     /* WAIT_1状态 */
#define TCP_FIN_WAIT_2                  6     /* WAIT_2状态 */
#define TCP_CLOSE_WAIT                  7     /* 等待关闭 */
#define TCP_CLOSING                     8     /* 正在关闭 */
#define TCP_LAST_ACK                    9     /* LAST_ACK*/
#define TCP_TIME_WAIT                   10    /* 2MSL等待 */

/* 以下值为固定值不可以更改 */
#define WCHNET_MEM_ALIGN_SIZE(size)    (((size) + WCHNET_MEM_ALIGNMENT - 1) & ~(WCHNET_MEM_ALIGNMENT - 1))
#define WCHNET_SIZE_IPRAW_PCB          0xFF   /* IPRAW PCB大小 */
#define WCHNET_SIZE_UDP_PCB            0x30   /* UDP PCB大小 */
#define WCHNET_SIZE_TCP_PCB            0xFF   /* TCP PCB大小 */
#define WCHNET_SIZE_TCP_PCB_LISTEN     0x40   /* TCP LISTEN PCB大小 */
#define WCHNET_SIZE_IP_REASSDATA       0x40   /* IP分片管理  */
#define WCHNET_SIZE_PBUF               0x40   /* Packet Buf */
#define WCHNET_SIZE_TCP_SEG            0x60   /* TCP SEG结构 */
#define WCHNET_SIZE_MEM                0x0c   /* sizeof(struct mem) */
#define WCHNET_SIZE_ARP_TABLE          0x20   /* sizeof ARP table */

#define WCHNET_SIZE_POOL_BUF           WCHNET_MEM_ALIGN_SIZE(WCHNET_TCP_MSS + 40 + 14) /* pbuf池大小 */
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

/* sokcet信息表 */
typedef struct _SCOK_INF
{
    uint32_t IntStatus;                       /* 中断状态 */
    uint32_t SockIndex;                       /* Socket索引值 */
    uint32_t RecvStartPoint;                  /* 接收缓冲区的开始指针 */
    uint32_t RecvBufLen;                      /* 接收缓冲区长度 */
    uint32_t RecvCurPoint;                    /* 接收缓冲区的当前指针 */
    uint32_t RecvReadPoint;                   /* 接收缓冲区的读指针 */
    uint32_t RecvRemLen;                      /* 接收缓冲区的剩余长度 */
    uint32_t ProtoType;                       /* 协议类型 */
    uint32_t ScokStatus;                      /* 低字节Socket状态，次低字节为TCP状态，仅TCP模式下有意义 */
    uint32_t DesPort;                         /* 目的端口 */
    uint32_t SourPort;                        /* 源端口在IPRAW模式下为协议类型 */
    uint8_t  IPAddr[4];                       /* Socket目标IP地址 32bit*/
    void *Resv1;                              /* 保留，内部使用，用于保存各个PCB */
    void *Resv2;                              /* 保留，内部使用，TCP Server使用 */
    pSockRecv AppCallBack;                    /* 接收回调函数*/
} SOCK_INF;

struct _WCH_CFG
{
  uint32_t TxBufSize;   /* MAC发送缓冲区大小，保留使用 */
  uint32_t TCPMss;      /* TCP MSS大小 */
  uint32_t HeapSize;    /* 堆分配内存大小 */
  uint32_t ARPTableNum; /* ARP列表个数 */
  uint32_t MiscConfig0; /* 杂项配置0 */
  /* 位0     TCP发送缓冲区复制 1：复制，0：不复制 */
  /* 位1     TCP接收复制优化，内部调试使用 */
  /* 位2     删除最早的TCP连接 1：启用，0：禁用  */
  /* 位3-7   IP分片的PBUF个数  */
  uint32_t MiscConfig1; /* 杂项配置1 */
  /* 位0-7   Socket的个数 */
  /* 位8-12  保留 */
  /* 位13    PING使能，1：开启  0：关闭  */
  /* 位14-18 TCP重传次数  */
  /* 位19-23 TCP重传周期，单位为50毫秒  */
  /* 位25    发送失败重试，1：启用，0：禁用 */
  /* 位26-31 保留 */
  led_callback led_link; /* PHY Link状态指示灯 */
  led_callback led_data; /* 以太网通信指示灯 */
  eth_tx_set net_send; /* 以太网发送 */
  eth_rx_set net_recv; /* 以太网接收 */
  uint32_t   CheckValid;/* 配置值有效标志，固定值 @WCHNET_CFG_VALID */
};

struct _NET_SYS
{
  uint8_t  IPAddr[4];        /* IP地址 32bit */
  uint8_t  GWIPAddr[4];      /* 网关地址 32bit */
  uint8_t  MASKAddr[4];      /* 子网掩码 32bit */
  uint8_t  MacAddr[8];       /* MAC地址 48bit */
  uint8_t  UnreachIPAddr[4]; /* 不可到达IP */
  uint32_t RetranCount;      /* 重试次数 默认为10次 */
  uint32_t RetranPeriod;     /* 重试周期,单位MS,默认500MS */
  uint32_t PHYStat;          /* PHY状态码 8bit */
  uint32_t NetStat;          /* 以太网的状态 ，包含是否打开等 */
  uint32_t MackFilt;         /*  MAC过滤，默认为接收广播，接收本机MAC 8bit */
  uint32_t GlobIntStatus;    /* 全局中断 */
  uint32_t UnreachCode;      /* 不可达 */
  uint32_t UnreachProto;     /* 不可达协议 */
  uint32_t UnreachPort;      /* 不可到达端口 */
  uint32_t SendFlag;
  uint32_t Flags;
};

/* KEEP LIVE配置结构体 */
struct _KEEP_CFG
{
  uint32_t KLIdle;  /* KEEPLIVE空闲时间，单位ms */
  uint32_t KLIntvl; /* KEEPLIVE周期，单位ms */
  uint32_t KLCount; /* KEEPLIVE次数 */
};

/**
 * @brief   库初始化.
 *
 * @param   ip - IP地址
 * @param   gwip - 网关
 * @param   mask - 子网掩码
 * @param   macaddr - MAC地址
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_Init(const uint8_t *ip, const uint8_t *gwip, const uint8_t *mask, const uint8_t *macaddr);

/**
 * @brief   获取版本号
 *
 * @param   None
 *
 * @return  库版本
 */
uint8_t WCHNET_GetVer(void);

/**
 * @brief   获取MAC地址
 *
 * @param(in)   macaddr - 内存地址
 *
 * @param(out)  mac地址
 *
 * @return  None
 */
void WCHNET_GetMacAddr(uint8_t *macaddr);

/**
 * @brief   库参数配置
 *
 * @param   cfg -配置参数 @_WCH_CFG
 *
 * @return  库配置初始化状态 @CFG_INIT_STAT
 */
uint8_t WCHNET_ConfigLIB(struct _WCH_CFG *cfg);

/**
 * @brief   库主任务函数，需要一直不断调用
 *
 * @param   None
 *
 * @return  None
 */
void WCHNET_MainTask(void);

/**
 * @brief   时钟中断服务函数，需配置时钟周期
 *
 * @param   timperiod - 时钟周期，单位ms
 *
 * @return  None
 */
void WCHNET_TimeIsr( uint16_t timperiod);

/**
 * @brief   以太网中断服务函数，产生以太网中断后调用
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
 * @return  PHY 状态 @PHY_STAT
 */
uint8_t WCHNET_GetPHYStatus(void);

/**
 * @brief   查询全局中断
 *
 * @param   None
 *
 * @return  GLOB_INT
 */
uint8_t WCHNET_QueryGlobalInt(void);

/**
 * @brief   读全局中断并将全局中断清零
 *
 * @param   None
 *
 * @return  GLOB_INT
 */
uint8_t WCHNET_GetGlobalInt(void);

/**
 * @brief   创建socket
 *
 * @param(in)   *socketid - 内存地址
 * @param       socinf - 创建socket的配置参数 @SOCK_INF
 *
 * @param(out)  *socketid - socketID值
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketCreat( uint8_t *socketid, SOCK_INF *socinf);

/**
 * @brief   Socket发送数据
 *
 * @param       socketid - socketID值
 * @param       *buf - 发送数据首地址
 * @param(in)   *len - 内存地址及期望发送的数据长度
 *
 * @param(out)  *len - 实际发送的数据长度
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketSend( uint8_t socketid, uint8_t *buf, uint32_t *len);

/**
 * @brief   Socket接收数据
 *
 * @param       socketid - socketID值
 * @param       *buf - 接收数据首地址
 * @param(in)   *len - 内存地址及期望读取的数据长度
 *
 * @param(out)  *buf - 写入读取到的数据内容
 * @param(out)  *len - 实际读取的数据长度
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketRecv( uint8_t socketid, uint8_t *buf, uint32_t *len);

/**
 * @brief   获取socket中断并清零
 *
 * @param   socketid - socketID值
 *
 * @return  Sn_INT
 */
uint8_t WCHNET_GetSocketInt( uint8_t socketid );

/**
 * @brief   获取socket接收长度
 *
 * @param       socketid - socketID值
 * @param(in)   *bufaddr - 内存地址
 *
 * @param(out)  *bufaddr - socket数据的首地址
 *
 * @return  数据长度
 */
uint32_t WCHNET_SocketRecvLen( uint8_t socketid, uint32_t *bufaddr);

/**
 * @brief   TCP连接
 *
 * @param   socketid - socketID值
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketConnect( uint8_t socketid);

/**
 * @brief   TCP监听
 *
 * @param   socketid - socketID值
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketListen( uint8_t socketid);

/**
 * @brief   关闭socket
 *
 * @param   socketid - socketID值
 * @param   mode - socket为TCP连接，参数为关闭的方式 @TCP关闭参数
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketClose( uint8_t socketid, uint8_t mode );

/**
 * @brief   修改接收缓冲区
 *
 * @param   socketid - socketID值
 * @param   bufaddr - 接收缓冲区地址
 * @param   bufsize - 接收缓冲区大小
 *
 * @return  None
 */
void WCHNET_ModifyRecvBuf( uint8_t socketid, uint32_t bufaddr, uint32_t bufsize);

/**
 * @brief   UDP发送，指定目的IP、目的端口
 *
 * @param       socketid - socketID值
 * @param       *buf - 发送数据的地址
 * @param(in)   *slen - 期望发送的长度地址
 * @param       *sip - 目的IP地址
 * @param       port - 目的端口号
 *
 * @param(out)  *slen - 实际发送的长度
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketUdpSendTo( uint8_t socketid, uint8_t *buf, uint32_t *slen, uint8_t *sip, uint16_t port);

/**
 * @brief   ASCII码地址转网络地址
 *
 * @param       *cp - 需转换的ASCII码地址
 * @param(in)   *addr - 转换后网络地址存放的内存首地址
 *
 * @param(out)  *addr - 转换后的网络地址
 *
 * @return  0:转换成功  1：转换失败
 */
uint8_t WCHNET_Aton(const char *cp, uint8_t *addr);

/**
 * @brief   网络地址转ASCII地址
 *
 * @param   *ipaddr - socketID值
 *
 * @return  转换后的ASCII地址
 */
uint8_t *WCHNET_Ntoa( uint8_t *ipaddr);

/**
 * @brief   设置socket的TTL
 *
 * @param   socketid - socketID值
 * @param   ttl - TTL值
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SetSocketTTL( uint8_t socketid, uint8_t ttl);

/**
 * @brief   立即启动TCP重传
 *
 * @param   socketid - socketID值
 *
 * @return  None
 */
void WCHNET_RetrySendUnack( uint8_t socketid);

/**
 * @brief   查询未发送成功的数据包
 *
 * @param       socketid - socketID值
 * @param(in)   *addrlist - 存放的内存首地址
 * @param       lislen - 存放的内存大小
 *
 * @param(out)  *addrlist - 未发送成功的数据包地址列表
 *
 * @return  未发送和未应答的数据段的个数
 */
uint8_t WCHNET_QueryUnack( uint8_t socketid, uint32_t *addrlist, uint16_t lislen );

/**
 * @brief   DHCP启动
 *
 * @param   dhcp - 应用层回调函数
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_DHCPStart( dhcp_callback dhcp );

/**
 * @brief   DHCP停止
 *
 * @param   None
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_DHCPStop( void );

/**
 * @brief   配置DHCP主机名
 *
 * @param   *name - DHCP主机名首地址
 *
 * @return  0成功，否则失败
 */
uint8_t WCHNET_DHCPSetHostname(char *name);

/**
 * @brief   DNS停止
 *
 * @param   None
 *
 * @return  None
 */
void WCHNET_InitDNS( uint8_t *dnsip, uint16_t port);

/**
 * @brief   Initialize the resolver: set up the UDP pcb and configure the default server
 *
 * @param   *dnsip - dns服务器IP地址
 * @param   port - dns服务器端口号
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
 * @brief   配置库KEEP LIVE参数
 *
 * @param   *cfg - KEEPLIVE配置参数
 *
 * @return  None
 */
void WCHNET_ConfigKeepLive( struct _KEEP_CFG *cfg );

/**
 * @brief   配置socket KEEP LIVE使能
 *
 * @param   socketid - socketID值
 * @param   enable - 1：启用  0：关闭
 *
 * @return  @ERR_T
 */
uint8_t WCHNET_SocketSetKeepLive( uint8_t socketid, uint8_t enable );

/**
 * @brief   PHY状态改变配置
 *
 * @param   phy_stat - PHY状态值
 *
 * @return  None
 */
void WCHNET_PhyStatus( uint32_t phy_stat );


#ifdef __cplusplus
}
#endif
#endif
