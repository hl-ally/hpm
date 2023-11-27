//#include "usb_lib.h"
#include "Api_Math.h"
//#include "Api_Adc.h"
//#include "Api_Gpio.h"
//#include "Api_Timer.h"
#include "TouchSignal.h"
#include "GlobalVariables.h"
//#include "scan.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "Evaluation.h"
//#include "OtherFunction.h"
//#include "ScanSolution.h"
#include "app_systick.h"

#define USB_SendPacket(p0, p1)               ((void)0U)

uint16_t* g_nAllAdcIndex = NULL;             //有效信号的索引
uint16_t  g_nAllAdcIndexCount = 0;           //有效信号的数量
uint16_t* g_nAllAdcIndexIndex = NULL;        //有效无效信号的索引
uint16_t  g_nAllAdcIndexIndexCount = 0;      //有效无效信号的数量

stTONIdxInfo_t* g_arrIndexSum = NULL;              //有效信号中，每一个发射灯的有效信号的Index相对此组接收信号位置的Shift
uint16_t* g_nLineLedRelationArray = NULL;    //记录所有有效光线的灯管位置信息(包括每个接收Group的可用光线数)
uint16_t* g_arrAdcIndexOfBlock = NULL;       //每组一发多收所依据的ADC的索引值
uint16_t  g_nBlockCount = 0;                 //有效一发多收的数量--->开灯次数
uint16_t  g_nBlockCountAxis[2] = {0,0};
uint16_t  g_nLedPositioCount = 0 ;           //LED所有灯数量
uint16_t  g_nLineLedRelationCount = 0 ;
uint8_t   g_bSendTouchData = 0;
uint8_t   g_eTouchData = TOUCH_NULL_REPORT_ID; //Modified by Roc

volatile int32_t g_bDebuging = 0;
volatile int32_t g_bRecvQTData = 0; //接收QT数据开关
volatile int32_t g_bRunAlg = 1;     //下位机仿真时是否仍然跑算法


// 为了编译通过，临时函数
int32_t GetUsableAdcCnt(adc_type_t* pAdc)
{
    return g_nAllAdcIndexCount;
}


/*
 * 发送光线状态的原始数据
 * @Parameter:nFrame: 帧号
 * @Parameter:nReportID: 数据类型
 * @Parameter:pAdc: 指向ADC数据的指针
 * @return   : 错误返回-1，否则返回发送的数据长度
 */
int32_t SendLineStatusArray(uint8_t nFrame, uint8_t nReportID, adc_type_t* pAdc)
{
    uint8_t pBuf[USB_PACKET_MAX_SIZE] = {nReportID, nFrame, 0x00};
    int32_t nAvailiableCnt = 0;
    int32_t i = 0, k = 0;
    int32_t nIndex = 64;
    uint8_t nStart = 8;
    uint32_t nCrc = 0;

    *((uint16_t*)(&pBuf[2])) = 0;   //包号
    *((uint32_t*)(&pBuf[4])) = ((g_nAllAdcIndexCount + 31) / 32) * 4 + 4;   //数据和CRC校验码的长度
    MemSet(&pBuf[8], 0xff, sizeof(pBuf) - nStart);
    Crc32ResetDT();

    while(i < g_nAllAdcIndexCount)
    {
        nAvailiableCnt = GetUsableAdcCnt(pAdc);
        for(; nIndex < sizeof(pBuf) * 8 && i < nAvailiableCnt; nIndex++)
        {
            //现在的光线索引表已经是连续有效的
            k = i++;
            if(g_ORG[k] < ERR_ADC || g_ORG[k] * THRESHOLD_RATE > pAdc[k])
            {
                // if light needs to be reported,then clear corresponding bit
//                pBuf[nIndex / 8] &= DataConversionClear[nIndex % 8];
            }
        }

        if(nIndex == sizeof(pBuf) * 8)
        {
            USB_SendPacket(pBuf, sizeof(pBuf));
            Crc32CalcBlkCRC((uint32_t*)(&pBuf[nStart]), (sizeof(pBuf) - nStart) / 4);
            nStart = 4;
            nIndex = 32;
            (*((uint16_t*)(&pBuf[2])))++;
            MemSet(&pBuf[4], 0xff, sizeof(pBuf) - nStart);
        }
    }

    nIndex = (nIndex + 31) / 32 * 32;
   nCrc = Crc32CalcBlkCRC((uint32_t*)(&pBuf[nStart]), nIndex / 32 - nStart / 4);
    if(nIndex == sizeof(pBuf) * 8)
    {
        USB_SendPacket(pBuf, sizeof(pBuf));
        (*((uint16_t*)(&pBuf[2])))++;
        MemSet(&pBuf[nStart], 0xff, sizeof(pBuf) - nStart);
        nIndex = 4 * 8;
    }
    *((uint32_t*)(&pBuf[nIndex / 8])) = nCrc;
    USB_SendPacket(pBuf, sizeof(pBuf));

    return 0;
}

