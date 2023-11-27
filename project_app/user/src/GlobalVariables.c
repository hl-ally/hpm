#include <stdio.h>
#include "Api_Math.h"
//#include "usb_lib.h"
//#include "Api_Timer.h"
#include "GlobalVariables.h"
#include "TouchSignal.h"
//#include "SystemDefine.h"
#include "CommService.h"
#include "FlashPara.h"
//#include "AlgSharedMem.h"
#include "Api_Flash.h"
#include "Api_UsbDesc.h"
#include "string.h"
//#include "Scan.h"
//#include "ScanSolution.h"
//#include "HardwareConfigInit.h"

#ifndef REAL_ADJ_AGC_EN
#define REAL_ADJ_AGC_EN         (1)      //实时调节Agc
#endif

uint8_t                     g_bSyncMode = 0;
stPoint_t                   g_arrPoint[MAX_POINT];
uint8_t                     g_nTouchCount = 0;
uint8_t                     g_bSendCoord = 1;
uint8_t                     g_bBoradLedEnable = 1;
uint8_t                     g_bAutoAGC = 1;
uint8_t                     g_bAutoAgcEn = 1;
uint32_t                    g_nDisableAgcTick = 0;
uint32_t                    g_nHaveLight = 0, g_nDownLight = 0;
volatile int32_t            g_bCheckReflection = 1;

volatile int32_t            g_bTouchPenetration = 0;

uint8_t  gUSB_CprsBuff[CprsBUFFLENG]  __attribute__((aligned(4)));

stTouchData                 g_TouchDataFifo[2];
volatile uint8_t            g_nTouchDataFifoFront = 0;
volatile uint8_t            g_nTouchDataFifoBack = 0;

uint8_t                    *g_pMidAdcData = NULL;
uint8_t                    *g_arrScanFLag = NULL;
uint16_t*                   g_nTestAdcIndex[eAxisCount][OBQ_COUNT];
uint16_t*                   g_nTestOrgIndex[eAxisCount][OBQ_COUNT];
uint16_t*                   g_nTestAgcIndex[eAxisCount][OBQ_COUNT];

adc_type_t*                 g_ORG = NULL;
uint16_t                    g_nAGCCnt = 0;
uint16_t                    g_nAGCCntAxis[] = {0,0};

stSgmH_t                   *g_stScanSeg[eAxisCount];
stSgmH_t                   *g_stScanSegTmp[eAxisCount];
uint16_t                    g_nBlockStartTLedTimes[eAxisCount][((FN_MAX(X_TOTAL,Y_TOTAL)+ADC_CH_TOTAL-1)/ADC_CH_TOTAL)*2];

volatile int16_t            g_nTotal[eAxisCount] = {X_TOTAL, Y_TOTAL};
int32_t                     g_nInterVal[eAxisCount][OBQ_COUNT] = {{X_TOTAL, 0, -X_TOTAL}, {Y_TOTAL, 0, -Y_TOTAL}};
stPosF_t*                   g_arrPos[eEdgeCount];
float                       g_fMaxRate[eAxisCount] = {MAX_RATE_X, MAX_RATE_Y};

uint16_t                    g_nFrame = 0;
stSlvSpecBits_t             g_stSlvSpecBits = {0};

volatile int32_t            g_bOpenTestTool = 0;
stMiddleAdc_t               g_stAmbienceVar = {
                                                .eEnvmPattern = ePatternNormal,
                                                .arrLedState[X] = g_stAmbienceVar.arrLedStateX,
                                                .arrLedState[Y] = g_stAmbienceVar.arrLedStateY,
                                                .arrLastMidAdc[X] = g_stAmbienceVar.arrLastMidAdcX,
                                                .arrLastMidAdc[Y] = g_stAmbienceVar.arrLastMidAdcY,
                                                .nRegularStableTick = 0,
                                                .nBBordOpenTick = 0,
                                                .bDelayRegularAdj = 0,
                                                .nEnterRegularAdjTick = 0,
                                                };
