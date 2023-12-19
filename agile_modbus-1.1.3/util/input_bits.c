#include <stdint.h>
#include <string.h>
#include "agile_modbus.h"
#include "agile_modbus_slave_util.h"
#include "ch32v30x_gpio.h"
#define INIO 16
static uint8_t _tab_input_bits[16] = {0, 1, 1, 0, 0, 1, 1, 0, 0, 1};

static int get_map_buf(void *buf, int bufsz)
{
    uint8_t *ptr = (uint8_t *)buf;
    uint16_t indata = GPIO_ReadInputData(GPIOE);
    for (int i = 0; i < INIO; i++) {
        //ptr[i] = _tab_input_bits[i];
        ptr[i] = (indata >> i) & 0x01;
    }
    return 0;
}

const agile_modbus_slave_util_map_t input_bit_maps[1] = {
    {0, 15, get_map_buf, NULL}};