/*
 * 发送光线值的原始数据
 * @Parameter:nFrame: 帧号
 * @Parameter:nReportID: 数据类型
 * @Parameter:pAdc: 指向ADC数据的指针
 * @return   : 错误返回-1，否则返回发送的数据长度
 */
int32_t SendLineValuesArray(uint8_t nFrame, uint8_t nReportID, adc_type_t* pAdc)
{
    uint8_t pBuf[USB_PACKET_MAX_SIZE] = {nReportID, nFrame, 0x00};
    int32_t nAvailiableCnt = 0;
    uint8_t nStart = 8;
    int32_t nIndex = 8;
    uint32_t nCrc = 0;
    int32_t i = 0;

    nStart = nStart;
    *((uint16_t*)(&pBuf[2])) = 0;   //包号
    *((uint32_t*)(&pBuf[4])) = (g_nAllAdcIndexCount + 3) / 4 * 4 + 4;   //数据和CRC校验码的长度
    Crc32ResetDT();

    while(i < g_nAllAdcIndexCount)
    {
        nAvailiableCnt = GetUsableAdcCnt(pAdc);
        for(; nIndex < sizeof(pBuf) && i < nAvailiableCnt; nIndex++)
        {
            pBuf[nIndex] = pAdc[i++];
        }

        if(nIndex == sizeof(pBuf))
        {
            USB_SendPacket(pBuf, sizeof(pBuf));
            Crc32CalcBlkCRC((uint32_t*)(&pBuf[nStart]), (sizeof(pBuf) - nStart) / 4);

            (*((uint16_t*)(&pBuf[2])))++;
            nStart = 4;
            nIndex = 4;
        }
    }

    nIndex = (nIndex + 3) / 4 * 4;
    nCrc = Crc32CalcBlkCRC((uint32_t*)(&pBuf[nStart]), nIndex / 4 - nStart / 4);
    if(nIndex == sizeof(pBuf))
    {
        USB_SendPacket(pBuf, sizeof(pBuf));
        (*((uint16_t*)(&pBuf[2])))++;
        nIndex = 4;
    }
    *((uint32_t*)(&pBuf[nIndex])) = nCrc;
    USB_SendPacket(pBuf, sizeof(pBuf));

    return 0;
}

/*
 * 发送光线值的压缩数据
 * @Parameter:nFrame: 帧号
 * @Parameter:nReportID: 数据类型
 * @Parameter:pAdc: 指向ADC数据的指针
 * @return   : 错误返回-1，否则返回发送的数据长度
 */
