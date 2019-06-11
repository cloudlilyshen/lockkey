#ifndef __PARA_H_
#define __PARA_H_



#define  STM32XX_FLASH_SIZE  (0x4000)
#define  STM32XX_FLASH_BASE  (0x8000000)
#define  STM32XX_PAGE_SIZE (0x800)
#define  STM32XX_PAGE_NUM (STM32XX_FLASH_SIZE/STM32XX_PAGE_SIZE)

#define  SYSTEM_ACTION_PARA_CONFIG_PAGE  (STM32XX_PAGE_NUM - 1)
// #define  SYSTEM_ACTION_PARA_BASE_ADDR  (STM32XX_FLASH_BASE + (STM32XX_PAGE_SIZE * SYSTEM_ACTION_PARA_CONFIG_PAGE))
#define  SYSTEM_ACTION_PARA_BASE_ADDR  0x08003C00



typedef struct System_Action_Para_s
{
    uint8_t  seq;       
    uint8_t  version;   
    uint8_t  panic;        
    uint8_t  rev00; 

    uint8_t  rev01; 
    uint8_t  rev02; 
    uint8_t  isConfig; 
    uint8_t  chk;           

}System_Action_Para_t,*pSystem_Action_Para_t;



#define SYSTEM_ACTION_PARA_SIZE (sizeof(System_Action_Para_t)/sizeof(uint8_t))


extern System_Action_Para_t actionPara;

void SystemReadActionParaWithIndex(uint8_t mainIndex, uint8_t subIndex,int32_t* paramValue);


void SystemWriteActionParamWithIndex(uint8_t mainIndex, uint8_t subIndex,int32_t paramValue);


void SystemLoadActionParamOnce(void);
void SystemLoadSaveDefaultPara(void);
void SystemSaveActionPara(void);

#endif



