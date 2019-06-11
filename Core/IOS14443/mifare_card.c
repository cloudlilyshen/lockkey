/****************************************Copyright (c)****************************************************
**                            Guangzhou ZLGMCU Technology Co., LTD
**
**                                 http://www.zlgmcu.com
**
**      ������������Ƭ���Ƽ����޹�˾���ṩ�����з�������ּ��Э���ͻ����ٲ�Ʒ���з����ȣ��ڷ������������?
**  ���κγ����ĵ������Խ����������֧�ֵ����Ϻ���Ϣ���������ο����ͻ���Ȩ��ʹ�û����вο��޸ģ�����˾��?
**  �ṩ�κε������ԡ��ɿ��Եȱ�֤�����ڿͻ�ʹ�ù��������κ�ԭ����ɵ��ر�ġ�żȻ�Ļ��ӵ���ʧ������˾��
**  �е��κ����Ρ�
**                                                                        ����������������Ƭ���Ƽ����޹�˾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           mifare_card.c
** Last modified Date:  2016-3-14
** Last Version:        V1.00
** Descriptions:        mifareϵ�п���ز���?
**
**--------------------------------------------------------------------------------------------------------
*/
#include "main.h"
#include "fm175xx.h"
#include "mifare_card.h"
#include "util.h"

/*********************************************************************************************************
** Function name:       Mifare_Auth    
** Descriptions:        mifare卡片校验
** input parameters:    mode，认证模式（0：key A认证，1：key B认证）
**                      sector，认证的扇区号（0~15）
**                      *mifare_key，6字节认证密钥数组
**                      card_uid 4字节卡片UID数组    
** output parameters:   
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
 uint8_t  Mifare_Auth(uint8_t  mode,uint8_t  sector,uint8_t  *mifare_key,uint8_t  *card_uid)
{
    uint8_t   send_buff[12],rece_buff[1],result;
    uint32_t  rece_bitlen;
    if(mode==0x0)
        send_buff[0]=0x60;                                               /* kayA认证 0x60                */
    if(mode==0x1)
        send_buff[0]=0x61;                                             /* keyB认证                     */
    send_buff[1] = sector*4;
    send_buff[2] = mifare_key[0];
    send_buff[3] = mifare_key[1];
    send_buff[4] = mifare_key[2];
    send_buff[5] = mifare_key[3];
    send_buff[6] = mifare_key[4];
    send_buff[7] = mifare_key[5];
    send_buff[8] = card_uid[0];
    send_buff[9] = card_uid[1];
    send_buff[10] = card_uid[2];
    send_buff[11] = card_uid[3];

    Pcd_SetTimer(5);
    Clear_FIFO();
    result =Pcd_Comm(MFAuthent,send_buff,12,rece_buff,&rece_bitlen);    /* Authent认证                  */
    if (result==TRUE) {
        if(Read_Reg(Status2Reg)&0x08)                                   /* 判断加密标志位，确认认证结果 */
            return TRUE;
        else
            return FALSE;
    }
    return FALSE;
}

/*********************************************************************************************************
** Function name:       Mifare_Blockset    
** Descriptions:        mifare卡设置卡片计数值
** input parameters:    block:块号
**                      buff:4字节初始值
** output parameters:   
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Mifare_Blockset(uint8_t  block,uint8_t  *buff)
{
    uint8_t   block_data[16],result;
    block_data[0]=buff[3];
    block_data[1]=buff[2];
    block_data[2]=buff[1];
    block_data[3]=buff[0];
    block_data[4]=~buff[3];
    block_data[5]=~buff[2];
    block_data[6]=~buff[1];
    block_data[7]=~buff[0];
    block_data[8]=buff[3];
    block_data[9]=buff[2];
    block_data[10]=buff[1];
    block_data[11]=buff[0];
    block_data[12]=block;
    block_data[13]=~block;
    block_data[14]=block;
    block_data[15]=~block;
    result= Mifare_Blockwrite(block,block_data);
    return result;
}

/*********************************************************************************************************
** Function name:       Mifare_Blockread    
** Descriptions:        mifare卡读取块函数
** input parameters:    block:块号   0x00-0x3f       
** output parameters:   buff:读出16字节数据 
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Mifare_Blockread(uint8_t  block,uint8_t  *buff)
{    
    uint8_t   send_buff[2],result;
    uint32_t  rece_bitlen;
    Set_BitMask(TxModeReg,0x80);                                        /* ��TX CRC                   */
    Set_BitMask(RxModeReg,0x80);                                        /* ��RX CRC                   */
    Pcd_SetTimer(1);
    send_buff[0]=0x30;                                                  /* ����������                   */
    send_buff[1]=block;                                                 /* ��������ַ                   */
    Clear_FIFO();
    result =Pcd_Comm(Transceive,send_buff,2,buff,&rece_bitlen);//
    if ((result!=TRUE )|(rece_bitlen!=16*8))                            /* ���յ������ݳ���Ϊ16         */
        return FALSE;
    return TRUE;
}