int32_t SendCompressLineValuesArray(uint8_t nFrame, uint8_t nReportID, adc_type_t* pAdc)
{
    uint8_t pBuf[USB_PACKET_MAX_SIZE] = {nReportID, nFrame, 0x00};
    int32_t nAvailiableCnt = 0;
    int32_t i = 0, k = 0;
    int32_t nIndex = 4;
    uint32_t nCrc = 0;

    uint32_t nTotalLen = 0;
    uint8_t bFinished = 0;
    int32_t nCount = 1;

    *((uint16_t*)(&pBuf[2])) = 0;   //包号
    Crc32ResetDT();

    while(i < g_nAllAdcIndexCount)
    {
        nAvailiableCnt = GetUsableAdcCnt(pAdc);
        for(; nIndex < sizeof(pBuf) && i < nAvailiableCnt; i++)
        {
            //现在的光线索引表已经是连续有效的
            k = i;
            if(pAdc[k] < g_ORG[k] * UNSTABLE_RATE || g_ORG[k] < pAdc[k] * UNSTABLE_RATE)
            {
                pBuf[nIndex++] = (uint8_t)((pAdc[k]) >> 1);
                g_ORG[k] = pAdc[k];
            }
            else
            {
                while(i < g_nAllAdcIndexCount)
                {
                    nAvailiableCnt = GetUsableAdcCnt(pAdc);
                    for(; i < nAvailiableCnt - 1 && nCount < 127; i++)
                    {
                        //现在的光线索引表已经是连续有效的
                        k = i + 1;
                        if(pAdc[k] < g_ORG[k] * UNSTABLE_RATE || g_ORG[k] < pAdc[k] * UNSTABLE_RATE)
                        {
                            bFinished = 1;
                            break;
                        }
                        nCount++;
                    }

                    if(bFinished == 1 || nCount == 127 || i == g_nAllAdcIndexCount - 1)
                    {
                        pBuf[nIndex++] = 0x80 | nCount;
                        bFinished = 0;
                        nCount = 1;
                        break;
                    }
                }
            }
        }

        if(nIndex == sizeof(pBuf))
        {
            nTotalLen += (sizeof(pBuf) - 4);
            USB_SendPacket(pBuf, sizeof(pBuf));
            nCrc = Crc32CalcBlkCRC((uint32_t*)(&pBuf[4]), (nIndex - 4) / 4);
            (*((uint16_t*)(&pBuf[2])))++;
            nIndex = 4;
        }
    }

    if(nIndex > 4)
    {
        nIndex = (nIndex + 3) / 4 * 4;
        nTotalLen += (nIndex - 4);
        USB_SendPacket(pBuf, sizeof(pBuf));
        nCrc = Crc32CalcBlkCRC((uint32_t*)(&pBuf[4]), (nIndex - 4) / 4);
    }

    *((uint16_t*)(&pBuf[2])) = 0xFFFF;
    *((uint32_t*)(&pBuf[4])) = nCrc;
    *((uint32_t*)(&pBuf[8])) = nTotalLen;
    USB_SendPacket(pBuf, sizeof(pBuf));

    return 0;
}

/*
 * 发送光线值的压缩数据的淹码(及原始值)
 * @Parameter:nFrame: 帧号
 * @Parameter:nReportID: 数据类型
 * @Parameter:pAdc: 指向ADC数据的指针
 * @return   : 错误返回-1，否则返回发送的数据长度
 */
int32_t SendCompressLineValuesMaskArray(uint8_t nFrame, uint8_t nReportID, adc_type_t* pAdc)
{
    uint8_t pBuf[USB_PACKET_MAX_SIZE] ;//= {nReportID, nFrame, 0x00};
    int32_t nAvailiableCnt = 0;
    int32_t i = 0, k = 0;
    int32_t nIndex = 4;
    uint32_t nCrc = 0;

    uint32_t nTotalLen = 0;

    *((uint16_t*)(&pBuf[2])) = 0;   //包号
    Crc32ResetDT();

    while(i < g_nAllAdcIndexCount)
    {
        nAvailiableCnt = GetUsableAdcCnt(pAdc);
        for(; nIndex < sizeof(pBuf) && i < nAvailiableCnt; i++)
        {
            //现在的光线索引表已经是连续有效的
            k = i;
            pBuf[nIndex++] = (uint8_t)((pAdc[k]) >> 1);
            g_ORG[k] = pAdc[k];
        }

        if(nIndex == sizeof(pBuf))
        {
            nTotalLen += (sizeof(pBuf) - 4);
            USB_SendPacket(pBuf, sizeof(pBuf));
            nCrc = Crc32CalcBlkCRC((uint32_t*)(&pBuf[4]), (nIndex - 4) / 4);
            (*((uint16_t*)(&pBuf[2])))++;
            nIndex = 4;
        }
    }

    if(nIndex > 4)
    {
        nIndex = (nIndex + 3) / 4 * 4;
        nTotalLen += (nIndex - 4);
        USB_SendPacket(pBuf, sizeof(pBuf));
        nCrc = Crc32CalcBlkCRC((uint32_t*)(&pBuf[4]), (nIndex - 4) / 4);
    }

    *((uint16_t*)(&pBuf[2])) = 0xFFFF;
    *((uint32_t*)(&pBuf[4])) = nCrc;
    *((uint32_t*)(&pBuf[8])) = nTotalLen;
    USB_SendPacket(pBuf, sizeof(pBuf));

    return 0;
}