USB_CprsQueue stUSB_CprsQueue =
{
    .pQueueData     = (uint8_t (*)[CprsQItemSize])gUSB_CprsBuff,
    .nQueueFront    = 0,
    .nQueueBack     = -1,
    .nQueueLen      = 0,
    .bQueueStatus   = CprsQFrameOvr,
    .nCprsADCInd    = 0
};

stBurnAging_t               g_stBurnAging = {.eMode = eNormalMode,
                                             .eCmdState = eAgingDisable,
                                             .eAgingMark = eAgingFailure,
                                             .eUsbEnumMask = eEnumFailure
                                            };

stTestStatus_t    g_stTestStatus =
{
                                .eStatus = eTestDefault,
                                .eCmdSource = eUsb0Ep2Mode,
                                .nStartTime =    0
};

#if defined GD415_ENV
#if (SCAN_SOLUTION == SCAN_SOLUTION_6CH_E5HG)
/*
 * 初始化GPIO 扫描时序表, 并保存在Flash中
 */
uint32_t DMACopyToGpioInitAndSave(uint32_t nFlashAddress)
{
    stDMACopyToGpio_t *pDMACopyToGpio  = (stDMACopyToGpio_t *)GetSharedMem(0);
    int32_t nTStartLed = 0;
    int32_t nTEndLed   = 0;
    int32_t nRLed      = 0;
    int32_t nTLed      = 0;
    int32_t nBlockNext = 0;

    memset((uint8_t *)pDMACopyToGpio, 0, (g_nBlockCount + 1) * sizeof(stDMACopyToGpio_t));
    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
    {
        for (nRLed = 0; nRLed < g_nTotal[eAxis]; nRLed += R_ADC_GROUP)
        {
            nTStartLed = g_stScanSeg[eAxis][nRLed / R_ADC_GROUP].nStart;
            nTEndLed = g_stScanSeg[eAxis][nRLed / R_ADC_GROUP].nEnd;

            for (nTLed = nTStartLed; nTLed <= nTEndLed && nBlockNext <= g_nBlockCount; nTLed++)
            {
                pDMACopyToGpio[nBlockNext].nColAddr = g_nColAddrMapTable[nTLed % T164_GROUP];

                /** One rising edge is required for every 6 lights, with the exception of no. 0 light */
                pDMACopyToGpio[nBlockNext].nT164Clk = (nTLed != 0 && (0 == (nTLed % T164_GROUP))) ? 1 : 0;
                pDMACopyToGpio[nBlockNext].nT164Data = 1;
                pDMACopyToGpio[nBlockNext].nR164Data = 1;
                pDMACopyToGpio[nBlockNext].nR164Clk = 1;
                nBlockNext++;
            }
        }
    }
    pDMACopyToGpio[nBlockNext] = pDMACopyToGpio[0];

    FLASH_WriteLenByte(nFlashAddress, (uint8_t *)pDMACopyToGpio, (g_nBlockCount + 1) * sizeof(stDMACopyToGpio_t));
    return ((g_nBlockCount + 1) * sizeof(stDMACopyToGpio_t));
}
#else
/*
 * 初始化GPIO 扫描时序表, 并保存在Flash中
 */
