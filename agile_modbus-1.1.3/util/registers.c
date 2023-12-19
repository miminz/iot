#include <stdint.h>
#include <string.h>
#include "agile_modbus.h"
#include "agile_modbus_slave_util.h"
#include <flashdb.h>
#include "io_app.h"
static uint16_t _tab_registers[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
extern struct fdb_kvdb kvdb;
static int get_map_buf(int index, int len, void *buf, int bufsz)
{
    uint16_t *ptr = (uint16_t *)buf;
    struct fdb_blob blob;
    // for (int i = 0; i < sizeof(_tab_registers) / sizeof(_tab_registers[0]); i++)
    // {
    //     ptr[i] = _tab_registers[i];
    // }
    rt_uint8_t *data = rt_malloc(12);
    fdb_kv_get_blob(&kvdb, "IPAddrs", fdb_blob_make(&blob, data, 4));
    fdb_kv_get_blob(&kvdb, "IPMask", fdb_blob_make(&blob, data+4, 4));
    fdb_kv_get_blob(&kvdb, "GWIPAddr", fdb_blob_make(&blob, data+8, 4));
    for(int i=0; i<12; i++)
    {
        ptr[i]=data[i];
    }
   // rt_memcpy(ptr,data,12);
    rt_free(data);
    return 0;
}

static int set_map_buf(int index, int len, void *buf, int bufsz)
{
    uint16_t *ptr = (uint16_t *)buf;

//    for (int i = 0; i < len; i++)
//    {
//        _tab_registers[index + i] = ptr[index + i];
//    }

    struct fdb_blob blob;
    switch (index)
    {
    case 0 /* constant-expression */: // 工作模式
        if (len == 12)
        {
            uint8_t data[4];
            for (uint8_t i = 0; i < 4; i++)
            {
                data[i] = *ptr & 0xff;
                ptr++;
            }
            fdb_kv_set_blob(&kvdb, "IPAddrs", fdb_blob_make(&blob, &data, sizeof(data)));
            rt_kprintf("set ip:%d.%d.%d.%d\n", data[0], data[1], data[2], data[3]);
            for (uint8_t i = 0; i < 4; i++)
            {
                data[i] = *ptr & 0xff;
                ptr++;
            }
            fdb_kv_set_blob(&kvdb, "IPMask", fdb_blob_make(&blob, &data, sizeof(data)));
            rt_kprintf("set IPMask:%d.%d.%d.%d\n", data[0], data[1], data[2], data[3]);
            for (uint8_t i = 0; i < 4; i++)
            {
                data[i] = *ptr & 0xff;
                ptr++;
            }
            fdb_kv_set_blob(&kvdb, "GWIPAddr", fdb_blob_make(&blob, &data, sizeof(data)));
            rt_kprintf("set GWIPAddr:%d.%d.%d.%d\n", data[0], data[1], data[2], data[3]);
        }
        break;

    default:
        break;
    }

    return 0;
}

const agile_modbus_slave_util_map_t register_maps[1] = {
    {0x00, 0x64, get_map_buf, set_map_buf}};
