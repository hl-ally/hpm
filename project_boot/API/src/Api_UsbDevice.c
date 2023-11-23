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
pUsbDevFuncCallback g_pUsbDevCallback[USB_DEV_REPORT_ID_TOTAL] = {NULL};
static stEpAppProcess_t sg_arrEpAppProcess[8] = {0};
static int8_t           sg_arrEpAppProcessMap[8] = {0};

static volatile int32_t sg_bUsbAppInit = 0;
static __attribute__((aligned(8))) uint8_t sg_arrUsbQueueMem[MAX_USB_QUEUE_MEM_SIZE];
static uint32_t sg_nUsb0SuspendTick = 0;
static volatile uint8_t sg_arrSetReport[eUsbDevCount] = {0};
volatile uint32_t g_arrSetup0ProcessLstTick[eUsbDevCount];

struct usbd_interface intf0;
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t read_buffer[USB_PACKET_MAX_SIZE];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t send_buffer[USB_PACKET_MAX_SIZE];

#define HID_STATE_IDLE 0
#define HID_STATE_BUSY 1

//获取端点x的应用逻辑操作句柄
stEpAppProcess_t *GetEpAppProcess(uint8_t nEpIdx)
{
    if (sg_arrEpAppProcessMap[nEpIdx] < 0)
    {
        return NULL;
    }
    return &sg_arrEpAppProcess[sg_arrEpAppProcessMap[nEpIdx]];
}


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
        usbd_ep_start_read(CUSTOM_TOUCH0_EP_OUT_ADDR, read_buffer, CUSTOM_TOUCH0_EP_OUT_SIZE);
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
//    USB_LOG_RAW("ep:%d, actual in len:%d\r\n",ep, nbytes);
    custom_state = HID_STATE_IDLE;
    stEpAppProcess_t *pEpApp = GetEpAppProcess(USB_EP_GET_IDX(ep));
    /*no data need send*/
    if (0 == pEpApp->stQueue.nQueueLen)
    {
        pEpApp->stQueue.bUsbTxBusy = 0;
        if (pEpApp->stQueue.nQueueFront == (pEpApp->stQueue.nQueueBack+1)%pEpApp->stQueue.nMaxQueueSize)
        {
            pEpApp->stQueue.nQueueFront = pEpApp->stQueue.nQueueBack;
        }
        return;
    }
    usbd_ep_start_write(ep, pEpApp->stQueue.arrQueueData[pEpApp->stQueue.nQueueFront], pEpApp->stQueue.arrQueueDataLen[pEpApp->stQueue.nQueueFront]);
    pEpApp->stQueue.nQueueLen = (pEpApp->stQueue.nQueueLen-1) < 0 ? 0 : (pEpApp->stQueue.nQueueLen-1);
    pEpApp->stQueue.nQueueFront = (pEpApp->stQueue.nQueueFront + 1) % pEpApp->stQueue.nMaxQueueSize;
}

static void usbd_hid_custom_out_callback(uint8_t ep, uint32_t nbytes)
{
//    USB_LOG_RAW("actual out len:%d\r\n", nbytes);
    usbd_ep_start_read(ep, read_buffer, nbytes);
//    read_buffer[0] = 0x02; /* IN: report id */
//    usbd_ep_start_write(CUSTOM_TOUCH0_EP_IN_ADDR, read_buffer, nbytes);

    if (g_pUsbDevCallback[read_buffer[0]] != NULL)
    {
        g_pUsbDevCallback[read_buffer[0]](read_buffer, nbytes, (eCmdSource_t)ep);
    }
}

static struct usbd_endpoint custom_in_ep = {
    .ep_cb = usbd_hid_custom_in_callback,
    .ep_addr = CUSTOM_TOUCH0_EP_IN_ADDR
};

static struct usbd_endpoint custom_out_ep = {
    .ep_cb = usbd_hid_custom_out_callback,
    .ep_addr = CUSTOM_TOUCH0_EP_OUT_ADDR
};



