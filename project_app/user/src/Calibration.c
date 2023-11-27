#include "GlobalDefaultDefine.h"
#include "Calibration.h"
#include "FlashPara.h"
#include "Api_Math.h"
//#include "usb_lib.h"
//#include "AlgCfgPara.h"
//#include "PersistentData.h"
#include "Api_UsbDevice.h"

volatile uint8_t g_bInCalibrating = 0;
volatile uint8_t g_bCalEn = 1;

volatile stCalibrationData  g_stCalibrationData = {0};
volatile stCoord_t g_arrCalCoord[4] = {0};

float g_fXScale = 1.0f;
float g_fYScale = 1.0f;
float g_fXOffset = 0.0f;
float g_fYOffset = 0.0f;

/*****************************************************************
Function:           CalibrationInit
Description:        根据校准结果获得校准运算的参数
Parameter:          void
Return:             void
*****************************************************************/
void CalibrationInit(void)
{
    //四点变两点
    float fX1 = (g_stCalibrationData.arrCoord[0].nX + g_stCalibrationData.arrCoord[3].nX) * 4.0f;
    float fX2 = (g_stCalibrationData.arrCoord[1].nX + g_stCalibrationData.arrCoord[2].nX) * 4.0f;
    float fY1 = (g_stCalibrationData.arrCoord[0].nY + g_stCalibrationData.arrCoord[1].nY) * 4.0f;
    float fY2 = (g_stCalibrationData.arrCoord[3].nY + g_stCalibrationData.arrCoord[2].nY) * 4.0f;

    //缩放参数
    g_fXScale = (CALREFX2 - CALREFX1) / (fX2 - fX1);
    g_fYScale = (CALREFY3 - CALREFY1) / (fY2 - fY1);

    //平移参数
    g_fXOffset = CALREFX1 - g_fXScale * fX1;
    g_fXOffset = (g_fXOffset + CALREFX2 - g_fXScale * fX2) / 2;
    g_fYOffset = CALREFY1 - g_fYScale * fY1;
    g_fYOffset = (g_fYOffset + CALREFY3 - g_fYScale * fY2) / 2;
}

/*****************************************************************
Function:           CalWork
Description:        校准操作过程
Parameter:          void
Return:             void
*****************************************************************/
void CalWork(stPoint_t stPoint)
{
    static uint8_t s_bPress;
    uint8_t pInBuf[USB_PACKET_MAX_SIZE] = {0xFC, CAL, 0x0};

    switch (g_stCalibrationData.nStep)
    {
    case CAL_BEGIN:
        s_bPress = 0;
        pInBuf[2] = CAL_STEP1;
        USBSendPacket(eUsbDev0, eUsbCfgTouch0CommBit, pInBuf, USB_PACKET_MAX_SIZE);
        g_bInCalibrating = 0;
        break;

    case CAL_STEP1:
    case CAL_STEP2:
    case CAL_STEP3:
    case CAL_STEP4:
        if (stPoint.eStatus == eLeftBtnDown)
        {
            s_bPress = 1;
        }
        else if ((stPoint.eStatus == eLeftBtnUp || stPoint.eStatus == eRightBtnUp) && s_bPress == 1)
        {
            s_bPress = 0;
            g_arrCalCoord[g_stCalibrationData.nStep - 1].nX = stPoint.stCoord.nX;
            g_arrCalCoord[g_stCalibrationData.nStep - 1].nY = stPoint.stCoord.nY;
            pInBuf[2] = g_stCalibrationData.nStep + 1;
            USBSendPacket(eUsbDev0, eUsbCfgTouch0CommBit, pInBuf, USB_PACKET_MAX_SIZE);
            g_bInCalibrating = 0;
        }
        break;

    default:
        break;
    }
}

