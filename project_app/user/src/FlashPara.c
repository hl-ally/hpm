//#include "HardWareInfo.h"
#include "FlashPara.h"
#include "GlobalVariables.h"
#include "Api_Flash.h"
//#include "TouchSignal.h"
#include "Api_Math.h"
//#include "scan.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "Api_Flash.h"
#include "Api_Crc.h"
//#include "AlgSharedMem.h"
//#include "ZwFlash.h"
#include "Command.h"
//#include "usb_lib.h"
//#include "ScanSolution.h"

stHardConfigData_t *g_pConfigData = NULL;
/*************************************************************************************
 * @Function : 获取配置数据的Flash地址
 * @Parameter: 无
 * @return   : 返回Flash参数的起始地址
 *************************************************************************************/
uint32_t GetFlashParaStartAddress(void)
{
//    return GetScanParaSaveAddr();
    return FLASH_PARAM_ADDRESS;
}

uint32_t GetScanParaSaveAddr(void)
{
    return FLASH_PARAM_ADDRESS;
}
/*
 * 获取扫描参数保存的地址
 */
uint32_t GetScanParaSaveLen(void)
{
    return FLASH_PARAM_LENGTH;
}


/*************************************************************************************
 * @Function : 清除Flash的相关配置数据
 * @Parameter: 无
 * @return   : 返回擦除结果
 *************************************************************************************/
int32_t ClearFlashPara(void)
{
    return (int32_t)FlashErase(GetScanParaSaveAddr(), GetScanParaSaveLen());
}

/*************************************************************************************
 * @Function : 擦除Boot,一般不使用
 * @Parameter: 无
 * @return   : 返回擦除结果
 *************************************************************************************/
int32_t ClearBootloader(void)
{
    return (int32_t)FlashErase(BOOTLOADER_ADDRESS, BOOTLOADER_MAX_SIZE);
}

/*************************************************************************************
 * @Function : 擦除固件
 * @Parameter: 无
 * @return   : 返回擦除结果
 *************************************************************************************/
int32_t ClearApp(void)
{
    return (int32_t)FlashErase(APPLICATION_ADDRESS, APPLICATION_MAX_LEN);
}

/*************************************************************************************
 * @Function : 保存配置的相关数据到指定的地址
 * @Parameter: [pBuf] 待写入的数据地址
             : [nLen] 待写入数据的长度
             : [nOffset] Flash写入的地址偏移
 * @return   : 返回写入结果
 *************************************************************************************/
int32_t SaveFlashPara(uint8_t* pBuf, int32_t nLen, uint32_t nOffsetAddress)
{
    FlashWrite(GetScanParaSaveAddr() + nOffsetAddress, (uint8_t*)pBuf, nLen);
    return 0;
}

/*************************************************************************************
 * @Function : 获取配置数据Flash可用的最大长度
 * @Parameter: 无
 * @return   : 最大的Flash 参数的存储长度
 *************************************************************************************/
uint32_t GetFlashParaMaxSize(void)
{
    return GetScanParaSaveLen();
}

/*************************************************************************************
 * @Function : 初始化相关的硬件参数
 * @Parameter: [pConfig] 参数的相关缓存指针
 * @return   : 返回结果
 *************************************************************************************/
