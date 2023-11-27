/*
 * Evaluation.c
 *
 *  Created on: 2018年12月11日
 *      Author: cvter
 */

#include "Evaluation.h"
#include "GlobalDataStructure.h"
#include "GlobalVariables.h"
#include "CommService.h"
//#include "SendCoord.h"
//#include "AlgInfraredVector.h"
//#include "AlgSharedMem.h"
#include "string.h"
#include "Api_Crc.h"
#include "Api_Flash.h"
//#include "usb_lib.h"
#include "TouchSignal.h"
#include "FlashPara.h"
//#include "ZwFlash.h"

volatile int32_t g_nCurFrame = 0;
volatile uint8_t g_bUsbDebug = 0; //使用usb bulk接口发送调试信息
volatile uint8_t g_nUsbPrintReportId = CMD_F0_REPORT_ID;
volatile uint32_t g_nEvaTick = 0;
volatile int32_t  g_bGetMaskData = 0;
eUsbDevice_t g_eUsbDevice = eUsbDev0;
eUsbCfgBitType_t g_eUsbSendCfgType = eUsbCfgBulkBit;
uint8_t g_nEvalReportId = TOUCH_LINE_STATU_COMPRESS_ID;

int32_t AddCompressEvalData(uint8_t *pBuf, int16_t nLen, eCmdSource_t eSource)
{
    static uint32_t s_nIndex = 0;
    uint8_t nFrame = pBuf[1];
    uint32_t nDatalen = 0;
    uint16_t nPktNo = FN_UINT16_SFX(&pBuf[2]);

    if (0 == nPktNo)
    {
        if(g_TouchDataFifo[g_nTouchDataFifoBack].eBufStatus != eTouchBufReady &&
                g_TouchDataFifo[g_nTouchDataFifoBack].eBufStatus != eTouchBufLock)
        {
            g_TouchDataFifo[g_nTouchDataFifoBack].eDataType = pBuf[0];
            g_TouchDataFifo[g_nTouchDataFifoBack].eBufStatus = eTouchBufTransfering;
            g_TouchDataFifo[g_nTouchDataFifoBack].nFrame = nFrame;
            s_nIndex = 0;

            nLen = FN_MIN(MAX_LINE_COUNT - s_nIndex, nLen - 4);
            MemCpy(&g_TouchDataFifo[g_nTouchDataFifoBack].pBuf[s_nIndex], &pBuf[4], nLen);
            s_nIndex += nLen;
        }
    }
    else if(nPktNo < (uint16_t)0xFFFF)
    {
        if(nFrame == g_TouchDataFifo[g_nTouchDataFifoBack].nFrame
            && g_TouchDataFifo[g_nTouchDataFifoBack].eBufStatus == eTouchBufTransfering)
        {
            nLen = FN_MIN(MAX_LINE_COUNT - s_nIndex, nLen - 4);
            MemCpy(&g_TouchDataFifo[g_nTouchDataFifoBack].pBuf[s_nIndex], &pBuf[4], nLen);
            s_nIndex += nLen;
        }
    }
    else if (g_TouchDataFifo[g_nTouchDataFifoBack].eBufStatus == eTouchBufTransfering)
    {
        nDatalen = FN_MIN(FN_UINT32_SFX(&pBuf[8]), MAX_LINE_COUNT);
        if (nFrame == g_TouchDataFifo[g_nTouchDataFifoBack].nFrame
            && nDatalen <= s_nIndex
            && FN_UINT32_SFX(&pBuf[4]) == GetCRC32(g_TouchDataFifo[g_nTouchDataFifoBack].pBuf, nDatalen))
        {
            g_TouchDataFifo[g_nTouchDataFifoBack].nLen = nDatalen;
            g_TouchDataFifo[g_nTouchDataFifoBack].nTimeStamp = (FN_UINT32_SFX(&pBuf[12]) == ~FN_UINT32_SFX(&pBuf[16])) ? FN_UINT32_SFX(&pBuf[12]) : DEFAULT_FRAME_TIMESTAMP;
            g_TouchDataFifo[g_nTouchDataFifoBack].nDataFrameNo = FN_UINT32_SFX(&pBuf[20]);
            g_TouchDataFifo[g_nTouchDataFifoBack].eBufStatus = eTouchBufReady;
            g_nTouchDataFifoBack = (g_nTouchDataFifoBack + 1) % (sizeof(g_TouchDataFifo)/sizeof(stTouchData));
        }
        else
        {
            printf("%d=%d, %d=%d, 0x%08x=0x%08x\n",nFrame,g_TouchDataFifo[g_nTouchDataFifoBack].nFrame,
                    nDatalen, s_nIndex,
                    FN_UINT32_SFX(&pBuf[4]), GetCRC32(g_TouchDataFifo[g_nTouchDataFifoBack].pBuf, nDatalen));
        }
    }
    return 0;
}

//与调试算法相关的变量重置
int32_t DebugAlgDataReset(void)
{
//    MemSet(&g_pAlgCoreVar->stActiveArea, 0, sizeof(g_pAlgCoreVar->stActiveArea));
//
//    stPointF_t arrPointF[MAX_POINT_SFX];
//    MemSet(arrPointF, 0, sizeof(arrPointF));
//    MatchPoints(arrPointF, 0);
    return 0;
}

