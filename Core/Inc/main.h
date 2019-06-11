/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "rfid.h"
#include "rs485.h"
#include "para.h"
#include "indicate.h"
// #include "SEGGER_RTT.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define PRINTF_DEBUG
typedef enum {FALSE = 0, TRUE = !FALSE} bool;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
typedef struct
{
  bool Time_1ms_Mark :1; 
  bool Time_10ms_Mark :1; 
  bool Time_100ms_Mark :1;
  bool Time_1s_Mark :1; 
  bool Time_10s_Mark :1; 
  bool Time_60s_Mark :1; 
} Timer_Mark_t;

#define no_delay 0
#define delay_1ms 1
#define delay_2ms 2
#define delay_3ms 3
#define delay_4ms 4
#define delay_5ms 5
#define delay_10ms 10
#define delay_15ms 15
#define delay_20ms 20
#define delay_30ms 30

#define delay_40ms 40
#define delay_50ms 50
#define delay_60ms 50
#define delay_70ms 50

#define delay_80ms 80
#define delay_90ms 90

#define delay_100ms 100
#define delay_120ms 120
#define delay_150ms 150
#define delay_200ms 200
#define delay_250ms 250
#define delay_300ms 300

#define delay_350ms 350

#define delay_400ms 400
#define delay_500ms 500
#define delay_600ms 600

#define delay_700ms 700

#define delay_800ms 800

#define delay_1s 1000
#define delay_2s 2000
#define delay_5s 5000
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FM_IRQ_Pin GPIO_PIN_0
#define FM_IRQ_GPIO_Port GPIOF
#define FM_IRQ_EXTI_IRQn EXTI0_1_IRQn
#define FM_RST_Pin GPIO_PIN_1
#define FM_RST_GPIO_Port GPIOF
#define BUZZER_Pin GPIO_PIN_0
#define BUZZER_GPIO_Port GPIOA
#define RS485_DE_Pin GPIO_PIN_1
#define RS485_DE_GPIO_Port GPIOA
#define SPI_NSS_Pin GPIO_PIN_4
#define SPI_NSS_GPIO_Port GPIOA
#define KEY_ON_Pin GPIO_PIN_1
#define KEY_ON_GPIO_Port GPIOB
#define LED_Y_Pin GPIO_PIN_9
#define LED_Y_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_10
#define LED_G_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */


typedef enum UserErrCode_e
{
  UserErrCode_None,
  UserErrCode_KeySeq,//开锁序号错误
  UserErrCode_Open,//开锁失败
  UserErrCode_No_Key,//无钥匙
  UserErrCode_Read_Card,//读卡失败
  UserErrCode_No_Ack,//下位机无回复  
  UserErrCode_End
}UserErrCode_t;

enum
{
  lock_status_locked_init,
  lock_status_nokey_unlocked,
  lock_status_havekey_locked,
  lock_status_nokey_locked,//已上锁，但读不到ID
};

#define KEY_LOCK()  HAL_GPIO_WritePin(KEY_ON_GPIO_Port, KEY_ON_Pin, GPIO_PIN_RESET)
#define KEY_UNLOCK()  HAL_GPIO_WritePin(KEY_ON_GPIO_Port, KEY_ON_Pin, GPIO_PIN_SET)

#define KEY_VAL()  HAL_GPIO_ReadPin(KEY_ON_GPIO_Port, KEY_ON_Pin)





typedef struct
{
  uint16_t   rfid_counts_1ms;
  uint16_t   led_counts_1ms;
  uint16_t   buzzer_counts_1ms;
  uint16_t   rs485_counts_1ms;    
  float version;

}Timer_Counts_t;


typedef enum AppStatus_e
{
    poweringOff,
    poweringOn,    
    rs485Ready,       
    indicationReady,       
    initialised,  
}AppStatus_t;


typedef struct AppTask_s
{
  Timer_Mark_t    Timer_Mark;
  Timer_Counts_t  Timer_Counts;    

  float version;
  uint8_t lock_status;
  uint8_t err;
  AppStatus_t AppStatus;
  
} AppTask_t, *pAppTask_t;
extern UART_HandleTypeDef huart1;
extern SPI_HandleTypeDef hspi1;




AppTask_t *getApp(void);
void RfidSPIInit(void);
void MXUsart1UartInit(void);
void Rs485MgrHandle(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
