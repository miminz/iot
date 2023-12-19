/**
 * @file    agile_modbus_slave_util.c
 * @brief   Agile Modbus 软件包提供的简易从机接入源文件
 * @author  马龙伟 (2544047213@qq.com)
 * @date    2022-07-28
 *
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 Ma Longwei.
 * All rights reserved.</center></h2>
 *
 */

#include "agile_modbus.h"
#include "agile_modbus_slave_util.h"
#include <string.h>
#include "fal.h"
#include "rtthread.h"

            extern const uint8_t _table_crc_lo[];
            extern const uint8_t _table_crc_hi[];
/** @addtogroup UTIL
 * @{
 */

/** @defgroup SLAVE_UTIL Slave Util
 * @{
 */

/** @defgroup SLAVE_UTIL_Private_Functions Slave Util Private Functions
 * @{
 */

/**
 * @brief   根据寄存器地址从映射对象数组中获取映射对象
 * @param   maps 映射对象数组
 * @param   nb_maps 数组数目
 * @param   address 寄存器地址
 * @return  !=NULL:映射对象; =NULL:失败
 */
static const agile_modbus_slave_util_map_t *get_map_by_addr(const agile_modbus_slave_util_map_t *maps, int nb_maps, int address)
{
    for (int i = 0; i < nb_maps; i++) {
        const agile_modbus_slave_util_map_t *map = &maps[i];
        if (address >= map->start_addr && address <= map->end_addr)
            return map;
    }

    return NULL;
}

/**
 * @brief   读取寄存器
 * @param   ctx modbus 句柄
 * @param   slave_info 从机信息体
 * @param   slave_util 从机功能结构体
 * @return  =0:正常;
 *          <0:异常
 *             (-AGILE_MODBUS_EXCEPTION_UNKNOW(-255): 未知异常，从机不会打包响应数据)
 *             (其他负数异常码: 从机会打包异常响应数据)
 */
static int read_registers(agile_modbus_t *ctx, struct agile_modbus_slave_info *slave_info, const agile_modbus_slave_util_t *slave_util)
{
    uint8_t map_buf[AGILE_MODBUS_MAX_PDU_LENGTH];
    int function = slave_info->sft->function;
    int address = slave_info->address;
    int nb = slave_info->nb;
    int send_index = slave_info->send_index;
    const agile_modbus_slave_util_map_t *maps = NULL;
    int nb_maps = 0;

    switch (function) {
    case AGILE_MODBUS_FC_READ_COILS: {
        maps = slave_util->tab_bits;
        nb_maps = slave_util->nb_bits;
    } break;

    case AGILE_MODBUS_FC_READ_DISCRETE_INPUTS: {
        maps = slave_util->tab_input_bits;
        nb_maps = slave_util->nb_input_bits;
    } break;

    case AGILE_MODBUS_FC_READ_HOLDING_REGISTERS: {
        maps = slave_util->tab_registers;
        nb_maps = slave_util->nb_registers;
    } break;

    case AGILE_MODBUS_FC_READ_INPUT_REGISTERS: {
        maps = slave_util->tab_input_registers;
        nb_maps = slave_util->nb_input_registers;
    } break;

    default:
        return -AGILE_MODBUS_EXCEPTION_ILLEGAL_FUNCTION;
    }

    if (maps == NULL)
        return 0;

    for (int now_address = address, i = 0; now_address < address + nb; now_address++, i++) {
        const agile_modbus_slave_util_map_t *map = get_map_by_addr(maps, nb_maps, now_address);
        if (map == NULL)
            continue;

        int map_len = map->end_addr - now_address + 1;
        if (map->get) {
            memset(map_buf, 0, sizeof(map_buf));
            int index = now_address - map->start_addr;
            int need_len = address + nb - now_address;
            map->get(index, need_len, map_buf, sizeof(map_buf));

            if (need_len > map_len) {
                need_len = map_len;
            }

            if (function == AGILE_MODBUS_FC_READ_COILS || function == AGILE_MODBUS_FC_READ_DISCRETE_INPUTS) {
                uint8_t *ptr = map_buf;
                for (int j = 0; j < need_len; j++) {
                    agile_modbus_slave_io_set(ctx->send_buf + send_index, i + j, ptr[index + j]);
                }
            } else {
                uint16_t *ptr = (uint16_t *)map_buf;
                for (int j = 0; j < need_len; j++) {
                    agile_modbus_slave_register_set(ctx->send_buf + send_index, i + j, ptr[j]);
                }
            }
        }

        now_address += map_len - 1;
        i += map_len - 1;
    }

    return 0;
}

