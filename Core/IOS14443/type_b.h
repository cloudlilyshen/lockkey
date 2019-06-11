#ifndef TYPE_B_H
#define TYPE_B_H 1
extern uint8_t  PUPI[4];
extern uint8_t  TypeB_Request(uint32_t *rece_len,uint8_t  *buff,uint8_t  *card_sn);
extern uint8_t  TypeB_Select(uint8_t  *card_sn,uint32_t *rece_len,uint8_t  *buff);
extern uint8_t  TypeB_GetUID(uint32_t *rece_len,uint8_t  *buff);
extern uint8_t  TypeB_Halt(uint8_t  *card_sn);
extern uint8_t  TypeB_WUP(uint32_t *rece_len,uint8_t  *buff,uint8_t  *card_sn);
#endif
