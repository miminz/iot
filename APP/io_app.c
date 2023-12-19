#include "modbus_app.h"
#include <board.h>
#include <agile_modbus.h>
#include "drivers/pin.h"
#include <rtdbg.h>
#include "ch32v30x_gpio.h"
#include "fal.h"
#include "io_app.h"
#include "flashdb.h"
ALIGN(RT_ALIGN_SIZE)
static char io_task_stack[128];
static struct rt_thread io_task_thread;
#define FDB_LOG_TAG "[main]"



/* Global Variable */
struct fal_partition *part_dev = NULL;
uint16_t io_data = 0;

void init_io(void)
{

    int result;
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    if ((part_dev = fal_partition_find("rundata")) != NULL)
    {
        rt_kprintf("Probed a flash partition | %s | flash_dev: %s | offset: %ld | len: %d |.\n",
                   part_dev->name, part_dev->flash_name, part_dev->offset, part_dev->len);
    }
    uint8_t *data = rt_malloc(2);
    result = fal_partition_read(part_dev, IO_EDATA, data, 2);
    io_data = *((uint16_t *)data);
    GPIO_Write(GPIOD, io_data);
    rt_free(data);
    PVD_Config();


}

void PVD_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};

    /* Enable PWR and BKP clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* 使能 PVD 中断 */
    NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 配置 EXTI16线(PVD 输出) 来产生上升下降沿中断*/
    EXTI_ClearITPendingBit(EXTI_Line16);
    EXTI_InitStructure.EXTI_Line = EXTI_Line16;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* PVD电压监视阈值设置 */
    /*具体级别根据自己的实际应用要求配置*/
    PWR_PVDLevelConfig(PWR_PVDLevel_2V7);

    /* 使能PVD输出 */
    PWR_PVDCmd(ENABLE);
}



// int flasha(void)
// {
//     extern uint32_t RTC_GetCounter(void);
//     fdb_err_t result;

// #ifdef FDB_USING_KVDB
//     { /* KVDB Sample */
//         struct fdb_default_kv default_kv;
//         struct fdb_blob blob;
//         default_kv.kvs = default_kv_table;
//         default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);
//         /* set the lock and unlock function if you want */
//         //fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_LOCK, (void *)lock);
//         //fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_UNLOCK, (void *)unlock);

//         result = fdb_kvdb_init(&kvdb, "env", "runenv", &default_kv, NULL);

//         if (result != FDB_NO_ERR)
//         {
//             return -1;
//         }
//         fdb_kv_get_blob(&kvdb, "baud_ratse", fdb_blob_make(&blob, &boot_count, sizeof(boot_count)));

//         /* the blob.saved.len is more than 0 when get the value successful */
//         if (blob.saved.len > 0)
//         {
//             FDB_INFO("get the 'boot_count' value is %d\n", boot_count);
//         }
//         else
//         {
//             FDB_INFO("get the 'boot_count' failed\n");
//             fdb_kv_set_default(&kvdb);
//         }
//         /* run basic KV samples */
//         kvdb_basic_sample(&kvdb);
//         /* run string KV samples */
//         kvdb_type_string_sample(&kvdb);
//         /* run blob KV samples */
//         kvdb_type_blob_sample(&kvdb);
//         kvdb_tarversal_sample(&kvdb);
//         fdb_kv_print(&kvdb);
//     }
// #endif /* FDB_USING_KVDB */

//     // #ifdef 0//FDB_USING_TSDB
//     //     { /* TSDB Sample */
//     //         /* set the lock and unlock function if you want */
//     //         fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_LOCK, (void *)lock);
//     //         fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_UNLOCK, (void *)unlock);
//     //         /* Time series database initialization
//     //          *
//     //          *       &tsdb: database object
//     //          *       "log": database name
//     //          * "fdb_tsdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
//     //          *              Please change to YOUR partition name.
//     //          *    get_time: The get current timestamp function.
//     //          *         128: maximum length of each log
//     //          *        NULL: The user data if you need, now is empty.
//     //          */
//     //         result = fdb_tsdb_init(&tsdb, "log", "runlog", RTC_GetCounter, 128, NULL);
//     //         /* read last saved time for simulated timestamp */
//     //         fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_GET_LAST_TIME, &counts);
//     //
//     //         if (result != FDB_NO_ERR) {
//     //             return -1;
//     //         }
//     //
//     //         /* run TSDB sample */
//     //         tsdb_sample(&tsdb);
//     //     }
//     // #endif /* FDB_USING_TSDB */

//     return 0;
// }
// MSH_CMD_EXPORT(flasha, flashdb test);