#define CPRSDEBUG(a)
#define PFData 0
#define CprsSendWaitCycle 20

/*
 * 发送stUSB_CprsQueue 中的数据
 * @Parameter:
 * @return   : 是否压缩完成
 */

/*********************************************************************************************************
 * @Function :     compress lights info and use send it
 * @Parameter:     nFrame        :
                 nReportID    :
                 pAdc        : ADC buff going to be compressed
 * @return   : CprsStatus    : compress results
 *********************************************************************************************************/
/*
** use USB peripheral library func to control USB1 EP1/2 interrupt enable register
*/
extern void USB0EP1IN_IsrCtl(uint8_t nState);
extern void USB0EP2IN_IsrCtl(uint8_t nState);

int32_t SendCompressLineStatusArray_New(uint8_t nFrame, uint8_t nReportID, adc_type_t* pAdc)
{
    uint8_t pBuf[64] = {nReportID, nFrame, 0x00};
    int32_t nAvailiableCnt = 0;
    int32_t i = 0, k = 0;
    int32_t nIndex = 4;
    uint32_t nCrc = 0;
    uint8_t  nAdc = 0;

    uint32_t nTotalLen = 0;
    uint8_t bFinished = 0;
    int32_t nCount = 1;

    *((uint16_t*)(&pBuf[2])) = 0;   //包号
    Crc32ResetDT();

    while(i < g_nAllAdcIndexCount)
    {
        nAvailiableCnt = g_nAllAdcIndexCount;
        for(; nIndex < sizeof(pBuf) && i < nAvailiableCnt; i++)
        {
            //现在的光线索引表已经是连续有效的
            k = i;
            nAdc = pAdc[k];
            if (nAdc < 40)
            {
                pBuf[nIndex++] = 0;
            }
            else
            {
                while(i < g_nAllAdcIndexCount)
                {
                    nAvailiableCnt = g_nAllAdcIndexCount;
                    for(; i < nAvailiableCnt - 1 && nCount < 127; i++)
                    {
                        //现在的光线索引表已经是连续有效的
                        k = i + 1;
                        nAdc = pAdc[k];
                        if(!(nAdc < 40))
                        {
                            bFinished = 1;
                            break;
                        }
                        nCount++;
                    }
                    if(bFinished == 1 || nCount == 127 || i == g_nAllAdcIndexCount - 1)
                    {
                        pBuf[nIndex++] = 0x80 | nCount;
                        bFinished = 0;
                        nCount = 1;
                        break;
                    }
                }
            }
        }
        if(nIndex == sizeof(pBuf))
        {
            nTotalLen += (sizeof(pBuf) - 4);
            USB_SendPacket(pBuf, sizeof(pBuf));
            nCrc = Crc32CalcBlkCRC((uint32_t*)(&pBuf[4]), (nIndex - 4) / 4);
            (*((uint16_t*)(&pBuf[2])))++;
            nIndex = 4;
        }
    }

    if(nIndex > 4)
    {
        nIndex = (nIndex + 3) / 4 * 4;
        nTotalLen += (nIndex - 4);
        USB_SendPacket(pBuf, sizeof(pBuf));
        nCrc = Crc32CalcBlkCRC((uint32_t*)(&pBuf[4]), (nIndex - 4) / 4);
    }

    *((uint16_t*)(&pBuf[2])) = 0xFFFF;
    *((uint32_t*)(&pBuf[4])) = nCrc;
    *((uint32_t*)(&pBuf[8])) = nTotalLen;
    USB_SendPacket(pBuf, sizeof(pBuf));
    return 0;
}

/*
 * ADC与ORG中buff 填充测试值
 * @Parameter:
 * @return   :
 */

void SetADCTst(void)
{
//    adc_type_t *p = g_stScanCoreVar.pADCCurrent;
//    MemSet(g_ORG,0xcc,g_stScanCoreVar.nAdcTotal);
//
//#if 0
//    uint32_t j;
//    srand(0);
//    for(j=0; j<g_stScanCoreVar.nAdcTotal; j++)
//    {
//        p[j] = rand()&0xFF;
//        //printf("[%d]:%02x\r\n",j,p[j]);
//    }
//    printf("Rand Over\r\n");
//#endif
//
//#if 1
//    MemSet(p,0xcf,g_stScanCoreVar.nAdcTotal);
//#endif
}