uint32_t DMACopyToGpioInitAndSave(uint32_t nFlashAddress)
{
//    stDMACopyToGpio_t *pDMACopyToGpio  = (stDMACopyToGpio_t *)GetSharedMem(0);
//    int32_t nTStartLed = 0;
//    int32_t nTEndLed   = 0;
//    int32_t nRLed      = 0;
//    int32_t nTLed      = 0;
//    int32_t nBlockNext = 0;
//
//    memset((uint8_t *)pDMACopyToGpio, 0, (g_nBlockCount + 1) * sizeof(stDMACopyToGpio_t));
//    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
//    {
//        for (nRLed = 0; nRLed < g_nTotal[eAxis]; nRLed += R_ADC_GROUP)
//        {
//            nTStartLed = g_stScanSeg[eAxis][nRLed / R_ADC_GROUP].nStart;
//            nTEndLed = g_stScanSeg[eAxis][nRLed / R_ADC_GROUP].nEnd;
//
//            for (nTLed = nTStartLed; nTLed <= nTEndLed && nBlockNext <= g_nBlockCount; nTLed++)
//            {
//                pDMACopyToGpio[nBlockNext].nColAddr = g_nColAddrMapTable[nTLed % T164_GROUP];
//
//                /** One rising edge is required for every 6 lights, with the exception of no. 0 light */
//                pDMACopyToGpio[nBlockNext].nT164Clk = (nTLed != 0 && (0 == (nTLed % T164_GROUP))) ? 1 : 0;
//                pDMACopyToGpio[nBlockNext].nT164Data = 0;
//                nBlockNext++;
//            }
//        }
//    }
//    pDMACopyToGpio[nBlockNext] = pDMACopyToGpio[0];
//
//    FLASH_WriteLenByte(nFlashAddress, (uint8_t *)pDMACopyToGpio, (g_nBlockCount + 1) * sizeof(stDMACopyToGpio_t));
//    return ((g_nBlockCount + 1) * sizeof(stDMACopyToGpio_t));

    return 0;
}
#endif
#else
uint32_t DMACopyToGpioInitAndSave(uint32_t nFlashAddress)
{
//    stDMACopyToGpio_t *pDMACopyToGpio  = (stDMACopyToGpio_t*)GetSharedMem(0);
//    int32_t  nMaxIndexCount = (GetCurSharedMem() / sizeof(stDMACopyToGpio_t));
//    int32_t nBlockNext = 0;
//    int32_t nRLed = 0, nTLed = 0;
//    int32_t nTStartLed = 0, nTEndLed = 0;
//    //总的内存大小个数
//    uint16_t nTotalBlockCount = g_nBlockCount + 1;
//    uint16_t *pDmaBuf = (uint16_t *)pDMACopyToGpio;
//
//    MemSet((uint8_t *)pDmaBuf, 0, sizeof(stDMACopyToGpio_t) * nTotalBlockCount);
//
//    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
//    {
//        for (nRLed = 0; nRLed < g_nTotal[eAxis]; nRLed += R_ADC_GROUP)
//        {
//            nTStartLed = g_stScanSeg[eAxis][nRLed / R_ADC_GROUP].nStart;
//            nTEndLed = g_stScanSeg[eAxis][nRLed / R_ADC_GROUP].nEnd;
//
//            for (nTLed = nTStartLed; nTLed <= nTEndLed && nBlockNext <= g_nBlockCount; nTLed++)
//            {
//                pDMACopyToGpio[nBlockNext].nAddr = g_nColAddrMapTable[nTLed % T164_GROUP];
//                pDMACopyToGpio[nBlockNext].nT164CLK = (nTLed != 0 && (0 == (nTLed % T164_GROUP))) ? 1 : 0;
//                pDMACopyToGpio[nBlockNext].nR164Data = (eAxis == Y && nRLed < R_ADC_GROUP) ? 0 : 1;
//                nBlockNext++;
//            }
//        }
//    }
//    pDMACopyToGpio[nBlockNext] = pDMACopyToGpio[0];
//
//    FLASH_WriteLenByte(nFlashAddress, (uint8_t *)pDMACopyToGpio, nTotalBlockCount * sizeof(stDMACopyToGpio_t));
//    return (nTotalBlockCount * sizeof(stDMACopyToGpio_t));

    return 0;
}
#endif