//仿真算法接口
int32_t Evaluation(uint8_t* pBuf, int16_t nLen, eCmdSource_t eMode)
{
    uint8_t pOutBuf[64] = {0xFC, 0xB2, 0x08, 0x03, 0x00, 0x00, 0x02, 0x00};

    switch (pBuf[1])
    {
        case CMD_F0_STOP_SLAVE_SCAN:  //开始仿真前的环境配置1
        {
            g_bSyncMode = 1;
            AnswerCommand(pOutBuf, sizeof(pOutBuf), eMode);
        }
        break;

        case CMD_F0_START_EVALUATION:  //开始仿真前的环境配置2
        {
            g_bSyncMode = 1;
            g_nTouchDataFifoFront = 0;
            g_nTouchDataFifoBack = 0;
            for(uint8_t i = 0; i < 2; i++)
            {
                g_TouchDataFifo[i].eBufStatus = eTouchBufNone;
            }
            g_bRecvQTData = 1;
            g_bDebuging = 0;

            UsbDevCallbackRegister(TOUCH_LINE_STATU_COMPRESS_ID, AddCompressEvalData);
            UsbDevCallbackRegister(TOUCH_LINE_STATU_COMPRESS_MASK_ID, AddCompressEvalData);
            AnswerCommand(pOutBuf, sizeof(pOutBuf), eMode);
            printf("Start evaluate!\r\n");
        }
        break;

        case CMD_F0_SET_FRAME_NO:
        {
            g_nCurFrame = *(int32_t *)(&pBuf[2]);
        }
        break;

        case CMD_F0_SET_RUN_ALG:
        {
            g_bRunAlg = *(int32_t *)(&pBuf[2]);
        }
        break;

        case CMD_F0_SET_USB_DEBUG_OUT:
        {
            g_bUsbDebug = *(uint8_t *)(&pBuf[2]);
            g_eUsbDevice = eMode < eUsb0EpsCount ? eUsbDev0 : eUsbDev1;
            uprintf("Usb printer:%d\r\n", g_bUsbDebug);
        }
        break;

        case CMD_F0_SET_USB_COORD_EN:
        {
//            g_stUsbCoordCfg[GetUsbDev(eMode)].bCoordEn = *(uint8_t *)(&pBuf[2]);
//            printf("bUsb%dCoordEn:%d\r\n", GetUsbDev(eMode), g_stUsbCoordCfg[GetUsbDev(eMode)].bCoordEn);
        }
        break;

        case CMD_F0_CONFIG_DATA: //接收上位机的配置
        {
            g_bSyncMode = 1; //关闭扫描

//            static uint8_t s_nGetSum = 0;
//            static int32_t s_nConfigBufLen = 0;
//
//            uint8_t *pSharedMem = GetShareMemHead(); //获取共享内存头部当作临时内存
//            uint8_t *arrOutData = pBuf;
//            if (*((uint16_t *)&arrOutData[7]) == 0) //首包
//            {
//                s_nGetSum = arrOutData[6];
//                s_nConfigBufLen = *((int32_t *)&arrOutData[2]);
//                printf("s_nConfigBufLen:%d, 0x%02x\n", s_nConfigBufLen, s_nGetSum);
//            }
//            else if (*((uint16_t *)&arrOutData[7]) != 0xffff)
//            {
//                uint16_t i = *(uint16_t *)&arrOutData[9];
//                MemCpy(&pSharedMem[i], &arrOutData[11], arrOutData[8]);
//            }
//            else
//            {
//                if (s_nGetSum == (uint8_t)GetSum(pSharedMem, s_nConfigBufLen))
//                {
//                    printf("Get CONFIG_DATA OK! SaveAlgTableCfgData...\r\n");
//                    AnswerCommand(pOutBuf, sizeof(pOutBuf), eMode);
//                    SaveAlgTableCfgData(pSharedMem, s_nConfigBufLen);
//                    SetAlgTableCfgLen(s_nConfigBufLen);
//                    printf("pls download simulated firmware...\r\n");
//                }
//                else
//                {
//                    printf("GetSum error! 0x%02x, 0x%02x\r\n", s_nGetSum, (uint8_t)GetSum(pSharedMem, s_nConfigBufLen));
//                }
//            }
//            AnswerCommand(pOutBuf, sizeof(pOutBuf), eMode);
        }
        break;

        case CMD_F0_SYNC_SIGNAL: //同步信号
        {

        }
        break;

        case CMD_F0_PLAYBACK_DATA_EN: //使能上传回放数据
        {

        }
        break;

        case CMD_F0_SYNC_PLAYBACK_DATA: //同步回放数据
        {

        }
        break;

        case CMD_F0_RESET_ALG_DATA: //清除算法数据
        {
            DebugAlgDataReset();
        }
        break;

        case CMD_F0_START_UPLOAD: //开始数据传输
        {
            g_bSendTouchData = pBuf[2];
            g_eUsbDevice = eMode < eUsb0EpsCount ? eUsbDev0 : eUsbDev1;
        }
        break;

        case CMD_F0_GET_MASK_DATA: //获取背景数据
        {
            g_bGetMaskData = pBuf[2];
        }
        break;

        default:
        break;
    }
    return 0;
}

//仿真调试初始化
int32_t EmulatorInit(void)
{
    UsbDevCallbackRegister(CMD_F0_REPORT_ID, Evaluation);
#if USE_ALG_EMULATOR
    g_bSyncMode = 1;
    printf("\r\n*******warning!!! using emulator!!!*******\r\n\r\n");
    stHwCfgData_t *pHwCfgData = (stHwCfgData_t *)GetAlgTableCfgData();
    int32_t nLineCount = FN_MIN_SFX(MAX_LINE_COUNT, pHwCfgData->nLineCount);

    for (uint8_t i = 0; i < 2; i++)
    {
        g_TouchDataFifo[i].pBuf = (uint8_t *)GetSharedMem(nLineCount);
    }
    FLASH_EraseLenByte(ALG_TABLE_PARA_ADDR, ALG_TABLE_PARA_LEN); //破坏慢速表，强制令其重建
#endif
    return 0;
}