int32_t InitCommonConfigData(stHardConfigData_t* pConfig)
{
    pConfig->nLineWidth = LIGHT_WIDTH;

    pConfig->nConfigVersion = CONFIG_VERSION;
#if 0
    pConfig->nUsb0VID = USB0_VID;
    pConfig->nUsb0PID = USB0_PID;
#endif
    pConfig->nUsb1VID = USB1_VID;
    pConfig->nUsb1PID = USB1_PID;

    pConfig->nXTLedNumForBoard[0] = XT_BOARD_1;
    pConfig->nXTLedNumForBoard[1] = XT_BOARD_2;
    pConfig->nXTLedNumForBoard[2] = XT_BOARD_3;
    pConfig->nXTLedNumForBoard[3] = XT_BOARD_4;
    pConfig->nXTLedNumForBoard[4] = XT_BOARD_5;
    pConfig->nXTLedNumForBoard[5] = XT_BOARD_6;
    pConfig->nXTLedNumForBoard[6] = XT_BOARD_7;
    pConfig->nXTLedNumForBoard[7] = XT_BOARD_8;

    pConfig->nXRLedNumForBoard[0] = XR_BOARD_1;
    pConfig->nXRLedNumForBoard[1] = XR_BOARD_2;
    pConfig->nXRLedNumForBoard[2] = XR_BOARD_3;
    pConfig->nXRLedNumForBoard[3] = XR_BOARD_4;
    pConfig->nXRLedNumForBoard[4] = XR_BOARD_5;
    pConfig->nXRLedNumForBoard[5] = XR_BOARD_6;
    pConfig->nXRLedNumForBoard[6] = XR_BOARD_7;
    pConfig->nXRLedNumForBoard[7] = XR_BOARD_8;

    pConfig->nYTLedNumForBoard[0] = YT_BOARD_1;
    pConfig->nYTLedNumForBoard[1] = YT_BOARD_2;
    pConfig->nYTLedNumForBoard[2] = YT_BOARD_3;
    pConfig->nYTLedNumForBoard[3] = YT_BOARD_4;
    pConfig->nYTLedNumForBoard[4] = YT_BOARD_5;
    pConfig->nYTLedNumForBoard[5] = YT_BOARD_6;
    pConfig->nYTLedNumForBoard[6] = YT_BOARD_7;
    pConfig->nYTLedNumForBoard[7] = YT_BOARD_8;

    pConfig->nYRLedNumForBoard[0] = YR_BOARD_1;
    pConfig->nYRLedNumForBoard[1] = YR_BOARD_2;
    pConfig->nYRLedNumForBoard[2] = YR_BOARD_3;
    pConfig->nYRLedNumForBoard[3] = YR_BOARD_4;
    pConfig->nYRLedNumForBoard[4] = YR_BOARD_5;
    pConfig->nYRLedNumForBoard[5] = YR_BOARD_6;
    pConfig->nYRLedNumForBoard[6] = YR_BOARD_7;
    pConfig->nYRLedNumForBoard[7] = YR_BOARD_8;

    pConfig->nOAWidth  = OAWIDTH;
    pConfig->nOAHeight = OAHEIGHT;

    pConfig->nPhysicalX = PHYSICAL_SIZE_X * PHYSICAL_SIZE_SCALE;
    pConfig->nPhysicalY = PHYSICAL_SIZE_Y * PHYSICAL_SIZE_SCALE;

//    pConfig->nTotal[X] = g_nTotal[X];
//    pConfig->nTotal[Y] = g_nTotal[Y];
//
//    pConfig->nLedStart[0][0] = 0;
//    pConfig->nLedStart[0][1] = 0;
//    pConfig->nLedStart[0][2] = 0;
//
//    pConfig->nLedStart[1][0] = 0;
//    pConfig->nLedStart[1][1] = 0;
//    pConfig->nLedStart[1][2] = 0;
//
//    pConfig->nLedEnd[0][0] = X_TOTAL - 1;
//    pConfig->nLedEnd[0][1] = X_TOTAL - 1;
//    pConfig->nLedEnd[0][2] = X_TOTAL - 1;
//
//    pConfig->nLedEnd[1][0] = Y_TOTAL - 1;
//    pConfig->nLedEnd[1][1] = Y_TOTAL - 1;
//    pConfig->nLedEnd[1][2] = Y_TOTAL - 1;
//
//    pConfig->nLedQTStart[0][0] = g_nInterVal[X][0];
//    pConfig->nLedQTStart[0][1] = 0;
//    pConfig->nLedQTStart[0][2] = 0;
//
//    pConfig->nLedQTStart[1][0] = g_nInterVal[Y][0];
//    pConfig->nLedQTStart[1][1] = 0;
//    pConfig->nLedQTStart[1][2] = 0;
//
//    pConfig->nLedQTEnd[0][0] = X_TOTAL;
//    pConfig->nLedQTEnd[0][1] = X_TOTAL;
//    pConfig->nLedQTEnd[0][2] = X_TOTAL + g_nInterVal[X][OBQ_COUNT - 1];
//
//    pConfig->nLedQTEnd[1][0] = Y_TOTAL;
//    pConfig->nLedQTEnd[1][1] = Y_TOTAL;
//    pConfig->nLedQTEnd[1][2] = Y_TOTAL + g_nInterVal[Y][OBQ_COUNT - 1];
//
//    //测试标准
//    pConfig->nTestMaxAdc[0] = TEST_OBQ_MAX_ADC;
//    pConfig->nTestMaxAdc[1] = TEST_MAX_ADC;
//    pConfig->nTestMaxAdc[2] = TEST_OBQ_MAX_ADC;
//
//    pConfig->nTestMinAdc[0] = TEST_OBQ_MIN_ADC;
//    pConfig->nTestMinAdc[1] = TEST_MIN_ADC;
//    pConfig->nTestMinAdc[2] = TEST_OBQ_MIN_ADC;
//
//    pConfig->nTestMaxAdcFactory[0] = TEST_OBQ_MAX_ADC_FAC;
//    pConfig->nTestMaxAdcFactory[1] = TEST_MAX_ADC_FAC;
//    pConfig->nTestMaxAdcFactory[2] = TEST_OBQ_MAX_ADC_FAC;
//
//    pConfig->nTestMinAdcFactory[0] = TEST_OBQ_MIN_ADC_FAC;
//    pConfig->nTestMinAdcFactory[1] = TEST_MIN_ADC_FAC;
//    pConfig->nTestMinAdcFactory[2] = TEST_OBQ_MIN_ADC_FAC;
//
//    pConfig->nTestMaxUndulateAdc = TEST_MAX_UNT_ADC_CLT;        //Client read unt will be 20
//
//    pConfig->nTestMaxReverseAdc = TEST_MAX_REVERSE_ADC;
//    pConfig->nTestMinReverseAdc = 0;
//
//    pConfig->nTestMaxAgc = TEST_MAX_AGC;
//    pConfig->nTestMinAgc = TEST_MIN_AGC;
//
//    pConfig->nTestMaxAgcFactory = TEST_MAX_AGC_FAC;
//    pConfig->nTestMinAgcFactory = TEST_MIN_AGC_FAC;
//
    MemSet(pConfig->strManufacturerDesc,0x00,MAX_STRING_LENGTH);
    MemSet(pConfig->strDeviceDesc,0x00,MAX_STRING_LENGTH);
    MemSet(pConfig->strModelDesc,0x00,MAX_STRING_LENGTH);
    MemCpy(pConfig->strManufacturerDesc, MANUFACTURER_DESC, FN_MIN(MAX_STRING_LENGTH, sizeof(MANUFACTURER_DESC) - 1));

    MemCpy(pConfig->strDeviceDesc, DEVICE_DESC, FN_MIN(MAX_STRING_LENGTH, sizeof(DEVICE_DESC) - 1));
    MemCpy(pConfig->strModelDesc, MODEL_DESC, FN_MIN(MAX_STRING_LENGTH, sizeof(MODEL_DESC) - 1));

    pConfig->nManufacturerDescLen = FN_MIN(MAX_STRING_LENGTH, sizeof(MANUFACTURER_DESC) - 1);
    pConfig->nDeviceDescLen = FN_MIN(MAX_STRING_LENGTH, sizeof(DEVICE_DESC) - 1);
    pConfig->nModelDescLen = FN_MIN(MAX_STRING_LENGTH, sizeof(MODEL_DESC) - 1);
//
//    pConfig->nREOX1 = REOX1;
//    pConfig->nREOX2 = REOX2;
//    pConfig->nREOY1 = REOY1;
//    pConfig->nREOY2 = REOY2;
//    pConfig->eRotationParameter = ROTATION_DIRECTION;
//
//    pConfig->eRotationDef= ROTATION_DEF;
//    pConfig->eRotationOsDef= ROTATION_OS_DEF;
//    pConfig->bReverse = REVERSE;
//
//    pConfig->bUsb1DeviceConfigType = (uint8_t)USB1_DEVICE_CONFIG_TYPE;
//
    pConfig->nUsePoint = USER_POINT;
//    pConfig->bTouchEn = TOUCH_EN;
//    pConfig->bUnderMac10 = UNDER_MAC10_EN;
//    pConfig->eFormat = USB0_DEFAULT_TOUCH_FMT;
//    pConfig->eFormatRe = USB0_DEFAULT_TOUCH_FMT;
//    pConfig->bTuioCoordEn = TUIO_COORD_EN;
//    pConfig->bUsbCoordEn = USB0_COORD_EN;
//    pConfig->bUartCoordEn = UART_COORD_EN;
//    pConfig->bTouchCoordWidthEn = TOUCH_COORD_WIDTH_EN;
//    pConfig->bTuioCoordWidthEn = TUIO_COORD_WIDTH_EN;
//    pConfig->bUartCoordWidthEn = UART_COORD_WIDTH_EN;
//    pConfig->eTouchCoordWidthType = TOUCH_COORD_WIDTH_TYPE;
//    pConfig->eTuioCoordWidthType = TUIO_COORD_WIDTH_TYPE;
//    pConfig->eUartCoordWidthType = UART_COORD_WIDTH_TYPE;
//    pConfig->nUartSendCoordNum = UART_SEND_COORD_NUM;
//    pConfig->bUartSamePacketNoSend = UART_SAME_PACKET_NO_SEND_EN;

    return 0;
}

/*************************************************************************************
 * @Function :初始化灯管的相关位置,而且对数据进行保存
 * @Parameter:[nAddress]: 保存灯管位置的地址
 * @return   :返回保存占用Flash的长度
 *************************************************************************************/
uint32_t InitLedPositionAndSave(uint32_t nFlashAddress)
{
//    eEdge_t  eEdge = eBottom;
//    stPosF_t* stLedPosArray = (stPosF_t*)GetSharedMem(0);
//    int32_t  nMaxLedPosArrayCount = 0;
//    uint32_t nTotal = 0;
//    uint32_t nOffset = 0;        //偏移一个位置，第一个位置保存总的灯数
//
//    nMaxLedPosArrayCount = GetCurSharedMem() / sizeof(stPosF_t) - 1;
//
//    /******Note by Roc
//    *Write LED-Position of 4 side into stLedPosArray
//    ******/
//    for (eEdge = eBottom; eEdge < eEdgeCount; eEdge++)
//    {
//        InitLedPosition(stLedPosArray+nOffset, nMaxLedPosArrayCount, eEdge);
//        nOffset +=  g_nTotal[eEdge / 2];
//        nTotal  +=  g_nTotal[eEdge / 2];
//    }
//
//    //写入Flash中
//    FLASH_WriteLenByte(nFlashAddress + 0, (uint8_t*)stLedPosArray, nTotal * sizeof(stPosF_t));
//    g_nLedPositioCount = nTotal;
//    nOffset = 0;
//
//    for (eEdge = eBottom; eEdge < eEdgeCount; eEdge++)
//    {
//        //g_arrPos->Initialized
//        g_arrPos[eEdge] = (stPosF_t*)(nFlashAddress + nOffset);
//        nOffset += g_nTotal[eEdge / 2] * sizeof(stPosF_t) ;
//    }
//
//#if 0
//    for (eEdge = eBottom; eEdge < eEdgeCount; eEdge++)
//    {
//        for (uint8_t i=0; i < g_nTotal[eEdge / 2]; i++)
//        {
//            printf("eEdge%d-%d:%f,%f \r\n", eEdge, g_nTotal[eEdge / 2], g_arrPos[eEdge][i].fX, g_arrPos[eEdge][i].fY);
//        }
//    }
//#endif
//
//    return (nTotal*sizeof(stPosF_t));

    return 0;
}

