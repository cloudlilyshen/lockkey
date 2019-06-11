#include "main.h"
#include <stdint.h>
#include "fm175xx.h"
#include "util.h"
#include <string.h>    
#include "spi.h"


uint8_t  FM175XX_IRQ;


#define MAXRLEN 64


void pcd_Init(void)
{
    // SPI_Init();
    // CD_CfgTPD();                                                     
}


uint8_t MCU_TO_PCD_TEST(void)
{
    volatile uint8_t ucRegVal=0;
    pcd_RST();                                                   
    ucRegVal = spi_GetReg(ControlReg);
    spi_SetReg(ControlReg, 0x10);                                     
    ucRegVal = spi_GetReg(ControlReg);
    spi_SetReg(GsNReg, 0xF0 | 0x04);                                 
    ucRegVal = spi_GetReg(GsNReg);
   
    if(ucRegVal != 0xF4)                                          
        return FALSE;
    return TRUE;
}

void pcd_RST(void)
{
    CD_SetTPD();
    Delay100us(3);
    CD_ClrTPD();                                                      
    Delay100us(5);
    CD_SetTPD();                                                     
    Delay100us(30);
}

void NRSTPD_CTRL(uint8_t  ctrltype)
{
    if(ctrltype == 0x0)
    CD_ClrTPD();
    else
    CD_SetTPD();
}


uint8_t  Read_Reg(uint8_t  reg_add)
{
    uint8_t   reg_value;       
    reg_value=spi_GetReg(reg_add);
    return reg_value;
}

uint8_t  GetReg(uint8_t  reg_add,uint8_t *regData)
{     
    *regData=spi_GetReg(reg_add);
    return *regData;
}


uint8_t  SetReg(uint8_t  reg_add,uint8_t ucRegVal)
{     
    spi_SetReg(reg_add,ucRegVal);
    return 1;
}


uint8_t  Read_Reg_All(uint8_t  *reg_value)
{
    for (uint8_t i=0;i<64;i++)       
        *(reg_value+i) = spi_GetReg(i);
    return TRUE;
}



uint8_t  Write_Reg(uint8_t  reg_add,uint8_t  reg_value)
{
    spi_SetReg(reg_add,reg_value);
    return TRUE;
}


void Read_FIFO(uint8_t  length,uint8_t  *fifo_data)
{     
    SPIRead_Sequence(length,FIFODataReg,fifo_data);
    return;
}


void Write_FIFO(uint8_t  length,uint8_t  *fifo_data)
{
    SPIWrite_Sequence(length,FIFODataReg,fifo_data);
    return;
}


uint8_t  Clear_FIFO(void)
{
    Set_BitMask(FIFOLevelReg,0x80);                                    
    if( spi_GetReg(FIFOLevelReg) == 0 )
        return TRUE;
    else
        return FALSE;
}


uint8_t Set_BitMask(uint8_t reg_add,uint8_t mask)
{
    uint8_t result;
    result=spi_SetReg(reg_add,Read_Reg(reg_add) | mask);               
    return result;
}

uint8_t  Clear_BitMask(uint8_t  reg_add,uint8_t  mask)
{
    uint8_t result;
    result=Write_Reg(reg_add,Read_Reg(reg_add) & ~mask);               
    return result;
}


uint8_t  Set_Rf(uint8_t  mode)
{
    uint8_t  result;
    if( (Read_Reg(TxControlReg)&0x03) == mode )
        return TRUE;
    // if( mode == 0 )
    //     result = Clear_BitMask(TxControlReg,0x03);                      /* �ر�TX1��TX2���             */
    // if( mode== 1 )
    //     result = Clear_BitMask(TxControlReg,0x01);                      /* ����TX1���                */
    // if( mode == 2)
    //     result = Clear_BitMask(TxControlReg,0x02);                      /* ����TX2���                */
    if (mode==3)
        result=Set_BitMask(TxControlReg,0x03);                          /* ��TX1��TX2���             */
//    Delay100us(2000);
    return result;
}
 