/**
 * @brief   写寄存器
 * @param   ctx modbus 句柄
 * @param   slave_info 从机信息体
 * @param   slave_util 从机功能结构体
 * @return  =0:正常;
 *          <0:异常
 *             (-AGILE_MODBUS_EXCEPTION_UNKNOW(-255): 未知异常，从机不会打包响应数据)
 *             (其他负数异常码: 从机会打包异常响应数据)
 */
static int write_registers(agile_modbus_t *ctx, struct agile_modbus_slave_info *slave_info, const agile_modbus_slave_util_t *slave_util)
{
    uint8_t map_buf[AGILE_MODBUS_MAX_PDU_LENGTH];
    int function = slave_info->sft->function;
    int address = slave_info->address;
    int nb = 0;
    const agile_modbus_slave_util_map_t *maps = NULL;
    int nb_maps = 0;

    switch (function) {
    case AGILE_MODBUS_FC_WRITE_SINGLE_COIL:
    case AGILE_MODBUS_FC_WRITE_MULTIPLE_COILS: {
        maps = slave_util->tab_bits;
        nb_maps = slave_util->nb_bits;
        if (function == AGILE_MODBUS_FC_WRITE_SINGLE_COIL) {
            nb = 1;
        } else {
            nb = slave_info->nb;
        }
    } break;

    case AGILE_MODBUS_FC_WRITE_SINGLE_REGISTER:
    case AGILE_MODBUS_FC_WRITE_MULTIPLE_REGISTERS: {
        maps = slave_util->tab_registers;
        nb_maps = slave_util->nb_registers;
        if (function == AGILE_MODBUS_FC_WRITE_SINGLE_REGISTER) {
            nb = 1;
        } else {
            nb = slave_info->nb;
        }
    } break;

    default:
        return -AGILE_MODBUS_EXCEPTION_ILLEGAL_FUNCTION;
    }

    if (maps == NULL)
        return 0;

    for (int now_address = address, i = 0; now_address < address + nb; now_address++, i++) {
        const agile_modbus_slave_util_map_t *map = get_map_by_addr(maps, nb_maps, now_address);
        if (map == NULL)
            continue;

        int map_len = map->end_addr - now_address + 1;
        if (map->set) {
            memset(map_buf, 0, sizeof(map_buf));
            int index = now_address - map->start_addr;
            int need_len = address + nb - now_address;
            if (map->get) {
                map->get(index, need_len, map_buf, sizeof(map_buf));
            }


            if (need_len > map_len) {
                need_len = map_len;
            }

            if (function == AGILE_MODBUS_FC_WRITE_SINGLE_COIL || function == AGILE_MODBUS_FC_WRITE_MULTIPLE_COILS) {
                uint8_t *ptr = map_buf;
                if (function == AGILE_MODBUS_FC_WRITE_SINGLE_COIL) {
                    int data = *((int *)slave_info->buf);
                    ptr[index] = data;
                } else {
                    for (int j = 0; j < need_len; j++) {
                        uint8_t data = agile_modbus_slave_io_get(slave_info->buf, i + j);
                        ptr[index + j] = data;
                    }
                }
            } else {
                uint16_t *ptr = (uint16_t *)map_buf;
                if (function == AGILE_MODBUS_FC_WRITE_SINGLE_REGISTER) {
                    int data = *((int *)slave_info->buf);
                    ptr[index] = data;
                } else {
                    for (int j = 0; j < need_len; j++) {
                        uint16_t data = agile_modbus_slave_register_get(slave_info->buf, i + j);
                        ptr[index + j] = data;
                    }
                }
            }

            int rc = map->set(index, need_len, map_buf, sizeof(map_buf));
            if (rc != 0)
                return rc;
        }

        now_address += map_len - 1;
        i += map_len - 1;
    }

    return 0;
}

