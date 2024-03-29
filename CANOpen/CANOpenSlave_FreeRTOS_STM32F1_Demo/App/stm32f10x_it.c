/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_can.h"
#include "bsp_usart.h"
#include "bsp_timer.h"
#include "canopen_drv.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/************************************************
函数名称 ： DEBUG_COM_IRQHandler
功    能 ： USARTx中断(调试)
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void DEBUG_COM_IRQHandler(void)
{
  static uint8_t Data;

  if(USART_GetITStatus(DEBUG_COM, USART_IT_RXNE) != RESET)
  {
    Data = USART_ReceiveData(DEBUG_COM);

//  CLI_RcvDateFromISR(Data);
    USART_SendData(DEBUG_COM, Data);             //暂未使用接收
  }
}

/************************************************
函数名称 ： CAN_RX_IRQHandler
功    能 ： CAN接收中断
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void CAN_RX_IRQHandler(void)
{
  static CanRxMsg RxMsg;

  CAN_Receive(CANx, CAN_FIFO0, &RxMsg);

  CANRcv_DateFromISR(&RxMsg);
}

/************************************************
函数名称 ： CANOPEN_TIM_IRQ_Handler
功    能 ： CANOpen定时中断
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void CANOPEN_TIM_IRQ_Handler(void)
{
  if(TIM_GetITStatus(CANOPEN_TIMx, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(CANOPEN_TIMx, TIM_IT_Update);

    TIMx_DispatchFromISR();
  }
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
