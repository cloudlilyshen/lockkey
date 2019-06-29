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
#include "rfid.h"
#include "para.h"
#include "util.h"
#include "rs485.h"
#include "fm175xx.h"
#include "type_a.h"
#include "mifare_card.h"


static RfidTask_t rfid;

// static uint8_t card_keys[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
// static uint8_t card_keys[6] = {0x01,0x00,0x00,0x00,0x00,0x00};
static uint8_t card_keys[6] = {0x57,0x43,0xa1,0xd6,0x8b,0x9f};//5743A1D68B9F
/*********************************************************************************************************
** Function name:       MifareCard_Test()
** Descriptions:        mifareCard测试
** input parameters:    
**                    0x01： Demo板会返回改例程的版本及相关信息
**                    0x02： Demo板执行激活TypeA卡命令，如果此时有卡片在天线附件则会通过串口打印卡片的ID号
**                    0x03： TypeA卡片密钥认证，卡片被激活后可以通过认证步骤打开卡片的读写权限
**                    0x04： 读TypeA卡片01块数据，读取成功后会通过串口打印读到的16字节数据(16进制格式)
**                    0x05： 向TypeA卡片01块写入16字节数据，写入的是BlockData数据中的数据，并且返回执行结果
**                    0x06： 读TypeB卡命令，Demo板收到这条命令后，会寻找TypeB类型的卡则读取卡
**                           得ID号并通过串口返回
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
uint8_t picc_uid[15];/* 储存卡片UID信息*/                                    
static uint8_t card_id[16];/* 储存卡片id信息*/                                    
// static uint8_t card_write[16];/* 储存卡片id信息*/    


static void  MifareCard_RegInit(void)
{                                                                                      
    Set_Rf( 3 );  /* 打开双天线 */                
    Pcd_ConfigISOType( 0 );   /* ISO14443寄存器初始化*/
}

uint8_t  MifareCard_Read(uint8_t blockAddr,uint8_t *dat)
{
    uint8_t statues;
                                            

    statues = Mifare_Auth(0x00,blockAddr>>2,card_keys,picc_uid);/* 校验密码  */
    if(!statues)
    return statues;

    statues = Mifare_Blockread(blockAddr,dat); /* 读块数据函数*/ 
    if(!statues)
    return statues;

    return statues;
}