/**
 * @brief   掩码写寄存器
 * @param   ctx modbus 句柄
 * @param   slave_info 从机信息体
 * @param   slave_util 从机功能结构体
 * @return  =0:正常;
 *          <0:异常
 *             (-AGILE_MODBUS_EXCEPTION_UNKNOW(-255): 未知异常，从机不会打包响应数据)
 *             (其他负数异常码: 从机会打包异常响应数据)
 */
static int mask_write_register(agile_modbus_t *ctx, struct agile_modbus_slave_info *slave_info, const agile_modbus_slave_util_t *slave_util)
{
    uint8_t map_buf[AGILE_MODBUS_MAX_PDU_LENGTH];
    int address = slave_info->address;
    const agile_modbus_slave_util_map_t *maps = slave_util->tab_registers;
    int nb_maps = slave_util->nb_registers;

    if (maps == NULL)
        return 0;

    const agile_modbus_slave_util_map_t *map = get_map_by_addr(maps, nb_maps, address);
    if (map == NULL)
        return 0;

    if (map->set) {
        memset(map_buf, 0, sizeof(map_buf));
        int index = address - map->start_addr;

        if (map->get) {
            map->get(index, 1, map_buf, sizeof(map_buf));
        }

        uint16_t *ptr = (uint16_t *)map_buf;
        uint16_t data = ptr[index];
        uint16_t and = (slave_info->buf[0] << 8) + slave_info->buf[1];
        uint16_t or = (slave_info->buf[2] << 8) + slave_info->buf[3];

        data = (data & and) | (or &(~and));
        ptr[index] = data;

        int rc = map->set(index, 1, map_buf, sizeof(map_buf));
        if (rc != 0)
            return rc;
    }

    return 0;
}

/**
 * @brief   写并读寄存器
 * @param   ctx modbus 句柄
 * @param   slave_info 从机信息体
 * @param   slave_util 从机功能结构体
 * @return  =0:正常;
 *          <0:异常
 *             (-AGILE_MODBUS_EXCEPTION_UNKNOW(-255): 未知异常，从机不会打包响应数据)
 *             (其他负数异常码: 从机会打包异常响应数据)
 */
static int write_read_registers(agile_modbus_t *ctx, struct agile_modbus_slave_info *slave_info, const agile_modbus_slave_util_t *slave_util)
{
    uint8_t map_buf[AGILE_MODBUS_MAX_PDU_LENGTH];
    int address = slave_info->address;
    int nb = (slave_info->buf[0] << 8) + slave_info->buf[1];
    int address_write = (slave_info->buf[2] << 8) + slave_info->buf[3];
    int nb_write = (slave_info->buf[4] << 8) + slave_info->buf[5];
    int send_index = slave_info->send_index;

    const agile_modbus_slave_util_map_t *maps = slave_util->tab_registers;
    int nb_maps = slave_util->nb_registers;

    if (maps == NULL)
        return 0;

    /* Write first. 7 is the offset of the first values to write */
    for (int now_address = address_write, i = 0; now_address < address_write + nb_write; now_address++, i++) {
        const agile_modbus_slave_util_map_t *map = get_map_by_addr(maps, nb_maps, now_address);
        if (map == NULL)
            continue;

        int map_len = map->end_addr - now_address + 1;

        if (map->set) {
            memset(map_buf, 0, sizeof(map_buf));
            int index = now_address - map->start_addr;
            uint16_t *ptr = (uint16_t *)map_buf;
            int need_len = address_write + nb_write - now_address;
            if (map->get) {
                map->get(index,need_len, map_buf, sizeof(map_buf));
            }


            if (need_len > map_len) {
                need_len = map_len;
            }

            for (int j = 0; j < need_len; j++) {
                uint16_t data = agile_modbus_slave_register_get(slave_info->buf + 7, i + j);
                ptr[index + j] = data;
            }

            int rc = map->set(index, need_len, map_buf, sizeof(map_buf));
            if (rc != 0)
                return rc;
        }

        now_address += map_len - 1;
        i += map_len - 1;
    }

    /* and read the data for the response */
    for (int now_address = address, i = 0; now_address < address + nb; now_address++, i++) {
        const agile_modbus_slave_util_map_t *map = get_map_by_addr(maps, nb_maps, now_address);
        if (map == NULL)
            continue;

        int map_len = map->end_addr - now_address + 1;
        if (map->get) {
            memset(map_buf, 0, sizeof(map_buf));
            int index = now_address - map->start_addr;
            uint16_t *ptr = (uint16_t *)map_buf;
            int need_len = address + nb - now_address;
            map->get(index, need_len,map_buf, sizeof(map_buf));

            if (need_len > map_len) {
                need_len = map_len;
            }

            for (int j = 0; j < need_len; j++) {
                agile_modbus_slave_register_set(ctx->send_buf + send_index, i + j, ptr[index + j]);
            }
        }

        now_address += map_len - 1;
        i += map_len - 1;
    }

    return 0;
}