void GlobalValidInit(void)
{
    int32_t  nExitNum = -1;
    int32_t  nAdcMallocLen = 0;     //当前ADC buf 分配内存的大小
#if (defined N32G457_ENV)
    //用于初始化非连续内存数组
    uint16_t *pAdc1Block = NULL;
    uint16_t *pAdc2Block = NULL;
    uint32_t nAdc1Cnt = 0, nAdc2Cnt = 0, nAdcTotal = 0; 
#elif (defined ATF403A_ENV || defined GD415_ENV)
    stDMACopyToGpio_t  *pDMA2Gpio = NULL;
#endif

#if (defined ATF403A_ENV || defined GD415_ENV)
    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
    {
        g_stScanSegTmp[eAxis] = (stSgmH_t *)GetSharedMem(sizeof(stSgmH_t) * ((g_nTotal[eAxis] + ADC_CH_TOTAL - 1) / ADC_CH_TOTAL));
        if (NULL == g_stScanSegTmp[eAxis])
        {
            nExitNum = 0;
            goto __exit;
        }
        MemSet(g_stScanSegTmp[eAxis], 0, sizeof(stSgmH_t) * ((g_nTotal[eAxis] + ADC_CH_TOTAL - 1) / ADC_CH_TOTAL));
        MemCpy(g_stScanSegTmp[eAxis], g_stScanSeg[eAxis], sizeof(stSgmH_t)*((g_nTotal[eAxis] + ADC_CH_TOTAL - 1) / ADC_CH_TOTAL));
        g_stScanSeg[eAxis] = (stSgmH_t *)g_stScanSegTmp[eAxis];
    }
#endif

//    for (eAxis_t eAxis = X; eAxis< eAxisCount; eAxis++)
//    {
//        for (uint16_t nRLed = 0; nRLed < g_nTotal[eAxis]; nRLed += R_ADC_GROUP)
//        {
//            stSgmH_t *stSgmHTmp = (stSgmH_t*)&g_stScanSeg[eAxis][nRLed / R_ADC_GROUP];
//
//            g_stScanCoreVar.nScanBlockLedMax = FN_MAX(g_stScanCoreVar.nScanBlockLedMax, (stSgmHTmp->nEnd - stSgmHTmp->nStart + 1));
//        }
//    }

#if (defined N32G457_ENV)
    g_stScanCoreVar.pArrScanBlockADCBuf.pAdc1Buf = (uint32_t *)GetSharedMem(g_stScanCoreVar.nScanBlockLedMax * ADC_CH_TOTAL * 2);
    if (NULL == g_stScanCoreVar.pArrScanBlockADCBuf.pAdc1Buf)
    {
        nExitNum = 1;
        goto __exit;
    }
    MemSet(g_stScanCoreVar.pArrScanBlockADCBuf.pAdc1Buf, 0, g_stScanCoreVar.nScanBlockLedMax * ADC_CH_TOTAL * 2);
    g_stScanCoreVar.pArrScanBlockADCBuf.pAdc2Buf = (uint32_t *)&g_stScanCoreVar.pArrScanBlockADCBuf.pAdc1Buf[ADC1_CH_NUM * g_stScanCoreVar.nScanBlockLedMax];

    g_stScanCoreVar.pAdc1BlockCurrent = g_stScanCoreVar.pArrScanBlockADCBuf.pAdc1Buf;
    g_stScanCoreVar.pAdc2BlockCurrent = g_stScanCoreVar.pArrScanBlockADCBuf.pAdc2Buf;
#elif (defined ATF403A_ENV)
    g_stScanCoreVar.pArrScanBlockADCBuf.pAdc1Buf = (uint32_t *)GetSharedMem(g_stScanCoreVar.nScanBlockLedMax * ADC_CH_TOTAL *2);
    if (NULL == g_stScanCoreVar.pArrScanBlockADCBuf.pAdc1Buf)
    {
        nExitNum = 1;
        goto __exit;
    }
    MemSet(g_stScanCoreVar.pArrScanBlockADCBuf.pAdc1Buf, 0, g_stScanCoreVar.nScanBlockLedMax * ADC_CH_TOTAL *2);

    g_stScanCoreVar.pAdc1BlockCurrent = g_stScanCoreVar.pArrScanBlockADCBuf.pAdc1Buf;
#elif (defined GD405_ENV || defined GD415_ENV)
    g_stScanCoreVar.pScanBlockADCBuf.pAdcBuf = (uint16_t *)GetSharedMem(g_stScanCoreVar.nScanBlockLedMax * ADC_CH_TOTAL *2);
    if (NULL == g_stScanCoreVar.pScanBlockADCBuf.pAdcBuf)
    {
        printf("Error, insufficient memory line:%d\r\n", __LINE__);
        return;
    }
    MemSet(g_stScanCoreVar.pScanBlockADCBuf.pAdcBuf, 0, g_stScanCoreVar.nScanBlockLedMax * ADC_CH_TOTAL *2);

    g_stScanCoreVar.pADCBlockCurrent = g_stScanCoreVar.pScanBlockADCBuf.pAdcBuf;
#endif

#if (defined GD405_ENV || defined GD415_ENV)
    nAdcMallocLen = g_nAllAdcIndexCount * sizeof(adc_type_t);
    for (int32_t i = 0; i < sizeof(g_stScanCoreVar.pArrAdcBuf)/sizeof(g_stScanCoreVar.pArrAdcBuf[0]); i++)
    {
        /*
         * GD 定义变量的时候不能使用柔性数组的形式分配, MCU会自动分配到 TCM 区,
         * TCM本身是栈属性, 导致异常, 所有与DMA相关的内存都不能使用TCM区分配, TCM区只能有内核访问, DMA不能访问TCM区
         */
        g_stScanCoreVar.pArrAdcBuf[i].pAdcBuf = (adc_type_t *)GetSharedMem(nAdcMallocLen);
        if (NULL == g_stScanCoreVar.pArrAdcBuf[i].pAdcBuf)
        {
            nExitNum = 2;
            goto __exit;
        }
        MemSet(g_stScanCoreVar.pArrAdcBuf[i].pAdcBuf, 0, nAdcMallocLen);
        g_TouchDataFifo[FN_MIN(1, i)].pBuf = (uint8_t *)g_stScanCoreVar.pArrAdcBuf[i].pAdcBuf;
    }
    g_stScanCoreVar.pADCLast = (adc_type_t *)g_stScanCoreVar.pArrAdcBuf[0].pAdcBuf;
    g_stScanCoreVar.pADCCurrent = (adc_type_t *)g_stScanCoreVar.pArrAdcBuf[1].pAdcBuf;
#else
    nAdcMallocLen = g_nAllAdcIndexCount * sizeof(adc_type_t);

//    for (int32_t i = 0; i < SIZEARR(g_stScanCoreVar.pArrAdcBuf); i++)
//    {
//        /*
//         * GD 定义变量的时候不能使用柔性数组的形式分配, MCU会自动分配到 TCM 区,
//         * TCM本身是栈属性, 导致异常, 所有与DMA相关的内存都不能使用TCM区分配, TCM区只能有内核访问, DMA不能访问TCM区
//         */
//        g_stScanCoreVar.pArrAdcBuf[i].pAdcBuf = (adc_type_t *)GetSharedMem(nAdcMallocLen);
//        if (NULL == g_stScanCoreVar.pArrAdcBuf[i].pAdcBuf)
//        {
//            nExitNum = 2;
//            goto __exit;
//        }
//        MemSet(g_stScanCoreVar.pArrAdcBuf[i].pAdcBuf, 0, nAdcMallocLen);
//        g_TouchDataFifo[FN_MIN(1, i)].pBuf = (uint8_t *)g_stScanCoreVar.pArrAdcBuf[i].pAdcBuf;
//    }    
//    g_stScanCoreVar.pADCLast    = (adc_type_t *)g_stScanCoreVar.pArrAdcBuf[0].pAdcBuf;
//    g_stScanCoreVar.pADCCurrent = (adc_type_t *)g_stScanCoreVar.pArrAdcBuf[1].pAdcBuf;
#endif

//    g_ORG = (adc_type_t *)GetSharedMem(nAdcMallocLen);
//    if (NULL == g_ORG)
//    {
//        nExitNum = 3;
//        goto __exit;
//    }
//    MemSet(g_ORG, 0, nAdcMallocLen);
//      
//    g_arrScanFLag = (uint8_t *)GetDTCMSharedMem(g_nBlockCount);
//    if (NULL == g_arrScanFLag)
//    {
//        g_arrScanFLag = (uint8_t *)GetSharedMem(g_nBlockCount);
//    }
//    if (NULL == g_arrScanFLag)
//    {
//        nExitNum = 4;
//        goto __exit;
//    }
//    MemSet(g_arrScanFLag, 0, g_nBlockCount);
//
//    g_pMidAdcData = (uint8_t *)GetDTCMSharedMem(nAdcMallocLen / INTV_CNT + 1);
//    if (NULL == g_pMidAdcData)
//    {
//        g_pMidAdcData = (uint8_t *)GetSharedMem(nAdcMallocLen / INTV_CNT + 1);
//    }
//    if (NULL == g_pMidAdcData)
//    {
//        nExitNum = 5;
//        goto __exit;
//    }
//    MemSet(g_pMidAdcData, 0, nAdcMallocLen / INTV_CNT + 1);

#if (SCAN_SOLUTION == SCAN_SOLUTION_6CH_E5HG)
    g_stScanCoreVar.pRAgc = (uint8_t (*)[eRAgcCount])GetSharedMem(g_nBlockCount * eRAgcCount);
    if (NULL == g_stScanCoreVar.pRAgc)
    {
        nExitNum = 6;
        goto __exit;
    }
    MemSet((void *)g_stScanCoreVar.pRAgc, 0, g_nBlockCount * eRAgcCount);

    //包含发射和接收 DAC 数据, 使用双DAC buf输出
    g_stScanCoreVar.pSetDacToAgc = (agc_type_t (*)[eRAgcCount])GetSharedMem((g_nBlockCount + 2) * sizeof(agc_type_t) * eRAgcCount);
    if (NULL == g_stScanCoreVar.pSetDacToAgc)
    {
        nExitNum = 7;
        goto __exit;
    }
    MemSet((void *)g_stScanCoreVar.pSetDacToAgc, 0, (g_nBlockCount + 2) * sizeof(agc_type_t) * eRAgcCount);
#else
//    g_stScanCoreVar.pTAgc = (uint8_t *)GetSharedMem(g_nBlockCount);
//    if (NULL == g_stScanCoreVar.pTAgc)
//    {
//        nExitNum = 6;
//        goto __exit;
//    }
//    MemSet(g_stScanCoreVar.pTAgc, 0, g_nBlockCount);
//
//    g_stScanCoreVar.pRAgc = (uint8_t *)GetSharedMem(g_nBlockCount);
//    if (NULL == g_stScanCoreVar.pRAgc)
//    {
//        nExitNum = 7;
//        goto __exit;
//    }
//    MemSet(g_stScanCoreVar.pRAgc, 0, g_nBlockCount);
//
//    //包含发射和接收 DAC 数据, 使用双DAC buf输出
//    g_stScanCoreVar.pSetDacToAgc = (agc_type_t *)GetSharedMem((g_nBlockCount + 2) * sizeof(agc_type_t));
//    if (NULL == g_stScanCoreVar.pSetDacToAgc)
//    {
//        nExitNum = 8;
//        goto __exit;
//    }
//    MemSet((uint8_t*)g_stScanCoreVar.pSetDacToAgc, 0, (g_nBlockCount + 2) * sizeof(agc_type_t));
#endif
#if (defined N32G457_ENV)
    //为指针数组申请内存, 只有是多个 ADC 控制器且内存不连续的平台使用
    //二级指针保存的是 ADC 数据的内存地址
    g_pScanNonContinueRAMTab = (ScanLNonContinueRAM_type_t **)GetDTCMSharedMem(sizeof(ScanLNonContinueRAM_type_t *) * ADC_CH_TOTAL * g_stScanCoreVar.nScanBlockLedMax);

    if (NULL == g_pScanNonContinueRAMTab)
    {
        g_pScanNonContinueRAMTab = (ScanLNonContinueRAM_type_t **)GetSharedMem(sizeof(ScanLNonContinueRAM_type_t *) * ADC_CH_TOTAL * g_stScanCoreVar.nScanBlockLedMax);
    }
    if (NULL == g_pScanNonContinueRAMTab)
    {
        nExitNum = 9;
        goto __exit;
    }
    //初始化指针数组, 保存一块 ADC 中每一个内存的地址, 扫描过程中加速连续访问
    pAdc1Block  = (uint16_t *)g_stScanCoreVar.pArrScanBlockADCBuf.pAdc1Buf;
    pAdc2Block  = (uint16_t *)g_stScanCoreVar.pArrScanBlockADCBuf.pAdc2Buf;

    for (int32_t n = 0; n < g_stScanCoreVar.nScanBlockLedMax * ADC1_CH_NUM; n++)
    {
        //按光线索引顺序排序赋值(ADC数据的地址)
        g_pScanNonContinueRAMTab[nAdcTotal++] = &pAdc1Block[nAdc1Cnt++];
        g_pScanNonContinueRAMTab[nAdcTotal++] = &pAdc1Block[nAdc1Cnt++];
        g_pScanNonContinueRAMTab[nAdcTotal++] = &pAdc2Block[nAdc2Cnt++];
        g_pScanNonContinueRAMTab[nAdcTotal++] = &pAdc2Block[nAdc2Cnt++];
    }
#endif

#if (defined ATF403A_ENV || defined GD415_ENV)
    if (NULL != g_pstScanLineIdxFlash)
    {
        g_pstScanLineIdx = (stScanLineIdx_t *)GetDTCMSharedMem(sizeof(stScanLineIdx_t) + (sizeof(stScanLine_t) * g_pstScanLineIdxFlash->nTotalLineStatu));

        if (NULL == g_pstScanLineIdx)
        {
            g_pstScanLineIdx = (stScanLineIdx_t *)GetSharedMem(sizeof(stScanLineIdx_t) + (sizeof(stScanLine_t) * g_pstScanLineIdxFlash->nTotalLineStatu));
        }
        if (NULL == g_pstScanLineIdx)
        {
            nExitNum = 10;
            goto __exit;
        }
        MemCpy(g_pstScanLineIdx, g_pstScanLineIdxFlash, sizeof(stScanLineIdx_t) + (sizeof(stScanLine_t) * g_pstScanLineIdxFlash->nTotalLineStatu));
    }

    pDMA2Gpio = (stDMACopyToGpio_t *)GetSharedMem((g_nBlockCount + 1) * sizeof(stDMACopyToGpio_t));
    if (NULL == pDMA2Gpio)
    {
        nExitNum = 11;
        goto __exit;
    }
    MemSet(pDMA2Gpio, 0, (g_nBlockCount + 1) * sizeof(stDMACopyToGpio_t));

    MemCpy(pDMA2Gpio, g_stScanCoreVar.pDMACopyToGpio, (g_nBlockCount + 1) * sizeof(stDMACopyToGpio_t));
    g_stScanCoreVar.pDMACopyToGpio = pDMA2Gpio;
#endif

//    g_stScanCoreVar.nAdcTotal = g_nAllAdcIndexCount;
    g_nAGCCnt = g_nBlockCount;
    g_nAGCCntAxis[X] = g_nBlockCountAxis[X];
    g_nAGCCntAxis[Y] = g_nBlockCountAxis[Y];

__exit:
    if (-1 != nExitNum)
    {
        printf("Error, insufficient memory, Line:%d\r\n", nExitNum);
    }
}

