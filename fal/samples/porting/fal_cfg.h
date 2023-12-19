/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include <rtconfig.h>
#include <board.h>

#define NOR_FLASH_DEV_NAME             "W25Q64"//"norflash0"

/* ===================== Flash device Configuration ========================= */
extern const struct fal_flash_dev stm32f2_onchip_flash;
extern struct fal_flash_dev nor_flash0;
#define FAL_PART_HAS_TABLE_CFG
/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &stm32f2_onchip_flash,                                           \
    &nor_flash0,                                                     \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
#define FAL_PART_TABLE                                                               \
{                                                                                    \
    {FAL_PART_MAGIC_WORD,        "bl",     "stm32_onchip",         0,   30*1024, 0}, \
    {FAL_PART_MAGIC_WORD,       "app",     "stm32_onchip",   30*1024,  193*1024, 0}, \
    {FAL_PART_MAGIC_WORD,       "rundata",     "stm32_onchip",   220*1024,  4*1024, 0}, \
    {FAL_PART_MAGIC_WORD,  "runenv", NOR_FLASH_DEV_NAME,         0, 1024*1024*1, 0}, \
    {FAL_PART_MAGIC_WORD,  "runlog", NOR_FLASH_DEV_NAME, 1024*1024, 1024*1024*6, 0}, \
    {FAL_PART_MAGIC_WORD,  "appflash", NOR_FLASH_DEV_NAME, 1024*1024*7, 1024*1024*1, 0}, \
}
#endif /* FAL_PART_HAS_TABLE_CFG */

#endif /* _FAL_CFG_H_ */