/**
 * @}
 */

/** @defgroup SLAVE_UTIL_Exported_Functions Slave Util Exported Functions
 * @{
 */

/**
 * @brief   从机回调函数
 * @param   ctx modbus 句柄
 * @param   slave_info 从机信息体
 * @param   data 私有数据
 * @return  =0:正常;
 *          <0:异常
 *             (-AGILE_MODBUS_EXCEPTION_UNKNOW(-255): 未知异常，从机不会打包响应数据)
 *             (其他负数异常码: 从机会打包异常响应数据)
 */
int agile_modbus_slave_util_callback(agile_modbus_t *ctx, struct agile_modbus_slave_info *slave_info, const void *data)
{
    int function = slave_info->sft->function;
    int ret = 0;
    const agile_modbus_slave_util_t *slave_util = (const agile_modbus_slave_util_t *)data;

    if (slave_util == NULL)
        return 0;

    if (slave_util->addr_check) {
        ret = slave_util->addr_check(ctx, slave_info);
        if (ret != 0)
            return ret;
    }

    switch (function) {
    case AGILE_MODBUS_FC_READ_COILS:
    case AGILE_MODBUS_FC_READ_DISCRETE_INPUTS:
    case AGILE_MODBUS_FC_READ_HOLDING_REGISTERS:
    case AGILE_MODBUS_FC_READ_INPUT_REGISTERS:
        ret = read_registers(ctx, slave_info, slave_util);
        break;

    case AGILE_MODBUS_FC_WRITE_SINGLE_COIL:
    case AGILE_MODBUS_FC_WRITE_MULTIPLE_COILS:
    case AGILE_MODBUS_FC_WRITE_SINGLE_REGISTER:
    case AGILE_MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
        ret = write_registers(ctx, slave_info, slave_util);
        break;

    case AGILE_MODBUS_FC_MASK_WRITE_REGISTER:
        ret = mask_write_register(ctx, slave_info, slave_util);
        break;

    case AGILE_MODBUS_FC_WRITE_AND_READ_REGISTERS:
        ret = write_read_registers(ctx, slave_info, slave_util);
        break;
    case AGILE_MODBUS_FC_TRANS_FILE://升级app程序
        ret = iap_set(ctx, slave_info, slave_util);
        break;
    default: {
        if (slave_util->special_function) {
            ret = slave_util->special_function(ctx, slave_info);
        } else {
            ret = -AGILE_MODBUS_EXCEPTION_ILLEGAL_FUNCTION;
        }
    } break;
    }

    if (slave_util->done) {
        slave_util->done(ctx, slave_info, ret);
    }

    return ret;
}

/**
 * @}
 */
