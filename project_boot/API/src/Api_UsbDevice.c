#include "usbd_core.h"
#include "usbd_hid.h"
#include "user_config.h"
#include "GlobalDefaultDefine.h"
#include "app_systick.h"
#include "Api_Math.h"
#include "Api_UsbDesc.h"
#include "Api_UsbDevice.h"

stUsbEpsInfo_t g_stUsbEpsInfo[eUsbDevCount] = {{.nEpsCount = 0}};
eUsbCfgType_t g_eUsbCfgType[eUsbDevCount] = {USB0_DEVICE_CONFIG_TYPE};
static stEpAppProcess_t sg_arrEpAppProcess[8] = {0};
static int8_t           sg_arrEpAppProcessMap[8] = {0};

static volatile int32_t sg_bUsbAppInit = 0;
static __attribute__((aligned(8))) uint8_t sg_arrUsbQueueMem[MAX_USB_QUEUE_MEM_SIZE];
static uint32_t sg_nUsb0SuspendTick = 0;
static volatile uint8_t sg_arrSetReport[eUsbDevCount] = {0};
volatile uint32_t g_arrSetup0ProcessLstTick[eUsbDevCount];




/*!< hidraw in endpoint */
#define HIDRAW_IN_EP                    0x81
#define HIDRAW_IN_EP_SIZE               64
#define HIDRAW_IN_INTERVAL              1

/*!< hidraw out endpoint */
#define HIDRAW_OUT_EP                   0x01
#define HIDRAW_OUT_EP_SIZE              64
#define HIDRAW_OUT_EP_INTERVAL          1

#define BOOT_USBD_VID                   0x1FF7
#define BOOT_USBD_PID                   0x0F0A
#define BOOT_USBD_MAX_POWER             100

/*!< config descriptor size */
#define USB_HID_CONFIG_DESC_SIZ         (9 + 9 + 9 + 7 + 7)

/*!< custom hid report descriptor size */
#define HID_CUSTOM_REPORT_DESC_SIZE     46

/*!< global descriptor */
static const uint8_t hid_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, BOOT_USBD_VID, BOOT_USBD_PID, 0x0002, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT(USB_HID_CONFIG_DESC_SIZ, 0x01, 0x01, 0xA0, BOOT_USBD_MAX_POWER),
    /************** Descriptor of Custom interface *****************/
    0x09,                          /* bLength: Interface Descriptor size */
    USB_DESCRIPTOR_TYPE_INTERFACE, /* bDescriptorType: Interface descriptor type */
    0x00,                          /* bInterfaceNumber: Number of Interface */
    0x00,                          /* bAlternateSetting: Alternate setting */
    0x02,                          /* bNumEndpoints */
    0x03,                          /* bInterfaceClass: HID */
    0x01,                          /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,                          /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,                             /* iInterface: Index of string descriptor */
    /******************** Descriptor of Custom HID ********************/
    0x09,                    /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE_HID, /* bDescriptorType: HID */
    0x11,                    /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                        /* bCountryCode: Hardware target country */
    0x01,                        /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                        /* bDescriptorType */
    HID_CUSTOM_REPORT_DESC_SIZE, /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Custom in endpoint ********************/
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HIDRAW_IN_EP,                 /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
    WBVAL(HIDRAW_IN_EP_SIZE),     /* wMaxPacketSize: 4 Byte max */
    HIDRAW_IN_INTERVAL,           /* bInterval: Polling Interval */
    /******************** Descriptor of Custom out endpoint ********************/
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HIDRAW_OUT_EP,                /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
    WBVAL(HIDRAW_OUT_EP_SIZE),    /* wMaxPacketSize: 4 Byte max */
    HIDRAW_OUT_EP_INTERVAL,       /* bInterval: Polling Interval */
    /* 73 */
    /*
     * string0 descriptor
     */
    USB_LANGID_INIT(USBD_LANGID_STRING),
    /*
     * string1 descriptor
     */
    0x14,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'C', 0x00,                  /* wcChar0 */
    'h', 0x00,                  /* wcChar1 */
    'e', 0x00,                  /* wcChar2 */
    'r', 0x00,                  /* wcChar3 */
    'r', 0x00,                  /* wcChar4 */
    'y', 0x00,                  /* wcChar5 */
    'U', 0x00,                  /* wcChar6 */
    'S', 0x00,                  /* wcChar7 */
    'B', 0x00,                  /* wcChar8 */
    /*
     * string2 descriptor
     */
    0x26,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'C', 0x00,                  /* wcChar0 */
    'h', 0x00,                  /* wcChar1 */
    'e', 0x00,                  /* wcChar2 */
    'r', 0x00,                  /* wcChar3 */
    'r', 0x00,                  /* wcChar4 */
    'y', 0x00,                  /* wcChar5 */
    'U', 0x00,                  /* wcChar6 */
    'S', 0x00,                  /* wcChar7 */
    'B', 0x00,                  /* wcChar8 */
    ' ', 0x00,                  /* wcChar9 */
    'H', 0x00,                  /* wcChar10 */
    'I', 0x00,                  /* wcChar11 */
    'D', 0x00,                  /* wcChar12 */
    ' ', 0x00,                  /* wcChar13 */
    'D', 0x00,                  /* wcChar14 */
    'E', 0x00,                  /* wcChar15 */
    'M', 0x00,                  /* wcChar16 */
    'O', 0x00,                  /* wcChar17 */
    /*
     * string3 descriptor
     */
    0x16,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    '2', 0x00,                  /* wcChar0 */
    '0', 0x00,                  /* wcChar1 */
    '2', 0x00,                  /* wcChar2 */
    '2', 0x00,                  /* wcChar3 */
    '1', 0x00,                  /* wcChar4 */
    '2', 0x00,                  /* wcChar5 */
    '3', 0x00,                  /* wcChar6 */
    '4', 0x00,                  /* wcChar7 */
    '5', 0x00,                  /* wcChar8 */
    '6', 0x00,                  /* wcChar9 */
