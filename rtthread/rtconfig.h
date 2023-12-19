/* RT-Thread config file */

#ifndef __RTTHREAD_CFG_H__
#define __RTTHREAD_CFG_H__


// <o>Maximal level of thread priority <8-256>
//  <i>Default: 16
#define RT_THREAD_PRIORITY_MAX  16

// <o>OS tick per second
//100us
#define RT_TICK_PER_SECOND  10000

// <o>Alignment size for CPU architecture data access
//  <i>Default: 4
#define RT_ALIGN_SIZE   4

// <o>the max length of object name<2-16>
//  <i>Default: 8
#define RT_NAME_MAX    8

//  <i>Using RT-Thread components initialization
#define RT_USING_COMPONENTS_INIT

#define RT_USING_USER_MAIN
// <o>the stack size of main thread<1-4086>
//  <i>Default: 512
#define RT_MAIN_THREAD_STACK_SIZE     1024*2

// Debug Configuration
//#define RT_DEBUG

//  <i>using hook
#define RT_USING_HOOK

//  <i>using idle hook
#define RT_USING_IDLE_HOOK

// <e>Software timers Configuration
// <i> Enables user timers
#define RT_USING_TIMER_SOFT         0
#if RT_USING_TIMER_SOFT == 0
    #undef RT_USING_TIMER_SOFT
#endif

// <o>The priority level of timer thread <0-31>
//  <i>Default: 4
#define RT_TIMER_THREAD_PRIO        4

// <o>The stack size of timer thread <0-8192>
//  <i>Default: 512
#define RT_TIMER_THREAD_STACK_SIZE  512

//  <i>Using Semaphore
#define RT_USING_SEMAPHORE
//  <i>Using Mutex
#define RT_USING_MUTEX
//  <i>Using Event
#define RT_USING_EVENT
//  <i>Using MailBox
#define RT_USING_MAILBOX
//  <i>Using Message Queue
#define RT_USING_MESSAGEQUEUE


// <h>Memory Management Configuration
// <c1>Dynamic Heap Management
//  <i>Dynamic Heap Management
//#define RT_USING_MEMPOOL
//#define RT_USING_MEMHEAP
#define RT_USING_SMALL_MEM
#define RT_USING_HEAP


// </c>
// <c1>using tiny size of memory
//  <i>using tiny size of memory
//#define RT_USING_TINY_SIZE
// </c>
// </h>

/* Kernel Device Object */

#define RT_USING_DEVICE
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 128
#define RT_CONSOLE_DEVICE_NAME  "uart1"


#define RT_USING_FINSH
#if defined(RT_USING_FINSH)
    #define FINSH_USING_MSH
    #define FINSH_USING_MSH_DEFAULT
    #define FINSH_USING_MSH_ONLY
    #define FINSH_THREAD_NAME "tshell"

    #define __FINSH_THREAD_PRIORITY     5
    #define FINSH_THREAD_PRIORITY       (RT_THREAD_PRIORITY_MAX / 8 * __FINSH_THREAD_PRIORITY + 1)
    // <o>the stack of finsh thread <1-4096>
    //  <i>the stack of finsh thread
    //  <i>Default: 4096  (4096Byte)
    #define FINSH_THREAD_STACK_SIZE     4096

    #define FINSH_USING_HISTORY
    #define FINSH_HISTORY_LINES         5
    #define FINSH_USING_SYMTAB
    #define FINSH_USING_DESCRIPTION
    #define FINSH_CMD_SIZE 80
#endif

// <<< end of configuration section >>>
#define RT_USING_DEVICE_IPC

/* �����ӿڣ������ʹ�ã�ע�ͺ�����Ҫ��components/drivers�¶�Ӧ�������������?  */
#define RT_USING_SERIAL
#define RT_USING_ADC
#define RT_USING_DAC
#define RT_USING_PIN
#define RT_USING_SPI


/* �ײ�Ӳ������ʹ��,�����ʹ�ÿ���ע�����º꣬��Ӧ�ĸ�Ŀ¼drivers�µ�Ӳ���������ݽ���������
 * ȥ��������ܺ󣬿ɽ�ʹ��RTOS�ںˣ�������Լ���ʼ����ʹ��? */
#define BSP_USING_UART1
#define BSP_USING_UART2
#define BSP_USING_UART3
//#define BSP_USING_UART4
//#define BSP_USING_UART5
//#define BSP_USING_UART6
//#define BSP_USING_UART7
//#define BSP_USING_UART8

#define BSP_USING_ADC1
#define BSP_USING_DAC_CHANNEL1

#define BSP_USING_SPI1
#define RT_USING_SFUD  //ʹ��SFUD
#define RT_SFUD_USING_SFDP
#define RT_SFUD_USING_FLASH_INFO_TABLE
#define RT_SFUD_SPI_MAX_HZ 1000000
#define BSP_USING_SPI_FLASH
#define BSP_USING_ONCHIP_RTC

#define FAL_USING_SFUD_PORT

#define RT_VER_NUM //fal开�?

#endif
