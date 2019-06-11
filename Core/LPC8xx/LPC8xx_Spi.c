/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:       LPC8xx_Spi.c
** Descriptions:    SPI��������
**
**------------------------------------------------------------------------------------------------------
** Created by:      ������
** Created date:    2016-01-04
** Version:         1.0
** Descriptions:    The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:     
** Modified date:   
** Version:         
** Descriptions:    
********************************************************************************************************/
#include   "LPC8xx.h"
#include   "LPC8xx_IO.h"
#include   <stdint.h>

/*
 * ��ֲ��������ʱ��Ҫ�޸����µĺ������ 
 */
#define     SSEL_EN         (0 << 16)
#define     SSEL_DIS        (1 << 16)
#define     EOT_EN          (1 << 20)
#define     EOT_DIS         (0 << 20)
#define     EOF_EN          (1 << 21)
#define     EOF_DIS         (0 << 21)
#define     RXIGNORE_EN     (1 << 22)
#define     RXIGNORE_DIS    (0 << 22)
#define     FLEN(n)         (((n) - 1) << 24)

#define TRUE  1
#define FALSE 0


/*********************************************************************************************************
** �������ƣ�SPIInit
** ����˵������ʼ��SPI
** �����������
** �����������
*********************************************************************************************************/
void  SPI_Init (void)
{
	
    /*
     * ����SPI0��SPI0_SCK ~ P0.6��SPI0_MOSI ~ P0.16��SPI0_MISO ~ P0.17��SPI0_SSEL ~ P0.14
     */
      
    LPC_SWM->PINASSIGN[3] &= ~( 0xFFUL << 24 );
    LPC_SWM->PINASSIGN[3] |=  ( 26  << 24 );                            /* SPI0_SCK  ~ P0.18            */ 
    LPC_SWM->PINASSIGN[4] &= ~( 0xFFFFUL <<  0 );
    LPC_SWM->PINASSIGN[4] |=  ( 27 <<  0 );                             /* SPI0_MOSI ~ P0.19            */
    LPC_SWM->PINASSIGN[4] |=  ( 16 <<  8 );                             /* SPI0_MISO ~ P0.20            */
    //LPC_SWM->PINASSIGN[4] |=  ( 24 << 16 );                             /* SPI0_SSEL ~ P0.17            */
    CD_CfgNSS();
    CD_DisNSS();
    LPC_SYSCON->SYSAHBCLKCTRL |=  (1 << 11);                            /* ��ʼ��SPI0 AHBʱ��           */
    LPC_SYSCON->PRESETCTRL    &= ~(1 << 0);                             /* ��λSPI0                     */
    LPC_SYSCON->PRESETCTRL    |=  (1 << 0);
    
    LPC_SPI0->DIV = SystemCoreClock / 1000000 - 1;                      /* ����ģʽ��SCKƵ�ʷ�Ƶ����    */
    LPC_SPI0->DLY = (0 << 0) |                                          /* ��ʼ�����ӳ٣�0 * T_SCK      */
                    (0 << 4) |                                          /* ���������ӳ٣�0 * T_SCK      */
                    (0 << 8) |                                          /* ֡������ӳ٣�0 * T_SCK      */
                    (0 << 12);                                          /* ��������ӳ٣�0 * T_SCK      */
    
    /*
     * TXCTRL ���Ϳ��������ã�
     * TXCTRL
     *      16������ӻ�ѡ��    0��ѡ��ӻ�     1��ȡ��ѡ��ӻ�     ����д����ǰSSEL�źŵ���Ч��
     *      20�������������    0�����䲻����   1���������         ����ÿ��д���ݺ�SSEL�źŵ���Ч��
     *      21��֡��������      0��֡������     1��֡����           �����´�д����ǰ�Ƿ����֡���ӳ�
     *      22�����պ��Կ���    0�������Խ���   1�����Խ���
     *   27:24��֡����          0x0 ~ 0xF����Ӧ 1 ~ 16 λ֡����
     */
    LPC_SPI0->TXCTRL = (1 << 16) |                                      /* �ӻ�ѡ��                     */
                       (0 << 20) |                                      /* ���䲻����                   */
                       (1 << 21) |                                      /* ֡����                       */
                       (0 << 22) |                                      /* �����Խ���                   */
                       (7 << 24);                                       /* ֡���ȣ�8λ                  */
    
    /*
     * CFG ���üĴ������ã�
     * CFG
     *      0��SPIʹ��λ        0��SPI����      1��SPIʹ��
     *      2��ģʽѡ��λ       0��SPI�ӻ�      1��SPI����
     *      3������λ��ѡ��     0����׼(MSBF)   1����ת(LSBF)
     *      4��ʱ����λѡ��     0����1���ز���  1����2���ز���
     *      5��ʱ�Ӽ���ѡ��     0��֡��SCKΪ0   1��֡��SCKΪ1
     *      7����дģʽʹ��     0����ֹ��д     1��ʹ�ܻ�д
     *      8��SSEL��Ч����ѡ�� 0���͵�ƽ��Ч   1���ߵ�ƽ��Ч
     */
    LPC_SPI0->CFG = (1 << 0) |                                          /* SPIʹ��                      */
                    (1 << 2) |                                          /* ����ģʽ                     */
                    (0 << 3) |                                          /* MSBF                         */
                    (0 << 4) |                                          /* CPHA = 0                     */
                    (0 << 5) |                                          /* CPOL = 0                     */
                    (0 << 7) |                                          /* ��ֹ��д                     */
                    (0 << 8);                                           /* SSEL�͵�ƽ��Ч               */
}

