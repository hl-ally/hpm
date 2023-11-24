#ifndef __USB_DESC_H
#define __USB_DESC_H
#include "stdint.h"
#include "GlobalDataStructure.h"


#define CUSTOMHID_SIZ_REPORT_DESC               724
#define CUSTOMHID_SIZ_DEVICE_DESC               18
#define CUSTOMHID_SIZ_CONFIG_DESC               255
#define CUSTOMHID_SIZ_STRING_LANGID             4
#define CUSTOMHID_SIZ_STRING_VENDOR             46
#define CUSTOMHID_SIZ_STRING_PRODUCT            26
#define CUSTOMHID_SIZ_STRING_SERIAL             22

#define KEYREPORT_DESC_SIZE                     67

#define MOUSE_REPORT_ID                         0x01
#define TOUCH_REPORT_ID                         0x02
#define TOUCH_MAXMIN_REPORT_ID                  0x03
#define TOUCH_DEV_CFG_REPORT_ID                 0x04
#define KEYBOARD_REPORT_ID                      0x05
#define TOUCH_V2_REPORT_ID                      0x32
#define TOUCH_V2_MAXMIN_REPORT_ID               0x07
#define INK_REPORT_ID                           0x08
#define FW_IN_REPORT_ID                         0xfc
#define FW_OUT_REPORT_ID                        0xfb
#define BOOT_OUT_REPORT_ID                      0xfe
#define BOOT_IN_REPORT_ID                       0xfd


#define CUSTOM_TOUCH0_EP_IN_ADDR                0x81
#define CUSTOM_TOUCH0_EP_IN_SIZE                64
#define CUSTOM_TOUCH0_EP_OUT_ADDR               0x01
#define CUSTOM_TOUCH0_EP_OUT_SIZE               64

#define CUSTOM_MOUSE_EP_IN_ADDR                 0x82
#define CUSTOM_MOUSE_EP_IN_SIZE                 8
#define CUSTOM_MOUSE_EP_OUT_ADDR                0x02
#define CUSTOM_MOUSE_EP_OUT_SIZE                8

#define CUSTOM_TOUCH1_EP_IN_ADDR                0x83
#define CUSTOM_TOUCH1_EP_IN_SIZE                64
#define CUSTOM_TOUCH1_EP_OUT_ADDR               0x03
#define CUSTOM_TOUCH1_EP_OUT_SIZE               64

#define CUSTOM_BULK_EP_IN_ADDR                  0x85
#define CUSTOM_BULK_EP_IN_SIZE                  64
#define CUSTOM_BULK_EP_OUT_ADDR                 0x05
#define CUSTOM_BULK_EP_OUT_SIZE                 64

#define CUSTOM_KEYBOARD_EP_IN_ADDR              0x84
#define CUSTOM_KEYBOARD_EP_IN_SIZE              8
#define CUSTOM_KEYBOARD_EP_OUT_ADDR             0x04
#define CUSTOM_KEYBOARD_EP_OUT_SIZE             8

#define CUSTOM_INK_EP_IN_ADDR                   0x86
#define CUSTOM_INK_EP_IN_SIZE                   64
#define CUSTOM_INK_EP_OUT_ADDR                  0x06
#define CUSTOM_INK_EP_OUT_SIZE                  64

#define ENDPx_START_ADDR                        (0)

typedef enum
{
    eTouchCommReportSection = 0,    //触控和通讯描述符索引ID
    eKeyboardReportSection,         //键盘描述符索引ID
    eTouch1ReportSection,           //Touch1触控描述符索引ID
    eMouseReportSection,            //Mouse描述符索引ID
    eInkReportSection,              //Ink描述符索引ID
    eReportSectionTotal
} eReportSectionID_t;

//usb字串类型种类
typedef enum
{
    eUsbStrLangID,
    eUsbStrVendor,
    eUsbStrProduct,
    eUsbStrSerialNumber,
    eUsbStrTotal
} eUsbStrType_t;

//usb描述符种类
typedef enum
{
    eUsbDev0Desc,
    eUsbDev1Desc,
    eUsbDev0CfgDesc,
    eUsbDev1CfgDesc,
    eUsbDev0HidTouch0Desc,
    eUsbDev1HidTouch0Desc,
    eUsbDev0HidTouch1Desc,
    eUsbDev1HidTouch1Desc,
    eUsbInkReportDesc,
    eUsbDev0VendorStrDesc,
    eUsbDev1VendorStrDesc,
    eUsbDev0ProductStrDesc,
    eUsbDev1ProductStrDesc,
    eUsbDev0SNStrDesc,
    eUsbDev1SNStrDesc,
    eUsbStrDescTotal
} eUsbDescStr_t;

typedef struct
{
    uint32_t nOffset;
    uint32_t nLen;
    uint32_t nMaxLen;
} stOneFlashDesc_t;


typedef struct
{
    uint32_t nTotalCrc32;
    stOneFlashDesc_t arrDesc[eUsbStrDescTotal];
    uint32_t nThisCrc32;
} stUsbDescFlash_t;

typedef struct
{
    uint8_t *Descriptor;
    uint16_t Descriptor_Size;
} stReportSection_t;


typedef struct
{
    uint16_t nUSBVid;
    uint16_t nUSBPid;
    uint16_t nVersion;
    eUsbCfgType_t eUsbCfgType;
    uint32_t nPhysicalX;
    uint32_t nPhysicalY;
    uint8_t  bTouchCoordWidthEn;
    eUsbDevice_t eUsbDev;
} stUsbEnumInfo_t;

extern stReportSection_t g_stUsbDefaultHidReportDesc;
extern stReportSection_t g_arrUsbDevDesc[eUsbDevCount];
extern stReportSection_t g_arrUsbCfgDesc[eUsbDevCount];
extern stReportSection_t g_arrHidString[eUsbStrTotal];

extern void InitUSBDesc(uint32_t nPoints, stUsbEnumInfo_t stEnumInfo);
//extern int32_t InitUsbStringUnicode(uint8_t* pString, uint8_t *pSrcMemStr, eUsbStrType_t eUsbStrType, eUsbDevice_t eUsbDev);
extern int32_t SaveUsbDescStringFlash(uint8_t *pStr, int32_t nLen, eUsbDescStr_t eUsbDescFlash);
extern int32_t InitUsbStrings(eUsbDevice_t eUsbDev);
#endif /* __USB_DESC_H */


