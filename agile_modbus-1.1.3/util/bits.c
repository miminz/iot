#include <stdint.h>
#include <string.h>
#include "agile_modbus.h"
#include "agile_modbus_slave_util.h"
#include "ch32v30x_gpio.h"
#include "flashdb.h"

extern uint16_t io_data;

#define OUTIO 16
static uint8_t _tab_bits[16] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1};

static int get_map_buf(int index, int len, void *buf, int bufsz)
{
    uint8_t *ptr = (uint8_t *)buf;
    uint16_t outdata = GPIO_ReadOutputData(GPIOD);
    for (int i = 0; i < OUTIO; i++) {
        ptr[i] = (outdata >> i) & 0x01;
    }

    return 0;
}

static int set_map_buf(int index, int len, void *buf, int bufsz)
{
    
    uint8_t *ptr = (uint8_t *)buf;
    for (int i = 0; i < len; i++) {
        if(ptr[index + i] == 1)
            io_data |= 1 << (index + i);
        else {
            io_data &= ~(1 << (index + i));
        }
       // _tab_bits[index + i] = ptr[index + i];
    }
    GPIO_Write(GPIOD,io_data);
   

    return 0;
}

const agile_modbus_slave_util_map_t bit_maps[1] = {
    {0, 15, get_map_buf, set_map_buf}};