#ifdef CONFIG_USB_HS
    /*
     * device qualifier descriptor
     */
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
#endif
    0x00
};

/*!< custom hid report descriptor */
static const uint8_t hid_custom_report_desc[HID_CUSTOM_REPORT_DESC_SIZE] = {
    /* USER CODE BEGIN 0 */
    0x06, 0x00, 0xff,               	/*   USAGE_PAGE (Undefined)     */
    0x09, 0x00,                     	/*   USAGE (Undefined)          */
    0xa1, 0x01,                     	/*   COLLECTION (Application)   */
    0x85, 0xFD,                     	/*   REPORT_ID (253)            */
    0x06, 0x00, 0xff,               	/*   USAGE_PAGE (Undefined)     */
    0x19, 0x01,                     	/*   USAGE_MINIMUM              */
    0x29, 0x3f,                     	/*   USAGE_MAXIMUM              */
    0x15, 0x00,                     	/*   LOGICAL_MINIMUM (0)        */
    0x25, 0xff,                     	/*   LOGICAL_MAXIMUM (255)      */
    0x75, 0x08,                     	/*   REPORT_SIZE (8)            */
    0x95, 0x3f,                     	/*   REPORT_COUNT (63)          */
    0x81, 0x02,                     	/*   INPUT (Data,Var,Abs)       */
    0x85, 0xFE,                     	/*   REPORT_ID (254)            */
    0x06, 0x00, 0xff,               	/*   USAGE_PAGE (Undefined)     */
    0x19, 0x01,                     	/*   USAGE_MINIMUM              */
    0x29, 0x3f,                     	/*   USAGE_MAXIMUM              */
    0x15, 0x00,                     	/*   LOGICAL_MINIMUM (0)        */
    0x25, 0xff,                     	/*   LOGICAL_MAXIMUM (255)      */
    0x75, 0x08,                     	/*   REPORT_SIZE (8)            */
    0x95, 0x3f,                     	/*   REPORT_COUNT (63)          */
    0x91, 0x02,                     	/*   OutPut (Data,Var,Abs)      */
    /* USER CODE END 0 */ 
    0xC0    /*     END_COLLECTION                 */
};

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t read_buffer[HIDRAW_OUT_EP_SIZE];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t send_buffer[HIDRAW_IN_EP_SIZE];

#define HID_STATE_IDLE 0
#define HID_STATE_BUSY 1

/*!< hid state ! Data can be sent only when state is idle  */
static volatile uint8_t custom_state;