int16_t USBEPSendPacket(eUsbDevice_t eDev, uint8_t nEpIdx, uint8_t *pBuffer, int16_t nLen)
{
    stEpAppProcess_t *pEpApp = GetEpAppProcess(nEpIdx);
    //判断枚举是否完成
    if (!usb_device_is_configured() || pEpApp == NULL)
    {
        printf("usb is not enum*********%d, %d, %x\n", nEpIdx, usb_device_is_configured(), pEpApp);
        return 0;
    }
    uint32_t nCurTick = GetCurrentTime();
    uint32_t nNopTimeCnt = 0;
    if (pEpApp->stQueue.nMaxQueueSize > 0) //若此端点带队列缓存区，则压入缓冲区，再执行发送操作
    {
        while (!(pEpApp->stQueue.nQueueLen < (pEpApp->stQueue.nMaxQueueSize-1)) &&
                pEpApp->nBlockingCount < 3 &&
                (GetCurrentTime() - nCurTick) < pEpApp->stQueue.nMaxQueueSize);
        if (!(pEpApp->stQueue.nQueueLen < pEpApp->stQueue.nMaxQueueSize))
        {
            pEpApp->stQueue.bUsbTxBusy = 0;
            pEpApp->stQueue.nQueueFront = (pEpApp->stQueue.nQueueFront + 1) % pEpApp->stQueue.nMaxQueueSize;
            pEpApp->stQueue.nQueueLen--;
            pEpApp->nBlockingCount = (pEpApp->nBlockingCount < 3) ? (pEpApp->nBlockingCount + 1) : pEpApp->nBlockingCount;  //统计连续堵塞次数
        }
        else if (pEpApp->stQueue.nQueueLen != (pEpApp->stQueue.nMaxQueueSize-1))
        {
            pEpApp->nBlockingCount = 0;
        }
        nLen = FN_MIN(nLen, pEpApp->stQueue.nEPacketSize);
        MemCpy(pEpApp->stQueue.arrQueueData[pEpApp->stQueue.nQueueBack], pBuffer, nLen);
        pEpApp->stQueue.arrQueueDataLen[pEpApp->stQueue.nQueueBack] = nLen;
        pEpApp->stQueue.nQueueBack = (pEpApp->stQueue.nQueueBack + 1) % pEpApp->stQueue.nMaxQueueSize;
        pEpApp->stQueue.nQueueLen++;
        pBuffer = pEpApp->stQueue.arrQueueData[pEpApp->stQueue.nQueueFront];
        nLen = pEpApp->stQueue.arrQueueDataLen[pEpApp->stQueue.nQueueFront];
    }
    else
    {
        nNopTimeCnt = 0;
        while (pEpApp->stQueue.bUsbTxBusy &&
               (GetCurrentTime() - nCurTick) < 4 &&
               (++nNopTimeCnt) < 100); //MCU空运行计数, 防止由于优先等原因无法及时更新 Tick 导致看门狗复位
        nLen = FN_MIN(nLen, pEpApp->stQueue.nEPacketSize);
    }
    if (pEpApp->stQueue.bUsbTxBusy == 0)
    {
        pEpApp->stQueue.bUsbTxBusy = 1;

        pEpApp->stQueue.nQueueLen = (pEpApp->stQueue.nQueueLen-1) < 0 ? 0 : (pEpApp->stQueue.nQueueLen-1);
        pEpApp->stQueue.nQueueFront = (pEpApp->stQueue.nQueueFront + 1) % pEpApp->stQueue.nMaxQueueSize;
        usbd_ep_start_write(nEpIdx|0x80, pBuffer, nLen);
    }
    return nLen;
}


/*********************************************************************************************************
 * @Function : USB的接口x发送函数
 * @Parameter: [pBuffer]待发送数据的Buffer
             : [nLen]待发送数据的长度
 * @return   : 返回发送长度
*********************************************************************************************************/
int16_t USBSendPacket(eUsbDevice_t eDev, eUsbCfgBitType_t eBitType, uint8_t *pBuffer, int16_t nLen)
{
    return USBEPSendPacket(eDev, g_stUsbEpsInfo[eDev].arrEpsReportInAddr[g_stUsbEpsInfo[eDev].arrInterfaceIdxMap[eBitType]] & 0x7F, pBuffer, nLen);
}

