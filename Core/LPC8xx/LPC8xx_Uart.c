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
** File name:           LPC8xx_Uart.c
** Last modified Date:  2016-1-14
** Last Version:        V1.00
** Descriptions:        操作LPC824的UART，用于数据输出
**
**--------------------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "LPC8xx.h"
#include "LPC8xx_Uart.h"
#include "stdint.h"

/*********************************************************************************************************
  宏定义
*********************************************************************************************************/
#define UART_BPS        115200                                          /* 串口通信波特率               */
#define BUFFSIZE        64                                              /* BUFF大小                     */

/*********************************************************************************************************
  全局变量
*********************************************************************************************************/
char     GucRcvBuf[BUFFSIZE];                                           /* 串口接收数据缓冲区           */
uint32_t GuiIn;                                                         /* 串口接收计数个数             */
uint32_t GuiOut;                                                        /* 串口发送计数个数             */
uint32_t GuiNum;                                                        /* BUFF中有效的数据个数         */


/*********************************************************************************************************
** UART 数据定义
*********************************************************************************************************/
volatile    uint8_t          GucRcvNew;                                 /* 串口接收到数据标志           */
uint8_t     Send_Buff[Maxlength];                                       /* 数据发送缓冲                 */
uint8_t     Recv_Buff[Maxlength];                                       /* 数据接收缓冲区               */
uint8_t     GulNum;                                                     /* 接收到数据个数               */

/*********************************************************************************************************
** Function name:       UARTInit
** Descriptions:        UART初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UARTInit (void)
{
    LPC_SWM->PINASSIGN[0] &= ~( 0xFFFF << 0 );
    LPC_SWM->PINASSIGN[0] |=  ( 4 << 0 );                               /* P0.4 ~ UART0_TXD             */
    LPC_SWM->PINASSIGN[0] |=  ( 0 << 8 );                               /* P0.0 ~ UART0_RXD             */
    
    LPC_SYSCON->UARTCLKDIV     = 1;                                     /* UART时钟分频值为 1           */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 14);                             /* 初始化UART AHB时钟           */
    
    LPC_USART0->BRG = SystemCoreClock * LPC_SYSCON->SYSAHBCLKDIV /
                      (LPC_SYSCON->UARTCLKDIV * 16 * UART_BPS) - 1;     /* 串口通信波特率               */
    LPC_USART0->CFG = (1 << 0) |                                        /* 使能UART                     */
                      (1 << 2) |                                        /* 8位数据位                    */
                      (0 << 4) |                                        /* 无校验                       */
                      (0 << 6);                                         /* 1位停止位                    */
    
    LPC_USART0->INTENSET = (1 << 0);                                    /* 使能接收中断                 */
    NVIC_EnableIRQ(UART0_IRQn);                                         /* 打开UART中断                 */
}

/*********************************************************************************************************
** Function name:       UART0_IRQHandler
** Descriptions:        UART0中断服务函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART0_IRQHandler (void)
{
    uint32_t delay = 0;
    if (LPC_USART0->STAT & 0x01) {                                      /* 接收中断                     */
          GulNum = 0;
          GucRcvNew = 1;                                                    /* 接收到数据标志置1            */
        Recv_Buff[GulNum++] = LPC_USART0->RXDATA;                       /* 接收数据                     */
        while(delay < 3000) {
            delay = 0;
            while( (LPC_USART0->STAT & 0x01) == 0x00 ) {         
                delay++;
                if(delay>5000) {
                    break;
                }
            }
            if(delay>5000) {
                break;
            }
            GulNum =GulNum > Maxlength?0:GulNum; 
              Recv_Buff[GulNum++] = LPC_USART0->RXDATA;    
        }
    }
    if(LPC_USART0->STAT & 0x04)   {                                     /* 发送中断                     */
        LPC_USART0->INTENCLR |=  (1 << 2);                              /* 发送完数据，关闭发送中断     */
    }
}

/*********************************************************************************************************
** Function name:       uartSendByte
** Descriptions:        发送字节数据
** input parameters:    ucDat: 需要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uartSendByte (uint8_t ucData)
{
    while ((LPC_USART0->STAT & 0x04) == 0);                             /* 等待发送数据准备好           */
    LPC_USART0->TXDATA = ucData;                                        /* 发送数据                     */
    LPC_USART0->INTENSET |= (1 << 2);                                   /* 打开发送中断，发送数据       */
}

/*********************************************************************************************************
** Function name:       uartSendHex
** Descriptions:        将十六进制数按照字符格式输出
** input parameters:    pucStr:十六进制数制作
**                      ulNum: 输出的个数
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void uartSendHex (uint8_t *pucStr,uint8_t num)
{
    uint8_t hChar,lChar;
    uint8_t *pChar;
      pChar = pucStr;
    while(num--)                 
    {
        hChar = *(pChar)>>4;
        lChar = *(pChar++) & 0x0f;
        if( hChar < 0x0a )  {
            hChar += '0';
        }
        else {
            hChar = hChar-10+'A';
        }
        if( lChar < 0x0a )  {
            lChar += '0';
        }
        else {
            lChar = lChar-10+'A';
        }
        uartSendByte(hChar);
        uartSendByte(lChar);
    }
}

/*********************************************************************************************************
** Function name:       uartSendStr
** Descriptions:        
** input parameters:    pucStr:  数据指针
**                      ulNum:   数据个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uartSendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    while (ulNum--) {
        uartSendByte(*pucStr++);
    }
}

/*********************************************************************************************************
** Function name:       uartPrintf()
** Descriptions:        uartPrintf
** input parameters:    需要打印的字符串
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uartPrintf(char *str,...)
{
    char buf[128];
    volatile uint8_t i,num = 0;

    va_list ptr;
    va_start(ptr,str);
    vsprintf(buf,str,ptr);

    while(buf[num])
    {
        uartSendByte(buf[num]);
          num++;
    }
}

void Uart_Send_Msg( char *msg)
{

   uartPrintf(msg);

}

void DEBGU(char *str,... )
{

    char buf[128];
    uint8_t num = 0;
    va_list ptr;
    va_start(ptr,str);
    vsprintf(buf,str,ptr);
    while(buf[num])
    {
        uartSendByte(buf[num]);
          num++;
    }

}
































