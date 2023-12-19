/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-01-26     armink       the first version
 */

#include <ch32v30x_flash.h>

#include "../../../fal/inc/fal.h"
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;


static int init(void)
{
    /* do nothing now */
}

static int read(long offset, uint8_t *buf, size_t size)
{
    size_t i;
    uint32_t addr = stm32f2_onchip_flash.addr + offset;
    for (i = 0; i < size; i++, addr++, buf++)
    {
        *buf = *(__IO uint8_t *) addr;
    }

    return size;
}

static int write(long offset, const uint8_t *buf, size_t size)
{
    size_t i;
    uint32_t read_data;
    uint32_t addr = stm32f2_onchip_flash.addr + offset;
    uint32_t end_addr = addr + size;
    rt_err_t result = RT_EOK;
    if(addr%2 != 0){
        printf("write addr must be 2-byte alignment");
        return -RT_EINVAL;
    }

    if((end_addr) > (stm32f2_onchip_flash.addr +  stm32f2_onchip_flash.len))
    {
        printf("write outrange flash size! addr is (0x%p)", (void *)(addr + size));
        return -RT_EINVAL;
    }
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_WRPRTERR);
    while((addr < end_addr) && (FLASHStatus == FLASH_COMPLETE)){
         FLASHStatus = FLASH_ProgramHalfWord(addr, *((uint16_t *)buf));
         if((*(__IO uint16_t *)addr) != *((uint16_t *)buf))
         {
            result = -RT_ERROR;
            break;
         }
         addr += 2;
         buf += 2;

    }
FLASH_Lock();
return size;
}

static int erase(long offset, size_t size)
{
    FLASH_Status flash_status;
    size_t erased_size = 0;
    uint32_t cur_erase_sector;
    uint32_t addr = stm32f2_onchip_flash.addr + offset;

    /* start erase */
    FLASH_Unlock_Fast();
    // FLASH_ClearFlag(
    //         FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR
    //                 | FLASH_FLAG_PGSERR);
    /* it will stop when erased size is greater than setting size */
    while (erased_size < size)
    {
        cur_erase_sector = addr + erased_size;
        FLASH_ErasePage_Fast(cur_erase_sector);
     
        erased_size += stm32f2_onchip_flash.blk_size;
    }
    FLASH_Lock_Fast();
    FLASH_Lock();

    return size;
}

const struct fal_flash_dev stm32f2_onchip_flash =
{
    .name       = "stm32_onchip",
    .addr       = 0x08000000,
    .len        = 224*1024,
    .blk_size   = 256,
    .ops        = {NULL, read, write, erase},
    .write_gran = 16
};

