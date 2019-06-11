#ifndef _UART_H_
#define _UART_H_

typedef void (*MCU_UartRecvIntProcFunc)(uint8_t dat);
typedef void (*MCU_UartErrProcFunc)(void);


typedef enum Board_RS_Serial_e
{
    BOARD_UART1,
    BOARD_UART3,
    BOARD_UART4,
    BOARD_UART5,
    BOARD_UART6,      
}Board_RS_Serial_t;
#define GPRS_UART_SERIAL BOARD_UART1

extern MCU_UartRecvIntProcFunc uart1RecvFuncPtr;
extern uint8_t uart1SingleData;
extern MCU_UartErrProcFunc uart1ErrFuncPtr;

extern MCU_UartRecvIntProcFunc uart3RecvFuncPtr;
extern uint8_t uart3SingleData;
extern MCU_UartErrProcFunc uart3ErrFuncPtr;

void BoardRSSerialInit(Board_RS_Serial_t serialNo,MCU_UartRecvIntProcFunc rxCallBack,MCU_UartErrProcFunc errCallBack);
void BoardRSSerialDeInit(Board_RS_Serial_t serialNo);
void BoardRSSerialRecIT(Board_RS_Serial_t serialNo);
uint8_t  BoardRSSerialGetRXNEFlag(Board_RS_Serial_t serialNo);
void BoardRSSerialSendBuffer(Board_RS_Serial_t serialNo, uint8_t* bufferPtr,uint16_t bufferLength);
void BoardRSSerialSendString(Board_RS_Serial_t serialNo,const char *string);


#endif


