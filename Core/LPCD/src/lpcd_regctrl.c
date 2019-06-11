/*********************************************************************
*                                                                    *
*   Copyright (c) 2010 Shanghai FuDan MicroElectronic Inc, Ltd.      *
*   All rights reserved. Licensed Software Material.                 *
*                                                                    *
*   Unauthorized use, duplication, or distribution is strictly       *
*   prohibited by law.                                               *
*                                                                    *
*********************************************************************/

/*********************************************************************************************************
**     FM175XX扩展寄存器关函数                                            
**     主要功能:                                                         
**     1.实现扩展寄存器的读、写、修改等配置                         
**     编制:罗挺松                                                      
**     编制时间:2014年7月18日                                                                     
*********************************************************************************************************/
#include "FM175XX_REG.h"
#include "MyType.h"
#include "LPC8xx_Spi.h"

/*********************************************************************************************************
** Function name:       GetReg_Ext(uint8_t  ExtRegAddr,uint8_t * ExtRegData)
** Descriptions:        读取扩展寄存器值
** input parameters:    ExtRegAddr:扩展寄存器地址   
** output parameters:   ExtRegData:读取的值
** Returned value:      uint8_t   TRUE：读取成功   FALSE:失败
*********************************************************************************************************/
uint8_t  GetReg_Ext(uint8_t  ExtRegAddr,uint8_t * ExtRegData)
{
    uint8_t  res;
    uint8_t  addr,regdata;

    addr = JREG_EXT_REG_ENTRANCE;                                       /* 扩展寄存器0x0f地址           */
    regdata = JBIT_EXT_REG_RD_ADDR + ExtRegAddr;                    /* JBIT_EXT_REG_RD_DATA写入二级地址 */
                                                                        /* ExtRegAddr 扩展寄存器地址    */
    res = spi_SetReg(addr,regdata);                                     /* 写入扩展寄存器地址           */
    if (res == FALSE) 
        return FALSE;

    addr = JREG_EXT_REG_ENTRANCE;                                       /* 扩展寄存器0x0f地址           */
    res = spi_GetReg2(addr,&regdata);                                   /* 读出扩展寄存器数据           */
    if (res == FALSE) 
        return FALSE;
    *ExtRegData = regdata;

    return TRUE;    
}


/*********************************************************************************************************
** Function name:       SetReg_Ext(uint8_t  ExtRegAddr,uint8_t * ExtRegData)
** Descriptions:        写扩展寄存器
** input parameters:    ExtRegAddr:扩展寄存器地址   
** output parameters:   ExtRegData:要写入的值
** Returned value:      uint8_t   TRUE：读取成功   FALSE:失败
*********************************************************************************************************/
uint8_t  SetReg_Ext(uint8_t  ExtRegAddr,uint8_t  ExtRegData)
{
    uint8_t  res;
    uint8_t  addr,regdata;

    addr = JREG_EXT_REG_ENTRANCE;                                       /* 扩展寄存器入口地址0x0f       */
    regdata = JBIT_EXT_REG_WR_ADDR + ExtRegAddr;                        /* 写入扩展寄存器地址           */
    res = spi_SetReg(addr,regdata);
    if (res == FALSE) 
        return FALSE;

    addr = JREG_EXT_REG_ENTRANCE;
    regdata = JBIT_EXT_REG_WR_DATA + ExtRegData;                        /* 读出扩展寄存器数据           */
    res = spi_SetReg(addr,regdata);
    if (res == FALSE) 
        return FALSE;
        
    return TRUE;    
}

/*********************************************************************************************************
** Function name:       modifyReg_Ext(uint8_t  ExtRegAddr,uint8_t * mask,uint8_t  set)
** Descriptions:        寄存器位操作
** input parameters:    ExtRegAddr:目标寄存器地址   mask:要改变的位  
**                      set:  0:标志的位清零   其它:标志的位置起
** output parameters:  
** Returned value:      uint8_t   TRUE：读取成功   FALSE:失败
*********************************************************************************************************/
uint8_t  ModifyReg_Ext(uint8_t  ExtRegAddr,uint8_t  mask,uint8_t  set)
{
    uint8_t  status;
    uint8_t  regdata;
    
    status = GetReg_Ext(ExtRegAddr,&regdata);
    if(status == TRUE) {
        if(set) {
            regdata |= mask;
            }
        else {
            regdata &= ~(mask);
        }
    }
    else
        return FALSE;

    status = SetReg_Ext(ExtRegAddr,regdata);
    return status;
}


