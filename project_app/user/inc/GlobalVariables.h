#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H
#include "GlobalDefaultDefine.h"
#include "GlobalDataStructure.h"
#include "HardWareInfo.h"
#include "Api_Math.h"
//#include "Api_CommandQueue.h"
//#include "AlgCfgPara.h"

#define MAX_LED_ON_CNT                 (3000)
#define ADC_BUFFER_LEN                 (MAX_LED_ON_CNT * ADC_CH_TOTAL)

#ifndef Firmware_Version
#define Firmware_Version                0x4800
#endif
#define PRODUCT_BOARD_TYPE              0x02
#define CRC32_DATA                      0x022B3C5F

#define EP_IN_ID                        0xFD
#define EP_OUT_ID                       0xFE



typedef uint8_t     adc_type_t;

/*Related to Compress Data and USB send*/
#define CprsBUFFLENG                    4096//64*64
#define CprsQItemSize                   64
#define CprsQLenMax                     (CprsBUFFLENG/CprsQItemSize)

#define CprsQFrameCtn                   1
#define CprsQFrameOvr                   0

#define CprsQueueEnable                 1

#if CprsQueueEnable
typedef struct
{
  uint8_t   (*pQueueData)[CprsQItemSize];
  uint16_t   nQueueDataByts[CprsQLenMax];

  volatile  int8_t      nQueueFront;
  volatile  int8_t      nQueueBack;
  volatile  int8_t      nQueueLen;
  volatile  uint8_t     bQueueStatus;        //a frame of data compressed yet
  volatile  uint32_t    nCprsADCInd;        //the ID of ADC have processed 
} USB_CprsQueue;
extern USB_CprsQueue stUSB_CprsQueue ;
#else
extern uint8_t gUSB_CprsBuff ;
#endif

typedef enum
{
    eTestDefault,
    eTestStart,
    eTestFinish
}eTestStatus_t;

typedef struct
{    
    eTestStatus_t eStatus;
    eCmdSource_t  eCmdSource;
    uint32_t      nStartTime;
} stTestStatus_t;


typedef enum
{
    ePatternNormal = 0,
    ePatternSun,                //natural sunlight
    ePatternSunBulb,            //Sun bulb test
    ePatternBBoardClose,        //black board
    ePatternBBoardOpen,        //black board
    ePatternGlassShaking,        //for mv series: on V series, glass are easily transformed by finger press
    ePatternRegularAdjust,
    ePatternFullAdjust,
    ePatternCount
} eAmbiencePattern_t;


typedef struct _MIDDLE_ADC_STAT
{
    eAmbiencePattern_t eEnvmPattern;                                              //current frame's enviroment pattern
    uint8_t arrLedStateX[X_TOTAL];
    uint8_t arrLedStateY[Y_TOTAL];
    uint8_t *arrLedState[eAxisCount];

    uint32_t nRegularStableTick;
    uint32_t nBBordOpenTick;
    int32_t  nAgcAdjCount;
    int32_t  nDelayAdjHelper;
    int32_t  nCalcAdcIntv;
    uint32_t nUnStableTick;
    int32_t  bDelayRegularAdj;          //进入常规调节的标记
    int32_t  nEnterRegularAdjTick;      //进入常规调节的时间戳
    adc_type_t arrLastMidAdcX[X_TOTAL];
    adc_type_t arrLastMidAdcY[Y_TOTAL];
    adc_type_t *arrLastMidAdc[eAxisCount];
    int16_t     arrUpCount[eAxisCount];
    int16_t     arrDownCount[eAxisCount];

    adc_type_t arrAbnormalAdcX[OBQ_COUNT][X_TOTAL];
    adc_type_t arrAbnormalAdcY[OBQ_COUNT][Y_TOTAL];
    int16_t nAbnormalAdcCnt[eAxisCount];
    adc_type_t arrBGroundAdcX[OBQ_COUNT][X_TOTAL];
    adc_type_t arrBGroundAdcY[OBQ_COUNT][Y_TOTAL];
    adc_type_t arrUndulateAdcMinX[OBQ_COUNT][X_TOTAL];
    adc_type_t arrUndulateAdcMinY[OBQ_COUNT][Y_TOTAL];
    adc_type_t arrUndulateAdcMaxX[OBQ_COUNT][X_TOTAL];
    adc_type_t arrUndulateAdcMaxY[OBQ_COUNT][Y_TOTAL];
} stMiddleAdc_t;


