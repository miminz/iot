/********************************** (C) COPYRIGHT *******************************
* File Name          : ch32v10x_it.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : Main Interrupt Service Routines.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#include "ch32v30x_it.h"
#include "board.h"
#include <rtthread.h>
#include <WCHNET.h>
#include "fal.h"

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void ETH_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

extern uint16_t io_data;
extern struct fal_partition *part_dev;
void PVD_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));//掉电中断
/**
  * @brief  This function handles the PVD Output interrupt request.
  * @param  None
  * @retval None
  */
void PVD_IRQHandler(void)//掉电中断
{
  if(EXTI_GetITStatus(EXTI_Line16) != RESET)
  {
      rt_kprintf("set the 'Io_out_data' value to %x\n", io_data);
    fal_partition_write(part_dev, 0, (u8 *)&io_data, 2);
        printf("ENter\r\n");
    /* Clear the Key Button EXTI line pending bit */
    EXTI_ClearITPendingBit(EXTI_Line16);
  }
}

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    rt_kprintf(" NMI Handler\r\n");
    rt_interrupt_leave();
    FREE_INT_SP();
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    rt_kprintf(" hardfult\r\n");
    while(1);
    rt_interrupt_leave();
    FREE_INT_SP();

}

/*********************************************************************
 * @fn      ETH_IRQHandler
 *
 * @brief   This function handles ETH exception.
 *
 * @return  none
 */
void ETH_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();

    WCHNET_ETHIsr();

    rt_interrupt_leave();
    FREE_INT_SP();
}


