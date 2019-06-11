#ifndef _RFID_H_
#define _RFID_H_


#define PID_CALC_INTERVAL_MS 250


typedef enum TempPidStatus_e
{
    TempPidStatus_Start,
    TempPidStatus_BelowLow,
    TempPidStatus_BetweenLowHigh,
    TempPidStatus_ExceedHigh,
    TempPidStatus_AlarmLimit,
    TempPidStatus_ReadErr,
    TempPidStatus_End
}TempPidStatus_t;

typedef enum RfidMsg_e
{
    RfidMsg_Idle,
    RfidMsg_Init,
    RfidMsg_FM175xxInit,
    RfidMsg_FM175xxRegInit,
    RfidMsg_MifareCardRequest,
    RfidMsg_MifareCardAnticollision,
    RfidMsg_MifareCardSelect,
    RfidMsg_MifareCardRead,
    RfidMsg_KeyClose,
    RfidMsg_KeyOpen,
    RfidMsg_KeyOpenDelay,
    RfidMsg_KeyOpenLeftOver,
    RfidMsg_ReadFail,

    RfidMsg_End
}RfidMsg_t;

typedef struct Temp_Channel_s
{ 
/*don,t change the following seq*/
    float Target;
    float ParaP;
    float ParaI;
    float ParaD;    
    float Offset;    
    float Period;  

    float BreakDisRise;    
    float BreakDisDrop;   
    float ForwardHeat;
    float ForwardCool;   
    float MaxPowerHeat;
    float MaxPowerCool; 
        
    float Jitter;    
    float Filter;       

    float ColdTempCur;

    float HotTempCur;
    float HotTempPre;
    float HotTempAvr;   

    float Diff;
    float Pidout;   

    float ParaPVal;
    float ParaIVal; 
    float ParaDVal;        

    float BreakRisePoint;    
    float BreakDropPoint;    
    float Hysteresis;   

    float AlarmPoint;        

    float ParaIPre;    
    float ParaDPre;   

    
   

    uint8_t PidInit;   
    uint8_t HighAlarm;

    uint8_t Mode;
    uint8_t errPre;
    uint8_t err;    

    TempPidStatus_t Status;
    TempPidStatus_t StatusPre;
}Temp_Channel_t,*pTemp_Channel_t;


enum
{
    TEMP_INDEX_REAGENT,
    TEMP_INDEX_REACTION,   
    TEMP_INDEX_WASH,   
    TEMP_INDEX_MAX,    
};

enum
{
    TEMP_MODE_COLD,
    TEMP_MODE_HOT,    
    TEMP_MODE_MAX,    
};


typedef struct TEMP_PARA_CONFIG
{
    GPIO_TypeDef  *port;
    uint32_t pin;
    const uint8_t *write;
    uint8_t *read;
    pTemp_Channel_t temp;
    void (*pulse_config) (uint32_t);
    void (*period_config) (uint32_t);    
}TEMP_PARA_CONFIG;



typedef struct RfidTask_s
{ 
    RfidMsg_t msg:8;
}RfidTask_t,*pRfidTask_t;

extern uint8_t picc_uid[15];

void RfidMgrHandle(void);
void KeyOpenProc(void);
void RfidStart(void);
#endif 



