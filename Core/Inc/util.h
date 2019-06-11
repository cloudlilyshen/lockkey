#ifndef __UTIL_H_
#define __UTIL_H_

#include "rs485.h"


uint8_t ModbusCheckCRC(uint8_t* recvBuffer);


void ModbusCreatePackage(uint8_t* sendBuffer,uint8_t destAddr,uint8_t framsNo,uint8_t funcCode,
                               uint16_t regAddr,uint16_t regValue);




uint16_t ModbusCreatePackageVarLen(uint8_t* sendBuffer,uint8_t* srcBuffer,uint8_t destAddr,uint8_t framsNo,uint8_t funcCode,
                               uint16_t bar_serial,uint16_t num_serial,uint16_t len);	
                               
uint8_t CommCheckHeadTailCS(uint8_t* dat,uint16_t len);   
uint8_t CommCheckATCMD(uint8_t* dat,uint16_t len);


uint32_t FourBytes2uint32BigEndian(uint8_t* dat);
int32_t FourBytes2int32BigEndian(uint8_t* dat);
uint32_t FourBytes2uint32LittleEndian(uint8_t* dat);
int32_t FourBytes2int32LittleEndian(uint8_t* dat);

uint32_t ThreeBytes2uint32BigEndian(uint8_t* dat);
int32_t ThreeBytes2int32BigEndian(uint8_t* dat);
uint32_t ThreeBytes2uint32LittleEndian(uint8_t* dat);
int32_t ThreeBytes2int32LittleEndian(uint8_t* dat);

uint16_t TwoBytes2uint16BigEndian(uint8_t* dat);
int16_t TwoBytes2int16BigEndian(uint8_t* dat);
uint16_t TwoBytes2uint16LittleEndian(uint8_t* dat);
int16_t TwoBytes2int16LittleEndian(uint8_t* dat);


void uint32ToFourBytesBigEndian(uint32_t dat,uint8_t* dst);
void uint32ToFourBytesLittleEndian(uint32_t dat,uint8_t* dst);
uint32_t GetByteSum(const void *p, uint32_t nBytes);
uint8_t GetByteBCC(const void *p, uint32_t nBytes);
void uDelay(unsigned int us);
void mDelay(uint32_t ms);
void Delay100us(uint32_t _100us);
uint8_t get_chk_bcc(uint8_t *dat, uint32_t len);
uint32_t String2Uint(uint8_t *src,uint16_t len);
uint8_t ModbusCS_CalcForProtocol(uint8_t* dat,uint16_t len);
#endif