/*************************************************************************************
 * @Function :初始化光线位置索引
 * @Parameter:[nAddress]: 保存光线的位置索引地址
 * @return   :返回占用Flash的大小
 *************************************************************************************/
uint32_t InitTLedRLedRelationAndSave(uint32_t nFlashAddress)
{
//    eEdge_t   eEdge = eBottom;
//    int32_t   nCount = 0;
//    uint32_t  nTotal = 0;
//    uint32_t  nOffset = 0;
//    uint16_t  *nTRLedRelation = (uint16_t*)GetSharedMem(0);
//    int32_t   nMaxIndexCount = GetCurSharedMem() / sizeof(uint16_t);
//
//    uint32_t  j = 0;
//
//    for (eEdge = eBottom; eEdge < eEdgeCount; eEdge++)
//    {
//        for (j = 0; j < g_nTotal[eEdge / 2]; j += ADC_CH_TOTAL)
//        {
//            //Devide into Nums of
//            nCount   = InitTLedRLedRelation(nTRLedRelation+nOffset, nMaxIndexCount, eEdge, j);
//            if (nCount > 0)
//            {
//                nOffset += nCount;
//                nTotal  += nCount;
//            }
//        }
//    }
//
//    /*
//    *Modified by Roc
//    */
//    //printf("TledRledRelation Data:%d\t\n",nTotal);
//
//    FLASH_WriteLenByte(nFlashAddress + 0, (uint8_t*)nTRLedRelation, nTotal * sizeof(uint16_t));
//
//#if 0
//    {
//        uint32_t add=0;
//        uint32_t j = 0;
//        uint32_t ii=0;
//        for(ii=0; ii<nTotal;)
//        {
//            printf("Pos:%d TLED:%d,number:%d: RLED:",ii,nTRLedRelation[ii+1],nTRLedRelation[ii]);
//            for(j=0; j<nTRLedRelation[ii]; j++)
//            {
//                printf(" %d",nTRLedRelation[ii+2+j]);
//                //printf("-%d@0x%x",*((uint16_t *)nFlashAddress+ii+2+j),(uint16_t *)nFlashAddress+ii+2+j);
//            }
//            printf("\r\n");
//            add = 2 + nTRLedRelation[ii] +(nTRLedRelation[ii])%2;
//            ii  += add;
//        }
//    }
//#endif
//
//    return (nTotal * sizeof(uint16_t));

    return 0;
}

/*************************************************************************************
 * @Function :初始化扫描段的发射范围，起始发射灯，与结束发射灯索引
 * @Parameter:[nAddress]: 保存段范围的起始地址
 * @return   :返回占用Flash的大小
 *************************************************************************************/
uint32_t InitScanBlockRangeAndSave(uint32_t nFlashAddress)
{
//    eAxis_t eAxis = X;
//    stSgmH_t* nSanSegmentArray = (stSgmH_t*)GetSharedMem(0);
//    int32_t nMaxSgmCount = GetCurSharedMem() / sizeof(stSgmH_t);
//    int32_t  nCount = 0;
//    uint32_t nTotal = 0;
//    uint32_t nOffset = 0;
//
//    for (eAxis = X; eAxis < eAxisCount; eAxis++)
//    {
//        g_stScanSeg[eAxis] = (stSgmH_t*)(nFlashAddress + nTotal * sizeof(stSgmH_t));
//        nCount = InitScanBlockRange(nSanSegmentArray + nOffset, nMaxSgmCount, eAxis);
//
//        nCount = (nCount + 3) / 4 * 4;
//        nOffset += nCount;
//        nTotal  += nCount;
//    }
//
//    FLASH_WriteLenByte(nFlashAddress + 0, (uint8_t*)nSanSegmentArray, nTotal * sizeof(stSgmH_t));
//#if 0
//    uint16_t  nRLed = 0 ;
//    for (nRLed = 0; nRLed < g_nTotal[X]; nRLed += ADC_CH_TOTAL)
//    {
//        stSgmH_t *tmp1 = (stSgmH_t*)&g_stScanSeg[X][nRLed/ADC_CH_TOTAL];
//        printf("X-%d:[%d,%d]\tRange:%d\r\n",nRLed/ADC_CH_TOTAL, tmp1->nStart, tmp1->nEnd, tmp1->nEnd - tmp1->nStart + 1);
//    }
//    for (nRLed = 0; nRLed < g_nTotal[Y]; nRLed += ADC_CH_TOTAL)
//    {
//        stSgmH_t *tmp1 = (stSgmH_t*)&g_stScanSeg[Y][nRLed/ADC_CH_TOTAL];
//        printf("Y-%d:[%d,%d]\tRange:%d\r\n",nRLed/ADC_CH_TOTAL,tmp1->nStart,tmp1->nEnd, tmp1->nEnd - tmp1->nStart + 1);
//    }
//#endif
//    return (nTotal*sizeof(stSgmH_t));

    return 0;
}

/*************************************************************************************
 * @Function :初始化扫描段之前发射灯的发射次数
 * @Parameter:[nAddress]: 保存段范围的起始地址
 * @return   :返回占用Flash的大小
 *************************************************************************************/
uint32_t InitBlockStartTLedTimesAndSave(uint32_t nFlashAddress)
{
//    eAxis_t  eAxis          = X;
//    int32_t  nCount         = 0;
//    uint32_t nTotal         = 0;
//    uint16_t *nStartTimesArray = (uint16_t*)GetSharedMem(0);
//    uint32_t nOffset        = 0;
//    int32_t  nMaxIndexCount = GetCurSharedMem() / sizeof(uint16_t);
//
//    //初始化每组一发多收在整个中的序号
//    for (eAxis = X; eAxis < eAxisCount; eAxis++)
//    {
//        nCount  = InitBlockStartTLedTimes(nStartTimesArray+nOffset, nMaxIndexCount, eAxis);
//        nCount  = (nCount + 3) / 4 * 4;
//        MemCpy((uint8_t *)g_nBlockStartTLedTimes[eAxis],(uint8_t *)(nStartTimesArray+nOffset),nCount*2);
//        nTotal  += nCount;
//        nOffset += nCount ;
//    }
//
//    FLASH_WriteLenByte(nFlashAddress + 0, (uint8_t*)nStartTimesArray, nTotal * sizeof(uint16_t));
//#if  0
//    for (uint8_t i = 0; i < (g_nTotal[X]+ADC_CH_TOTAL-1)/ADC_CH_TOTAL; i++)
//    {
//        printf("X:%d-%d\r\n", i,g_nBlockStartTLedTimes[X][i]);
//    }
//
//    for (uint8_t i = 0; i < (g_nTotal[Y]+ADC_CH_TOTAL-1)/ADC_CH_TOTAL; i++)
//    {
//        printf("Y:%d-%d\r\n",i,g_nBlockStartTLedTimes[Y][i]);
//    }
//#endif
//    return (nTotal*sizeof(uint16_t));

    return 0;
}

/*************************************************************************************
 * @Function :初始化ADC信号的索引,用于QT测试软件
 * @Parameter:[nAddress]: 保存段范围的起始地址
 * @return   :返回占用Flash的大小
 *************************************************************************************/