//获取触摸框的尺寸
int32_t GetDeviceSize(char *pModelStr, int32_t nLen)
{
    int32_t nSize = 0;

    for (int32_t i = 0; i < nLen; i++)
    {
        if ((pModelStr[i] >= '0') && (pModelStr[i] <= '9'))
        {
            nSize = nSize*10 + pModelStr[i] - '0';//这里是用字符的ASCLL码值进行比较，要减去0的ASCLL码值即为实际的数字
        }
        else if (nSize != 0)
        {
            break;
        }
    }
    return nSize;
}

//根据客户规则生成指定的字符串 可重定义
ATTR_WEAK int32_t GetProductString(uint8_t *pStr, eUsbDevice_t eUsbDev)
{
    int32_t nLen = g_pConfigData->nModelDescLen;
    char *pModelStr = (char *)g_pConfigData->strModelDesc;
    int32_t nTouchSize = GetDeviceSize(pModelStr, nLen);
    char *pMcuVendor = "N32G455";
    char *pUsbDevType = eUsbDev == eUsbDev0 ? "FS" : "HS";
//    char arrCompileTime[sizeof(USB_DESC5_COMPILE_TIME)] = {0};
//
//    MemSet(arrCompileTime, 0, sizeof(arrCompileTime));
//    MemCpy(arrCompileTime, USB_DESC5_COMPILE_TIME, 8);
//#ifdef GD405_ENV
//    pMcuVendor = "GD405";
//#elif (defined ATF403A_ENV)
//    pMcuVendor = "ATF403A";
//#elif (defined GD415_ENV)
//    pMcuVendor = "GD415";
//#endif
//    sprintf((char *)pStr, "%s_%d_0x%X_%s_S%s_%s_U%s", pModelStr,
//                                                g_pConfigData->nUsePoint,
//                                                GetFwCheckSum(), pMcuVendor,
//                                                USB_DESC4_SVN_ID, arrCompileTime, pUsbDevType);
    return nLen;
}

