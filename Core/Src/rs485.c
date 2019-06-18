/**
  ******************************************************************************
  * @file    
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   This sample code shows how to use STM32L0xx I2C HAL API to transmit 
  *          and receive a data buffer with a communication process based on
  *          IT transfer. 
  *          The communication is done using 2 Boards.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rs485.h"
#include "util.h"
#include "para.h"
#include "rfid.h"
#include "uart.h"
#include "indicate.h"

static Rs485Task_t rs485;

uint8_t rs485TxBuffer[RS485_TX_BYTES] = {0};
uint16_t rs485TxHeader = 0;
uint16_t rs485TxTailer = 0;

uint8_t rs485RxBuffer[RS485_RX_BYTES] = {0};
uint16_t rs485RxHeader=0;
uint16_t rs485RxTailer=0;

static Rs485Comm_t bufferRs485;
//static uint8_t debugBuffer[RS485_DEBUG_BYTES_MAX];

static void BoardRSSerialRecvMsgInit(void)
{
  rs485RxHeader = 0;
  rs485RxTailer = 0;  
  memset(rs485RxBuffer,0,RS485_RX_BYTES);    
  RS485_DE_REC();
  BoardRSSerialRecIT(RS485_UART_RS);
}

static void Rs485RecvDataHandle(uint8_t recvDat)
{
  rs485RxBuffer[rs485RxTailer] = recvDat;
  rs485RxTailer = (rs485RxTailer + 1)%RS485_RX_BYTES;  
  BoardRSSerialRecIT(RS485_UART_RS);
}

static void Rs485RecErrHandle(void)
{
  rs485.msg = Rs485Msg_Init;
  BoardRSSerialDeInit(RS485_UART_RS);
}


static void Rs485TxBufferLoad(uint8_t cmd)
{
  pAppTask_t app;          
  app = getApp();
  bufferRs485.header = '#';      
  bufferRs485.lock_seq = actionPara.seq;    
  bufferRs485.cmd = cmd;
  if(app->lock_status == lock_status_havekey_locked)
  memcpy(bufferRs485.key_id,picc_uid,5); 
  else
  memset(bufferRs485.key_id,0,5); 
  bufferRs485.lock_status = app->lock_status;
  bufferRs485.err_code = app->err;
  bufferRs485.chk = ModbusCS_CalcForProtocol((uint8_t *)&bufferRs485.header,sizeof(Rs485Comm_t)-2);
  bufferRs485.tailer = '*';
}

static void Rs485WorkDoRecParse(void)
{
  pAppTask_t app;          
  app = getApp();
  if(bufferRs485.lock_seq != actionPara.seq)
  return;
  switch(bufferRs485.cmd)
  {
    case Rs485Cmd_Read:
      Rs485TxBufferLoad(Rs485Cmd_Read);
      RS485_DE_SEND();
      BoardRSSerialSendBuffer(RS485_UART_RS,(uint8_t *)&bufferRs485,sizeof(Rs485Comm_t)); 
      RS485_DE_REC();      
    break;
    case Rs485Cmd_Open:
      if((app->lock_status!=lock_status_havekey_locked)||(app->lock_status!=lock_status_nokey_locked))
      {
        app->err = UserErrCode_No_Key;
      }
      else
      {
        app->err = UserErrCode_None;      
        KeyOpenProc();
      }
      Rs485TxBufferLoad(Rs485Cmd_Open);
      RS485_DE_SEND();
      BoardRSSerialSendBuffer(RS485_UART_RS,(uint8_t *)&bufferRs485,sizeof(Rs485Comm_t)); 
      RS485_DE_REC();       
    break;
    default:
    break;
  }
}

static void Rs485WorkDoRecFullPacket(void)
{

  uint16_t total_len;
  uint16_t len_mod,data_len;
  uint8_t min_bytes;

  min_bytes = sizeof(Rs485Comm_t);
  total_len = (rs485RxTailer >= rs485RxHeader) ?(rs485RxTailer - rs485RxHeader):(RS485_RX_BYTES-(rs485RxHeader - rs485RxTailer));
  
  if(total_len>=min_bytes)
  {
    if(rs485RxBuffer[rs485RxHeader] == '#') 
    {
      data_len = sizeof(Rs485Comm_t);
      if((rs485RxHeader +  data_len) > RS485_RX_BYTES)
      {
          len_mod = (RS485_RX_BYTES - rs485RxHeader);
          memcpy((uint8_t *)&bufferRs485,(rs485RxBuffer + rs485RxHeader),len_mod);
          memcpy(((uint8_t *)&bufferRs485 + len_mod),rs485RxBuffer,(data_len - len_mod));
      }
      else
      {
          memcpy((uint8_t *)&bufferRs485,(rs485RxBuffer + rs485RxHeader),data_len);
      }
      if(CommCheckHeadTailCS((uint8_t *)&bufferRs485,sizeof(Rs485Comm_t)))
      {
        Rs485WorkDoRecParse();
        rs485RxHeader = (rs485RxHeader + data_len)%RS485_RX_BYTES;      
      }
      else
      {
        rs485RxHeader = (rs485RxHeader + 1)%RS485_RX_BYTES;
      }
    }
    else
    {
      rs485RxHeader = (rs485RxHeader + 1)%RS485_RX_BYTES;//release the data
    }
  }
}


static void Rs485MspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_GPIO_DeInit(GPIOA,RS485_DE_Pin);
  HAL_GPIO_DeInit(GPIOB,KEY_ON_Pin);
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(KEY_ON_GPIO_Port, KEY_ON_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BUZZER_Pin RS485_DE_Pin LED_Y_Pin LED_G_Pin */
  GPIO_InitStruct.Pin = RS485_DE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RS485_DE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY_ON_Pin */
  GPIO_InitStruct.Pin = KEY_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(KEY_ON_GPIO_Port, &GPIO_InitStruct);
}

void Rs485MgrHandle(void)
{
  pAppTask_t app;          
  app = getApp();
  switch(rs485.msg)
  {
    case Rs485Msg_Idle:
      rs485.msg = Rs485Msg_Init;
    break;

    case Rs485Msg_Init:	
          Rs485MspInit();
          BoardRSSerialInit(RS485_UART_RS,Rs485RecvDataHandle,Rs485RecErrHandle);
          BoardRSSerialRecvMsgInit();   
          app->Timer_Counts.rs485_counts_1ms = 0;  
          app->AppStatus = rs485Ready;
          rs485.msg = Rs485Msg_WaitRecd;  
    break;
    case Rs485Msg_WaitRecd:
      if(app->Timer_Counts.rs485_counts_1ms > delay_10ms)
          {
            if(BoardRSSerialGetRXNEFlag(RS485_UART_RS) == 0)
            {
              BoardRSSerialRecIT(RS485_UART_RS);
            }
            Rs485WorkDoRecFullPacket();  	
            app->Timer_Counts.rs485_counts_1ms = 0;  			
          }
    break;
    case Rs485Msg_End:
      if(app->Timer_Counts.rs485_counts_1ms > delay_1s)//finished   
      {
        app->Timer_Counts.rs485_counts_1ms = 0;
        __asm("nop");    

      }

    break;        
  }
}