uint32_t InitTestAdcIndexAndSave(uint32_t nFlashAddress)
{
//    uint32_t nTotal         = 0;
//    eAxis_t    eAxis        = X;
//    uint16_t *nTestAdcIndex = (uint16_t*)GetSharedMem(0);
//    int32_t  nMaxIndexCount = GetCurSharedMem() / sizeof(uint16_t);
//    uint32_t nOffset        = 0;
//    uint32_t i = 0;
//    uint32_t arrTotal[eAxisCount] = {(X_TOTAL + 3) / 4 * 4, (Y_TOTAL + 3) / 4 * 4};
//
//    //初始化并保存测试用的Index数据
//    for (eAxis = X; eAxis < eAxisCount; eAxis++)
//    {
//        for (i = 0; i < OBQ_COUNT; i++)
//        {
//            InitTestAdcIndex(nTestAdcIndex+nOffset, nMaxIndexCount, eAxis, i);
//            nOffset += arrTotal[eAxis];
//            nTotal += arrTotal[eAxis];
//        }
//    }
//
//    FLASH_WriteLenByte(nFlashAddress + 0, (uint8_t*)nTestAdcIndex, nTotal * sizeof(uint16_t));
//    return (nTotal*sizeof(uint16_t));

    return 0;
}

/*************************************************************************************
 * @Function :初始化AGC信号的索引
 * @Parameter:[nAddress]: 保存段范围的起始地址
 * @return   :返回占用Flash的大小
 *************************************************************************************/
uint32_t InitTestAGCIndexAndSave(uint32_t nFlashAddress)
{
//    uint32_t nTotal         = 0;
//    eAxis_t    eAxis        = X;
//    uint16_t *nTestAgcIndex = (uint16_t*)GetSharedMem(0);
//    int32_t  nMaxIndexCount = GetCurSharedMem() / sizeof(uint16_t);
//    uint32_t nOffset        = 0;
//    uint32_t i = 0;
//    uint32_t arrTotal[eAxisCount] = {(X_TOTAL + 3) / 4 * 4, (Y_TOTAL + 3) / 4 * 4};
//    //初始化并保存测试用的Index数据
//    for (eAxis = X; eAxis < eAxisCount; eAxis++)
//    {
//        for (i = 0; i < OBQ_COUNT; i++)
//        {
//            InitTestAgcIndex(nTestAgcIndex+nOffset, nMaxIndexCount, eAxis, i);
//            nOffset += arrTotal[eAxis];
//            nTotal += arrTotal[eAxis];
//        }
//    }
//
//    FLASH_WriteLenByte(nFlashAddress + 0, (uint8_t*)nTestAgcIndex, nTotal * sizeof(uint16_t));
//    return (nTotal*sizeof(uint16_t));

    return 0;
}

/*************************************************************************************
 * @Function :初始化ADC信号的索引
 * @Parameter:[nAddress]: 保存段范围的起始地址
 * @return   :返回占用Flash的大小
 *************************************************************************************/
uint32_t InitAllAdcIndexAndSave(uint32_t nFlashAddress)
{
    uint32_t nTotal = 0;

//    nTotal = 0;//g_nAllAdcIndexCount;
//#if 0
//    //不再保存Adc有效索引表数据
//    for (uint32_t i = 0; i < g_nAllAdcIndexCount; i += 1)
//    {
//        g_nAllAdcIndex[i] = g_nAllAdcIndexIndex[g_nAllAdcIndex[i]];
//    }
//#endif
//    FLASH_WriteLenByte(nFlashAddress + 0, (uint8_t*)&g_nAllAdcIndexCount, sizeof(g_nAllAdcIndexCount));
//#if 0
//    //不再保存Adc有效索引表数据
//    FLASH_WriteLenByte(nFlashAddress + sizeof(g_nAllAdcIndexCount), (uint8_t*)g_nAllAdcIndex, nTotal * sizeof(uint16_t));
//    g_nAllAdcIndex = (uint16_t*)nFlashAddress + 1;
//#endif
//    g_nAllAdcIndexCount = *(uint16_t*)nFlashAddress;
//    nTotal++;

    return (nTotal*sizeof(uint16_t));
}
/*************************************************************************************
 * @Function :初始化ADC信号连续光线状态的索引(连续有效无效)
 * @Parameter:[nAddress]: 保存段范围的起始地址
 * @return   :已初始化的数据大小
 *************************************************************************************/
uint32_t InitAllAdcContinuStatuIdxDataAndSave(uint32_t nFlashAddress)
{
//    stScanLineIdx_t *pstScanLineIdx = (stScanLineIdx_t *)GetSharedMem(0);
//    int32_t  nMaxIndexCount = GetCurSharedMem() / sizeof(stScanLineIdx_t);
//    //只能清除 g_nAllAdcIndexCount * sizeof(uint16_t) 内存, 后面的内存是光线索引表中表, 需要共享内存过度
//    MemSet((uint8_t*)pstScanLineIdx, 0, g_nAllAdcIndexCount * sizeof(uint16_t));
//
//    for (uint32_t i = 0, bBreak = 0; i < g_nAllAdcIndexIndexCount && !bBreak; )
//    {
//        pstScanLineIdx->stScanLine[pstScanLineIdx->nTotalLineStatu].eLineStatu = (g_nAllAdcIndexIndex[i] != 0xffff) ? eValidLine : eInvalidLine;
//
//        pstScanLineIdx->stScanLine[pstScanLineIdx->nTotalLineStatu].stScanSgmH.nStart = i;
//        if (eValidLine == pstScanLineIdx->stScanLine[pstScanLineIdx->nTotalLineStatu].eLineStatu)
//        {
//            //连续有效光线
//            while (0xffff != g_nAllAdcIndexIndex[i++])
//            {
//                pstScanLineIdx->stScanLine[pstScanLineIdx->nTotalLineStatu].nContinuLineCnt++;
//                if (i >= g_nAllAdcIndexIndexCount)
//                {
//                    bBreak = 1;
//                    break;
//                }
//            }
//        }
//        else
//        {
//            //连续无效光线
//            while (0xffff == g_nAllAdcIndexIndex[i++])
//            {
//                pstScanLineIdx->stScanLine[pstScanLineIdx->nTotalLineStatu].nContinuLineCnt++;
//                if (i >= g_nAllAdcIndexIndexCount)
//                {
//                    bBreak = 1;
//                    break;
//                }
//            }
//        }
//        pstScanLineIdx->stScanLine[pstScanLineIdx->nTotalLineStatu].stScanSgmH.nEnd = !bBreak ? i - 2 : i - 1;
//        if (!bBreak)
//        {
//            i = pstScanLineIdx->stScanLine[pstScanLineIdx->nTotalLineStatu].stScanSgmH.nStart +
//                    pstScanLineIdx->stScanLine[pstScanLineIdx->nTotalLineStatu].nContinuLineCnt;
//        }
//        pstScanLineIdx->nTotalLineStatu++;
//    }
//#if 0
//    printf("pstScanLineIdx->nTotalLineStatu:%d-Size:%d\r\n\r\n", pstScanLineIdx->nTotalLineStatu, sizeof(stScanLineIdx_t) + (sizeof(stScanLine_t) * pstScanLineIdx->nTotalLineStatu));
//    for (uint32_t j = 0; j < pstScanLineIdx->nTotalLineStatu; j++)
//    {
//        printf("[%d]-Start:%d-End:%d-Total:%d-%s\r\n", j, pstScanLineIdx->stScanLine[j].stScanSgmH.nStart, pstScanLineIdx->stScanLine[j].stScanSgmH.nEnd,
//                pstScanLineIdx->stScanLine[j].nContinuLineCnt, (pstScanLineIdx->stScanLine[j].eLineStatu == eValidLine) ? "eValidLine" : "eInvalidLine");
//    }
//#endif
//    FLASH_WriteLenByte(nFlashAddress, (uint8_t*)pstScanLineIdx, (sizeof(stScanLineIdx_t) + (sizeof(stScanLine_t) * pstScanLineIdx->nTotalLineStatu)));
//
//    /*
//     * 打印这句证明当前建立连续 ADC 状态索引表的时候超出设定区域,
//     * 篡改了 g_nAllAdcIndexIndex 共享内存的数据,
//     * 可能会导致后面建立其他索引表的时候数据异常,
//     * 需重新调整共享内存的数据
//     */
//    if ((sizeof(stScanLineIdx_t) + (sizeof(stScanLine_t) * pstScanLineIdx->nTotalLineStatu)) >
//            (g_nAllAdcIndexCount * sizeof(uint16_t)))
//    {
//        printf("waning! share memory spill over g_nAllAdcIndexIndex table data abnormal\r\n");
//    }
//    return (sizeof(stScanLineIdx_t) + (sizeof(stScanLine_t) * pstScanLineIdx->nTotalLineStatu));

    return 0;
}
/*************************************************************************************
 * @Function :初始化ADC信号的索引
 * @Parameter:[nAddress]: 保存段范围的起始地址
 * @return   :已初始化的数据大小
 *************************************************************************************/