static int iap_set(agile_modbus_t *ctx, struct agile_modbus_slave_info *slave_info, const void *data){

    static const struct fal_partition *flash_dev = NULL;

    static int __is_start = 0;
    static uint32_t __file_size = 0;
    static uint32_t __write_file_size = 0;
    int ret = 0;
    int function = slave_info->sft->function;
    int send_index = slave_info->send_index;
    uint8_t *data_ptr = slave_info->buf;
    int cmd = (data_ptr[0] << 8) + data_ptr[1];
    int cmd_data_len = (data_ptr[2] << 8) + data_ptr[3];
    uint8_t *cmd_data_ptr = data_ptr + 4;

    switch (cmd) {
    case TRANS_FILE_CMD_START: {
        if (__is_start) {
            rt_kprintf("update is already start, now abort.");
            __is_start = 0;
        }

        if (cmd_data_len <= 4) {
            rt_kprintf("cmd start date_len must be greater than 4.");
            ret = -1;
            break;
        }
        __file_size = (((uint32_t)cmd_data_ptr[0] << 24) +
                       ((uint32_t)cmd_data_ptr[1] << 16) +
                       ((uint32_t)cmd_data_ptr[2] << 8) +
                       (uint32_t)cmd_data_ptr[3]);
        flash_dev = fal_partition_find("appflash");//挂载flash
        uint16_t page = __file_size / 1024 + 1;
        for(u8 i = 0; i<page;i++)
        {
            fal_partition_erase(flash_dev,i*1024, 1);//擦除flash
        }
        __write_file_size = 0;

        char *file_name = (char *)(data_ptr + 8);
        if (strlen(file_name) >= 256) {
            rt_kprintf("file name must be less than 256.");
            ret = -1;
            break;
        }

        rt_kprintf("file name is %s, file size is %u", file_name, __file_size);
        printf("\r\n\r\n");

        __is_start = 1;
    } break;

    case TRANS_FILE_CMD_DATA: {
        if (__is_start == 0) {
            rt_kprintf("Haven't received the start command yet.");
            ret = -1;
            break;
        }

        if (cmd_data_len <= 0) {
            rt_kprintf("cmd data data_len must be greater than 0");
            ret = -1;
            break;
        }

        int flag = cmd_data_ptr[0];
        int file_len = cmd_data_len - 1;
        if (file_len > 0) {

            if (fal_partition_write(flash_dev, __write_file_size, cmd_data_ptr + 1, file_len) != file_len) {
                rt_kprintf("write to flash error.");
                ret = -1;
                break;
            }
        }
        __write_file_size += file_len;

       // print_progress(__write_file_size, __file_size);

        if (flag == TRANS_FILE_FLAG_END) {
            __is_start = 0;

            if (__write_file_size != __file_size) {
                rt_kprintf("_write_file_size (%u) != _file_size (%u)", __write_file_size, __file_size);
                ret = -1;
                break;
            }
            rt_kprintf("_write_file_size (%u)",__write_file_size);
            rt_kprintf("_write_file_size (%u)",__write_file_size);
            uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
            uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
            unsigned int i;        /* will index into CRC lookup */

            __write_file_size = 0;
            uint32_t asize=0;
            uint32_t k = 0;
            /* pass through message buffer */
            uint8_t *data = rt_malloc(1024);
            while (__write_file_size < __file_size)
            {

                uint8_t *p = data;
                k = __file_size - __write_file_size;
                if (k > 1024)
                    asize = 1024;
                else
                    asize = __file_size - __write_file_size;
                fal_partition_read(flash_dev, __write_file_size, data, asize);
                __write_file_size = __write_file_size + asize;

                while (asize--)
                {
                    i = crc_hi ^ *p++; /* calculate the CRC  */
                    crc_hi = crc_lo ^ _table_crc_hi[i];
                    crc_lo = _table_crc_lo[i];
                }

            }
            rt_free(data);
            //return (crc_hi << 8 | crc_lo);

            rt_kprintf("%X %X",crc_hi,crc_lo);


            rt_kprintf("success.");
        }
    } break;

    default:
        ret = -1;
        break;
    }

    ctx->send_buf[send_index++] = data_ptr[0];
    ctx->send_buf[send_index++] = data_ptr[1];
    ctx->send_buf[send_index++] = (ret == 0) ? 0x01 : 0x00;
    *(slave_info->rsp_length) = send_index;

    return 0;

}
/**
 * @}
 */

/**
 * @}
 */