unsigned char Pcd_Comm(  unsigned char Command, 
                         unsigned char *pInData, 
                         unsigned char InLenByte,
                         unsigned char *pOutData, 
                         unsigned int *pOutLenBit)
{
    uint8_t status  = FALSE;
    uint8_t irqEn   = 0x00;                                            
    uint8_t waitFor = 0x00;                                            
    uint8_t lastBits;
    uint8_t n,sendLen,sendPi,revLen,fifoLen,errorReg,collPos;
    uint32_t i;
	  sendPi = 0x00;
	  revLen = 0x00;
    fifoLen = 0x00;
    lastBits = 0x00;
    Write_Reg(ComIrqReg, 0x7F);                                        
    Write_Reg(TModeReg,0x80);                                          
    SetReg(DivIrqReg,0x7f); 
    
    switch (Command) {
    case MFAuthent:                                                     
        irqEn   = 0x12;
        waitFor = 0x10;
        break;
    case Transceive:                                      
        irqEn   = 0x77;
  			waitFor = 0x30;
        break;
    default:
        break;
    }
           
    Write_Reg(CommandReg, Idle);
    Set_BitMask(FIFOLevelReg, 0x80);
		Set_BitMask(CollReg, 0x80);
    sendLen = InLenByte>40?40:InLenByte;
    sendPi += sendLen;
//    revLen = 0x00;
		InLenByte -= sendLen;
		
		spi_SetReg(ComIrqReg,0x7f);
		fifoLen = Read_Reg(FIFOLevelReg); 
    Write_FIFO(sendLen,pInData);
    Write_Reg(CommandReg, Command);
		Write_Reg(ComIEnReg, irqEn | 0x80);
    if (Command == Transceive) {
        Set_BitMask(BitFramingReg, 0x80);
    }
    i = 3000;                                             
    do {
        n = spi_GetReg(ComIrqReg);                      
		    fifoLen = spi_GetReg(FIFOLevelReg);
		    if(( InLenByte >0 )&&( fifoLen <0x10 ))                            
	    	{
					  sendLen = InLenByte>30?30:InLenByte;
					  Write_FIFO(sendLen, &pInData[sendPi]);
			      sendPi += sendLen;
			      InLenByte -=sendLen;
		    }
		    if( ( InLenByte == 0)&&( fifoLen >0x28 ) )
		    {
					  Read_FIFO(fifoLen,&pOutData[revLen]);
			      revLen += fifoLen;
		    }
        i--;
    } while ((i != 0) && !(n & 0x03) && !(n & waitFor));           
    if (i != 0) {
			  errorReg = Read_Reg(ErrorReg);
        if(!(errorReg & 0x1B)) {
            status = TRUE;
            if (n & irqEn & 0x01) {
                status = CMD_TIMER_OVER;
            }
            if (Command == Transceive) {
                fifoLen = Read_Reg(FIFOLevelReg);
                lastBits = Read_Reg(ControlReg) & 0x07;
                if (lastBits) {
                    *pOutLenBit = lastBits;
									  if(fifoLen > 1)
											 *pOutLenBit += (fifoLen-1+revLen) * 8;
										if (fifoLen == 0) {
                       fifoLen = 1;
											 *pOutLenBit += (revLen) * 8;
                    }
                }
								else {
                    *pOutLenBit = (fifoLen+revLen) * 8;
                }
                Read_FIFO(fifoLen,&pOutData[revLen]);
            }
        }
        else if(errorReg & 0x08)                                                       
        {
					  collPos = spi_GetReg(CollReg);                                      
		        collPos &= 0x1f;
		        *pOutLenBit = (collPos == 0 )?32:collPos;
					  fifoLen =*pOutLenBit/8 +(*pOutLenBit%8?1:0);
					  Read_FIFO(fifoLen,&pOutData[revLen]);
            status = Anticollision;
				}					
				else {
            status = FALSE;
        }
    }
    Clear_BitMask(BitFramingReg,0x80);
    SetReg(ComIEnReg, 0x80);                                   
    SetReg(DivIEnReg, 0x80);
    SetReg(ComIrqReg,0x7f); 
    SetReg(DivIrqReg,0x7f);          
		SetReg(FIFOLevelReg,0x80);  
    Clear_BitMask(BitFramingReg, 0x80);                                 
    Set_BitMask(ControlReg,0x80);                                        
    return status;
}

