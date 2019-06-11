/****************************************Copyright (c)****************************************************
**                            Guangzhou ZLGMCU Technology Co., LTD
**
**                                 http://www.zlgmcu.com
**
**      ������������Ƭ���Ƽ����޹�˾���ṩ�����з�������ּ��Э���ͻ����ٲ�Ʒ���з����ȣ��ڷ�����������ṩ
**  ���κγ����ĵ������Խ����������֧�ֵ����Ϻ���Ϣ���������ο����ͻ���Ȩ��ʹ�û����вο��޸ģ�����˾��
**  �ṩ�κε������ԡ��ɿ��Եȱ�֤�����ڿͻ�ʹ�ù��������κ�ԭ����ɵ��ر�ġ�żȻ�Ļ��ӵ���ʧ������˾��
**  �е��κ����Ρ�
**                                                                        ����������������Ƭ���Ƽ����޹�˾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           LPC8xx_Gpio.c
** Last modified Date:  2016-1-4
** Last Version:        V1.00
** Descriptions:        ����LPC824��GPIO����Ҫ���ڿ���ָʾ��LED�Ϳ���FM175xx����оƬ��λ��
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
** Descriptions:        ����IO�ж�
** input parameters:    
** output parameters:   void
** Returned value:      ��
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
** Descriptions:        PMU��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void PMUInit (void)
{
//    LPC_SYSCON->PDRUNCFG  &= ~WDT_OSC_PD;                               /* ����WDT                      */
    NVIC_ClearPendingIRQ(PIN_INT0_IRQn);                                /* ����ж�����                 */
    LPC_GPIO_PORT->DIR[0] &= ~IRQ; 
    LPC_SYSCON->PINTSEL[0] = IRQ;                                       /* �ж�ѡ���ж�0��Ϊ�ж�����    */
    GPIOSetPinInterrupt(0,0,24,1,0);                                    /* P0.4Ϊ�͵�ƽ�ж�             */

    NVIC_EnableIRQ(PIN_INT0_IRQn);                                      /* ���ж�ͨ�� 0               */
    
	  NVIC_ClearPendingIRQ(PIN_INT1_IRQn);                                /* ����ж�����                 */
    LPC_GPIO_PORT->DIR[1] &= ~UART_RXD; 
    LPC_SYSCON->PINTSEL[1] = UART_RXD;                                       /* �ж�ѡ���ж�0��Ϊ�ж�����    */
    GPIOSetPinInterrupt(1,0,0,1,0);                                    /* P0.4Ϊ�͵�ƽ�ж�             */

    NVIC_EnableIRQ(PIN_INT1_IRQn);                                      /* ���ж�ͨ�� 0               */
}

/*********************************************************************************************************
** Function name:       PMUInit
** Descriptions:        PMU��ʼ��������MCU�������ģʽ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void PowerDown(void)
{
    LPC_SPI0->CFG = 0x00;
    PMUInit();
    LPC_IOCON->PIO0[14]  &= ~(0x3 << 3); 
	  LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<9);
    LPC_SYSCON->PDAWAKECFG  =  LPC_SYSCON->PDRUNCFG;                    /* ���û��ѼĴ���               */
//    LPC_SYSCON->PDSLEEPCFG &= ~(WDT_OSC_PD | BOD_PD);                   /* ���˯��ģʽ�´�WDT��BOD   */
    LPC_SYSCON->STARTERP0 |= (0x01|0x02);                                      /* �����ж�0����                */
  
    SCB->SCR     |= 0x04 ;                                              /* ���ý������˯��ģʽ         */
    LPC_PMU->PCON = 0x2;                                                /* �������ģʽ                 */
    
	   CD_ClrTPD(); 
	   NVIC_EnableIRQ(PIN_INT0_IRQn);                                      /* ʹ�ܶ������ж�               */
	   NVIC_EnableIRQ(PIN_INT1_IRQn);                                      /* ʹ�ܶ������ж�               */
    __WFI();                                                            /* �������˯��ģʽ             */
}


/*********************************************************************************************************
** Function name:       GPIOInit
** Descriptions:        GPIO��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void GPIOInit (void)  {
    
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* ��ʼ��GPIO AHBʱ��           */
    LPC_IOCON->PIO0[IOCON_PIO10] &=  ~(0x03UL<<8);                      /* ����P0_10 P0_11 IO����       */
    LPC_IOCON->PIO0[IOCON_PIO10] |=   (0x01UL<<8); 
    LED_GreenCfg();                                                     /* �����̵�IOΪ���             */
    LED_GreenOff();                                                     /* �����̵�                     */
    LED_RedCfg();
    LED_RedOff();                   
    CON_Input();                                                        /* ����CONΪ��������            */
}





































