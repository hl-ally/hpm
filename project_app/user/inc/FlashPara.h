#ifndef _CONFIG_PARA_H_
#define _CONFIG_PARA_H_

#include "GLobalDataStructure.h"
#include "GlobalDefaultDefine.h"
//#include "ZwFlash.h"

#define CONFIG_VERSION                  (0x10000000)
//#define GET_FLASH_INDEX_ADD()           (FN_FLASH_ALIGN_PAGE(g_pFwAddrDataDef->nAddrCodeRORWDataLimit))
//#define GET_FLASH_INDEX_HDL()           (*(stFlashAddrIndex_t *)GET_FLASH_INDEX_ADD())

typedef struct
{
    uint32_t nConfigLength;
    uint32_t nAppChekSum;
    uint32_t nConfigCrc32Sum;   //该后面的参数进行CRC的计算

    uint32_t nConfigVersion;    //配置的版本信息
    uint32_t nUsePoint;         //固件支持触摸的点数

    uint32_t nLineCount;        //光线的总数
    uint32_t nLineWidth;        //以1um为单位上传光线宽度

    uint32_t nOAWidth;
    uint32_t nOAHeight;

    uint32_t nPhysicalX;
    uint32_t nPhysicalY;

    /*********USB的VID/PID*********/
    uint32_t nUsb1VID;
    uint32_t nUsb1PID;
    
    int16_t nTotal[2];
    int16_t nLedStart[2][OBQ_COUNT];
    int16_t nLedEnd[2][OBQ_COUNT];
    
    int16_t nLedQTStart[2][OBQ_COUNT];
    int16_t nLedQTEnd[2][OBQ_COUNT];
    
    /*********客户ADC各个方向测试的标准*********/
    int16_t nTestMaxAdc[OBQ_COUNT];
    int16_t nTestMinAdc[OBQ_COUNT];
    
    /*********工厂ADC各个方向测试的标准*********/
    int16_t nTestMaxAdcFactory[OBQ_COUNT];
    int16_t nTestMinAdcFactory[OBQ_COUNT];

    int16_t nTestMaxUndulateAdc;
    
    int16_t nTestMaxReverseAdc;
    int16_t nTestMinReverseAdc;
    
    int16_t nTestMaxAgc;
    int16_t nTestMinAgc;
    
    int16_t nTestMaxAgcFactory;
    int16_t nTestMinAgcFactory;

    /*********USB字串信息*********/
    uint8_t strManufacturerDesc[MAX_STRING_LENGTH];
    uint8_t strDeviceDesc[MAX_STRING_LENGTH];
    uint8_t strModelDesc[MAX_STRING_LENGTH];
    int32_t nManufacturerDescLen;
    int32_t nDeviceDescLen;
    int32_t nModelDescLen;

    /*********默认校准参数*********/
    int32_t nREOX1;
    int32_t nREOX2;
    int32_t nREOY1;
    int32_t nREOY2;
    uint8_t eRotationParameter;

    /*********根据需求调整旋转参数*********/
    uint8_t eRotationDef;
    uint8_t eRotationOsDef;
    uint8_t bReverse;
    
    /*********USB数据传输的类型*********/
    uint8_t bUsb1DeviceConfigType;

    /*********默认的坐标发送的相关参数*********/
    uint8_t bUnderMac10;
    uint8_t bTuioCoordEn;
    uint8_t bUsbCoordEn;
    uint8_t bUartCoordEn;
    uint8_t bTouchEn;
    uint8_t eFormat;
    uint8_t eFormatRe;
    uint8_t bTouchCoordWidthEn;
    uint8_t bTuioCoordWidthEn;
    uint8_t bUartCoordWidthEn;
    uint8_t eTouchCoordWidthType;
    uint8_t eTuioCoordWidthType;
    uint8_t eUartCoordWidthType;
    uint8_t nUartSendCoordNum;
    uint8_t bUartSamePacketNoSend;
    
    /*********每个轴上的每块板上的灯管数量*********/
    uint8_t nXTLedNumForBoard[8];
    uint8_t nYTLedNumForBoard[8];
    uint8_t nXRLedNumForBoard[8];
    uint8_t nYRLedNumForBoard[8];
} stHardConfigData_t;

/********************************************************************************************************/
typedef enum
{
    eHardwareConfigArray,
    eLedPositionArray,
    eLineLedRelationArray,
    eScanBlockRangeArray,
    eBlockStartTLedTimesArray,
    eTestADCIndexArray,
    eTestAgcIndexArray,
    eAllAdcIndexArray,
    eAllAdcChMapArray,
    eAllAdcIndexIndexArray,
    eAllAllAdcContinuStatuIdxArray,
    eAllAdcFastIndexIndexArray,    
    eTLedRLedLableCountSumArray,
    eRBlockAdcStartIndexArray,
    eLedInterValArray,
    eDMACopyToLedOn,
    eCOUNT,
} eIndex_t;

typedef struct
{
    uint32_t  nAddressStart;
    uint32_t  nAddressLenght;
} stAddrIndex_t;

typedef struct
{
    uint32_t     nCrc32;
    uint32_t     nAppChecksum;
    uint32_t     nAllCfgDataLen;
    uint32_t     nFlashFwTableInfoAddr;        //地址配置信息开始地址, 该配置默认分配 2K 大小
    uint32_t     nFlashFwTableInfoSize;        //该配置默认分配 2K 大小
    uint32_t     nFlashScanParaInfoAddr;       //扫描数据表开始地址, 实报实销
    uint32_t     nFlashScanParaInfoSize;       //扫描数据表大小
    uint32_t     nFlashZwAddr;
    uint32_t     nFlashZwSize;
//    stZWPara_t   stZWPara;                     //零等待区信息
    stAddrIndex_t  stIndex[eCOUNT];
} stFlashAddrIndex_t;

/********************************************************************************************************/
extern stHardConfigData_t* g_pConfigData;

extern int32_t ClearBootloader(void);
extern int32_t ClearApp(void);

#if (defined N32G457_ENV)
extern int32_t ClearFlashPara(uint32_t nAddr, uint32_t nLen);
extern stZWPara_t ZWParaInitDefault(uint32_t nStartAddress);
#elif (defined ATF403A_ENV) 
extern int32_t ClearFlashPara(void);
#endif
extern int32_t InitFlashPara(void);
extern uint8_t *GetAlgTableCfgData(void);
extern int32_t GetAlgTableCfgLen(void);
extern int32_t SaveAlgTableCfgData(uint8_t *pData, int32_t nLen);
extern int32_t SetAlgTableCfgLen(uint32_t nLen);

#endif//_CONFIG_PARA_H_


