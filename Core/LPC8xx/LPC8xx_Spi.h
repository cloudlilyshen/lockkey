/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:       LPC8xx_Spi.h
** Descriptions:    SPI驱动
**
**------------------------------------------------------------------------------------------------------
** Created by:      林善文
** Created date:    2016-01-06
** Version:         1.0
** Descriptions:    The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:     
** Modified date:   
** Version:         
** Descriptions:    
********************************************************************************************************/

#ifndef __LPC8XX_SPI_H
#define __LPC8XX_SPI_H



#include <stdint.h>


/*********************************************************************************************************
** 函数名称：SPIInit
** 功能描述：初始化MCU与FM175xx读卡芯片的控制和通讯接口
** 输入参数：无
** 输入参数：无
** 返 回 值：无
*********************************************************************************************************/
extern  void  SPI_Init(void);

/*********************************************************************************************************
** Function name:       spi_SetReg
** Descriptions:        SPI写读卡芯片寄存器函数
** input parameters:    ucRegAddr：寄存器地址
**                      ucRegVal：要写入的值
** output parameters:   无
** Returned value:      TRUE
*********************************************************************************************************/
extern uint8_t spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal);


/*********************************************************************************************************
** Function name:       spi_GetReg
** Descriptions:        SPI读读卡芯片寄存器函数
** input parameters:    ucRegAddr：寄存器地址
** output parameters:   无
** Returned value:      目标寄存器的值
*********************************************************************************************************/
extern uint8_t spi_GetReg(uint8_t ucRegAddr);
/*********************************************************************************************************
** Function name:       SPIRead_Sequence
** Descriptions:        SPI读FIFO寄存器的值
** input parameters:    sequence_length 数据长度 ucRegAddr：寄存器地址  *reg_value 数据指针
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void SPIRead_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value);


/*********************************************************************************************************
** Function name:       SPIWrite_Sequence
** Descriptions:        SPI写FIFO的值
** input parameters:    sequence_length 数据长度 ucRegAddr：寄存器地址  *reg_value 数据指针
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void SPIWrite_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value);
extern uint8_t spi_GetReg2(uint8_t ucRegAddr,uint8_t *p);

#endif             // __LPC8XX_IO_H





