/*********************************************************************************************************
** ��������: SendRecv_Byte
** �������ܣ�һ�δ����ڲ��ĵ��ֽ�֡���������
** �������: �������Ϸ��ͳ�������
** �������: �������Ͻ��յ�������
** �� �� ֵ����
*********************************************************************************************************/
uint8_t SPI_SendByte (uint32_t ucData)
{
    while (!(LPC_SPI0->STAT & (1 << 1)));                               /* �ȴ�����׼������             */
    LPC_SPI0->TXDATCTL =  ucData;                                       /* 8 λ��֡����                 */    
    
    while (!(LPC_SPI0->STAT & (1 << 0)));                               /* �ȴ������������             */
    ucData = LPC_SPI0->RXDAT;                                           /* ��������                     */
    
    return ucData;
}


/*********************************************************************************************************
** Function name:       spi_SetReg
** Descriptions:        SPIд����оƬ�Ĵ�������
** input parameters:    ucRegAddr���Ĵ�����ַ
**                      ucRegVal��Ҫд���ֵ
** output parameters:   ��
** Returned value:      TRUE
*********************************************************************************************************/
uint8_t spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal)
{
    CD_EnNSS ();
    SPI_SendByte ( FLEN(8)  | (ucRegAddr<<1) );                         /* 8 λ��֡����                 */ 
      SPI_SendByte ( FLEN(8) | EOF_EN | ucRegVal );
    CD_DisNSS ();
    return TRUE;
}

/*********************************************************************************************************
** Function name:       spi_GetReg
** Descriptions:        SPI������оƬ�Ĵ�������
** input parameters:    ucRegAddr���Ĵ�����ַ
** output parameters:   ��
** Returned value:      Ŀ��Ĵ�����ֵ
*********************************************************************************************************/
uint8_t spi_GetReg(uint8_t ucRegAddr)
{
    unsigned char ucRegVal;
      CD_EnNSS ();
    SPI_SendByte ( FLEN(8)  | 0x80|(ucRegAddr<<1) );
      ucRegVal = SPI_SendByte ( FLEN(8) | EOF_EN  | 0x00 );
      CD_DisNSS ();
    return ucRegVal;
}


/*********************************************************************************************************
** Function name:       spi_GetReg
** Descriptions:        SPI������оƬ�Ĵ�������
** input parameters:    ucRegAddr���Ĵ�����ַ
** output parameters:   ��
** Returned value:      Ŀ��Ĵ�����ֵ
*********************************************************************************************************/
uint8_t spi_GetReg2(uint8_t ucRegAddr,uint8_t *p)
{
      CD_EnNSS ();
    SPI_SendByte ( FLEN(8)  | 0x80|(ucRegAddr<<1) );
      *p = SPI_SendByte ( FLEN(8) | EOF_EN  | 0x00 );
      CD_DisNSS ();
    return TRUE;
}

/*********************************************************************************************************
** Function name:       SPIRead_Sequence
** Descriptions:        SPI��FIFO�Ĵ�����ֵ
** input parameters:    sequence_length ���ݳ��� ucRegAddr���Ĵ�����ַ  *reg_value ����ָ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void SPIRead_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value)    
{
    uint8_t i;
    if (sequence_length==0)
    return;
    CD_EnNSS ();
    for(i=0;i<sequence_length;i++) {
       *(reg_value+i) = spi_GetReg(ucRegAddr);
    }
      CD_DisNSS ();
    return;

}

/*********************************************************************************************************
** Function name:       SPIWrite_Sequence
** Descriptions:        SPIдFIFO��ֵ
** input parameters:    sequence_length ���ݳ��� 
**                      ucRegAddr���Ĵ�����ַ  
**                      *reg_value ����ָ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void SPIWrite_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value)
{
    
    uint8_t i;
    if(sequence_length==0)
        return;
    for(i=0;i<sequence_length;i++) {
       spi_SetReg(ucRegAddr, *(reg_value+i));
    }
    CD_DisNSS ();
    return ;    
}




