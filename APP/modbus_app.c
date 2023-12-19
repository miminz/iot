#include "modbus_app.h"
#include <board.h>
#include <agile_modbus.h>
#include "drivers/pin.h"
#include "flashdb.h"
#include "ch32v30x_gpio.h"
#define DBG_TAG "rs485"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define AGILE_MODBUS_FC_TRANS_FILE 0x50
#define TRANS_FILE_CMD_START       0x0001
#define TRANS_FILE_CMD_DATA        0x0002
#define TRANS_FILE_FLAG_END        0x00
#define TRANS_FILE_FLAG_NOT_END    0x01

ALIGN(RT_ALIGN_SIZE)
static char rtu_task_stack[1024*3];
static struct rt_thread rtu_task_thread;

extern const agile_modbus_slave_util_map_t bit_maps[1];
extern const agile_modbus_slave_util_map_t input_bit_maps[1];
extern const agile_modbus_slave_util_map_t register_maps[1];
extern const agile_modbus_slave_util_map_t input_register_maps[1];

static int addr_check(agile_modbus_t *ctx, struct agile_modbus_slave_info *slave_info)
{
    int slave = slave_info->sft->slave;
    if ((slave != ctx->slave) && (slave != AGILE_MODBUS_BROADCAST_ADDRESS) && (slave != 0xFF))
        return -AGILE_MODBUS_EXCEPTION_UNKNOW;

    return 0;
}

const agile_modbus_slave_util_t slave_util = {
    bit_maps,
    sizeof(bit_maps) / sizeof(bit_maps[0]),
    input_bit_maps,
    sizeof(input_bit_maps) / sizeof(input_bit_maps[0]),
    register_maps,
    sizeof(register_maps) / sizeof(register_maps[0]),
    input_register_maps,
    sizeof(input_register_maps) / sizeof(input_register_maps[0]),
    addr_check,
    NULL,
    NULL};

// A10 就是 10.
// C9 就是 2*16+9=41.
// H1 就是 7*16+1=113.

// 8bit 是 (X - A) * 8 + n

#define RS485_TX_EN() GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define RS485_RX_EN() GPIO_ResetBits(GPIOB, GPIO_Pin_15)

static rt_device_t _dev = RT_NULL;
static rt_sem_t _rx_sem = RT_NULL;

static rt_err_t rs485_ind_cb(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(_rx_sem);

    return RT_EOK;
}

int rs485_send(uint8_t *buf, int len)
{
    RS485_TX_EN();
    rt_thread_mdelay(2);
    rt_device_write(_dev, 0, buf, len);
    rt_thread_mdelay(2);
    RS485_RX_EN();

    return len;
}

int rs485_receive(uint8_t *buf, int bufsz, int timeout, int bytes_timeout)
{
    int len = 0;

    while(1)
    {
        rt_sem_control(_rx_sem, RT_IPC_CMD_RESET, RT_NULL);

        int rc = rt_device_read(_dev, 0, buf + len, bufsz);
        if(rc > 0)
        {
            timeout = bytes_timeout;
            len += rc;
            bufsz -= rc;
            if(bufsz == 0)
                break;

            continue;
        }

        if(rt_sem_take(_rx_sem, rt_tick_from_millisecond(timeout)) != RT_EOK)
            break;
        timeout = bytes_timeout;
    }

    return len;
}



static uint8_t compute_meta_length_after_function_callback(agile_modbus_t *ctx, int function,
                                                           agile_modbus_msg_type_t msg_type)
{
    int length;

    if (msg_type == AGILE_MODBUS_MSG_INDICATION) {
        length = 0;
        if (function == AGILE_MODBUS_FC_TRANS_FILE)
            length = 4;
    } else {
        /* MSG_CONFIRMATION */
        length = 1;
        if (function == AGILE_MODBUS_FC_TRANS_FILE)
            length = 3;
    }

    return length;
}

static int compute_data_length_after_meta_callback(agile_modbus_t *ctx, uint8_t *msg,
                                                   int msg_length, agile_modbus_msg_type_t msg_type)
{
    int function = msg[ctx->backend->header_length];
    int length;

    if (msg_type == AGILE_MODBUS_MSG_INDICATION) {
        length = 0;
        if (function == AGILE_MODBUS_FC_TRANS_FILE)
            length = (msg[ctx->backend->header_length + 3] << 8) + msg[ctx->backend->header_length + 4];
    } else {
        /* MSG_CONFIRMATION */
        length = 0;
    }

    return length;
}

int rs485_init(void)
{
    GPIO_InitTypeDef  GPIO={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO.GPIO_Pin = GPIO_Pin_15;
    GPIO.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO);
    RS485_RX_EN();

    _rx_sem = rt_sem_create("rs485", 0, RT_IPC_FLAG_FIFO);
    if(_rx_sem == RT_NULL)
    {
        LOG_E("create rx_sem failed.");
        return -RT_ERROR;
    }

    _dev = rt_device_find("uart3");
    if (_dev == RT_NULL)
    {
        LOG_E("can't find device uart3.");
        rt_sem_delete(_rx_sem);
        return -RT_ERROR;
    }

    rt_device_set_rx_indicate(_dev, rs485_ind_cb);
    rt_device_open(_dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);

    return RT_EOK;
}

void rtu_task_entry(void *parameter)
{
    rs485_init();
    //init_io();
    uint8_t ctx_send_buf[AGILE_MODBUS_MAX_ADU_LENGTH];
    uint8_t ctx_read_buf[AGILE_MODBUS_MAX_ADU_LENGTH];
    //uint16_t hold_register[10];

    agile_modbus_rtu_t ctx_rtu;
    agile_modbus_t *ctx = &ctx_rtu._ctx;
    agile_modbus_rtu_init(&ctx_rtu, ctx_send_buf, sizeof(ctx_send_buf), ctx_read_buf, sizeof(ctx_read_buf));
    agile_modbus_set_slave(ctx, 1);
    agile_modbus_set_compute_meta_length_after_function_cb(ctx, compute_meta_length_after_function_callback);
    agile_modbus_set_compute_data_length_after_meta_cb(ctx, compute_data_length_after_meta_callback);


    

    LOG_I("Running.");

    while (1)
    {
        rt_thread_mdelay(100);

        // int send_len = agile_modbus_serialize_read_registers(ctx, 0, 10);
        // rs485_send(ctx->send_buf, send_len);
        int read_len = rs485_receive(ctx->read_buf, ctx->read_bufsz, 1000, 20);

        if (read_len == 0)
        {
            LOG_W("Receive timeout.");
            continue;
        }

       //int rc = agile_modbus_deserialize_read_registers(ctx, read_len, hold_register);
        int send_len = agile_modbus_slave_handle(ctx, read_len, 0, agile_modbus_slave_util_callback, &slave_util, NULL);

        if(send_len > 0)
            rs485_send(ctx->send_buf, send_len);

    }
}

void rtu_task_init(void)
{
    rt_thread_init(&rtu_task_thread,
                   "rtu",
                   rtu_task_entry,
                   RT_NULL,
                   &rtu_task_stack[0],
                   sizeof(rtu_task_stack),
                   10, 210);

    rt_thread_startup(&rtu_task_thread);
}
