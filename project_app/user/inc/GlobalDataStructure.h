#ifndef _GLOBALDATASTRUCTURE_
#define _GLOBALDATASTRUCTURE_
#include <stdint.h>
#include "AlgBaseStructure.h"

#define LOGICAL_VALUE_PER_MM            (g_stCfgData.fLogicalValuePerMM)

#if TRACE_STARTUP_LOG == 1
#define TRACE_LOG(format,...)           do {\
                                            printf(format, ##__VA_ARGS__);\
                                        }while(0)
#else
#define TRACE_LOG(format,...)
#endif

#ifndef USB0_COORD_EN
#define USB0_COORD_EN                   1
#endif

#ifndef USB1_COORD_EN
#define USB1_COORD_EN                   0
#endif

#ifndef SYSTEM_SUPPORT_INK_V1
#define SYSTEM_SUPPORT_INK_V1           (0U)   //旧版WhiteBoard, 支持大小笔和橡皮擦
#endif

#ifndef SYSTEM_SUPPORT_INK_V2
#define SYSTEM_SUPPORT_INK_V2           (1U)  //新版WhiteBoard, 不支持大小笔
#endif

#if (1 == USB1_COORD_EN)
typedef enum
{
    eUsbDev0 = 0,
    eUsbDev1,
    eUsbDevCount,
    eUsbDevNone
} eUsbDevice_t;
#else
typedef enum
{
    eUsbDev0 = 0,
    eUsbDev1 = eUsbDev0,
    eUsbDevCount,
    eUsbDevNone
} eUsbDevice_t;
#endif

#define GetUsbDev(x)     ((x) <= eUsb0EpsCount ? eUsbDev0 : eUsbDev1)

typedef enum
{
    /*******USB0相关数据传输类型***********/
    eUsb0Ep0Mode = 0,
    eUsb0Ep1Mode,
    eUsb0Ep2Mode,
    eUsb0Ep3Mode,
    eUsb0Ep4Mode,
    eUsb0Ep5Mode,
    eUsb0Ep6Mode,
    eUsb0Ep7Mode,
    eUsb0EpsCount = eUsb0Ep7Mode,

    /*******USB1相关数据传输类型***********/
    eUsb1Ep0Mode,
    eUsb1Ep1Mode,
    eUsb1Ep2Mode,
    eUsb1Ep3Mode,
    eUsb1Ep4Mode,
    eUsb1Ep5Mode,
    eUsb1Ep6Mode,
    eUsb1Ep7Mode,

    eUsart3Mode,
    /*******通信的类型总数***********/
    eCmdModeCount
} eCmdSource_t;

typedef enum
{
    eUsbCfgTouch0CommBit = 0x00,  //Touch & communication interface
    eUsbCfgKeyboardBit = 0x01,    //Keyboard interface
    eUsbCfgBulkBit = 0x02,        //Bulk interface
    eUsbCfgTouch1Bit = 0x04,      //Touch only interface
    eUsbCfgMouseBit = 0x08,       //Mouse only interface
    eUsbCfgInkBit = 0x10,         //Ink bit
    eUsbCfgBitCount
} eUsbCfgBitType_t;

typedef enum
{
    eUsbCfgStandard = eUsbCfgTouch0CommBit | eUsbCfgMouseBit,
    eUsbCfgStandardWithKeyboard = eUsbCfgStandard | eUsbCfgKeyboardBit,
    eUsbCfgStandardWithBulk = eUsbCfgStandard | eUsbCfgBulkBit,
    eUsbCfgStandardWithBulkAndKeyBoard = eUsbCfgStandardWithBulk | eUsbCfgKeyboardBit,
    eUsbCfgStandardWithDualHid = eUsbCfgStandard | eUsbCfgTouch1Bit,
    eUsbCfgStandardWithKeyboardDualHid = eUsbCfgStandardWithKeyboard | eUsbCfgTouch1Bit,

    eUsbCfgStandardInk = eUsbCfgStandard | eUsbCfgInkBit,
    eUsbCfgStandardInkWithKeyboard = eUsbCfgStandardInk | eUsbCfgKeyboardBit,
    eUsbCfgStandardInkWithBulk = eUsbCfgStandardInk | eUsbCfgBulkBit,
    eUsbCfgStandardInkWithBulkAndKeyBoard = eUsbCfgStandardInkWithBulk | eUsbCfgKeyboardBit,
    eUsbCfgStandardInkWithDualHid = eUsbCfgStandardInk | eUsbCfgTouch1Bit,
    eUsbCfgStandardInkWithKeyboardDualHid = eUsbCfgStandardInkWithKeyboard | eUsbCfgTouch1Bit,

    eUsbCfgBootDefault = 0xfe,
    eUsbCfgAppDefault = 0xff
} eUsbCfgType_t;

typedef struct
{
    uint8_t  nEpsCount;
    uint16_t arrEpsType[7];
    uint16_t arrEpsSize[7];
    uint8_t  arrEpsReportInAddr[7];
    uint16_t arrEpsPhyInAddr[7];
    uint8_t  arrEpsReportOutAddr[7];
    uint16_t arrEpsPhyOutAddr[7];
    uint8_t  arrQueueSize[7];
    uint8_t  arrMaxEPacketSize[7];
    uint8_t  arrInterfaceIdxMap[eUsbCfgBitCount];
} stUsbEpsInfo_t;

#endif//_GLOBALDATASTRUCTURE_