/*****************************************************************
Function:              AdjustCoordinate
Description:           调整X与Y是否互换，X是否需要镜像，Y是否需要镜像
Parameter:             void
Return:                void
*****************************************************************/
void AdjustCoordinate(stPoint_t* stPoint, int32_t nWidth, int32_t nHeight)
{
//    int32_t i;
//
//    switch(g_stCfgData.stHwCfgData.eRotationParameter)
//    {
//        case 0:
//        {
//            for(i = 0; i < USER_POINT; i++)
//            {
//                stPoint[i].stCoord.nX = MAX_LOGICAL_VALUE - stPoint[i].stCoord.nX * MAX_LOGICAL_VALUE / (nWidth - 1);
//                stPoint[i].stCoord.nY = MAX_LOGICAL_VALUE - stPoint[i].stCoord.nY * MAX_LOGICAL_VALUE / (nHeight - 1);
//            }
//        }
//        break;
//
//        case 1:
//        {
//            for(i = 0; i < USER_POINT; i++)
//            {
//                stPoint[i].stCoord.nX = stPoint[i].stCoord.nX + stPoint[i].stCoord.nY;
//                stPoint[i].stCoord.nY = stPoint[i].stCoord.nX - stPoint[i].stCoord.nY;
//                stPoint[i].stCoord.nX = stPoint[i].stCoord.nX - stPoint[i].stCoord.nY;
//
//                stPoint[i].stCoord.nX = MAX_LOGICAL_VALUE - stPoint[i].stCoord.nX * MAX_LOGICAL_VALUE / (nWidth - 1);
//                stPoint[i].stCoord.nY = MAX_LOGICAL_VALUE - stPoint[i].stCoord.nY * MAX_LOGICAL_VALUE / (nHeight - 1);
//            }
//        }
//        break;
//
//        case 2:
//        {
//            for(i = 0; i < USER_POINT; i++)
//            {
//                stPoint[i].stCoord.nX = MAX_LOGICAL_VALUE - stPoint[i].stCoord.nX * MAX_LOGICAL_VALUE / (nWidth - 1);
//                stPoint[i].stCoord.nY = stPoint[i].stCoord.nY * MAX_LOGICAL_VALUE / (nHeight - 1);
//            }
//        }
//        break;
//
//        case 3:
//        {
//            for(i = 0; i < USER_POINT; i++)
//            {
//                stPoint[i].stCoord.nX = stPoint[i].stCoord.nX + stPoint[i].stCoord.nY;
//                stPoint[i].stCoord.nY = stPoint[i].stCoord.nX - stPoint[i].stCoord.nY;
//                stPoint[i].stCoord.nX = stPoint[i].stCoord.nX - stPoint[i].stCoord.nY;
//
//                stPoint[i].stCoord.nX = MAX_LOGICAL_VALUE - stPoint[i].stCoord.nX * MAX_LOGICAL_VALUE / (nWidth - 1);
//                stPoint[i].stCoord.nY = stPoint[i].stCoord.nY * MAX_LOGICAL_VALUE / (nHeight - 1);
//            }
//        }
//        break;
//
//        case 4:
//        {
//            for(i = 0; i < USER_POINT; i++)
//            {
//                stPoint[i].stCoord.nX = stPoint[i].stCoord.nX * MAX_LOGICAL_VALUE / (nWidth - 1);
//                stPoint[i].stCoord.nY = MAX_LOGICAL_VALUE - stPoint[i].stCoord.nY * MAX_LOGICAL_VALUE / (nHeight - 1);
//            }
//        }
//        break;
//
//        case 5:
//        {
//            for(i = 0; i < USER_POINT; i++)
//            {
//                stPoint[i].stCoord.nX = stPoint[i].stCoord.nX + stPoint[i].stCoord.nY;
//                stPoint[i].stCoord.nY = stPoint[i].stCoord.nX - stPoint[i].stCoord.nY;
//                stPoint[i].stCoord.nX = stPoint[i].stCoord.nX - stPoint[i].stCoord.nY;
//
//                stPoint[i].stCoord.nX = stPoint[i].stCoord.nX * MAX_LOGICAL_VALUE / (nWidth - 1);
//                stPoint[i].stCoord.nY = MAX_LOGICAL_VALUE - stPoint[i].stCoord.nY * MAX_LOGICAL_VALUE / (nHeight - 1);
//            }
//        }
//        break;
//
//        case 6:
//        {
//            for(i = 0; i < USER_POINT; i++)
//            {
//                stPoint[i].stCoord.nX = stPoint[i].stCoord.nX * MAX_LOGICAL_VALUE / (nWidth - 1);
//                stPoint[i].stCoord.nY = stPoint[i].stCoord.nY * MAX_LOGICAL_VALUE / (nHeight - 1);
//            }
//        }
//        break;
//
//        case 7:
//        {
//            for(i = 0; i < USER_POINT; i++)
//            {
//                stPoint[i].stCoord.nX = stPoint[i].stCoord.nX + stPoint[i].stCoord.nY;
//                stPoint[i].stCoord.nY = stPoint[i].stCoord.nX - stPoint[i].stCoord.nY;
//                stPoint[i].stCoord.nX = stPoint[i].stCoord.nX - stPoint[i].stCoord.nY;
//
//                stPoint[i].stCoord.nX = stPoint[i].stCoord.nX * MAX_LOGICAL_VALUE / (nWidth - 1);
//                stPoint[i].stCoord.nY = stPoint[i].stCoord.nY * MAX_LOGICAL_VALUE / (nHeight - 1);
//            }
//        }
//        break;
//    }
}