void usbd_event_handler(uint8_t event)
{
    switch (event) {
    case USBD_EVENT_RESET:
        break;
    case USBD_EVENT_CONNECTED:
        break;
    case USBD_EVENT_DISCONNECTED:
        break;
    case USBD_EVENT_RESUME:
        break;
    case USBD_EVENT_SUSPEND:
        break;
    case USBD_EVENT_CONFIGURED:
        /* setup first out ep read transfer */
        usbd_ep_start_read(HIDRAW_OUT_EP, read_buffer, HIDRAW_OUT_EP_SIZE);
        printf("[%02X %02X %02X %02X]\n", read_buffer[0], read_buffer[1], read_buffer[2], read_buffer[3]);
        break;
    case USBD_EVENT_SET_REMOTE_WAKEUP:
        break;
    case USBD_EVENT_CLR_REMOTE_WAKEUP:
        break;

    default:
        break;
    }
}

static void usbd_hid_custom_in_callback(uint8_t ep, uint32_t nbytes)
{
    USB_LOG_RAW("actual in len:%d\r\n", nbytes);
    custom_state = HID_STATE_IDLE;
}

static void usbd_hid_custom_out_callback(uint8_t ep, uint32_t nbytes)
{
    USB_LOG_RAW("actual out len:%d\r\n", nbytes);
    usbd_ep_start_read(HIDRAW_OUT_EP, read_buffer, 64);
    read_buffer[0] = 0x02; /* IN: report id */
    usbd_ep_start_write(HIDRAW_IN_EP, read_buffer, nbytes);
}

static struct usbd_endpoint custom_in_ep = {
    .ep_cb = usbd_hid_custom_in_callback,
    .ep_addr = HIDRAW_IN_EP
};

static struct usbd_endpoint custom_out_ep = {
    .ep_cb = usbd_hid_custom_out_callback,
    .ep_addr = HIDRAW_OUT_EP
};

/* function ------------------------------------------------------------------*/
/**
 * @brief            hid custom init
 * @pre              none
 * @param[in]        none
 * @retval           none
 */
struct usbd_interface intf0;

void boot_hid_init(void)
{
    uint64_t nUsbEnumTime =0;

    usbd_desc_register(hid_descriptor);
    usbd_add_interface(usbd_hid_init_intf(&intf0, hid_custom_report_desc, HID_CUSTOM_REPORT_DESC_SIZE));
    usbd_add_endpoint(&custom_in_ep);
    usbd_add_endpoint(&custom_out_ep);

    usbd_initialize();

    nUsbEnumTime = GetCurrentTimeUs();
    while(!usb_device_is_configured()) 
    {
        printf("wait usb device configured...\n");
    }
    printf("USB enum time is %dus\n", GetCurrentTimeUs()- nUsbEnumTime);
}




void boot_hid_test(void)
{
    if(!usb_device_is_configured())
    {
        printf("usb device no configured\n");
        return;
    }
    memset(send_buffer, 0, 64);
    send_buffer[0] = 0xFC;
    send_buffer[1] = 0x30;
    send_buffer[2] = 0x31;

    usbd_ep_start_write(HIDRAW_IN_EP, send_buffer, 12);
}


//获取端点x的应用逻辑操作句柄
stEpAppProcess_t *GetEpAppProcess(uint8_t nEpIdx)
{
    if (sg_arrEpAppProcessMap[nEpIdx] < 0)
    {
        return NULL;
    }
    return &sg_arrEpAppProcess[sg_arrEpAppProcessMap[nEpIdx]];
}

/*
 * 检查usb队列是否在忙 忙则返回1 否则返回0
 */
int32_t GetUsbQueueBusy(eUsbDevice_t eUsbDev)
{
    for (int32_t i = 0; i < g_stUsbEpsInfo[eUsbDev].nEpsCount; i++)
    {
        uint8_t nEpNum = g_stUsbEpsInfo[eUsbDev].arrEpsReportOutAddr[i];
        stEpAppProcess_t *pEpApp = GetEpAppProcess(nEpNum);
        if (pEpApp != NULL && pEpApp->stQueue.bUsbTxBusy)
        {
            return 1;
        }
    }
    return 0;
}

