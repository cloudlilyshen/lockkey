#ifndef _15693_H
#define _15693_H
extern uint8_t  rfid[8];

extern uint8_t  ISO15693_Init(void);
extern uint8_t  ISO15693_Stayquite(void);
extern uint8_t  ISO15693_Inventory(uint8_t  *rece_len,uint8_t  *buff,uint8_t  *card_uid,uint8_t  *slot_num);
extern uint8_t  ISO15693_Select(uint8_t  *card_uid,uint8_t  *rece_len,uint8_t  *buff);
extern uint8_t  ISO15693_Read(uint8_t  block,uint8_t  *rece_len,uint8_t  *buff);
extern uint8_t  ISO15693_Write(uint8_t  card_type,uint8_t  block, uint8_t  *block_data,uint8_t  *rece_len,uint8_t  *buff);
extern uint8_t  ISO15693_Getinfo(uint8_t  *rece_len,uint8_t  *buff);
extern uint8_t  ISO15693_Inventory_16slot(uint8_t  *card_num,uint8_t  *card_uid);
extern uint8_t  ISO15693_Mult_Read(uint8_t  block1,uint8_t  block2,uint8_t  *rece_len,uint8_t  *buff);
#endif