uint32_t InitAllAdcIndexData(uint16_t *nAdcIndexArray, int32_t nMaxIndexCount)
{
    uint32_t nTotal         = 0;
//    int32_t  nCount         = 0;
//    eEdge_t eEdge           = eBottom;
//    uint32_t nOffset        = 1;
//    uint32_t i = 0;
//
//    //uint32_t j = 0;
//    nTotal = nOffset;
//
//    //初始化并保存有用光线的索引
//    nCount = 0;
//    g_nAllAdcIndexCount = 0;
//    for (eEdge = eBottom; eEdge < eEdgeCount; eEdge++)
//    {
//        for (i = 0; i < g_nTotal[eEdge / 2]; i += ADC_CH_TOTAL)
//        {
//            nCount = InitAllAdcIndex(nAdcIndexArray+nOffset, nMaxIndexCount, eEdge, i);
//            //printf("nCount:%d, nOffset:%d , eEdge:%d,i:%d\r\n",nCount,nOffset,eEdge,i);
//            if (nCount > 0)
//            {
//                nOffset += nCount;
//                nTotal  += nCount;
//                g_nAllAdcIndexCount += nCount ;
//            }
//        }
//    }
//    nAdcIndexArray[0] = g_nAllAdcIndexCount ;
//    nAdcIndexArray[nOffset] = nAdcIndexArray[g_nAllAdcIndexCount] + 1; //Roc for Cprs Data
//    nTotal++;          //Roc

    return (nTotal*sizeof(uint16_t));
}
/*************************************************************************************
 * @Function :初始化ADC信号的索引
 * @Parameter:[nAddress]: 保存段范围的起始地址
 * @return   :已初始化的数据大小
 *************************************************************************************/
uint32_t InitAllAdcIndexIndexData(uint16_t *nAdcIndexArray, int32_t nMaxIndexCount)
{
    uint32_t nTotal = 0;
//    uint32_t i = 0;
//
//    nTotal = g_nBlockCount * ADC_CH_TOTAL;
//    MemSet(nAdcIndexArray, 0xff, nTotal * sizeof(uint16_t));
//
//    for (i = 0; i < g_nAllAdcIndexCount; i += 1)
//    {
//        uint16_t nAdcIdx = g_nAllAdcIndex[i];
//        nAdcIndexArray[nAdcIdx] = i;
//    }

    return (nTotal * sizeof(uint16_t));
}
/*
 * 保存所有ADC 通道对应的ADC 采样映射通道索引表
 */
uint32_t InitAllAdcChMapIdxAndSave(uint32_t nFlashAddress, uint16_t *nAdcIndexArray)
{
//    uint32_t nAdcTotal = g_nBlockCount * ADC_CH_TOTAL;
//    uint32_t nSaveId = 0;
//    uint32_t nAddrOffset = 0;
//    uint8_t nAdcCh = 0;
//    uint8_t arrAdcCh[4 * 1000];
//
//    for (uint32_t i = 0; i < nAdcTotal; i++)
//    {
//        if (nAdcIndexArray[i] == 0xffff)
//        {
//            continue;
//        }
//        nAdcCh = i % ADC_CH_TOTAL;      //ADC 通道对半分, 一个左 RAGC, 一个右 RAGC
//        arrAdcCh[nSaveId++] = nAdcCh;
//        if (nSaveId >= sizeof(arrAdcCh))
//        {
//            FLASH_WriteLenByte(nFlashAddress + nAddrOffset, (uint8_t*)arrAdcCh, nSaveId);
//            nAddrOffset += nSaveId;
//            nSaveId = 0;
//        }
//    }
//    if (nSaveId != 0)
//    {
//        FLASH_WriteLenByte(nFlashAddress + nAddrOffset, (uint8_t*)arrAdcCh, nSaveId);
//        nAddrOffset += nSaveId;
//    }
//
//    return (nAddrOffset * sizeof(uint8_t));

    return 0;
}
/*************************************************************************************
 * @Function :初始化ADC信号的索引
 * @Parameter:[nAddress]: 保存段范围的起始地址
 * @return   :已初始化的数据大小
 *************************************************************************************/
uint32_t InitAllAdcIndexIndexDataAndSave(uint32_t nFlashAddress)
{   
    uint32_t nTotal  = 0;
//    uint32_t i = 0;
//    uint16_t *nAdcIndexArray  = (uint16_t*)GetSharedMem(0);
//
//    /** The total number of rays saved by the first data */
//    nAdcIndexArray = &nAdcIndexArray[g_nAllAdcIndexCount + 1];
//    int32_t  nMaxIndexCount = (GetCurSharedMem()-(g_nAllAdcIndexCount * sizeof(uint16_t))) / sizeof(uint16_t);
//
//    nTotal = g_nBlockCount*ADC_CH_TOTAL;
//    MemSet(nAdcIndexArray, 0xff, nTotal * sizeof(uint16_t));
//
//    for (i = 0; i < g_nAllAdcIndexCount; i += 1)
//    {
//        uint16_t nAdcIdx = g_nAllAdcIndex[i];
//        nAdcIndexArray[nAdcIdx] = i;
//    }
//
//    FLASH_WriteLenByte(nFlashAddress, (uint8_t*)nAdcIndexArray, nTotal * sizeof(uint16_t));
    return (nTotal*sizeof(uint16_t));   
}


/*************************************************************************************
 * @Function :保存每个发射灯对应的 有效光线相对此组R_Group 中有效光线的偏移，最前面保存总共的组数
 * @Parameter:[nAddress]: 保存段范围的起始地址
 * @return   : 返回占用Flash的大小
 *************************************************************************************/
