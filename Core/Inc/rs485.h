/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : rs485.h
  * @brief          : Header for comm.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _RS485_H
#define _RS485_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#define RS485_RX_BYTES 64
#define RS485_TX_BYTES 64
#define RS485_DEBUG_BYTES_MAX 16
#define RS485_DEBUG_BYTES_MIN 6
#define RS485_UART_RS BOARD_UART1





#define RS485_DE_SEND() \
    HAL_GPIO_WritePin(RS485_DE_GPIO_Port,RS485_DE_Pin,GPIO_PIN_SET) 
#define RS485_DE_REC() \
    HAL_GPIO_WritePin(RS485_DE_GPIO_Port,RS485_DE_Pin,GPIO_PIN_RESET) 






typedef enum Rs485Cmd_e
{
    Rs485Cmd_Start,
    Rs485Cmd_Read,
    Rs485Cmd_Open,
    Rs485Cmd_Config,    

    Rs485Cmd_End
}Rs485Cmd_t;

typedef struct Rs485Comm_s  
{
    uint8_t header;
    uint8_t lock_seq;          
    uint8_t cmd;  
    uint8_t key_id[5];         
    uint8_t lock_status;            
    uint8_t err_code;               
    uint8_t chk;  
    uint8_t tailer;         
}Rs485Comm_t,*pRs485Comm_t;   

typedef enum Rs485Msg_s
{
    Rs485Msg_Idle,
    Rs485Msg_Init,
    Rs485Msg_WaitRecd,
    Rs485Msg_Parse,       

    Rs485Msg_End
}Rs485Msg_t;


typedef struct Rs485Task_s
{ 
    Rs485Msg_t msg:8;
    uint8_t lockStatus;
}Rs485Task_t, *pRs485Task_t;

void CommMgrHandle(void);

#ifdef __cplusplus
}
#endif

#endif /* __COMM_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