uint8_t  Pcd_SetTimer(unsigned long delaytime)
{
    unsigned long TimeReload;
    uint32_t Prescaler;

    Prescaler=0;
    TimeReload=0;
    while(Prescaler<0xfff) {
        TimeReload = ((delaytime*(long)13560)-1)/(Prescaler*2+1);
        if( TimeReload<0xffff)
            break;
        Prescaler++;
    }
    TimeReload=TimeReload&0xFFFF;
    Set_BitMask(TModeReg,Prescaler>>8);
    Write_Reg(TPrescalerReg,Prescaler&0xFF);            
    Write_Reg(TReloadMSBReg,TimeReload>>8);
    Write_Reg(TReloadLSBReg,TimeReload&0xFF);
    return TRUE;
}


uint8_t  Pcd_ConfigISOType(uint8_t type)
{
    if (type == 0)   {                                                  
        Set_BitMask(ControlReg, 0x10);                                
        Set_BitMask(TxAutoReg, 0x40);                                  
        Write_Reg(TxModeReg, 0x00);                
        Write_Reg(RxModeReg, 0x00);               
    }
    // if (type == 1) {                                                   
    //     Write_Reg(ControlReg,0x10);
    //     Write_Reg(TxModeReg,0x83);                                    
    //     Write_Reg(RxModeReg,0x83);                                     
    //     Write_Reg(TxAutoReg,0x00);
    //     Write_Reg(RxThresholdReg,0x55);
    //     Write_Reg(RFCfgReg,0x48);
    //     Write_Reg(TxBitPhaseReg,0x87);                                 
    //     Write_Reg(GsNReg,0x83);                                        
    //     Write_Reg(CWGsPReg,0x30);                                      
    //     Write_Reg(GsNOffReg,0x38);
    //     Write_Reg(ModGsPReg,0x20);
    // }
//    Delay100us(30);
    return TRUE;
}


uint8_t   FM175X_SoftReset(void)
{    
    Write_Reg(CommandReg,SoftReset);
    return Set_BitMask(ControlReg,0x10);                              
}


uint8_t  FM175X_HardReset(void)
{    
    CD_ClrTPD();
    Delay100us(1);
    CD_SetTPD();
    Delay100us(1);
    return TRUE;
}


uint8_t  FM175X_SoftPowerdown(void)
{
    if(Read_Reg(CommandReg)&0x10) {
        Clear_BitMask(CommandReg,0x10);                               
        return FALSE;
    }
    else
    Set_BitMask(CommandReg,0x10);                                     
    return TRUE;
}


uint8_t  FM175X_HardPowerdown(void)
{    
    //NPD=~NPD;
    //if(NPD==1)                                                         
    return TRUE;                                
//    else
    //return FALSE;                                                    
}


uint8_t  Read_Ext_Reg(uint8_t  reg_add)
{
    Write_Reg(0x0F,0x80+reg_add);
    return Read_Reg(0x0F);
}


uint8_t  Write_Ext_Reg(uint8_t  reg_add,uint8_t  reg_value)
{
    Write_Reg(0x0F,0x40+reg_add);
    return (Write_Reg(0x0F,0xC0+reg_value));
}

uint8_t  FM175XX_Initial(void)
{
    uint8_t  regdata,res;
    
    regdata = 0x20;         
    res = spi_SetReg(WaterLevelReg,regdata);
    if(res != TRUE)
    return FALSE;
    
    return TRUE;
}


void FM175XX_HPD(unsigned char mode)
{
	if(mode == 0x0)
		{
        CD_ClrTPD(); 
		}
	  else
		{
		    CD_SetTPD();
		    Delay100us(100);
		}
	return;
}





