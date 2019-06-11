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
** Descriptions:        操作LPC824函数头文件
**
**--------------------------------------------------------------------------------------------------------
** Created By:           linshan wen 
** Created date:         2016-01-08
** Version:              1.0
** Descriptions:         First version
**
**--------------------------------------------------------------------------------------------------------
*/

#ifndef __LPC8xx_UART_H
#define __LPC8xx_UART_H
#include "Stdint.h"

//串口发送和接收数据帧格式定义
#define    Frame_End      0x03
#define    Maxlength      100

//帧格式定义
#define    DataLength     0                                             /* 数据帧的长度                 */
#define    Data            1                                            /* 数据帧数据起始               */
#define    Cammand         0                                            /* 命令编号                     */
  

extern volatile   uint8_t          GucRcvNew;                           /* 串口接收到数据标志           */
extern uint8_t     Send_Buff[Maxlength];                                /* 数据发送缓冲                 */
extern uint8_t     Recv_Buff[Maxlength];                                /* 数据接收缓冲区               */
extern uint8_t     GulNum;                                              /* 接收到数据个数               */
extern uint32_t    TotalNum;                                            /* 接收到数据总个数             */

void uartInit (void);
void uartSendByte (uint8_t ucDat);
void uartSendStr (uint8_t const *pucStr, uint32_t ulNum);

#endif

/*********************************************************************************************************
** Function name:       UARTInit
** Descriptions:        UART初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void UARTInit (void);
/*********************************************************************************************************

*********************************************************************************************************/
extern void uartSendByte (uint8_t ucDat);

extern void uartSendByteS (uint8_t *ucData,uint8_t num);
/*********************************************************************************************************

*********************************************************************************************************/
extern void uartSendStr(uint8_t const *pucStr, uint32_t ulNum);
extern void uartSendHex (uint8_t *pucStr,uint8_t num);
extern void uartPrintf(char *str,...);
extern void Uart_Send_Msg( char *msg);
extern void DEBGU(char *str,... );