void PfCprsQueue1ItemTST(uint8_t* pbf)
{
    if(PFData)
    {
        uint8_t j;
        printf("___________S______________\r\n");
        for(j=0; j<CprsQItemSize; j++)
        {
            if(j<4)continue;
            if(j==32)
            {
                printf("\r\n");
            }
            printf("%02x\t",pbf[j]);
        }
        printf("\r\n");
        printf("__________E_______________\r\n\n\n");
    }
}


/*
 * 压缩ADC数据,输出光线状态
 * @Parameter:pAdc: 指向ADC数据的指针
 * @return   : 一帧数据是否已经处理完: 0-> 处理完；1->未处理完
 */
#define AI_DEMAND     0            //usded for AI processing


int32_t CompressLineStatus(uint8_t nFrame, uint8_t nReportID, adc_type_t* pAdc)
{
#if 0
    uint8_t*    pBuf;
    int16_t     nIndex = 3;             //Byte Index of a USB Packet
    int32_t i = 0, k = 0;           //i => light index
    int32_t nCount;

    static uint16_t    nPacketNum;
    static uint32_t    nTotalLen;            //USB Transfered Bytes of valid data
    static uint32_t    nCrc;
    static int32_t     nAvailiableCnt;

    if(stUSB_CprsQueue.bQueueStatus == CprsQFrameOvr)
    {
        //start of a new frame
        //init relative var;

        Crc32ResetDT();
        nCrc            = 0;
        nPacketNum      = 0;
        nTotalLen       = 0;
        nAvailiableCnt  = 0;
        g_nhaveLinePre     = g_nhaveLine;
        g_nDownLightPre = g_nDownLight;
        g_nhaveLine     = 0;
        g_nDownLight = 0;
        /*
        MemSet((char*)stBlockedLight.BlockedLightInd,0x00,sizeof(stBlockedLight.BlockedLightInd));
        stBlockedLight.LightCnt = 0;
        */
    }
    else if(stUSB_CprsQueue.bQueueStatus == CprsQFrameCtn)
    {
        //recovery from last unfinshed frame
        //revovery relative var
        i = stUSB_CprsQueue.nCprsADCInd;
    }

    //important ****bookmark1****
    //make sure when checking pAdc[last valid lights] will cause a break 100%
    g_ORG[g_nAllAdcIndexCount - 1]     = 0x7F;
    pAdc[g_nAllAdcIndexCount - 1]     = 0x00;
    //important ****bookmark1****


    if(stUSB_CprsQueue.nQueueLen < CprsQLenMax-1 && i < g_nAllAdcIndexCount)
    {
        //the 1st CprsBuff
        pBuf = (uint8_t*)stUSB_CprsQueue.pQueueData[(stUSB_CprsQueue.nQueueBack+1)%CprsQLenMax];
        MemSet((void*)(&pBuf[4]),0x00, CprsQItemSize-4);
        pBuf[0] = nReportID;
        pBuf[1] = nFrame;
        *((uint16_t*)(&pBuf[2])) = nPacketNum;
    }

    while(stUSB_CprsQueue.nQueueLen < CprsQLenMax-1 && i < g_nAllAdcIndexCount)
    {
        //not a full queue -1
        //uncompressed lights remains

        if(nAvailiableCnt!= g_nAllAdcIndexCount && nAvailiableCnt -i < 127)
        {
            //make sure one byte size of ADC data are ready;
            nAvailiableCnt = GetUsableAdcCnt(pAdc);
        }

        //LedOn();
        while(nIndex < CprsQItemSize-1 && i < nAvailiableCnt)
        {
            //Compressing One packet's data
            //until pBuf fulled or packet ends
            //现在的光线索引表已经是连续有效的
            k = i;
            i++;

            //if(cond1)  :
            if(pAdc[k] <= (uint8_t)(ERR_ADC))
            {
            //ADC value to small
            //should place in the first place
                pBuf[++nIndex] = 0;
                g_nhaveLine++;
                g_nDownLight++;
            }

            //if(cond2)
#if 1 == AI_DEMAND
            else if(g_ORG[k] > pAdc[k]*3/4)
#else
            else if(g_ORG[k] > pAdc[k])
#endif
            {
                //processing a byte of blocked lights
                //transfer light shade percentage
                //calculate percentage takes a lot of time
                pBuf[++nIndex] = (pAdc[k] * THRESHOLD_CPRS_MUL / g_ORG[k]) & 0x7F;
                g_nhaveLine++;
            }

            else
            {
                //processing a byte of non-blocked lights
                //count how many conjucted un-blocked lights together;
                for(nCount=1; nCount < 127; nCount++)
                {
                    //important ****bookmark1****
                    //by leaving (i<nAvailiableCnt) boundary out here, could save 130us time together;
                    //现在的光线索引表已经是连续有效的
                    k = i;      //processing next;
                    //if(cond1 || cond2 ||...)
#if 1 == AI_DEMAND
                    if(g_ORG[k] > pAdc[k]*3/4)
#else
                    if(g_ORG[k] > pAdc[k])
#endif
                    {
                        //g_nhaveLine = 1;
                        break;
                    }
                    i++;
                }
                pBuf[++nIndex] = 0x80|nCount;
            }
        }
        //LedOff();
        if(nIndex == CprsQItemSize-1)
        {
            //if it's a full pbuff
            //packet num / queue processing for next packet
            //LedOn();
            nTotalLen += (nIndex - 3);
            nCrc = Crc32CalcBlkCRC((uint32_t*)(&pBuf[4]), ((nIndex+1) -4) / 4);
            //CPRSDEBUG(printf("CRC:%0x,\tlen:%d \tPckInd:%d\ti=:%d\r\n",nCrc,((nIndex+1) -4) / 4,nPacketNum,i);)     //Roc
            CPRSDEBUG(PfCprsQueue1ItemTST(pBuf);)

            //This queue finished, adding packet information
            //pBuf reassign
            //another new packet

            NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
            stUSB_CprsQueue.nQueueBack = (stUSB_CprsQueue.nQueueBack+1)%CprsQLenMax ;
            stUSB_CprsQueue.nQueueDataByts[stUSB_CprsQueue.nQueueBack] = nIndex+1;
            stUSB_CprsQueue.nCprsADCInd = i;
            stUSB_CprsQueue.nQueueLen ++;
            NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
            //printf("%3d,%3d--(%d,%d-%d)\t\t%d\r\n",nPacketNum,nIndex, stUSB_CprsQueue.nQueueLen,stUSB_CprsQueue.nQueueFront,stUSB_CprsQueue.nQueueBack,stUSB_CprsQueue.nCprsADCInd);
            if(g_stScanCoreVar.bScanEnd)
            {
                if(0 == stUsb0EP2SendQueue.bUsbTxBusy)
                {
                    //USB0EP2IN_IsrCtl(1);
                    NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
//                    USB0_EP2SendCprsData();
                    //USB Endpoint 2 sends data
                    usb_packet_sent_ep2 = 1; 
                    //USB0EP2IN_IsrCtl(0);
                    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
                }

            }
            nIndex = 3;
            nPacketNum = *(uint16_t*)(&pBuf[2]) + 1;


            pBuf = (uint8_t*)stUSB_CprsQueue.pQueueData[(stUSB_CprsQueue.nQueueBack+1)%CprsQLenMax];
            MemSet((void*)(&pBuf[4]),0x00, CprsQItemSize-4);
            pBuf[0] = nReportID;
            pBuf[1] = nFrame;
            *((uint16_t*)(&pBuf[2])) = nPacketNum;
            //LedOff();
        }

        if(i == g_nAllAdcIndexCount && nIndex < CprsQItemSize-1 && nIndex > 3)
        {
            //all lights are examined, and not a full packet
            //modify the last data packet

            //modify nIndex to satified int32_t data type
            //4 byte alligned and byte stuff
            nIndex = (nIndex + 4) / 4 * 4 - 1;
            nTotalLen += (nIndex - 3);
            nCrc = Crc32CalcBlkCRC((uint32_t*)(&pBuf[4]), ((nIndex+1) -4) / 4);
            CPRSDEBUG(printf("CRC:%0x,\tlen:%d \tPckInd:%d\ti=:%d\r\n",nCrc,((nIndex+1) -4) / 4,nPacketNum,i);)     //Roc
            CPRSDEBUG(PfCprsQueue1ItemTST(pBuf);)
            CPRSDEBUG(printf("--------new end---------\r\n\n\n\n\n\n");)

            NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
            stUSB_CprsQueue.nQueueBack = (stUSB_CprsQueue.nQueueBack+1)%CprsQLenMax ;
            stUSB_CprsQueue.nQueueDataByts[stUSB_CprsQueue.nQueueBack] = nIndex+1;
            stUSB_CprsQueue.nCprsADCInd = i;
            stUSB_CprsQueue.nQueueLen ++;
            NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
            //printf("%3d,%3d--(%d,%d-%d)\t\t%d\r\n",nPacketNum,nIndex, stUSB_CprsQueue.nQueueLen,stUSB_CprsQueue.nQueueFront,stUSB_CprsQueue.nQueueBack,stUSB_CprsQueue.nCprsADCInd);

            nIndex = 3;
            nPacketNum = *(uint16_t*)(&pBuf[2]) + 1;

        }
        //Unitll this place , a complete Frame's Data is ready!!!
    }

    if(stUSB_CprsQueue.nQueueLen < CprsQLenMax && i >= g_nAllAdcIndexCount )
    {
        //if all lights are examined, then adding checking packet

        pBuf = (uint8_t*)stUSB_CprsQueue.pQueueData[(stUSB_CprsQueue.nQueueBack+1)%CprsQLenMax];
        MemSet(&pBuf[12],0x00, CprsQItemSize-12);

        pBuf[0] = nReportID;
        pBuf[1] = nFrame;
        *((uint16_t*)(&pBuf[2])) = 0xFFFF;
        *((uint32_t*)(&pBuf[4])) = nCrc;
        *((uint32_t*)(&pBuf[8])) = nTotalLen;

        NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
        stUSB_CprsQueue.nQueueBack = (stUSB_CprsQueue.nQueueBack+1) % CprsQLenMax ;
        stUSB_CprsQueue.nQueueDataByts[stUSB_CprsQueue.nQueueBack] = CprsQItemSize;
        stUSB_CprsQueue.nQueueLen ++;
        stUSB_CprsQueue.bQueueStatus = CprsQFrameOvr;
        stUSB_CprsQueue.nCprsADCInd = 0;
        NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);

        //printf("Last Packet--(%d,%d-%d)\t\t%d\r\n", stUSB_CprsQueue.nQueueLen,stUSB_CprsQueue.nQueueFront,stUSB_CprsQueue.nQueueBack,stUSB_CprsQueue.nCprsADCInd);

        //a frame is over, clear reigster
        //..............clear............
        nPacketNum = 0;
        i = 0;
    }
    else if(stUSB_CprsQueue.nQueueLen >= CprsQLenMax-1)
    {
        //a frame compressing is for continue, store reigster
        //..............store............
        stUSB_CprsQueue.bQueueStatus = CprsQFrameCtn;
    }

    return stUSB_CprsQueue.bQueueStatus ;