////初始化USB字符串
//int32_t InitUsbStrings(eUsbDevice_t eUsbDev)
//{
//    uint8_t arrStrTmp[512], arrProductStrTmp[512];
//    uint8_t *pVendorStr = g_pConfigData->strManufacturerDesc;
//    uint8_t *pProductStr = arrProductStrTmp;
//    uint8_t *pSeriesNumStr = NULL;
//    uint8_t arrStrSeriesNum[128];
//
//    if (pVendorStr != NULL)
//    {
//        InitUsbStringUnicode(pVendorStr, arrStrTmp, eUsbStrVendor, eUsbDev);
//    }
//    if (pProductStr != NULL)
//    {
//        GetProductString(pProductStr, eUsbDev);
//        InitUsbStringUnicode(pProductStr, arrStrTmp, eUsbStrProduct, eUsbDev);
//    }
//    if (0 && pSeriesNumStr == NULL) //20210827 关闭序列号字符串初始化 使用默认
//    {
//        uint32_t arrSerialNum[3];
//        GetSerialNum(arrSerialNum);
//        sprintf((char *)arrStrSeriesNum, "%04x%04x%04x", arrSerialNum[0], arrSerialNum[1], arrSerialNum[2]);
//        pSeriesNumStr = arrStrSeriesNum;
//    }
//    if (pSeriesNumStr != NULL)
//    {
//        InitUsbStringUnicode(pSeriesNumStr, arrStrTmp, eUsbStrSerialNumber, eUsbDev);
//    }
//    return 0;
//}
//启动测试工具后处理的事件
void OpenTestToolEventPro(void)
{
//#if REAL_ADJ_AGC_EN
//    int32_t bScanOnce = g_stScanCoreVar.bDisablePingPoing ? 1 : 0;
//    if (1 == g_bAutoAgcEn)
//    {
//        g_bAutoAGC = 0; //关闭实时调整AGC 等待超时自动开启
//        g_nDisableAgcTick = GetSystickTime();
//        if (1 == bScanOnce)
//        {
//            ScanOnce();
//            bScanOnce = 0;
//        }
//    }
//#endif

    g_bOpenTestTool = 1;
}

