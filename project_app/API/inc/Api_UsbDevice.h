#ifndef _API_USB_DEVICE_
#define _API_USB_DEVICE_
#include "Api_UsbDesc.h"

/*************usb发送Buffer的队列深度与长度***************/
#ifndef USB_TOUCH_COMM_QUEUE_SIZE
#define USB_TOUCH_COMM_QUEUE_SIZE           16
#endif

#ifndef USB_KEYBOARD_QUEUE_SIZE
#define USB_KEYBOARD_QUEUE_SIZE             0
#endif

#ifndef USB_MOUSE_QUEUE_SIZE
#define USB_MOUSE_QUEUE_SIZE                0
#endif

#ifndef USB_BULK_QUEUE_SIZE
#define USB_BULK_QUEUE_SIZE                 0
#endif

#ifndef USB_TOUCH_1_QUEUE_SIZE
#define USB_TOUCH_1_QUEUE_SIZE              0
#endif

#ifndef USB_INK_QUEUE_SIZE
#define USB_INK_QUEUE_SIZE                  0
#endif

#define USB_PACKET_MAX_SIZE                 64

#define USB_QUEUE_SIZE_TOTAL                (USB_TOUCH_COMM_QUEUE_SIZE+\
                                             USB_KEYBOARD_QUEUE_SIZE+\
                                             USB_MOUSE_QUEUE_SIZE+\
                                             USB_BULK_QUEUE_SIZE+\
                                             USB_TOUCH_1_QUEUE_SIZE+\
                                             USB_INK_QUEUE_SIZE)
#define MAX_USB_QUEUE_MEM_SIZE              (sizeof(uint8_t)*USB_QUEUE_SIZE_TOTAL + \
                                                USB_PACKET_MAX_SIZE*USB_QUEUE_SIZE_TOTAL)
#define USB_DEV_REPORT_ID_TOTAL             (0xff)  //包含所有的report id

typedef struct
{
    uint8_t (*arrQueueData)[USB_PACKET_MAX_SIZE];
    uint8_t *arrQueueDataLen;
    int32_t  nEPacketSize;
    int16_t  nMaxQueueSize;
    volatile int16_t nQueueFront;
    volatile int16_t nQueueBack;
    volatile int16_t nQueueLen;
    volatile uint8_t bUsbTxBusy;
} stUsbSendQueue_t;

//端点在应用端的数据发送接收操作
typedef struct
{
    stUsbSendQueue_t stQueue;
    int32_t          nBlockingCount;
} stEpAppProcess_t;

typedef int32_t (*pUsbDevFuncCallback)(uint8_t *pBuf, int16_t nLen, eCmdSource_t eSource);
extern pUsbDevFuncCallback g_pUsbDevCallback[USB_DEV_REPORT_ID_TOTAL];
extern stUsbEpsInfo_t g_stUsbEpsInfo[eUsbDevCount];

extern int32_t InitAEpInfo(eUsbDevice_t eDev, uint8_t nEpIdx, uint16_t nEpType, uint16_t nEpSize, uint16_t nEpsPhyOffset,
        uint8_t nReportInAddr, uint8_t nReportOutAddr, uint8_t nQueueSize,
        uint8_t nMaxEPacketSize, eUsbCfgBitType_t eBitType);
extern int16_t USBEPSendPacket(eUsbDevice_t eDev, uint8_t nEpIdx, uint8_t *pBuffer, int16_t nLen);
extern int16_t USBSendPacket(eUsbDevice_t eDev, eUsbCfgBitType_t eBitType, uint8_t *pBuffer, int16_t nLen);
extern void UsbDevCallbackRegister(uint8_t nReportId, pUsbDevFuncCallback pCallback);

extern int32_t StartUsbDev(stUsbEnumInfo_t stUsbEnumInfo);



#endif //_API_USB_DEVICE_

