#include "main.h"
#include "util.h"

uint8_t CommCheckHeadTailCS(uint8_t* dat,uint16_t len)
{
	if((dat[0] != '#')||(dat[len-1] != '*'))
	{
		return 0;
	}
	if(ModbusCS_CalcForProtocol(dat,(len-2)) != (dat[len-2]))
	{
		return 0;
	}
	return 1;
}

uint8_t CommCheckATCMD(uint8_t* dat,uint16_t len)
{
	if((dat[0] != 'A')||(dat[0] != 'a'))
	{
		return 0;
	}
	if((dat[1] != 'T')||(dat[1] != 't'))
	{
		return 0;
	}	
	if(dat[len-1] != '\r')
	{
		return 0;
	}	
	return 1;
}


uint32_t FourBytes2uint32BigEndian(uint8_t* dat)
{
	return (dat[0]<<24)+(dat[1]<<16)+(dat[2]<<8)+dat[3];
}

int32_t FourBytes2int32BigEndian(uint8_t* dat)
{
	return (dat[0]<<24)+(dat[1]<<16)+(dat[2]<<8)+dat[3];
}

uint32_t FourBytes2uint32LittleEndian(uint8_t* dat)
{
	return (dat[3]<<24)+(dat[2]<<16)+(dat[1]<<8)+dat[0];
}

int32_t FourBytes2int32LittleEndian(uint8_t* dat)
{
	return (dat[3]<<24)+(dat[2]<<16)+(dat[1]<<8)+dat[0];
}


uint32_t ThreeBytes2uint32BigEndian(uint8_t* dat)
{
	return (dat[0]<<16)+(dat[1]<<8)+dat[2];
}

int32_t ThreeBytes2int32BigEndian(uint8_t* dat)
{
	return (dat[0]<<16)+(dat[1]<<8)+dat[2];
}

uint32_t ThreeBytes2uint32LittleEndian(uint8_t* dat)
{
	return (dat[2]<<16)+(dat[1]<<8)+dat[0];
}

int32_t ThreeBytes2int32LittleEndian(uint8_t* dat)
{
	return (dat[2]<<16)+(dat[1]<<8)+dat[0];
}


uint16_t TwoBytes2uint16BigEndian(uint8_t* dat)
{
	return ((dat[0]<<8)+dat[1]);
}

int16_t TwoBytes2int16BigEndian(uint8_t* dat)
{
	return ((dat[0]<<8)+dat[1]);
}

uint16_t TwoBytes2uint16LittleEndian(uint8_t* dat)
{
	return ((dat[1]<<8)+dat[0]);
}

int16_t TwoBytes2int16LittleEndian(uint8_t* dat)
{
	return ((dat[1]<<8)+dat[0]);
}

void uint32ToFourBytesBigEndian(uint32_t dat,uint8_t* dst)
{
	*dst++ = (dat>>24);
	*dst++ = (dat>>16);
	*dst++ = (dat>>8);
	*dst = dat;
}


void uint32ToFourBytesLittleEndian(uint32_t dat,uint8_t* dst)
{
	*dst++ = dat;
	*dst++ = (dat>>8);
	*dst++ = (dat>>16);
	*dst = (dat>>24);
}



void Delay100us(uint32_t _100us)
{
    uint32_t i;
    for ( ; 0 != _100us; _100us--) {
        for(i = 0; i < 1000; i++);
    }
}
                        
void mDelay( uint32_t ms)
{
    Delay100us(10*ms);
}


void uDelay(unsigned int us)			
{
	unsigned int i;
	unsigned int j;
	for(j=0;j<us;j++)
	{
		for(i=0;i<3;i++)
			;
	}
}
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


uint8_t get_chk_bcc(uint8_t *dat, uint32_t len)
{
    uint8_t temp=0;
    for(uint8_t i=0;i<len;i++)
    {
        temp=dat[i]^temp;
    }
    return (temp);
}
uint8_t ModbusCS_CalcForProtocol(uint8_t* dat,uint16_t len)
{
    uint16_t csData = 0;
	uint8_t csDataMod=0;

    
	while(len--)
	{
		csData   += *(dat++);
	}
	csDataMod = csData%256;
	return csDataMod;	
}
uint32_t String2Uint(uint8_t *src,uint16_t len)  
{    
 uint32_t num = 0;  
 uint32_t val = 0;  
 for(uint16_t i=0;i<len;i++)	
 {  
  val = *src- '0';  
  num = num * 10 + val;   
  src++;  
 }  
  return num;  
}  




