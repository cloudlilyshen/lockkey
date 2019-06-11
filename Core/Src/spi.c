
#include "main.h"
#include "spi.h"
#include "fm175xx.h"

uint8_t spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal)
{
  HAL_StatusTypeDef err;
  ucRegAddr <<= 1;
  ucRegAddr &= 0x7F;  
  CD_EnNSS ();
  err = HAL_SPI_Transmit(&hspi1,&ucRegAddr,1,1000);     
  err = HAL_SPI_Transmit(&hspi1,&ucRegVal,1,1000); 
  CD_DisNSS ();
  if(err != HAL_OK)   
  return FALSE;
  return TRUE;
}

uint8_t spi_GetReg(uint8_t ucRegAddr)
{
  HAL_StatusTypeDef err;  
  uint8_t ucRegVal=0;
//  uint8_t regAddr;
  ucRegAddr <<= 1;
  ucRegAddr |= 0x80;  
  CD_EnNSS ();
  err = HAL_SPI_Transmit(&hspi1,&ucRegAddr,1,1000);      
  err = HAL_SPI_Receive(&hspi1,&ucRegVal,1,1000); 
  CD_DisNSS ();
  if(err != HAL_OK)   
  return FALSE;      
  return ucRegVal;
}


uint8_t spi_GetReg2(uint8_t ucRegAddr,uint8_t *p)
{
	HAL_StatusTypeDef err;  
  ucRegAddr <<= 1;
  ucRegAddr |= 0x80;    
  CD_EnNSS ();
  err = HAL_SPI_Transmit(&hspi1,&ucRegAddr,1,1000);      
  err = HAL_SPI_Receive(&hspi1,p,1,100);  
  CD_DisNSS ();
  if(err != HAL_OK)   
  return FALSE;   
  return TRUE;
}


void SPIRead_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value)    
{
  if (sequence_length==0)
  return;
  CD_EnNSS ();

  for(uint8_t i=0;i<sequence_length;i++) 
  {
    *(reg_value+i) = spi_GetReg(ucRegAddr);//ucRegAddr is add auto?
  }

  // HAL_StatusTypeDef err;    
  // err = HAL_SPI_Transmit(&hspi1,(uint8_t *)&ucRegAddr,1,1000);      
  // err = HAL_SPI_Receive(&hspi1,(uint8_t *)&reg_value,sequence_length,1000); 
  // if(err != HAL_OK)   
  // __asm("nop");

  CD_DisNSS ();

  return;
}

void SPIWrite_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value)
{
     
  if(sequence_length==0)
  return;

  for(uint8_t i=0;i<sequence_length;i++) 
  {
    spi_SetReg(ucRegAddr, *(reg_value+i));//ucRegAddr is add auto?
  }

  // HAL_StatusTypeDef err;
  // err = HAL_SPI_Transmit(&hspi1,(uint8_t *)&ucRegAddr,1,1000);     
  // err = HAL_SPI_Transmit(&hspi1,(uint8_t *)&reg_value,sequence_length,1000);  
  // if(err != HAL_OK)   
  // __asm("nop");

  CD_DisNSS ();
  return ;    
}




