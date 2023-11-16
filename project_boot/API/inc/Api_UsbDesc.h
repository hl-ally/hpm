#ifndef __USB_DESC_H
#define __USB_DESC_H
#include "stdint.h"

#define USB_HID_CONFIG_DESC_LEN          196
#define USB_HID_REPORT_DESC_LEN          724


typedef enum
{
    eUsbConfigTouch0AndCommBit      = 0x00,  //Touch & communication interface
    eUsbConfigKeyboardBit           = 0x01,  //Keyboard interface
    eUsbConfigBulkBit               = 0x02,  //Bulk interface
    eUsbConfigTouch1Bit             = 0x04,  //Touch only interface
    eUsbConfigMouseBit              = 0x08,  //Mouse only interface
    eUsbConfigPenBit                = 0x10,  //Pen bit
    eUsbConfigBitTotal
} eUsbConfigBitType_t;


typedef enum
{
    eUsbConfigStandard                      = eUsbConfigTouch0AndCommBit,
    eUsbConfigStandardWithKeyboard          = eUsbConfigTouch0AndCommBit | eUsbConfigKeyboardBit,
    eUsbConfigStandardWithBulk              = eUsbConfigTouch0AndCommBit | eUsbConfigBulkBit,
    eUsbConfigStandardWithBulkAndKeyBoard   = eUsbConfigTouch0AndCommBit | eUsbConfigKeyboardBit | eUsbConfigBulkBit,
    eUsbConfigStandardWithDualHid           = eUsbConfigTouch0AndCommBit | eUsbConfigTouch1Bit,
    eUsbConfigStandardWithKeyboardDualHid   = eUsbConfigTouch0AndCommBit | eUsbConfigTouch1Bit | eUsbConfigKeyboardBit,
    eUsbConfigBootDefault = 0xFE,
    eUsbConfigAppDefault = 0xFF
} eUsbConfigType_t;

#define USB_CONFIG_STANDARD                             (0x00)
#define USB_CONFIG_STANDARD_WITH_KEYBOARD               (0x01)
#define USB_CONFIG_STANDARD_WITH_BULK                   (0x02)
#define USB_CONFIG_STANDARD_WITH_BULK_KEYBOARD          (0x03)
#define USB_CONFIG_STANDARD_WITH_DUALHID                (0x04)
#define USB_CONFIG_STANDARD_WITH_KEYBOARD_DUALHID       (0x05)

#define USB_CONFIG_STANDARDINK                          (0x10)
#define USB_CONFIG_STANDARDINK_WITH_KEYBOARD            (0x11)
#define USB_CONFIG_STANDARDINK_WITH_BULK                (0x12)
#define USB_CONFIG_STANDARDINK_WITH_BULK_KEYBOARD       (0x13)
#define USB_CONFIG_STANDARDINK_WITH_DUALHID             (0x14)
#define USB_CONFIG_STANDARDINK_WITH_KEYBOARD_DUALHID    (0x15)

#define USB_CONFIG_BOOT_DEFAULT                         (0xFE)
#define USB_CONFIG_APP_DEFAULT                          (0xFF)



typedef struct
{
    uint16_t nUSBVid;
    uint16_t nUSBPid;
    uint16_t nVersion;
    eUsbConfigType_t eUsbConfigType;
    uint32_t nPhysicalX;
    uint32_t nPhysicalY;
    uint8_t  bTouchCoordWidthEn;
} stUsbEnumInfo_t;


#endif /* __USB_DESC_H */


