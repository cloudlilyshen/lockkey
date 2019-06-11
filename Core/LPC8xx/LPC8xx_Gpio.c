/****************************************Copyright (c)****************************************************
**                            Guangzhou ZLGMCU Technology Co., LTD
**
**                                 http://www.zlgmcu.com
**
**      广州周立功单片机科技有限公司所提供的所有服务内容旨在协助客户加速产品的研发进度，在服务过程中所提供
**  的任何程序、文档、测试结果、方案、支持等资料和信息，都仅供参考，客户有权不使用或自行参考修改，本公司不
**  提供任何的完整性、可靠性等保证，若在客户使用过程中因任何原因造成的特别的、偶然的或间接的损失，本公司不
**  承担任何责任。
**                                                                        ——广州周立功单片机科技有限公司
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           LPC8xx_Gpio.c
** Last modified Date:  2016-1-4
** Last Version:        V1.00
** Descriptions:        操作LPC824的GPIO，主要用于控制指示灯LED和控制FM175xx读卡芯片复位。
**
**--------------------------------------------------------------------------------------------------------
*/

#include "LPC8xx.h"
#include "LPC8xx_IO.h"
#include "LPC8xx_Function.h"
#include "LPC8xx_Uart.h"
#include "fm175xx.h"
#include "MyType.h"
#include "LPCD_API.h"




extern  unsigned char Event_Flag; 
extern struct lpcd_struct Lpcd;


/*********************************************************************************************************
** Function name:       GPIOSetPinInterrupt
** Descriptions:        设置IO中断
** input parameters:    
** output parameters:   void
** Returned value:      无
*********************************************************************************************************/
void GPIOSetPinInterrupt( uint32_t channelNum, uint32_t portNum, uint32_t bitPosi,
        uint32_t sense, uint32_t event )
{
    /*
    ** right now, there is only port 0 on LPC8xx, more ports may be added, save
    ** temporarily for future use. 
    */
#if 1
    LPC_SYSCON->PINTSEL[channelNum] = bitPosi; 
    //NVIC_EnableIRQ((IRQn_Type)(PININT0_IRQn+channelNum));
#endif
    
  if ( sense  ==  0 )
  {
    LPC_PININT->ISEL &=  ~(0x1<<channelNum);                           /* Edge trigger */
    if ( event  ==  0 )
    {
        LPC_PININT->IENF |=  (0x1<<channelNum);                        /* faling edge */
    }
    else
    {
        LPC_PININT->IENR |=  (0x1<<channelNum);                        /* Rising edge */
    }
  }
  else
  {
    LPC_PININT->ISEL |=  (0x1<<channelNum);                            /* Level trigger. */
    LPC_PININT->IENR |=  (0x1<<channelNum);                            /* Level enable */
    if ( event  ==  0 )
    {
        LPC_PININT->IENF &=  ~(0x1<<channelNum);                       /* active-low */
    }
    else
    {
        LPC_PININT->IENF |=  (0x1<<channelNum);                        /* active-high */
    }
  }
  return;
}


/*********************************************************************************************************
** Function name:       PMUInit
** Descriptions:        PMU初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void PMUInit (void)
{
//    LPC_SYSCON->PDRUNCFG  &= ~WDT_OSC_PD;                               /* 运行WDT                      */
    NVIC_ClearPendingIRQ(PIN_INT0_IRQn);                                /* 清楚中断屏蔽                 */
    LPC_GPIO_PORT->DIR[0] &= ~IRQ; 
    LPC_SYSCON->PINTSEL[0] = IRQ;                                       /* 中断选择中断0作为中断引脚    */
    GPIOSetPinInterrupt(0,0,24,1,0);                                    /* P0.4为低电平中断             */

    NVIC_EnableIRQ(PIN_INT0_IRQn);                                      /* 打开中断通道 0               */
    
	  NVIC_ClearPendingIRQ(PIN_INT1_IRQn);                                /* 清楚中断屏蔽                 */
    LPC_GPIO_PORT->DIR[1] &= ~UART_RXD; 
    LPC_SYSCON->PINTSEL[1] = UART_RXD;                                       /* 中断选择中断0作为中断引脚    */
    GPIOSetPinInterrupt(1,0,0,1,0);                                    /* P0.4为低电平中断             */

    NVIC_EnableIRQ(PIN_INT1_IRQn);                                      /* 打开中断通道 0               */
}

/*********************************************************************************************************
** Function name:       PMUInit
** Descriptions:        PMU初始化，配置MCU进入掉电模式
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void PowerDown(void)
{
    LPC_SPI0->CFG = 0x00;
    PMUInit();
    LPC_IOCON->PIO0[14]  &= ~(0x3 << 3); 
	  LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<9);
    LPC_SYSCON->PDAWAKECFG  =  LPC_SYSCON->PDRUNCFG;                    /* 配置唤醒寄存器               */
//    LPC_SYSCON->PDSLEEPCFG &= ~(WDT_OSC_PD | BOD_PD);                   /* 深度睡眠模式下打开WDT和BOD   */
    LPC_SYSCON->STARTERP0 |= (0x01|0x02);                                      /* 设置中断0唤醒                */
  
    SCB->SCR     |= 0x04 ;                                              /* 设置进入深度睡眠模式         */
    LPC_PMU->PCON = 0x2;                                                /* 进入掉电模式                 */
    
	   CD_ClrTPD(); 
	   NVIC_EnableIRQ(PIN_INT0_IRQn);                                      /* 使能读卡器中断               */
	   NVIC_EnableIRQ(PIN_INT1_IRQn);                                      /* 使能读卡器中断               */
    __WFI();                                                            /* 进入深度睡眠模式             */
}


/*********************************************************************************************************
** Function name:       GPIOInit
** Descriptions:        GPIO初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void GPIOInit (void)  {
    
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* 初始化GPIO AHB时钟           */
    LPC_IOCON->PIO0[IOCON_PIO10] &=  ~(0x03UL<<8);                      /* 设置P0_10 P0_11 IO功能       */
    LPC_IOCON->PIO0[IOCON_PIO10] |=   (0x01UL<<8); 
    LED_GreenCfg();                                                     /* 配置绿灯IO为输出             */
    LED_GreenOff();                                                     /* 点亮绿灯                     */
    LED_RedCfg();
    LED_RedOff();                   
    CON_Input();                                                        /* 配置CON为输入引脚            */
}





































