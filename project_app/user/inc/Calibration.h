#ifndef __CALIBRATION_H__
#define __CALIBRATION_H__
#include "GlobalDataStructure.h"


#define CAL                             0x01
#define CAL_BEGIN                       0x00
#define CAL_STEP1                       0x01
#define CAL_STEP2                       0x02
#define CAL_STEP3                       0x03
#define CAL_STEP4                       0x04
#define CAL_CHECK                       0x05
#define CAL_RESET                       0x06
#define CAL_END                         0xFF

#define MAX_CAL_X                       0xfff
#define MAX_CAL_Y                       0xfff

#define X_LOGICAL_VALUE                 MAX_SEND_X
#define Y_LOGICAL_VALUE                 MAX_SEND_Y
#define MAX_LOGICAL_VALUE               0x7FFF

#define CALREFX1                        (1.0f * ((MAX_LOGICAL_VALUE + 1) / 8 + 64))
#define CALREFY1                        (1.0f * ((MAX_LOGICAL_VALUE + 1) / 8 + 64))
#define CALREFX2                        (1.0f * ((MAX_LOGICAL_VALUE + 1) - (MAX_LOGICAL_VALUE + 1) / 8 + 64))
#define CALREFY2                        (1.0f * ((MAX_LOGICAL_VALUE + 1) / 8 + 64))
#define CALREFX3                        (1.0f * ((MAX_LOGICAL_VALUE + 1) - (MAX_LOGICAL_VALUE + 1) / 8 + 64))
#define CALREFY3                        (1.0f * ((MAX_LOGICAL_VALUE + 1) - (MAX_LOGICAL_VALUE + 1) / 8 + 64))
#define CALREFX4                        (1.0f * ((MAX_LOGICAL_VALUE + 1) / 8 + 64))
#define CALREFY4                        (1.0f * ((MAX_LOGICAL_VALUE + 1) - (MAX_LOGICAL_VALUE + 1) / 8 + 64))

typedef struct
{
    stCoord_t arrCoord[4];
    uint8_t nStep;
    uint32_t nREOX1;
    uint32_t nREOX2;
    uint32_t nREOY1;
    uint32_t nREOY2;
} stCalibrationData;

typedef struct
{
    stCoord_t arrCoord[4];
    uint32_t nREOX1;
    uint32_t nREOX2;
    uint32_t nREOY1;
    uint32_t nREOY2;    
} stFlashParaOfCalibration_t;

extern volatile uint8_t g_bInCalibrating;
extern volatile uint8_t g_bCalEn;

extern volatile stCalibrationData  g_stCalibrationData;
extern volatile stCoord_t g_arrCalCoord[4];

extern void DoCalibration(stPoint_t* stPoint, unsigned int nCount, int32_t nWidth, int32_t nHeight);
extern void CalibrationInit(void);
extern uint32_t GetDeviation(void);

extern int32_t SaveCalibrationData(void);
extern int32_t ResetCalibration(void);
extern int32_t InitCalibration(stFlashParaOfCalibration_t stFlashParaOfCalibration);
extern int32_t GetDefaultCalibrationFlashPara(stFlashParaOfCalibration_t* pFlashParaOfCalibration);
extern int32_t InitCalibrationParameter(void);
extern int32_t SetCalibrationData(void);

#endif