#endif
    return 0;
}

/*
 * 发送空的同步包
 * @Parameter:nFrame: 帧号
 * @Parameter:nReportID: 数据类型
 * @Parameter:pAdc: 指向ADC数据的指针
 * @return   : 错误返回-1，否则返回发送的数据长度
 */
int32_t SendNullData(uint8_t nFrame, uint8_t nReportID, adc_type_t* pAdc)
{
    uint8_t pBuf[USB_PACKET_MAX_SIZE];// = {nReportID, nFrame, 0x00, 0x00};
    UNUSED(pBuf[0]);
    USB_SendPacket(pBuf, sizeof(pBuf));
    return 0;
}

/*
 * 发送触摸数据
 * @Parameter:eType: 数据类型
 * @Parameter:pAdc: 指向ADC数据的指针
 * @return   : 错误返回-1，否则返回发送的数据长度
 */
int32_t SendTouchData(uint8_t eType, adc_type_t* pAdc)
{
    static uint8_t s_nFrame = 0;
    if(g_bSendTouchData == 1)
    {
        switch(eType)
        {
        case TOUCH_NULL_REPORT_ID:
        case TOUCH_NULL_REPORT_MASK_ID:
        {
            SendNullData(s_nFrame, eType, pAdc);
        }
        break;

        case TOUCH_LINE_STATU_REPORT_ID:
        case TOUCH_LINE_STATU_REPORT_MASK_ID:
        {
            SendLineStatusArray(s_nFrame, eType, pAdc);
        }
        break;

        case TOUCH_LINE_VALUE_REPORT_ID:
        case TOUCH_LINE_VALUE_REPORT_MASK_ID:
        {
            SendLineValuesArray(s_nFrame, eType, pAdc);
        }
        break;

        case TOUCH_LINE_VALUE_COMPRESS_ID:
        {
            SendCompressLineValuesArray(s_nFrame, eType, pAdc);
        }
        break;

        case TOUCH_LINE_VALUE_COMPRESS_MASK_ID:
        {
            SendCompressLineValuesMaskArray(s_nFrame, eType, pAdc);
        }
        break;

        case TOUCH_LINE_STATU_COMPRESS_ID:
        case TOUCH_LINE_STATU_COMPRESS_MASK_ID:
        {
            //SetADCTst();
            //NVIC_DisableIRQ(USB1_IRQn);
            //T_On();
#if CprsQueueEnable
            //reorganize the data first and then USB send
            SendCompressLineStatusArray_New(s_nFrame, eType, pAdc);
#else
            //reorganization and USB sending are processed simutanously
            SendCompressLineStatusArray(s_nFrame, eType, pAdc);
#endif
            //T_Off();
            //NVIC_EnableIRQ(USB1_IRQn);
        }
        break;

        default:
        {
        }
        break;
        }
    }

    s_nFrame++;
    return 0;
}

