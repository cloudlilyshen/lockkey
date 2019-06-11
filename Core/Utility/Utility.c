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
** File name:           Utility.c
** Last modified Date:  2016-3-4
** Last Version:        V1.00
** Descriptions:        ���Ӽ����ֽ����ȡ��У�麯��
**
**--------------------------------------------------------------------------------------------------------
*/
#include "Utility.h"


/*********************************************************************************************************
** Function name:        GetByteSum
** Descriptions:        �����ܺ�
** input parameters:    *p      -- ���������
**                      nBytes  -- �ֽ���
** output parameters:   N/A
** Returned value:      �ֽ��ۼӺ�
*********************************************************************************************************/
uint32_t GetByteSum(const void *p, uint32_t nBytes)
{
    const uint8_t	*pBuf	= (const uint8_t *)p;
    uint32_t		sum		= 0;
    while (nBytes--) {
		    sum += *pBuf++;
    }
    return sum;
}


/*********************************************************************************************************
** Function name:        �����ֽ����ȡ��
** Descriptions:        �ӳ٣�100usΪ��λ�� ����LPC824,24MHz
** input parameters:    *p      -- ���������
**                      nBytes  -- �ֽ���
** output parameters:   N/A
** Returned value:      �ֽ����ȡ��
*********************************************************************************************************/
uint8_t GetByteBCC(const void *p, uint32_t nBytes)
{
    const uint8_t *pBuf = (const uint8_t *)p;
    uint8_t		BCC = 0;
    uint32_t	i   = 0;

    for (i = 0; i < nBytes; i++) {
        BCC ^= *(pBuf + i);
    }
    return ~BCC;
}

uint32_t GetByteSum(const void *p, uint32_t nBytes)
{
    const uint8_t	*pBuf	= (const uint8_t *)p;
    uint32_t		sum		= 0;
    while (nBytes--) {
		    sum += *pBuf++;
    }
    return sum;
}
/*********************************************************************************************************
** Function name:       Delay100us 
** Descriptions:        �ӳ٣�100usΪ��λ�� ����LPC824,24MHz
** input parameters:    N/A
** output parameters:   N/A
** Returned value:      
*********************************************************************************************************/