/*********************************************************************************************************
** Function name:       mifare_blockwrite    
** Descriptions:        mifare卡写块函数
** input parameters:    block:块号   0x00-0x3f    
**                      buff:读出16字节数据 
** output parameters:   
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Mifare_Blockwrite(uint8_t  block,uint8_t  *buff)
{    
    uint8_t   result,send_buff[16],rece_buff[1];
    uint32_t  rece_bitlen;
    Set_BitMask(TxModeReg,0x80);                                        /* ��TX CRC                   */
    Clear_BitMask(RxModeReg,0x80);                                      /* �ر�RX CRC                   */
    Pcd_SetTimer(1);
    send_buff[0]=0xa0;                                                  /* д������                     */
    send_buff[1]=block;                                                 /* ����?                       */

    Clear_FIFO();
    result =Pcd_Comm(Transceive,send_buff,2,rece_buff,&rece_bitlen);
    if ((result!=TRUE )|((rece_buff[0]&0x0F)!=0x0A))                      /* ���δ���յ�?0x0A����ʾ��ACK  */
        return(FALSE);
		
    Pcd_SetTimer(5);
    Clear_FIFO();
    result =Pcd_Comm(Transceive,buff,16,rece_buff,&rece_bitlen);
    if ((result!=TRUE )|((rece_buff[0]&0x0F)!=0x0A))                    /* ���δ���յ�?0x0A����ʾ��ACK  */
        return FALSE;
    return TRUE;
}

/*********************************************************************************************************
** Function name:       mifare_blockwrite    
** Descriptions:        mifare卡写块函数
** input parameters:    block:块号   0x00-0x3f    
**                      buff:读出16字节数据 
** output parameters:   
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t Mifare_BlockSet(uint8_t block,uint8_t *buff)
{
    uint8_t  block_data[17],result;
    block_data[0]=buff[3];
    block_data[1]=buff[2];
    block_data[2]=buff[1];
    block_data[3]=buff[0];
    block_data[4]=~buff[3];
    block_data[5]=~buff[2];
    block_data[6]=~buff[1];
    block_data[7]=~buff[0];
    block_data[8]=buff[3];
    block_data[9]=buff[2];
    block_data[10]=buff[1];
    block_data[11]=buff[0];
    block_data[12]=block;
    block_data[13]=~block;
    block_data[14]=block;
    block_data[15]=~block;
    result= Mifare_Blockwrite(block,block_data);
    return result;
}

/*********************************************************************************************************
** Function name:       Mifare_Blockset        
** Descriptions:        mifare卡设置卡片计数值
** input parameters:    block:块号
**                      buff:4字节初始值
** output parameters:   
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t Mifare_BlockGet(uint8_t blockNum,uint8_t *buff)
{
    uint8_t result,i,num;
    result = Mifare_Blockread(blockNum,buff);              /* �������ݺ���                 */
    if( result == TRUE )
    {
        for(i=0;i<4;i++)
        {
             num = ~buff[i+4];
             if( buff[i] !=  num )
             {
                 result = FALSE;
             }
             if(buff[i] != buff[i+8])
             {
                 result = FALSE;
             }
        } 
        if(buff[12] != buff[14])
            result = FALSE;
        if(buff[13] != buff[15])
            result = FALSE;
        num = ~buff[13];
        if(buff[12] != num )
            result = FALSE;
    }
    num = buff[0];
    buff[0] = buff[3];
    buff[3] = num;
    num = buff[1];
    buff[1] = buff[2];
    buff[2] = num;
    return result;
}
/*********************************************************************************************************
** Function name:       Mifare_Blockdec    
** Descriptions:        mifare卡片减值操作    
** input parameters:    block:块号   0x00-0x3f    
**                      buff:减少4字节数据 
** output parameters:   
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Mifare_Blockinc(uint8_t  block,uint8_t  *buff)
{    
    uint8_t   result,send_buff[2],rece_buff[1];
    uint32_t  rece_bitlen;
    Set_BitMask(TxModeReg,0x80);                                        /* ��TX CRC                   */
    Clear_BitMask(RxModeReg,0x80);                                      /* �ر�RX CRC                   */
    Pcd_SetTimer(5);
    send_buff[0]=0xc1;                                                  /* ��ֵ��������                 */
    send_buff[1]=block;                                                 /* ����?                       */
    Clear_FIFO();
    result = Pcd_Comm(Transceive,send_buff,2,rece_buff,&rece_bitlen);
    if ((result != TRUE )|((rece_buff[0]&0x0F) != 0x0A))                  /* ���δ���յ�?0x0A����ʾ��ACK  */
        return FALSE;
    Pcd_SetTimer(5);
    Clear_FIFO();
    Pcd_Comm(Transceive,buff,4,rece_buff,&rece_bitlen);
    return result;
}