//注册USB回调
void UsbDevCallbackRegister(uint8_t nReportId, pUsbDevFuncCallback pCallback)
{
    g_pUsbDevCallback[nReportId] = pCallback;
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


uint8_t g_arrUsbDesc[1000] = {0};
/*
 * USB设备初始化 开始枚举
 */
int32_t StartUsbDev(stUsbEnumInfo_t stUsbEnumInfo)
{
    StopUsbDev(eUsbDev0);
    if (!sg_bUsbAppInit)
    {
        printf("usb device start enum ...\n");
        stUsbEnumInfo.eUsbDev = eUsbDev0;
        g_eUsbCfgType[stUsbEnumInfo.eUsbDev] = stUsbEnumInfo.eUsbCfgType;
        if (eUsbCfgAppDefault != stUsbEnumInfo.eUsbCfgType)
        {
            InitUsbStrings(stUsbEnumInfo.eUsbDev);    //初始化USB字符串
            printf("init usb strings finish\n");
        }
        InitUSBDesc(USER_POINT, stUsbEnumInfo); /* USB protocol and register initialize*/
        UsbQueueInit(&g_stUsbEpsInfo[stUsbEnumInfo.eUsbDev]);  //根据枚举类型，对USB队列内存进行分割
//        UsbPlugInSimulate(); //模拟USB插拔动作

        #if 1
        // 设备描述符
        printf("usb device desc(%d):", g_arrUsbDevDesc[stUsbEnumInfo.eUsbDev].Descriptor_Size);
        for(int i=0;i<g_arrUsbDevDesc[stUsbEnumInfo.eUsbDev].Descriptor_Size;i++)
        {
            printf("%02X ", g_arrUsbDevDesc[stUsbEnumInfo.eUsbDev].Descriptor[i]);
        }
        printf("\n");

        // 配置描述符
        printf("usb config desc(%d):", g_arrUsbCfgDesc[stUsbEnumInfo.eUsbDev].Descriptor_Size);
        for(int i=0;i<g_arrUsbCfgDesc[stUsbEnumInfo.eUsbDev].Descriptor_Size;i++)
        {
            printf("%02X ", g_arrUsbCfgDesc[stUsbEnumInfo.eUsbDev].Descriptor[i]);
        }
        printf("\n");

        // 报告描述符
        printf("usb report  desc(%d):", g_stUsbDefaultHidReportDesc.Descriptor_Size);
        for(int i=0;i<g_stUsbDefaultHidReportDesc.Descriptor_Size;i++)
        {
            printf("%02X ", g_stUsbDefaultHidReportDesc.Descriptor[i]);
        }
        printf("\n");
        #endif

        int i=  0;
        int index = 0;
        for (i = 0;i<g_arrUsbDevDesc[stUsbEnumInfo.eUsbDev].Descriptor_Size;i++)
        {
            g_arrUsbDesc[index + i] = g_arrUsbDevDesc[stUsbEnumInfo.eUsbDev].Descriptor[i];
        }
        
        index += g_arrUsbDevDesc[stUsbEnumInfo.eUsbDev].Descriptor_Size;
        for (i = 0;i<g_arrUsbCfgDesc[stUsbEnumInfo.eUsbDev].Descriptor_Size;i++)
        {
            g_arrUsbDesc[index + i] = g_arrUsbCfgDesc[stUsbEnumInfo.eUsbDev].Descriptor[i];
        }
        index += g_arrUsbCfgDesc[stUsbEnumInfo.eUsbDev].Descriptor_Size;
        
        for (eUsbStrType_t nType = eUsbStrLangID;nType < eUsbStrTotal; nType++)
        {
            for (i = 0;i<g_arrHidString[nType].Descriptor_Size;i++)
            {
                g_arrUsbDesc[index + i] = g_arrHidString[nType].Descriptor[i];
            }
            index += g_arrHidString[nType].Descriptor_Size;
        }
        
        usbd_desc_register(g_arrUsbDesc);
        usbd_add_interface(usbd_hid_init_intf(&intf0, g_stUsbDefaultHidReportDesc.Descriptor, g_stUsbDefaultHidReportDesc.Descriptor_Size));
        usbd_add_endpoint(&custom_in_ep);
        usbd_add_endpoint(&custom_out_ep);

//        for (int32_t i = 0; i < g_stUsbEpsInfo[eUsbDev0].nEpsCount; i++)
//        {
//            /* Initialize Endpoints */
//            uint8_t bEpNum = (uint8_t)(g_stUsbEpsInfo[eUsbDev0].arrEpsReportOutAddr[i]);
//            SetEPType(bEpNum, g_stUsbEpsInfo[eUsbDev0].arrEpsType[i]);
//            SetEPTxAddr(bEpNum, g_stUsbEpsInfo[eUsbDev0].arrEpsPhyInAddr[i]);
//            SetEPRxAddr(bEpNum, g_stUsbEpsInfo[eUsbDev0].arrEpsPhyOutAddr[i]);
//            SetEPTxCount(bEpNum, g_stUsbEpsInfo[eUsbDev0].arrEpsSize[i]);
//            SetEPRxCount(bEpNum, g_stUsbEpsInfo[eUsbDev0].arrEpsSize[i]);
//            SetEPRxStatus(bEpNum, EP_RX_VALID);
//            SetEPTxStatus(bEpNum, EP_TX_NAK);
//        }

        usbd_initialize();

        uint64_t nUsbEnumTime = GetCurrentTimeUs();
        while( !usb_device_is_configured() && (GetCurrentTimeUs() - nUsbEnumTime <= 300*1000)) 
        {
//            printf("wait usb device configured...%llu-%llu\n", nUsbEnumTime, GetCurrentTimeUs());
        }
        printf("USB enum time is %llu(us)\n", (GetCurrentTimeUs()- nUsbEnumTime));

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