uint32_t InitTLedRLedLableCountSumAndSave(uint32_t nFlashAddress)
{
    uint32_t nTotal         = 0;
//    int32_t  nCount         = 0;
//    eEdge_t  eEdge          = eBottom;
//    uint16_t *nIndexIndexArray      = (uint16_t*)GetSharedMem(0);
//    int32_t  nMaxIndexCount = GetCurSharedMem() / sizeof(uint16_t);
//    uint32_t nOffset        = 0;
//    uint32_t i = 0;
//
//    //初始化并保存每组一发多收中第一个信号在DataIndex中的位置
//    nIndexIndexArray[0] = 0;
//    nIndexIndexArray[1] = 0;
//    nIndexIndexArray[2] = 0;
//    nIndexIndexArray[3] = 0;
//    g_nBlockCount = 0;
//    nOffset += 4;
//
//    nIndexIndexArray[4] = 0;
//    nCount   = 1;
//
//    for (eEdge = eBottom; eEdge < eEdgeCount; eEdge++)
//    {
//        for (i = 0; i < g_nTotal[eEdge / 2]; i += ADC_CH_TOTAL)
//        {
//            // nCount=>Receive LED Group num
//            nCount =  InitTLedRLedLableCountSum(nIndexIndexArray+nOffset, nMaxIndexCount, eEdge, i, nIndexIndexArray[nOffset-1]);
//            if (nCount > 0)
//            {
//#if 0
//                for (int32_t j = 0; j < nCount; j++)
//                {
//                    printf("%d,%d,%d,%d,eEdge:%d,i:%d\r\n", j, nIndexIndexArray[nOffset+j], nCount, g_nBlockCount, eEdge, i);
//                }
//#endif
//
//                nOffset += nCount ;
//                g_nBlockCount += nCount;
//                nTotal  += nCount ;
//                nCount %= 2;
//            }
//        }
//        if (eEdge == eBottom)
//        {
//            //rocord BlockCount of X Axis
//            *((uint16_t *)&nIndexIndexArray[1]) = g_nBlockCount;
//        }
//    }
//
//    *((uint16_t *)&nIndexIndexArray[0]) = g_nBlockCount;
//
//    nTotal += 4;
//
//    FLASH_WriteLenByte(nFlashAddress + 0, (uint8_t*)nIndexIndexArray, (nTotal)*sizeof(uint16_t));
#if 0
    printf("g_nBlockCount:%d\r\n",g_nBlockCount);
    for(i=0; i<g_nBlockCount+4; i++)
    {
        printf("nIndexIndexArray:[%d]: %d   \r\n",i,nIndexIndexArray[i]);
    }
#endif
    return (nTotal*sizeof(uint16_t));
}


/*************************************************************************************
 * @Function : 初始化接收R_Group在ADC Buffer中的起始索引
 * @Parameter: [nAddress]: 保存段范围的起始地址
 * @return   : 返回占用Flash的大小
 *************************************************************************************/
uint32_t InitRBlockAdcStartIndexAndSave(uint32_t nFlashAddress)
{
    uint32_t nTotal         = 0;
//    int32_t  nCount         = 0;
//    eEdge_t  eEdge          = eBottom;
//    uint16_t *nRBlockStartIndexArray      = (uint16_t*)GetSharedMem(0);
//    int32_t  nMaxIndexCount = GetCurSharedMem() / sizeof(uint16_t);
//    uint32_t nOffset        = 0;
//    uint32_t j = 0;
//
//    //初始化每组一发多收所依据的ADC的索引值
//    nCount = 0;
//    for (eEdge = eBottom; eEdge < eEdgeCount; eEdge++)
//    {
//        for (j = 0; j < g_nTotal[eEdge / 2]; j += ADC_CH_TOTAL)
//        {
//            nCount = InitRBlockAdcStartIndex(nRBlockStartIndexArray+nOffset, nMaxIndexCount, eEdge, j);
//            if (nCount > 0)
//            {
//                nOffset += nCount ;
//                nTotal  += nCount ;
//                nCount %= 2;
//            }
//        }
//    }
//
//    FLASH_WriteLenByte(nFlashAddress + 0, (uint8_t*)nRBlockStartIndexArray, nTotal*sizeof(uint16_t));
#if 0
    for (uint8_t i=0; i<nTotal; i++,nRBlockStartIndexArray[i])
    {
        printf("nRBlockStartIndexArray:[%d]: %d   \r\n",i,nRBlockStartIndexArray[i]);
    }
#endif

    return (nTotal * sizeof(uint16_t));
}

/*************************************************************************************
 * @Function : InitTestLedMaxInterValAndSave
 * @Parameter:[nAddress]: 保存段范围的起始地址
 * @return   : 返回占用Flash的大小
 *************************************************************************************/
uint32_t InitTestLedMaxInterValAndSave(uint32_t nFlashAddress)
{
//    InitLedMaxInterval();
//
//    FLASH_WriteLenByte(nFlashAddress + 0, (uint8_t*)g_nInterVal, sizeof(g_nInterVal));
//#if 0
//    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
//    {
//        for (uint32_t nDir = 0; nDir < OBQ_COUNT; nDir++)
//        {
//            printf("eAxis:%s-nDir:%d-InterVal:%d\r\n", X == eAxis ? "X" : "Y", nDir, g_nInterVal[X][nDir]);
//        }
//    }
//#endif
//    return  sizeof(g_nInterVal);

    return 0;
}
/*************************************************************************************
 * @Function : InitCommonConfigDataAndSave
 * @Parameter:[nAddress]: 保存段范围的起始地址
 * @return   : 返回占用Flash的大小
 *************************************************************************************/
uint32_t InitCommonConfigDataAndSave(uint32_t nFlashAddress)
{
    stHardConfigData_t stHwCfData;

    InitCommonConfigData(&stHwCfData);
    stHwCfData.nAppChekSum = GetFwCheckSum();
    stHwCfData.nConfigLength = sizeof(stHardConfigData_t);
//    stHwCfData.nLineCount = g_nAllAdcIndexCount;
    stHwCfData.nConfigCrc32Sum = GetCRC32(((uint8_t*)&stHwCfData) + 12, stHwCfData.nConfigLength-12);

    FlashWrite(nFlashAddress, (uint8_t *)&stHwCfData, sizeof(stHwCfData));
    return sizeof(stHardConfigData_t);
}

/*
 * 初始化flash配置
 */