/*****************************************************************
Function:         DoCalibration
Description:      校准相关的主函数
Parameter:        void
Return:           void
*****************************************************************/
void DoCalibration(stPoint_t* stPoint, unsigned int nCount, int32_t nWidth, int32_t nHeight)
{
    AdjustCoordinate(stPoint, nWidth, nHeight);

    if (1 == g_bInCalibrating)
    {
        CalWork(stPoint[0]);
    }

    if(1 == g_bCalEn)
    {
        for (int32_t i = 0; i < USER_POINT; i++)
        {
            stPoint[i].stCoord.nX = g_fXScale * stPoint[i].stCoord.nX + g_fXOffset;
            stPoint[i].stCoord.nY = g_fYScale * stPoint[i].stCoord.nY + g_fYOffset;
        }
    }
    for (int32_t i = 0; i < USER_POINT; i++)
    {
        stPoint[i].stCoord.nX = FN_MIN(MAX_LOGICAL_VALUE, FN_MAX(0, stPoint[i].stCoord.nX));
        stPoint[i].stCoord.nY = FN_MIN(MAX_LOGICAL_VALUE, FN_MAX(0, stPoint[i].stCoord.nY));
    }
}

/*****************************************************************
Function:         GetDeviation
Description:      计算校准偏差
Parameter:        void
Return:           偏差值
*****************************************************************/
uint32_t GetDeviation()
{
    uint32_t nXlen = 0;
    uint32_t nYLen = 0;
    uint32_t nXDeviation = 0;
    uint32_t nYDeviation = 0;

    if(FN_ABS(g_stCalibrationData.arrCoord[0].nX - g_stCalibrationData.arrCoord[3].nX) < FN_ABS(g_stCalibrationData.arrCoord[0].nX - g_stCalibrationData.arrCoord[1].nX))
    {
        nXlen = FN_ABS((g_stCalibrationData.arrCoord[0].nX + g_stCalibrationData.arrCoord[3].nX) - (g_stCalibrationData.arrCoord[1].nX + g_stCalibrationData.arrCoord[2].nX)) / 2;
        nYLen = FN_ABS((g_stCalibrationData.arrCoord[0].nY + g_stCalibrationData.arrCoord[1].nY) - (g_stCalibrationData.arrCoord[2].nY + g_stCalibrationData.arrCoord[3].nY)) / 2 ;
        nXDeviation = (FN_ABS(g_stCalibrationData.arrCoord[3].nX - g_stCalibrationData.arrCoord[0].nX) / 2 + FN_ABS(g_stCalibrationData.arrCoord[1].nX - g_stCalibrationData.arrCoord[2].nX) / 2) / 2;
        nYDeviation = (FN_ABS(g_stCalibrationData.arrCoord[3].nY - g_stCalibrationData.arrCoord[2].nY) / 2 + FN_ABS(g_stCalibrationData.arrCoord[1].nY - g_stCalibrationData.arrCoord[0].nY) / 2) / 2;
    }
    else
    {
        nXlen = FN_ABS((g_stCalibrationData.arrCoord[0].nX + g_stCalibrationData.arrCoord[1].nX) - (g_stCalibrationData.arrCoord[3].nX + g_stCalibrationData.arrCoord[2].nX)) / 2;
        nYLen = FN_ABS((g_stCalibrationData.arrCoord[0].nY + g_stCalibrationData.arrCoord[3].nY) - (g_stCalibrationData.arrCoord[2].nY + g_stCalibrationData.arrCoord[1].nY)) / 2 ;
        nXDeviation = (FN_ABS(g_stCalibrationData.arrCoord[1].nX - g_stCalibrationData.arrCoord[0].nX) / 2 + FN_ABS(g_stCalibrationData.arrCoord[3].nX - g_stCalibrationData.arrCoord[2].nX) / 2) / 2;
        nYDeviation = (FN_ABS(g_stCalibrationData.arrCoord[3].nY - g_stCalibrationData.arrCoord[0].nY) / 2 + FN_ABS(g_stCalibrationData.arrCoord[1].nY - g_stCalibrationData.arrCoord[2].nY) / 2) / 2;
    }
    return nXDeviation * 500 / nXlen + nYDeviation * 500 / nYLen;
}

