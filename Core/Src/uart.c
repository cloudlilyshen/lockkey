/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
MCU_UartRecvIntProcFunc uart1RecvFuncPtr = NULL;
uint8_t uart1SingleData;
MCU_UartErrProcFunc uart1ErrFuncPtr = NULL;

void BoardRSSerialInit(Board_RS_Serial_t serialNo,MCU_UartRecvIntProcFunc rxCallBack,MCU_UartErrProcFunc errCallBack)
{
  switch(serialNo)
  {
    case BOARD_UART1:
        HAL_UART_DeInit(&huart1);
        MXUsart1UartInit();
        uart1RecvFuncPtr = rxCallBack;
        uart1ErrFuncPtr = errCallBack;
        break;
  }  
}

void BoardRSSerialDeInit(Board_RS_Serial_t serialNo)
{
  switch(serialNo)
  {
    case BOARD_UART1:
        HAL_UART_DeInit(&huart1);
        // SEGGER_RTT_printf(0,"BoardRSSerialDeInit\r\n");
        break;		
  }
}


void BoardRSSerialRecIT(Board_RS_Serial_t serialNo)
{
  switch(serialNo)
  {
    case BOARD_UART1:
          HAL_UART_Receive_IT(&huart1,&uart1SingleData,1);      
        break;           
  }

}

uint8_t  BoardRSSerialGetRXNEFlag(Board_RS_Serial_t serialNo)
{
  uint8_t flag;
  switch(serialNo)
  {
    case BOARD_UART1:
          flag = __HAL_UART_GET_IT_SOURCE(&huart1,UART_IT_RXNE);
        break;   
  }
  return flag;
}

void BoardRSSerialSendBuffer(Board_RS_Serial_t serialNo, uint8_t* bufferPtr,uint16_t bufferLength)
{
  switch(serialNo)
  {
    case BOARD_UART1:
          HAL_UART_Transmit(&huart1,bufferPtr,bufferLength,100);
        break;
  }
}

void BoardRSSerialSendString(Board_RS_Serial_t serialNo,const char *string)
{
  switch(serialNo)
  {
      case BOARD_UART1:
          HAL_UART_Transmit(&huart1,(uint8_t *)string,strlen((char const *)string),100);
          break;
  }
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