int32_t InitFlashPara(void)
{
    uint32_t nAddress = 0, nCommCfgAddress = GetScanParaSaveAddr();  //将硬件配置信息放在最开始的地方
//    uint32_t nFlashAddrIndexAddress = FLASH_FW_TABLE_ADDRESS;
    uint32_t nLen = 0, nOffset = 0;
//    uint32_t arrTotal[eAxisCount] = {(X_TOTAL + 3) / 4 * 4, (Y_TOTAL + 3) / 4 * 4};
//    eEdge_t eEdge = eBottom;
//    eAxis_t eAxis = X;
//    uint32_t i = 0;
//    uint32_t nFWCheckSum = GetFwCheckSum();
//    stFlashAddrIndex_t stFlashAddressIndex = *(stFlashAddrIndex_t *)nFlashAddrIndexAddress;

    g_pConfigData = (stHardConfigData_t *)nCommCfgAddress;

//    if (GetCRC32((uint8_t *)(nCommCfgAddress + 12), sizeof(stHardConfigData_t) - 12) != g_pConfigData->nConfigCrc32Sum ||
//            g_pConfigData->nAppChekSum != nFWCheckSum ||
//            stFlashAddressIndex.nCrc32 != GetCRC32((uint8_t *)(nFlashAddrIndexAddress + 4), sizeof(stFlashAddrIndex_t) - 4) ||
//            stFlashAddressIndex.nAppChecksum != nFWCheckSum)
//    {
//        printf("Board flash parameter reinitialization...\n");
//        MemSet((uint8_t *)&stFlashAddressIndex, 0, sizeof(stFlashAddressIndex));
//
//        ClearFlashPara(); //清除Flash的相关配置数据
//
//        nOffset = FN_ALIGN_4BYTES_SFX(sizeof(stHardConfigData_t));   //将硬件配置信息放在最开始的地方
//        nAddress = FN_ALIGN_4BYTES_SFX(nCommCfgAddress + nOffset);
//
//        //Position of LEDs, 86寸 nLen = 426个LED * 8 = 3408 + 8
//        nLen = InitLedPositionAndSave(nAddress);
//        stFlashAddressIndex.stIndex[eLedPositionArray].nAddressStart = nAddress;
//        stFlashAddressIndex.stIndex[eLedPositionArray].nAddressLenght = nLen;
//        nAddress = FN_ALIGN_4BYTES_SFX(nAddress + nLen);
//
//        //Valid light's information about start and end LED Index
//        nLen = InitTLedRLedRelationAndSave(nAddress);
//        stFlashAddressIndex.stIndex[eLineLedRelationArray].nAddressStart = nAddress;
//        stFlashAddressIndex.stIndex[eLineLedRelationArray].nAddressLenght = nLen;
//        nAddress = FN_ALIGN_4BYTES_SFX(nAddress + nLen);
//
//        nLen = InitTestLedMaxInterValAndSave(nAddress);
//        stFlashAddressIndex.stIndex[eLedInterValArray].nAddressStart = nAddress;
//        stFlashAddressIndex.stIndex[eLedInterValArray].nAddressLenght = nLen;
//        nAddress = FN_ALIGN_4BYTES_SFX(nAddress + nLen);
//
//        //Start and End LED's Index of a certain Scan region
//        nLen = InitScanBlockRangeAndSave(nAddress);
//        stFlashAddressIndex.stIndex[eScanBlockRangeArray].nAddressStart = nAddress;
//        stFlashAddressIndex.stIndex[eScanBlockRangeArray].nAddressLenght = nLen;
//        nAddress = FN_ALIGN_4BYTES_SFX(nAddress + nLen);
//
//        //Cnt of lighted LED before a certain Receiving PD Scan region selected
//        nLen = InitBlockStartTLedTimesAndSave(nAddress);
//        stFlashAddressIndex.stIndex[eBlockStartTLedTimesArray].nAddressStart = nAddress;
//        stFlashAddressIndex.stIndex[eBlockStartTLedTimesArray].nAddressLenght = nLen;
//        nAddress = FN_ALIGN_4BYTES_SFX(nAddress + nLen);
//
//        nLen = InitTLedRLedLableCountSumAndSave(nAddress);
//        stFlashAddressIndex.stIndex[eTLedRLedLableCountSumArray].nAddressStart = nAddress;
//        stFlashAddressIndex.stIndex[eTLedRLedLableCountSumArray].nAddressLenght = nLen;
//        nAddress = FN_ALIGN_4BYTES_SFX(nAddress + nLen);
//        //使用共享内存保存光线索引表, 生成光线索引表中表之后消除这份表
//        uint16_t *pAllAdcIdxTmp = (uint16_t *)GetSharedMem(0);
//        nLen = InitAllAdcIndexData(pAllAdcIdxTmp, (int32_t)(GetCurSharedMem()/sizeof(uint16_t)));
//        g_nAllAdcIndex = (uint16_t*)(uint32_t)pAllAdcIdxTmp + 1;
//        g_nAllAdcIndexCount  = *(uint16_t*)pAllAdcIdxTmp;
//
//        //使用共享内存保存光线索引表中表, 生成连续光线状态索引表之后消除这份表
//        g_nAllAdcIndexIndex = NULL;
//        uint16_t *pAllAdcIdxIdxTmp = (uint16_t *)&pAllAdcIdxTmp[nLen];
//        if (NULL != pAllAdcIdxIdxTmp)
//        {
//            g_nAllAdcIndexIndex = (uint16_t *)pAllAdcIdxIdxTmp;
//            nLen = InitAllAdcIndexIndexData(pAllAdcIdxIdxTmp, (int32_t)((GetCurSharedMem() - nLen) / sizeof(uint16_t)));
//        }
//        g_nAllAdcIndexIndexCount = g_nBlockCount * ADC_CH_TOTAL;
//
//#if (SCAN_SOLUTION == SCAN_SOLUTION_6CH_E5HG)
//        nLen = InitAllAdcChMapIdxAndSave(nAddress, pAllAdcIdxIdxTmp);
//        stFlashAddressIndex.stIndex[eAllAdcChMapArray].nAddressStart = nAddress;
//        stFlashAddressIndex.stIndex[eAllAdcChMapArray].nAddressLenght = nLen;
//        nAddress = FN_ALIGN_4BYTES_SFX(nAddress + nLen);
//#endif
//
//        nLen = InitAllAdcIndexAndSave(nAddress);
//        stFlashAddressIndex.stIndex[eAllAdcIndexArray].nAddressStart = nAddress;
//        stFlashAddressIndex.stIndex[eAllAdcIndexArray].nAddressLenght = nLen;
//        nAddress = FN_ALIGN_4BYTES_SFX(nAddress + nLen);
//        
//        nLen = InitAllAdcContinuStatuIdxDataAndSave(nAddress);
//        stFlashAddressIndex.stIndex[eAllAllAdcContinuStatuIdxArray].nAddressStart = nAddress;
//        stFlashAddressIndex.stIndex[eAllAllAdcContinuStatuIdxArray].nAddressLenght = nLen;
//        nAddress = FN_ALIGN_4BYTES_SFX(nAddress + nLen);
//
//        nLen = InitTestAdcIndexAndSave(nAddress);
//        stFlashAddressIndex.stIndex[eTestADCIndexArray].nAddressStart = nAddress;
//        stFlashAddressIndex.stIndex[eTestADCIndexArray].nAddressLenght = nLen;
//        nAddress = FN_ALIGN_4BYTES_SFX(nAddress + nLen);
//
//        nLen = InitTestAGCIndexAndSave(nAddress);
//        stFlashAddressIndex.stIndex[eTestAgcIndexArray].nAddressStart = nAddress;
//        stFlashAddressIndex.stIndex[eTestAgcIndexArray].nAddressLenght = nLen;
//        nAddress = FN_ALIGN_4BYTES_SFX(nAddress + nLen);
//
//        nLen = InitRBlockAdcStartIndexAndSave(nAddress);
//        stFlashAddressIndex.stIndex[eRBlockAdcStartIndexArray].nAddressStart = nAddress;
//        stFlashAddressIndex.stIndex[eRBlockAdcStartIndexArray].nAddressLenght = nLen;
//        nAddress = FN_ALIGN_4BYTES_SFX(nAddress + nLen);
//
//        nLen = DMACopyToGpioInitAndSave(nAddress);
//        stFlashAddressIndex.stIndex[eDMACopyToLedOn].nAddressStart = nAddress;
//        stFlashAddressIndex.stIndex[eDMACopyToLedOn].nAddressLenght = nLen;
//        nAddress = FN_ALIGN_4BYTES_SFX(nAddress + nLen);
//
        nLen = InitCommonConfigDataAndSave(nCommCfgAddress);
//        stFlashAddressIndex.stIndex[eHardwareConfigArray].nAddressStart = nCommCfgAddress;
//        stFlashAddressIndex.stIndex[eHardwareConfigArray].nAddressLenght = nLen;
//
//        stFlashAddressIndex.nAppChecksum = nFWCheckSum;
//        stFlashAddressIndex.nAllCfgDataLen = stFlashAddressIndex.stIndex[eHardwareConfigArray].nAddressLenght +
//                                             stFlashAddressIndex.stIndex[eLedPositionArray].nAddressLenght +
//                                             stFlashAddressIndex.stIndex[eLineLedRelationArray].nAddressLenght;
//        stFlashAddressIndex.nCrc32 = GetCRC32((uint8_t *)(((uint32_t)&stFlashAddressIndex) + 4), sizeof(stFlashAddrIndex_t) - 4);
//        if (API_FLASH_PROC_OK != SaveDataList(eFwTableData, (uint8_t *)&stFlashAddressIndex, sizeof(stFlashAddrIndex_t)))
//        {
//            return -1;
//        }
//    }
//
//    g_nLedPositioCount = 2 * (g_nTotal[X] + g_nTotal[Y]); //计算灯管总数
//
//    nAddress  =  stFlashAddressIndex.stIndex[eLedPositionArray].nAddressStart;
//    nLen  =  stFlashAddressIndex.stIndex[eLedPositionArray].nAddressLenght;
//    for (eEdge = eBottom; eEdge < eEdgeCount; eEdge++)
//    {
//        g_arrPos[eEdge] = (stPosF_t*)nAddress;
//        nAddress += g_nTotal[eEdge / 2] * sizeof(stPosF_t);
//    }
//
//    nAddress  =  stFlashAddressIndex.stIndex[eLineLedRelationArray].nAddressStart;
//    nLen  =  stFlashAddressIndex.stIndex[eLineLedRelationArray].nAddressLenght;
//    g_nLineLedRelationArray = (uint16_t *)nAddress ;
//    g_nLineLedRelationCount = nLen/sizeof(uint16_t);
//
//    nAddress  =  stFlashAddressIndex.stIndex[eScanBlockRangeArray].nAddressStart;
//    nLen  =  stFlashAddressIndex.stIndex[eScanBlockRangeArray].nAddressLenght;
//    for (eAxis = X; eAxis < eAxisCount; eAxis++)
//    {
//        g_stScanSeg[eAxis] = (stSgmH_t*)nAddress;
//        nAddress += (((g_nTotal[eAxis] + R_ADC_GROUP - 1) / R_ADC_GROUP + 3) / 4 * 4 * sizeof(stSgmH_t));
//    }
//
//    nAddress  =  stFlashAddressIndex.stIndex[eBlockStartTLedTimesArray].nAddressStart;
//    nLen  =  stFlashAddressIndex.stIndex[eBlockStartTLedTimesArray].nAddressLenght;
//    for (eAxis = X; eAxis < eAxisCount; eAxis++)
//    {
//        uint32_t len = (((g_nTotal[eAxis] + R_ADC_GROUP - 1) / R_ADC_GROUP + 3) / 4 * 4 * sizeof(uint16_t));
//        MemCpy((uint16_t*)g_nBlockStartTLedTimes[eAxis],(uint16_t*)nAddress,len);
//        nAddress += len;
//    }
//
//    nAddress  =  stFlashAddressIndex.stIndex[eTestADCIndexArray].nAddressStart;
//    nLen  =  stFlashAddressIndex.stIndex[eTestADCIndexArray].nAddressLenght;
//    for (eAxis = X; eAxis < eAxisCount; eAxis++)
//    {
//        for (i = 0; i < OBQ_COUNT; i++)
//        {
//            g_nTestAdcIndex[eAxis][i] = (uint16_t*)nAddress;
//            g_nTestOrgIndex[eAxis][i] = (uint16_t*)nAddress;
//            nAddress += (arrTotal[eAxis] * sizeof(uint16_t));
//        }
//    }
//
//    nAddress  =  stFlashAddressIndex.stIndex[eTestAgcIndexArray].nAddressStart;
//    nLen  =  stFlashAddressIndex.stIndex[eTestAgcIndexArray].nAddressLenght;
//    for (eAxis = X; eAxis < eAxisCount; eAxis++)
//    {
//        for (i = 0; i < OBQ_COUNT; i++)
//        {
//            g_nTestAgcIndex[eAxis][i] = (uint16_t*)nAddress;
//            nAddress += (arrTotal[eAxis] * sizeof(uint16_t));
//        }
//    }
//    //Adc 第一份索引表在初始化表中表后即无效, 不能再调用该指针
//    g_nAllAdcIndex = NULL;//(uint16_t*)stFlashAddressIndex.stIndex[eAllAdcIndexArray].nAddressStart + 1;
//    g_nAllAdcIndexCount = *(uint16_t*)stFlashAddressIndex.stIndex[eAllAdcIndexArray].nAddressStart;
//    
//    nAddress = stFlashAddressIndex.stIndex[eTLedRLedLableCountSumArray].nAddressStart;
//    nLen = stFlashAddressIndex.stIndex[eTLedRLedLableCountSumArray].nAddressLenght;
//    g_nBlockCount = *(uint16_t*)nAddress;
//    g_nBlockCountAxis[X] = *((uint16_t*)nAddress + 1);
//    g_nBlockCountAxis[Y] = g_nBlockCount - g_nBlockCountAxis[X];
//    g_arrIndexSum = (uint16_t*)nAddress + 3;
//    //Adc 第二份索引表中表在初始化连续状态光线索引表后即无效, 不能再调用该指针
//    g_nAllAdcIndexIndex = NULL;
//    //g_nBlockCount 的初始化需要在前面
//    g_nAllAdcIndexIndexCount = g_nBlockCount * ADC_CH_TOTAL;
//
//    nAddress = stFlashAddressIndex.stIndex[eRBlockAdcStartIndexArray].nAddressStart ;
//    nLen = stFlashAddressIndex.stIndex[eRBlockAdcStartIndexArray].nAddressLenght ;
//    g_arrAdcIndexOfBlock = (uint16_t*)nAddress;
//
//    nAddress = stFlashAddressIndex.stIndex[eLedInterValArray].nAddressStart ;
//    nLen = stFlashAddressIndex.stIndex[eLedInterValArray].nAddressLenght ;
//    MemCpy((uint8_t*)g_nInterVal, (uint8_t*)nAddress, sizeof(g_nInterVal));
//
//#if (SCAN_SOLUTION == SCAN_SOLUTION_6CH_E5HG)
//    g_stScanCoreVar.pAdcChMap = (uint8_t *)stFlashAddressIndex.stIndex[eAllAdcChMapArray].nAddressStart;
//#endif
//    g_stScanCoreVar.pDMACopyToGpio = (stDMACopyToGpio_t *)stFlashAddressIndex.stIndex[eDMACopyToLedOn].nAddressStart;
//
//    g_pstScanLineIdxFlash = (stScanLineIdx_t  *)stFlashAddressIndex.stIndex[eAllAllAdcContinuStatuIdxArray].nAddressStart;
    return 1;
}

