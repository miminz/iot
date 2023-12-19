#include <stdio.h>
#include <board.h>
#include "rtthread.h"
#include <flashdb.h>
#include "drivers/rtc.h"
#include "time.h"
#define FDB_LOG_TAG "FLASHDB"
/* key */
#define KEY_BOOT_COUNT  "boot_count"
#define KEY_DEVICE_ID   "device_id"
/* value */
static uint16_t boot_count = 0;
static char value_device_id[] = "54549921";
/* kvdb & tsdb */
struct fdb_kvdb kvdb = {0};
struct fdb_tsdb _global_tsdb = {0};
/*运行数据默认值*/
const rt_uint16_t baud_rate = 9600;
const rt_uint8_t IPAddrs[4] = {192, 168, 1, 10};
const rt_uint8_t GWIPAddrs[4] = {192, 168, 1, 1};
const rt_uint8_t IPMasks[4] = {255, 255, 255, 0};
const rt_uint8_t DESIPs[4] = {192, 168, 1, 232};
const rt_uint8_t BroadcastIPs[4] = {255, 255, 255, 255};
const rt_uint8_t DNSServerIPs[4] = {192, 168, 1, 1};
const rt_uint8_t runmode = 1;
const rt_uint16_t ListenPort = 1;
/* default KV nodes */
static struct fdb_default_kv_node default_kv_table[] = {
//    {"username", "armink", 0},                       /* string KV */
//    {"password", "123456", 0},                       /* string KV */
    {"boot_count", &boot_count, sizeof(boot_count)},      // 重启标志
    {"ListenPort", &ListenPort, sizeof(ListenPort)},      // 侦听端口

    {"runmode", &runmode, sizeof(runmode)},              // 通信工作模式
    {"baud_rate", &baud_rate, sizeof(baud_rate)},  // 波特率
    {"IPAddrs", &IPAddrs, sizeof(IPAddrs)},          // Ip地址
    {"GWIPAddr", &GWIPAddrs, sizeof(GWIPAddrs)},         // 网关
    {"IPMask", &IPMasks, sizeof(IPMasks)},           // 子网掩码
    {"DESIP", &DESIPs, sizeof(DESIPs)},            // 目的IP地址
    {"BroadcastIP", &BroadcastIPs, sizeof(BroadcastIPs)},      // 广播IP
    {"DNSServerIP", &DNSServerIPs, sizeof(DNSServerIPs)},      // DNS服务器
};

