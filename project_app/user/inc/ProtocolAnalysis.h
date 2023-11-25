#ifndef _PROTOCOLANALYSIS_H_
#define _PROTOCOLANALYSIS_H_
#include <stdint.h>
#include "GlobalDefaultDefine.h"

#define SEEWO_UART_START_1          0x1F
#define SEEWO_UART_START_2          0xF7
#define FCT_UART_START_1            0x55
#define FCT_UART_START_2            0xaa
#define TPV_UART_START_1            0x55
#define TPV_UART_START_2            0x54


typedef enum
{
    eUartStepWait,
    eUartStepStart1,
    eUartStepStart2,
    eUartStepCom
} eUartStep_t;

typedef enum
{
    eCmdTypeNone,
    eCmdTypeCD,
    eCmdTypeFCT,
    eCmdTypeUnknown,
} eCmdType_t;

#endif//_PROTOCOLANALYSIS_H_
