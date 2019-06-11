#ifndef ULTRALIGHT_H
#define ULTRALIGHT_H 1


extern uint8_t  Ultra_Read(uint8_t  block_address,uint8_t  *buff);
extern uint8_t  Ultra_Write(uint8_t  block_address,uint8_t  *buff);

#endif
