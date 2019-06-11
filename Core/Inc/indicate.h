#ifndef __IND_H_
#define __IND_H_





#define LED_R_ON()  HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_SET)
#define LED_R_OFF()  HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET)
#define LED_R_TOGGLE()  HAL_GPIO_TogglePin(LED_Y_GPIO_Port, LED_Y_Pin)

#define LED_G_ON()  HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET)
#define LED_G_OFF()  HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET)
#define LED_G_TOGGLE()  HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin)

#define LED_Y_ON()  HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_SET)
#define LED_Y_OFF()   HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET)


#if 0
#define BUZZER_ON()  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET)
#define BUZZER_OFF()  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET)
typedef enum BuzzMsg_e
{
    BuzzMsg_Idle,
    BuzzMsg_Init,


    BuzzMsg_End
}BuzzMsg_t;
typedef struct BuzzTask_s
{ 
    BuzzMsg_t msg:8;
}BuzzTask_t,*pBuzzTask_t;
void BuzzMgrHandle(void);
#endif
typedef enum LedMsg_e
{
    LedMsg_Idle,
    LedMsg_Init,
    LedMsg_Flash_Seq_Start,
    LedMsg_Flash_Seq_OFF_Delay,
    LedMsg_Flash_Seq_ON_Delay,

    LedMsg_Flash_Lock,
    LedMsg_Flash_Lock_Suc,
    LedMsg_Flash_Lock_Suc_Delay,
    LedMsg_Flash_UnLock,
    LedMsg_Flash_LockNoKey,//上锁读不到Id 




    LedMsg_End
}LedMsg_t;
typedef struct LedTask_s
{ 
    LedMsg_t msg:8;
}LedTask_t,*pLedTask_t;

void LedMgrHandle(void);



void LedStartDisplay(void);
void LedEndDisplay(void);
void LedLockDisplay(void);
void LedUnLockDisplay(void);
void LedLockNoKeyDisplay(void);
void LedLockSucDisplay(void);
#endif