//发送同步信号
int32_t SendSyncData(void)
{
    uint8_t arrData[USB_PACKET_MAX_SIZE] = {CMD_F0_REPORT_ID, CMD_F0_SYNC_SIGNAL, 1};

    *(int32_t *)&arrData[3] = g_nCurFrame; //set frame num
    USBSendPacket(g_eUsbDevice, eUsbCfgBulkBit, arrData, sizeof(arrData));
    return 0;
}

//获取touch数据
int32_t GetTouchData(stETouchData_t *pETouchData)
{
    static uint32_t s_nTickSync = 0;

    pETouchData->nLen = 0;
    if (!g_bRecvQTData && !USE_ALG_EMULATOR)
    {
//        if (g_stScanCoreVar.nScanCount > 0 && SleepMode())
//        {
//            StartScan();
//            pETouchData->pData = (uint8_t *)(g_stScanCoreVar.bDisablePingPoing ? g_stScanCoreVar.pADCCurrent : g_stScanCoreVar.pADCLast);
//            pETouchData->nLen = g_nAllAdcIndexCount;
//            pETouchData->eDataType = TOUCH_LINE_VALUE_REPORT_ID;
//            pETouchData->nTimestamp = GetCurrentTime();
//            g_nCurFrame = g_nFrame;
//            s_nTickSync = GetCurrentTime();
//        }
//        else if (g_stScanCoreVar.nScanCount == 0 && (GetCurrentTime()-s_nTickSync) > 5000)
//        {
//            g_stScanCoreVar.nScanCount = 1;
//            s_nTickSync = GetCurrentTime();
//        }
    }
    else if (g_TouchDataFifo[g_nTouchDataFifoFront].eBufStatus == eTouchBufReady)
    {
        pETouchData->eDataType = g_TouchDataFifo[g_nTouchDataFifoFront].eDataType;
        pETouchData->nLen = FN_MIN(MAX_LINE_COUNT, g_TouchDataFifo[g_nTouchDataFifoFront].nLen);
        pETouchData->pData = g_TouchDataFifo[g_nTouchDataFifoFront].pBuf;
        pETouchData->nTimestamp = g_TouchDataFifo[g_nTouchDataFifoFront].nTimeStamp;
        g_nCurFrame = g_TouchDataFifo[g_nTouchDataFifoFront].nDataFrameNo;
        g_TouchDataFifo[g_nTouchDataFifoFront].eBufStatus = eTouchBufNone;
        g_nTouchDataFifoFront = (g_nTouchDataFifoFront + 1) % (sizeof(g_TouchDataFifo)/sizeof(stTouchData));

        SendSyncData();  //发送同步信号
        s_nTickSync = GetCurrentTime();
    }
    else if (g_bRecvQTData)
    {
        if ((GetCurrentTime() - s_nTickSync) > 1000)
        {
            SendSyncData();  //发送同步信号
            s_nTickSync = GetCurrentTime();
        }
    }
    return pETouchData->nLen;
}

