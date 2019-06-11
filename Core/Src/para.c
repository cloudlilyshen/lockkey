#include "main.h"
#include "para.h"
#include "util.h"

System_Action_Para_t actionPara;
void SystemLoadSaveDefaultPara(void)
{
    pAppTask_t app;          
    app = getApp();
    memset((uint8_t*)&actionPara,0,SYSTEM_ACTION_PARA_SIZE);
   

    actionPara.seq=1;
   
    actionPara.version = app->version;
    actionPara.isConfig = 0xab;  

    actionPara.chk = get_chk_bcc((uint8_t*)&actionPara,(SYSTEM_ACTION_PARA_SIZE-1));
    SystemSaveActionPara(); 
}

void SystemLoadActionParamOnce(void)
{
    pAppTask_t app;          
    app = getApp();
    memcpy((uint8_t*)&actionPara,(uint8_t*)SYSTEM_ACTION_PARA_BASE_ADDR,SYSTEM_ACTION_PARA_SIZE); 
    if(actionPara.isConfig != 0xab)
    {
        SystemLoadSaveDefaultPara();      
    }
    else if(actionPara.version != app->version)
    {
        actionPara.version = app->version;
        actionPara.chk = get_chk_bcc((uint8_t*)&actionPara,(SYSTEM_ACTION_PARA_SIZE-1));        
        SystemSaveActionPara();         
    }
}

void SystemSaveActionPara(void)
{
    uint32_t PageError,dst;
    uint32_t *src;
	FLASH_EraseInitTypeDef FlashEraseInit;
	FlashEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	FlashEraseInit.PageAddress = SYSTEM_ACTION_PARA_BASE_ADDR;
	FlashEraseInit.NbPages = 1;
    dst = SYSTEM_ACTION_PARA_BASE_ADDR;
    src = (uint32_t *)&actionPara;

	HAL_FLASH_Unlock();
	if(HAL_OK != HAL_FLASHEx_Erase(&FlashEraseInit, &PageError))
    {
        __asm("nop");        
    }

    for(uint8_t i=0;i<SYSTEM_ACTION_PARA_SIZE/4;i++)
    {
	    if(HAL_OK == HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, dst, *src))
        {
            dst += 4;
            src++;
        }   
        else
        {
            __asm("nop");
        }
    }
    HAL_FLASH_Lock();
}