void kvdb_tarversal_sample(void)
{
    struct fdb_kv_iterator iterator;
    fdb_kv_t cur_kv;
    struct fdb_blob blob;
    size_t data_size;
    uint8_t *data_buf;

    fdb_kv_iterator_init(&kvdb, &iterator);

    while (fdb_kv_iterate(&kvdb, &iterator))
    {
        cur_kv = &(iterator.curr_kv);
        data_size = (size_t)cur_kv->value_len;
        data_buf = (uint8_t *)rt_malloc(data_size);
        if (data_buf == NULL)
        {
            FDB_INFO("Error: malloc failed.\n");
            break;
        }
        fdb_blob_read(&kvdb, fdb_kv_to_blob(cur_kv, fdb_blob_make(&blob, data_buf, data_size)));
        /*
         * balabala do what ever you like with blob...
         */
        if(cur_kv->value_len > 3){
            rt_kprintf("%s value is: ", cur_kv->name);
            for(uint8_t i=0;i<cur_kv->value_len;i++){
                rt_kprintf("%d.", data_buf[i]);}
            rt_kprintf("\n");
        }
        else
        {
            if(data_size == 1)
             rt_kprintf("%s = %d\n", cur_kv->name, data_buf);
            else if(data_size == 2){
                uint16_t k = data_buf[1]<<8;
                k |= data_buf[0];
                rt_kprintf("%s = %d\n", cur_kv->name, k);
            }
            else if (data_size == 4) {
                uint32_t k = data_buf[3]<<24;
                k = k | (data_buf[2]<<16);
                k = k | (data_buf[1]<<8);
                k = k | (data_buf[0]);
                rt_kprintf("%s = %d\n", cur_kv->name, k);
            }
        }
        rt_free(data_buf);
    }
    { /* GET the KV value */
        /* get the "boot_count" KV value */
        fdb_kv_get_blob(&kvdb, KEY_BOOT_COUNT, fdb_blob_make(&blob, &boot_count, sizeof(boot_count)));
        /* the blob.saved.len is more than 0 when get the value successful */
        if (blob.saved.len > 0) {
            FDB_INFO("get the 'boot_count' value is %d\n", boot_count);
        } else {
            FDB_INFO("get the 'boot_count' failed\n");
        }
    }
    { /* CHANGE the KV value */
        /* increase the boot count */
        boot_count ++;
        /* change the "boot_count" KV's value */
        fdb_kv_set_blob(&kvdb, KEY_BOOT_COUNT, fdb_blob_make(&blob, &boot_count, sizeof(boot_count)));
        FDB_INFO("set the 'boot_count' value to %d\n", boot_count);
    }
}
static fdb_time_t get_time(void)
{
    return (fdb_time_t)time(RT_NULL);
}
static int print_system_info(void)
{
    uint8_t ip[4];
    struct fdb_blob blob;
    { /* GET the KV value */
        /* get the "device_id" KV value */
        fdb_kv_get_blob(&kvdb, KEY_DEVICE_ID, fdb_blob_make(&blob, value_device_id, sizeof(value_device_id)));
        /* the blob.saved.len is more than 0 when get the value successful */
        if (blob.saved.len > 0) {
            FDB_INFO("get the 'device_id' value is %s\n", value_device_id);
        } else {
            FDB_INFO("get the 'device_id' failed\n");
            { /* CHANGE the KV value */
                /* change the "boot_count" KV's value */
                fdb_kv_set_blob(&kvdb, KEY_DEVICE_ID, fdb_blob_make(&blob, value_device_id, sizeof(value_device_id)));
                FDB_INFO("set the 'device_id' value to %s\n", value_device_id);
            }
        }
    }
    { /* GET the KV value */
        /* get the "boot_count" KV value */
        fdb_kv_get_blob(&kvdb, KEY_BOOT_COUNT, fdb_blob_make(&blob, &boot_count, sizeof(boot_count)));
        /* the blob.saved.len is more than 0 when get the value successful */
        if (blob.saved.len > 0) {
            FDB_INFO("get the 'boot_count' value is %d\n", boot_count);
        } else {
            FDB_INFO("get the 'boot_count' failed\n");
        }
    }
    { /* CHANGE the KV value */
        /* increase the boot count */
        boot_count ++;
        /* change the "boot_count" KV's value */
        fdb_kv_set_blob(&kvdb, KEY_BOOT_COUNT, fdb_blob_make(&blob, &boot_count, sizeof(boot_count)));
        FDB_INFO("set the 'boot_count' value to %d\n", boot_count);
    }
    { /* GET the KV value */
        /* get the "boot_count" KV value */
        fdb_kv_get_blob(&kvdb, "IPAddrs", fdb_blob_make(&blob, &ip, sizeof(ip)));
        /* the blob.saved.len is more than 0 when get the value successful */
        if (blob.saved.len > 0) {
            FDB_INFO("get the 'ip' value is %d.%d.%d.%d\n", ip[0],ip[1],ip[2],ip[3]);
        } else {
            FDB_INFO("get the 'ip' failed\n");
        }
    }
    FDB_INFO("===========================================================\n");
}
static int datbase_init(void)
{
    fdb_err_t result;
    struct fdb_default_kv default_kv;
    default_kv.kvs = default_kv_table;
    default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);
    //init函数的第三个参数,是用于存储数据的fal分区,根据自己的情况选择
    result = fdb_kvdb_init(&kvdb, "envfd", "runenv", &default_kv, NULL);
    if (result != FDB_NO_ERR)
    {
        return -RT_ERROR;
    }
    else
    {
       // print_system_info();
        kvdb_tarversal_sample();
    }
    // result = fdb_tsdb_init(&_global_tsdb, "storage_data", "flashts", get_time, 128, RT_NULL);
    // if (result != FDB_NO_ERR)
    // {
    //     return -RT_ERROR;
    // }
    // else
    // {
    //     extern void tsdb_sample(fdb_tsdb_t tsdb);
    //     tsdb_sample(&_global_tsdb);
    //     FDB_INFO("init tsdb success.\n");
    // }
    return RT_EOK;
}
void flasha(void){
    kvdb_tarversal_sample();
//    fdb_kv_set_default(&kvdb);
//    kvdb_tarversal_sample();

}
INIT_ENV_EXPORT(datbase_init);

MSH_CMD_EXPORT(flasha, flashdb test);
