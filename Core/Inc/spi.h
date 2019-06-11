#ifndef __SPI_H
#define __SPI_H

#define CD_EnNSS()      (HAL_GPIO_WritePin(SPI_NSS_GPIO_Port,SPI_NSS_Pin, GPIO_PIN_RESET)) 
#define CD_DisNSS()     (HAL_GPIO_WritePin(SPI_NSS_GPIO_Port,SPI_NSS_Pin, GPIO_PIN_SET)) 


/*
** 读卡芯片TPD复位操作
*/
#define CD_SetTPD()        (HAL_GPIO_WritePin(FM_RST_GPIO_Port,FM_RST_Pin, GPIO_PIN_SET))       /* 拉高TPD                      */
#define CD_ClrTPD()        (HAL_GPIO_WritePin(FM_RST_GPIO_Port,FM_RST_Pin, GPIO_PIN_RESET) )       /* 拉低TPD                      */





void  SPI_Init(void);

uint8_t spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal);

uint8_t spi_GetReg(uint8_t ucRegAddr);

void SPIRead_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value);

void SPIWrite_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value);
uint8_t spi_GetReg2(uint8_t ucRegAddr,uint8_t *p);

#endif          





