static void rfidMspInit(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  HAL_GPIO_DeInit(GPIOF,FM_IRQ_Pin|FM_RST_Pin);
  HAL_GPIO_DeInit(GPIOB,KEY_ON_Pin);
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(KEY_ON_GPIO_Port, KEY_ON_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : KEY_ON_Pin */
  GPIO_InitStruct.Pin = KEY_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(KEY_ON_GPIO_Port, &GPIO_InitStruct);


  /*Configure GPIO pin : FM_IRQ_Pin */
  GPIO_InitStruct.Pin = FM_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(FM_IRQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : FM_RST_Pin */
  GPIO_InitStruct.Pin = FM_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FM_RST_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);    

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(FM_RST_GPIO_Port, FM_RST_Pin, GPIO_PIN_SET);  

  RfidSPIInit();     
}
void KeyOpenProc(void)
{
    rfid.msg = RfidMsg_KeyOpen;
}
uint16_t read_interval = 0;   
void RfidStart(void)
{
  rfid.msg = RfidMsg_Init;
}

void RfidMgrHandle(void)
{
    pAppTask_t app;          
    app = getApp();

    uint8_t picc_atqa[2]; /* 储存寻卡返回卡片类型信息*/                                           
    uint8_t picc_sak[3];  /* 储存卡片应答信息*/  



  switch(rfid.msg)
  {
    case RfidMsg_Idle:
        if(app->AppStatus >= indicationReady)
        rfid.msg = RfidMsg_Init;
    break;

    case RfidMsg_Init:	
          if(app->Timer_Counts.rfid_counts_1ms > delay_100ms)
          {
            rfidMspInit();
            memset(picc_uid,0,15);//先清0
            memset(card_id,0,16);        
            KEY_LOCK(); //初始狀態未加點，處於鎖定狀態              
            app->lock_status = lock_status_locked_init;//初始状态
            read_interval = delay_200ms; 
            app->Timer_Counts.rfid_counts_1ms = 0;                              
            rfid.msg = RfidMsg_FM175xxInit;
            app->AppStatus = initialised;
          }
    break;

    case RfidMsg_FM175xxInit:
        if(app->Timer_Counts.rfid_counts_1ms > read_interval) 
        {
            if(MCU_TO_PCD_TEST())
            rfid.msg = RfidMsg_FM175xxRegInit;
            else
            rfid.msg = RfidMsg_ReadFail;       
        }   
    break;

    case RfidMsg_FM175xxRegInit:
        MifareCard_RegInit();
        rfid.msg = RfidMsg_MifareCardRequest;
    break;

    case RfidMsg_MifareCardRequest:
        if(TypeA_Request(picc_atqa))    
        rfid.msg = RfidMsg_MifareCardAnticollision;
        else
        rfid.msg = RfidMsg_ReadFail;           
    break;

    case RfidMsg_MifareCardAnticollision:                          
        if(TypeA_Anticollision(0x93,picc_uid))//读出uid  M1卡,ID号只有4位   
        rfid.msg = RfidMsg_MifareCardSelect;      
        else
        rfid.msg = RfidMsg_ReadFail;            
    break;

    case RfidMsg_MifareCardSelect:
        if(TypeA_Select(0x93,picc_uid,picc_sak))                         /* 选择UID */
        rfid.msg = RfidMsg_MifareCardRead;        
        else
        rfid.msg = RfidMsg_ReadFail; 
    break;

    case RfidMsg_MifareCardRead:
        if(MifareCard_Read(4,card_id))
        {
            uint32_t id = FourBytes2uint32BigEndian(&card_id[1]);
 
            if((card_id[0]==0)&&(id<=64))//配置id号
            {
                actionPara.seq=id;	
                actionPara.chk = get_chk_bcc((uint8_t*)&actionPara,(SYSTEM_ACTION_PARA_SIZE-1));        
                SystemSaveActionPara();    
                NVIC_SystemReset();               
            }
            else //读到ID卡，关锁
            {
                switch(app->lock_status)
                {
                    case lock_status_locked_init://干上电，锁是关闭的，里面有key,保持锁关闭状态，不需要显示
                        app->lock_status = lock_status_havekey_locked; 
                        read_interval = delay_200ms;                    
                        // read_interval = delay_5s;                    
                        rfid.msg = RfidMsg_FM175xxInit;
                    break;
                    case lock_status_nokey_unlocked://已处于开锁状态，进入用户还钥匙流程
                        rfid.msg = RfidMsg_KeyClose;                     
                    break;
                    case lock_status_havekey_locked://保持状态不变
                        // app->lock_status = lock_status_nokey_locked;
                        LedEndDisplay();                       
                        // read_interval = delay_5s;                    
                        read_interval = delay_200ms;                    
                        rfid.msg = RfidMsg_FM175xxInit;
                    break;
                    case lock_status_nokey_locked://切换状态
                        app->lock_status = lock_status_havekey_locked;                    
                        read_interval = delay_200ms;                    
                        rfid.msg = RfidMsg_FM175xxInit;
                    break;
                    default:
                    break;
                }
            }
        }
        else
        {
            rfid.msg = RfidMsg_ReadFail;                 
        }      
        app->Timer_Counts.rfid_counts_1ms = 0;      
    break;
    case RfidMsg_KeyClose://延时500ms,让用户方钥匙
        if(app->Timer_Counts.rfid_counts_1ms>delay_500ms)
        {
            KEY_LOCK();  
            app->lock_status = lock_status_havekey_locked;
            rfid.msg = RfidMsg_FM175xxInit;
            app->Timer_Counts.rfid_counts_1ms = 0;    
            read_interval = delay_200ms;  
            // read_interval = delay_5s;  
            LedLockSucDisplay();           
        }
    break;
    case RfidMsg_KeyOpen:
        LedUnLockDisplay();
        KEY_UNLOCK();             
        rfid.msg = RfidMsg_KeyOpenDelay;
        app->Timer_Counts.rfid_counts_1ms = 0;   
    break;    

    case RfidMsg_KeyOpenDelay:
        if(!TypeA_Request(picc_atqa))//钥匙已取走,提前结束流程
        rfid.msg = RfidMsg_KeyOpenLeftOver;
        else if(app->Timer_Counts.rfid_counts_1ms>delay_5s)//超过5s没取走，回到初始状态
        rfid.msg = RfidMsg_KeyOpenLeftOver;
    break;
    case RfidMsg_KeyOpenLeftOver:
        rfid.msg = RfidMsg_FM175xxInit;
        app->lock_status = lock_status_nokey_unlocked;
        LedEndDisplay();
        read_interval = delay_200ms; 
        app->Timer_Counts.rfid_counts_1ms = 0;
    break;

    case RfidMsg_ReadFail://未讀到id
        switch(app->lock_status)
        {
            case lock_status_locked_init://干上电，锁是关闭的，需要打开锁,进入无key,开锁状态
                KEY_UNLOCK();
                app->lock_status = lock_status_nokey_unlocked;
            break;
            case lock_status_nokey_unlocked://已处于开锁状态，保存原有状态
            break;
            case lock_status_havekey_locked://关闭状态，只是读不到标签，不能开锁
                app->lock_status = lock_status_nokey_locked;
                LedLockNoKeyDisplay();
            break;
            case lock_status_nokey_locked://关闭状态，只是读不到标签，还是读不到，保存原来状态
            break;
            default:
            break;
        }
        //既然读不到标签，就必须加快读的频次
        read_interval = delay_200ms; 
        app->Timer_Counts.rfid_counts_1ms = 0;  
        rfid.msg = RfidMsg_FM175xxInit;                 
    break;
    case RfidMsg_End:
    break;        
  }
}
