#ifndef _PRINTF_H_
#define _PRINTF_H_

#include "stm32f0xx_hal.h"

#ifdef PRINTF_DEBUG
#define LOGA(info,...)  do{\
                            printf("[");\
                            printf(COMPID##",");\
                            printf(info,##__VA_ARGS__);\
                            printf("]\r\n");\
                          }while(0);  

#define LOG(info)       do{\
                            printf("[");\
                            printf(COMPID##",");\
                            printf(info);\
                            printf("]\r\n");\
                          }while(0); 
#else
#define LOGA(info,...)
#define LOG(info)
#endif
#endif


