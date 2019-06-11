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
** File name:           mifare_card.h
** Last modified Date:  2016-01-04
** Last Version:        V1.00
** Descriptions:        The LPC824 IO function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          LinShanwen
** Created date:        2016-01-04
** Version:             V1.00
** Descriptions:        
*/
#ifndef __MIFARE_CARD_H
#define __MIFARE_CARD_H


#include <stdint.h>
uint8_t  Mifare_Transfer(uint8_t  block);
uint8_t  Mifare_Restore(uint8_t  block);
uint8_t  Mifare_Blockset(uint8_t  block,uint8_t  *buff);
uint8_t  Mifare_Blockinc(uint8_t  block,uint8_t  *buff);
uint8_t  Mifare_Blockdec(uint8_t  block,uint8_t  *buff);
uint8_t  Mifare_Blockwrite(uint8_t  block,uint8_t  *buff);
uint8_t  Mifare_Blockread(uint8_t  block,uint8_t  *buff);
uint8_t  Mifare_Auth(uint8_t  mode,uint8_t  sector,uint8_t  *mifare_key,uint8_t  *card_uid);
uint8_t Mifare_BlockSet(uint8_t block,uint8_t *buff);
uint8_t Mifare_BlockGet(uint8_t blockNum,uint8_t *buff);
uint8_t Mifare_BlockValue(uint8_t incOrDec,uint8_t block,uint8_t *buff);


#endif //__MIFARE_CARD_H