typedef enum
{
    eTouchBufNone,
    eTouchBufTransfering,
    eTouchBufReady,
    eTouchBufLock
} eTouchBufStatus_t;

typedef struct
{
    uint8_t *pBuf;
    uint8_t  eDataType;
    uint32_t nLen;
    uint32_t nTimeStamp;
    uint32_t nFrame;
    int32_t  nDataFrameNo;
    eTouchBufStatus_t eBufStatus;
} stTouchData;

#if (defined N32G457_ENV)
typedef struct
{
    uint8_t nAddr       :5;
    uint8_t nT164Data   :1;
    uint8_t nT164CLK    :1;
    uint8_t nT164Mr     :1;
    uint8_t nR164CLK    :1;
    uint8_t nR164Data   :1;
    uint8_t nUART       :2;
    uint8_t nADC        :2;
    uint8_t nNULL1      :1;
    uint8_t nAdcHold    :1;
}stDMACopyToGpio_t;
#elif (defined ATF403A_ENV)
typedef struct
{
    uint8_t nNull1      :1;     //悬空引脚
    uint8_t nT164Data   :1;     //发射 164 Data
    uint8_t nBOOT1      :1;     //BOOT1引脚
    uint8_t nAddr       :4;     //发射列选地址
    uint8_t nT164CLK    :1;     //发射 164 CLK
    uint8_t nR164CLK    :1;     //接收 164 Clk
    uint8_t nR164Data   :1;     //接收 164 Data
    uint8_t nUART       :2;     //串口引脚
    uint8_t nNull2      :1;     //悬空引脚
    uint8_t nAdcHold    :1;     //保持引脚
    uint8_t nVersionHw  :2;     //硬件版本识别
}stDMACopyToGpio_t;
#elif (defined GD415_ENV)

#if (SCAN_SOLUTION == SCAN_SOLUTION_6CH_E5HG)
typedef struct
{
    uint8_t nNULL:3;
    uint8_t nColAddr:5;
    uint8_t nT164Clk:1;
    uint8_t nT164Data:1;
    uint8_t nNULL1:2;
    uint8_t nR164Clk:1;
    uint8_t nR164Data:1;
    uint8_t nHWUSB:2;
}stDMACopyToGpio_t;
#else
typedef struct
{
    uint8_t nT164Data:1;
    uint8_t nNULL1:2;
    uint8_t nColAddr:4;
    uint8_t nT164Clk:1;
}stDMACopyToGpio_t;
#endif
#endif



typedef enum
{
    eNormalMode,
    eAgingMode
} eRunMode_t;

typedef enum
{
    eUnstable = 0,
    eStable	
}eStableState_t;

typedef enum
{
    eEnumSuccess,
    eEnumFailure
}eUsbEnumMask_t;

typedef enum
{
    eAgingDisable = 0,
    eAgingEnable,
    eAgingSuccess,
    eAgingFailure
}eBurnAgingState_t;

typedef struct
{
    eRunMode_t  eMode;
    eBurnAgingState_t eCmdState;
    eBurnAgingState_t eAgingMark;
    eUsbEnumMask_t eUsbEnumMask;

}stBurnAging_t;




extern adc_type_t*                      g_ORG;

extern stSgmH_t                        *g_stScanSeg[eAxisCount];
extern stSgmH_t                        *g_stScanSegTmp[eAxisCount];
extern uint16_t                         g_nBlockStartTLedTimes[eAxisCount][((FN_MAX(X_TOTAL,Y_TOTAL)+ADC_CH_TOTAL-1)/ADC_CH_TOTAL)*2];

extern volatile  int16_t                g_nTotal[eAxisCount];
extern int32_t                          g_nInterVal[eAxisCount][OBQ_COUNT];
extern stPosF_t*                        g_arrPos[eEdgeCount];
extern float                            g_fMaxRate[eAxisCount];

extern stTouchData                      g_TouchDataFifo[2];
extern volatile uint8_t                 g_nTouchDataFifoFront;
extern volatile uint8_t                 g_nTouchDataFifoBack;



#endif  // GLOBAL_VARIABLES_H

