/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-09     WCH        the first version
 */

#include <rtthread.h>


#if defined(BSP_USING_SPI_FLASH)
#include "spi_flash.h"
#include "spi_flash_sfud.h"
#include "drv_spi.h"

/* SPI flash connect to spi3 on EVT board */
static int rt_hw_spi_flash_init(void)
{
    /* PA15--->CS */
    rt_hw_spi_device_attach("spi1", "spi10", GPIOA, GPIO_Pin_4);
    if (RT_NULL == rt_sfud_flash_probe("W25Q64", "spi10"))
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_spi_flash_init);

#endif
