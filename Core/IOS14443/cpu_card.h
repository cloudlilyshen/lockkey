#ifndef CPU_CARD_H
#define CPU_CARD_H 1
//struct apdu_frame
//{
//uint8_t  pcb;
//uint8_t  cid;
//uint8_t  buff[62];
//};

extern uint8_t   PCB,CID;

extern void CPU_PCB(void);
extern uint8_t  CPU_BaudRate_Set(uint8_t  mode);
extern uint8_t  CPU_TPDU(uint8_t  send_len,uint8_t  *send_buff,uint8_t  *rece_len,uint8_t  *rece_buff);
extern uint8_t  CPU_Rats(uint8_t  param1,uint8_t  param2,uint8_t  *rece_len,uint8_t  *buff);
extern uint8_t  CPU_PPS(uint8_t  pps0,uint8_t  pps1,uint8_t  *rece_len,uint8_t  *buff);
extern uint8_t  CPU_Random(uint8_t  *rece_len,uint8_t  *buff);
extern uint8_t  CPU_I_Block(uint8_t  send_len,uint8_t  *send_buff,uint8_t  *rece_len,uint8_t  *rece_buff);
extern uint8_t  CPU_R_Block(uint8_t  *rece_len,uint8_t  *rece_buff);
extern uint8_t  CPU_S_Block(uint8_t  pcb_byte,uint8_t  *rece_len,uint8_t  *rece_buff);
//extern uint8_t  CPU_auth(uint8_t  *key_buff,uint8_t  *rece_len,uint8_t  *buff);
#endif