//获取算法配置表数据的开头
uint8_t *GetAlgTableCfgData(void)
{
    return (uint8_t *)GetScanParaSaveAddr();
}

//获取算法配置表数据的长度
int32_t GetAlgTableCfgLen(void)
{
    stFlashAddrIndex_t stFlashAddressIndex = *(stFlashAddrIndex_t *)FLASH_FW_TABLE_ADDRESS;
    return stFlashAddressIndex.nAllCfgDataLen;
}

//更新算法配置表数据的长度
int32_t SetAlgTableCfgLen(uint32_t nLen)
{
    uint32_t nFlashAddrIndexAddress = FLASH_FW_TABLE_ADDRESS;
    stFlashAddrIndex_t stFlashAddressIndex = *(stFlashAddrIndex_t *)nFlashAddrIndexAddress;
    stFlashAddressIndex.nAllCfgDataLen = nLen;

    stFlashAddressIndex.nCrc32 = GetCRC32((uint8_t *)(((uint32_t)&stFlashAddressIndex) + 4), sizeof(stFlashAddrIndex_t) - 4);
    if (status_success != SaveDataList(eFwTableData, (uint8_t *)&stFlashAddressIndex, sizeof(stFlashAddrIndex_t)))
    {
        return -1;
    }
    return 0;
}

//写入算法配置表数据
int32_t SaveAlgTableCfgData(uint8_t *pData, int32_t nLen)
{
    FlashErase(GetScanParaSaveAddr(), GetScanParaSaveLen());
    FlashWrite(GetScanParaSaveAddr(), pData, nLen);
    return 0;
}

