#ifndef _UPGRADE_H_
#define _UPGRADE_H_

#include "CommService.h"

typedef enum
{
    T0_X_ADC,
    T0_Y_ADC,
    T1_X_ADC,
    T1_Y_ADC,
    T2_X_ADC,
    T2_Y_ADC,
    T0_X_ORG,
    T0_Y_ORG,
    T1_X_ORG,
    T1_Y_ORG,
    T2_X_ORG,
    T2_Y_ORG,
    T0_X_AGC,
    T0_Y_AGC,
    T1_X_AGC,
    T1_Y_AGC,
    T2_X_AGC,
    T2_Y_AGC,
    T0_X_UNT,
    T0_Y_UNT,
    T1_X_UNT,
    T1_Y_UNT,
    T2_X_UNT,
    T2_Y_UNT,
    TEST_BUF_END
}NEW_TEST_BUF;

extern CmdAnswerType Communication(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock);

#endif