/*********************************************************************************************************
** Function name:       Mifare_Transfer    
** Descriptions:        mifare卡片传输块
** input parameters:    block:块号   0x00-0x3f    
** output parameters:   
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Mifare_Blockdec(uint8_t  block,uint8_t  *buff)
{    
    uint8_t   result,send_buff[2],rece_buff[1];
    uint32_t  rece_bitlen;
    Set_BitMask(TxModeReg,0x80);                                        /* ��TX CRC                   */
    Clear_BitMask(RxModeReg,0x80);                                      /* �ر�RX CRC                   */
    Pcd_SetTimer(5);
    send_buff[0]=0xc0;
    send_buff[1]=block;                                                 /* ����?                       */
    Clear_FIFO();
    result = Pcd_Comm(Transceive,send_buff,2,rece_buff,&rece_bitlen);
    if ((result!=TRUE )|((rece_buff[0]&0x0F)!=0x0A))                      /* ���δ���յ�?0x0A����ʾ��ACK  */
        return FALSE;
    Pcd_SetTimer(5);
    Clear_FIFO();
    Pcd_Comm(Transceive,buff,4,rece_buff,&rece_bitlen);
    return result;
}


/*********************************************************************************************************
** Function name:       Mifare_Restore    
** Descriptions:        mifare卡片储存命令
** input parameters:    block:块号   0x00-0x3f    
** output parameters:   
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Mifare_Transfer(uint8_t  block)
{    
    uint8_t    result,send_buff[2],rece_buff[1];
    uint32_t   rece_bitlen;
    Set_BitMask(TxModeReg,0x80);                                        /* ��TX CRC                   */
    Clear_BitMask(RxModeReg,0x80);                                      /* �ر�RX CRC                   */
    Pcd_SetTimer(5);
    send_buff[0] = 0xb0;
    send_buff[1] = block;                                               /* ����?                       */
    Clear_FIFO();
    result=Pcd_Comm(Transceive,send_buff,2,rece_buff,&rece_bitlen);
    if ((result != TRUE )|((rece_buff[0]&0x0F) != 0x0A))                /* ���δ���յ�?0x0A����ʾ��ACK  */
        return FALSE;
    return result;
}


/*********************************************************************************************************
** Function name:       Mifare_Blockinc        
** Descriptions:        mifare卡片增值操作    
** input parameters:    incOrDec:inc 0xc1 dec 0xc0   
**                      buff:增加4字节数据 
** output parameters:   
** Returned value:      TRUE：操作成功 ERROR：操作失败    
*********************************************************************************************************/
uint8_t  Mifare_Restore(uint8_t  block)
{    
    uint8_t  result,send_buff[4],rece_buff[1];
    uint32_t  rece_bitlen;
    Set_BitMask(TxModeReg,0x80);                                        /* ��TX CRC                   */
    Clear_BitMask(RxModeReg,0x80);                                      /* �ر�RX CRC                   */
    Pcd_SetTimer(5);
    send_buff[0]=0xc2;
    send_buff[1]=block;                                                 /* ����?                       */
    Clear_FIFO();
    result = Pcd_Comm(Transceive,send_buff,2,rece_buff,&rece_bitlen);
    if ((result != TRUE )|((rece_buff[0]&0x0F) != 0x0A))                  /* ���δ���յ�?0x0A����ʾ��ACK  */
        return FALSE;
    Pcd_SetTimer(5);
    send_buff[0]=0x00;
    send_buff[1]=0x00;
    send_buff[2]=0x00;
    send_buff[3]=0x00;
    Clear_FIFO();
    Pcd_Comm(Transceive,send_buff,4,rece_buff,&rece_bitlen);
    return result;
}

/*********************************************************************************************************
** Function name:       Mifare_Blockinc        
** Descriptions:        mifare��Ƭ��ֵ����    
** input parameters:    incOrDec:inc 0xc1 dec 0xc0   
**                      buff:����4�ֽ����� 
** output parameters:   
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t Mifare_BlockValue(uint8_t block,uint8_t incOrDec,uint8_t *buff)
{    
    uint8_t   result,send_buff[4],rece_buff[1];
    uint32_t  rece_bitlen;
    Set_BitMask(TxModeReg,0x80);                                        /* ��TX CRC                   */
    Clear_BitMask(RxModeReg,0x80);                                      /* �ر�RX CRC                   */
    Pcd_SetTimer(5);
    send_buff[0]=  incOrDec;
    send_buff[1]=  block;                                               /* ����?                       */
    Clear_FIFO();
    result = Pcd_Comm(Transceive,send_buff,2,rece_buff,&rece_bitlen);
    if ((result!=TRUE )|((rece_buff[0]&0x0F)!=0x0A))                    /* ���δ���յ�?0x0A����ʾ��ACK  */
        return FALSE;
    Pcd_SetTimer(5);
    Clear_FIFO();
    send_buff[0] = buff[3];
    send_buff[1] = buff[2];
    send_buff[2] = buff[1];
    send_buff[3] = buff[0];
    result = Pcd_Comm(Transceive,send_buff,4,rece_buff,&rece_bitlen);
    if (result!= CMD_TIMER_OVER )                                       /* ���δ���յ�?0x0A����ʾ��ACK  */
        return FALSE;
    return TRUE;
}