int32_t GetAllUsbQueueBusy(void)
{
    for (eUsbDevice_t eDev = eUsbDev0; eDev < eUsbDevCount; eDev++)
    {
        if (GetUsbQueueBusy(eDev))
        {
            return 1;
        }
    }
    return 0;
}


/*
 * 对USB内存队列清零
 */
int32_t UsbQueueReset(void)
{
    for (int32_t i = 1; i < 8; i++)
    {
        stEpAppProcess_t *pEpApp = GetEpAppProcess(i);
        if (pEpApp != NULL)
        {
            pEpApp->nBlockingCount = 0;
            pEpApp->stQueue.bUsbTxBusy = 0;
            pEpApp->stQueue.nQueueLen = 0;
            pEpApp->stQueue.nQueueBack = 0;
            pEpApp->stQueue.nQueueFront = 0;
        }
    }
    return 0;
}


//初始化一个端点信息
int32_t InitAEpInfo(eUsbDevice_t eDev, uint8_t nEpIdx, uint16_t nEpType, uint16_t nEpSize, uint16_t nEpsPhyOffset,
        uint8_t nReportInAddr, uint8_t nReportOutAddr, uint8_t nQueueSize,
        uint8_t nMaxEPacketSize, eUsbCfgBitType_t eBitType)
{
    g_stUsbEpsInfo[eDev].arrEpsType[nEpIdx] = nEpType;
    g_stUsbEpsInfo[eDev].arrEpsSize[nEpIdx] = nEpSize;
    g_stUsbEpsInfo[eDev].arrEpsReportInAddr[nEpIdx] = nReportInAddr;
    g_stUsbEpsInfo[eDev].arrEpsPhyInAddr[nEpIdx] = nEpsPhyOffset;
    nEpsPhyOffset += g_stUsbEpsInfo[eDev].arrEpsSize[nEpIdx];
    g_stUsbEpsInfo[eDev].arrEpsReportOutAddr[nEpIdx] = nReportOutAddr;
    g_stUsbEpsInfo[eDev].arrEpsPhyOutAddr[nEpIdx] = nEpsPhyOffset;
    nEpsPhyOffset += g_stUsbEpsInfo[eDev].arrEpsSize[nEpIdx];
    g_stUsbEpsInfo[eDev].arrQueueSize[nEpIdx] = nQueueSize;
    g_stUsbEpsInfo[eDev].arrMaxEPacketSize[nEpIdx] = nMaxEPacketSize;
//    EpsCallbackRegister(nReportInAddr, EpsInCallback);
//    EpsCallbackRegister(nReportOutAddr, EpsOutCallback);
    g_stUsbEpsInfo[eDev].arrInterfaceIdxMap[eBitType] = nEpIdx;
    return nEpsPhyOffset;
}

/*
 * 根据枚举类型，对USB队列内存进行分割
 */
int32_t UsbQueueInit(stUsbEpsInfo_t *pUsbEpsInfo)
{
    uint8_t *pUsbQueueMem = sg_arrUsbQueueMem;
    int32_t  nMemOffset = 0;

    MemSet(sg_arrUsbQueueMem, 0, sizeof(sg_arrUsbQueueMem));
    MemSet(sg_arrEpAppProcess, 0, sizeof(sg_arrEpAppProcess));
    for (int32_t i = 0; i < sizeof(sg_arrEpAppProcessMap)/sizeof(sg_arrEpAppProcessMap[0]); i++)
    {
        sg_arrEpAppProcessMap[i] = -1;
    }
    for (int32_t i = 0; i < pUsbEpsInfo->nEpsCount; i++)   //匹配则进行内存分配
    {
        sg_arrEpAppProcess[i].stQueue.arrQueueData = (uint8_t (*)[USB_PACKET_MAX_SIZE])&pUsbQueueMem[nMemOffset];
        nMemOffset += (pUsbEpsInfo->arrQueueSize[i] * USB_PACKET_MAX_SIZE);
        sg_arrEpAppProcess[i].stQueue.arrQueueDataLen = (uint8_t *)&pUsbQueueMem[nMemOffset];
        nMemOffset += pUsbEpsInfo->arrQueueSize[i];
        sg_arrEpAppProcess[i].stQueue.nEPacketSize = pUsbEpsInfo->arrMaxEPacketSize[i];
        sg_arrEpAppProcess[i].stQueue.nMaxQueueSize = pUsbEpsInfo->arrQueueSize[i];

        sg_arrEpAppProcessMap[pUsbEpsInfo->arrEpsReportOutAddr[i]] = i; //建立端点和队列控制器的映射关系
    }
    return 0;
}