int32_t SaveCalibrationData(void)
{
//    stFlashParaOfCalibration_t stFlashParaOfCalibration;
//    stFlashPersistentData_t    stFlashPersistentData;
//    
//    GetPersistentData(&stFlashPersistentData);
//    
//    stFlashParaOfCalibration.arrCoord[0] = g_stCalibrationData.arrCoord[0];
//    stFlashParaOfCalibration.arrCoord[1] = g_stCalibrationData.arrCoord[1];
//    stFlashParaOfCalibration.arrCoord[2] = g_stCalibrationData.arrCoord[2];
//    stFlashParaOfCalibration.arrCoord[3] = g_stCalibrationData.arrCoord[3];
//    stFlashParaOfCalibration.nREOX1 = g_stCalibrationData.nREOX1;
//    stFlashParaOfCalibration.nREOX2 = g_stCalibrationData.nREOX2;
//    stFlashParaOfCalibration.nREOY1 = g_stCalibrationData.nREOY1;
//    stFlashParaOfCalibration.nREOY2 = g_stCalibrationData.nREOY2;
//
//    stFlashPersistentData.stFlashParaOfCalibration = stFlashParaOfCalibration;
//    
//    if(API_FLASH_PROC_OK == SavePersistentData(stFlashPersistentData))
//    {
//        return 0;
//    }
    return -1;  
}

int32_t ResetCalibration(void)
{   
//    stFlashParaOfCalibration_t stParaOfCalibration;
//    stFlashPersistentData_t    stFlashPersistentData;
//
//    GetPersistentData(&stFlashPersistentData);
//    
//    GetDefaultCalibrationFlashPara(&stParaOfCalibration);
//    InitCalibration(stParaOfCalibration);
//
//    stFlashPersistentData.stFlashParaOfCalibration = stParaOfCalibration;
//    if(API_FLASH_PROC_OK == SavePersistentData(stFlashPersistentData))
//    {
//        return 0;
//    }
    return -1;
}

int32_t InitCalibration(stFlashParaOfCalibration_t stFlashParaOfCalibration)
{
    g_stCalibrationData.arrCoord[0] = stFlashParaOfCalibration.arrCoord[0];
    g_stCalibrationData.arrCoord[1] = stFlashParaOfCalibration.arrCoord[1];
    g_stCalibrationData.arrCoord[2] = stFlashParaOfCalibration.arrCoord[2];
    g_stCalibrationData.arrCoord[3] = stFlashParaOfCalibration.arrCoord[3];

    g_stCalibrationData.nREOX1 = stFlashParaOfCalibration.nREOX1;
    g_stCalibrationData.nREOX2 = stFlashParaOfCalibration.nREOX2;
    g_stCalibrationData.nREOY1 = stFlashParaOfCalibration.nREOY1;
    g_stCalibrationData.nREOY2 = stFlashParaOfCalibration.nREOY2;

    CalibrationInit();
    return 0;
}

int32_t GetDefaultCalibrationFlashPara(stFlashParaOfCalibration_t* pFlashParaOfCalibration)
{
    pFlashParaOfCalibration->arrCoord[0].nX = g_pConfigData->nREOX1;
    pFlashParaOfCalibration->arrCoord[0].nY = g_pConfigData->nREOY1;
    pFlashParaOfCalibration->arrCoord[1].nX = g_pConfigData->nREOX2;
    pFlashParaOfCalibration->arrCoord[1].nY = g_pConfigData->nREOY1;
    pFlashParaOfCalibration->arrCoord[2].nX = g_pConfigData->nREOX2;
    pFlashParaOfCalibration->arrCoord[2].nY = g_pConfigData->nREOY2;
    pFlashParaOfCalibration->arrCoord[3].nX = g_pConfigData->nREOX1;
    pFlashParaOfCalibration->arrCoord[3].nY = g_pConfigData->nREOY2;

    pFlashParaOfCalibration->nREOX1 = REOX1;
    pFlashParaOfCalibration->nREOX2 = REOX2;
    pFlashParaOfCalibration->nREOY1 = REOY1;
    pFlashParaOfCalibration->nREOY2 = REOY2;

    return 0;
}


int32_t SetCalibrationData(void)
{
    g_stCalibrationData.arrCoord[0].nX = g_stCalibrationData.nREOX1;
    g_stCalibrationData.arrCoord[0].nY = g_stCalibrationData.nREOY1;
    g_stCalibrationData.arrCoord[1].nX = g_stCalibrationData.nREOX2;
    g_stCalibrationData.arrCoord[1].nY = g_stCalibrationData.nREOY1;
    g_stCalibrationData.arrCoord[2].nX = g_stCalibrationData.nREOX2;
    g_stCalibrationData.arrCoord[2].nY = g_stCalibrationData.nREOY2;
    g_stCalibrationData.arrCoord[3].nX = g_stCalibrationData.nREOX1;
    g_stCalibrationData.arrCoord[3].nY = g_stCalibrationData.nREOY2;
    
    CalibrationInit();
    return 0;
}
