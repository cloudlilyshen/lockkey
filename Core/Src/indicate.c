#include "main.h"
#include "para.h"
#include "indicate.h"

static LedTask_t led;
#if 0
static BuzzTask_t buzzer;


static void buzzMspInit(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  HAL_GPIO_DeInit(GPIOA,BUZZER_Pin);
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, BUZZER_Pin,GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = BUZZER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);   

  BUZZER_OFF();                       
}
 
void BuzzMgrHandle(void)
{
  switch(buzzer.msg)
  {
    case BuzzMsg_Idle:
        buzzer.msg = BuzzMsg_Init;
    break;
    case BuzzMsg_Init:	
        buzzMspInit();
        buzzer.msg = BuzzMsg_End;     
    break;

    case BuzzMsg_End:
    break;        
  }
}
#endif

static void ledMspInit(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_GPIO_DeInit(GPIOA,LED_Y_Pin|LED_G_Pin);

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_Y_Pin|LED_G_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = LED_Y_Pin|LED_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 

  LED_G_OFF();       
}

static uint16_t led_delay_times_ms;

uint8_t flash_times; 


void LedStartDisplay(void)
{
  led.msg = LedMsg_Init;
}
void LedEndDisplay(void)
{
  led.msg = LedMsg_End;
}

void LedLockDisplay(void)
{
  led.msg = LedMsg_Flash_Lock;
}
void LedLockSucDisplay(void)
{
  led.msg = LedMsg_Flash_Lock_Suc;
}
void LedUnLockDisplay(void)
{
  led.msg = LedMsg_Flash_UnLock;
}

void LedLockNoKeyDisplay(void)
{
  led.msg = LedMsg_Flash_LockNoKey;
}


void LedMgrHandle(void)
{
    pAppTask_t app;          
    app = getApp();

  switch(led.msg)
  {
    case LedMsg_Idle:
      if(app->AppStatus >= rs485Ready)
      led.msg = LedMsg_Init;
    break;

    case LedMsg_Init:	
        ledMspInit();
        if((actionPara.seq<=64)&&(actionPara.seq>=1))
        {
          led_delay_times_ms = actionPara.seq;
          led.msg = LedMsg_Flash_Seq_Start;
        }
        else
        led.msg = LedMsg_End; 
  
    break;

    case LedMsg_Flash_Seq_Start:
          LED_G_OFF();
          app->Timer_Counts.led_counts_1ms = 0;
          flash_times = 0;
          led.msg = LedMsg_Flash_Seq_OFF_Delay;          
    break;

    case LedMsg_Flash_Seq_OFF_Delay:
      if(app->Timer_Counts.led_counts_1ms > delay_400ms)
      {
        LED_G_ON();    
        // LED_Y_ON();    
        LED_R_ON();
        app->Timer_Counts.led_counts_1ms = 0;
        led.msg = LedMsg_Flash_Seq_ON_Delay;              
      }
    break;
    case LedMsg_Flash_Seq_ON_Delay:
      if(app->Timer_Counts.led_counts_1ms > delay_200ms)
      {
        LED_G_OFF();        
        app->Timer_Counts.led_counts_1ms = 0;
        flash_times++;
        if(flash_times<led_delay_times_ms)     
        led.msg = LedMsg_Flash_Seq_OFF_Delay;  
        else
        {
          app->AppStatus = indicationReady;
          RfidStart();
          led.msg = LedMsg_End;  
        }
      }
    break;

    case LedMsg_Flash_Lock:
      if(app->Timer_Counts.led_counts_1ms > delay_100ms)
      {
        LED_G_TOGGLE();        
      }    
    break;
    case LedMsg_Flash_Lock_Suc:
      LED_G_ON();       
      app->Timer_Counts.led_counts_1ms = 0; 
      led.msg = LedMsg_Flash_Lock_Suc_Delay;
    break;
    case LedMsg_Flash_Lock_Suc_Delay:
      if(app->Timer_Counts.led_counts_1ms > delay_2s)
      {
        LED_G_OFF();
        led.msg = LedMsg_End;      
      }    
    break;
    case LedMsg_Flash_UnLock:
      if(app->Timer_Counts.led_counts_1ms > delay_200ms)
      {
        LED_G_TOGGLE();        
      }    
    break;
    case LedMsg_Flash_LockNoKey:
      if(app->Timer_Counts.led_counts_1ms > delay_200ms)
      {
        LED_R_TOGGLE();        
      }    
    break;

    case LedMsg_End:

    break;        
  }
}