/*
 * 关闭usb枚举
 */
int32_t StopUsbDev(eUsbDevice_t eDev)
{
    int32_t nRet = 0;
    if (sg_bUsbAppInit)
    {
        sg_bUsbAppInit = 0;
//        usbd_disconnect(&g_stUsbFsDriver);
//        rcu_periph_reset_enable(RCU_USBFSRST);
//    	rcu_periph_reset_disable(RCU_USBFSRST);
        usbd_deinitialize();
        nRet = 1;
    }
    return nRet;
}

int32_t StopAllUsbDev(void)
{
    for (eUsbDevice_t eDev = eUsbDev0; eDev < eUsbDevCount; eDev++)
    {
        StopUsbDev(eDev);
    }
    return 0;
}


/*
 * USB设备初始化 开始枚举
 */
int32_t StartUsbDev(stUsbEnumInfo_t stUsbEnumInfo)
{
    StopUsbDev(stUsbEnumInfo.eUsbDev);
    if (!sg_bUsbAppInit)
    {
        stUsbEnumInfo.eUsbDev = eUsbDev0;
        g_eUsbCfgType[stUsbEnumInfo.eUsbDev] = stUsbEnumInfo.eUsbCfgType;
        if (eUsbCfgAppDefault != stUsbEnumInfo.eUsbCfgType)
        {
//            InitUsbStrings(stUsbEnumInfo.eUsbDev);    //初始化USB字符串
        }
//        InitUSBDesc(USER_POINT, stUsbEnumInfo); /* USB protocol and register initialize*/
//        UsbQueueInit(&g_stUsbEpsInfo[stUsbEnumInfo.eUsbDev]);  //根据枚举类型，对USB队列内存进行分割
//        UsbPlugInSimulate(); //模拟USB插拔动作

//        uint64_t nUsbEnumTime =0;
//        usbd_desc_register(g_arrUsbDevDesc[stUsbEnumInfo.eUsbDev].Descriptor);
//        usbd_add_interface(usbd_hid_init_intf(&intf0, g_stUsbDefaultHidReportDesc.Descriptor, g_stUsbDefaultHidReportDesc.Descriptor_Size));
//        usbd_add_endpoint(&custom_in_ep);
//        usbd_add_endpoint(&custom_out_ep);
//
//        usbd_initialize();
//
//        nUsbEnumTime = GetCurrentTimeUs();
//        while(!usb_device_is_configured()) 
//        {
//            printf("wait usb device configured...\n");
//        }
//        printf("USB enum time is %dus\n", GetCurrentTimeUs()- nUsbEnumTime);

        
        sg_nUsb0SuspendTick = GetCurrentTime();
        g_arrSetup0ProcessLstTick[eUsbDev0] = GetCurrentTime();
        sg_bUsbAppInit = 1;
    }
    
    return 0;
}

int32_t GetUsbAppInit(eUsbDevice_t eUsbDev)
{
    return sg_bUsbAppInit;
}

uint8_t GetUsbReportStatus(eUsbDevice_t eUsbDev)
{
    return sg_arrSetReport[eUsbDev];
}

uint8_t SetUsbReportStatus(eUsbDevice_t eUsbDev, uint8_t bSetReport)
{
    sg_arrSetReport[eUsbDev] = bSetReport;
    return sg_arrSetReport[eUsbDev];
}

//等待枚举完成 超时退出
int32_t WaitWhileUsbEuming(eUsbDevice_t eUsbDev, uint32_t nTimeOut)
{
    uint32_t nLastTime = GetCurrentTime();
//    while ((!GetUsbReportStatus(eUsbDev) || (GetSystickTime()-g_arrSetup0ProcessLstTick[eUsbDev]) < 300) &&
//            ((GetCurrentTimeUs()-nLastTime) < nTimeOut));
      while ((!GetUsbReportStatus(eUsbDev)) &&
                ((GetCurrentTime()-nLastTime) < nTimeOut));

    return 0;
}



