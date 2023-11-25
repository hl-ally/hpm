#include "CommService.h"
//#include "SystemDefine.h"
#include "Command.h"
#include <string.h>
#include <stdio.h>
#include "API_Math.h"
#include "Api_Flash.h"
#include "Api_Crc.h"
//#include "Api_Gpio.h"
//#include "upgrade.h"
#include "FlashPara.h"
#include "GlobalVariables.h"
#include "GlobalDefaultDefine.h"
#include "Api_UsbDevice.h"
#include "hpm_common.h"
#include "hpm_gpio_drv.h"
#include "hpm_ppor_drv.h"
#include "Evaluation.h"
#include "app_systick.h"



#define CALIB_INFO_DUG          (0u)      //校准打印输出
#ifndef REAL_ADJ_AGC_EN
#define REAL_ADJ_AGC_EN         (1u)      //实时调节Agc
#endif
#define AGC_UPLOAD_GEAR_OFFSET  (5)

#define MAX_INVERSE_VAL_REQ     (20)      //最大反向值的阈值

typedef struct _APP_SIGNAL_TEST
{
    CMD_QUEUE_BLOCK  stSingalData[7];
    uint8_t dataStep;

} APP_SIGNAL_TEST;

static APP_SIGNAL_TEST  g_stSingalTest;//用于信号测试，先收完数据保存，待收完后再一起回应

static void SoftwareRest(void)
{
    uint32_t resetTime = 1*24*1000*1000; 
    printf("softerware reset after %ds\r\n", resetTime/(24*1000*1000));
    ppor_sw_reset(HPM_PPOR, resetTime);
    while(1);
}

/*****************************************************************
 * @Function: 服务程序获取版本号的通讯
 * @Parameter: pCmdBlock: 通讯输入的内容
              pReCmdBlock：通讯输出的内容
 * @return   :  应答方式
*****************************************************************/
ATTR_WEAK CmdAnswerType CmdVersion(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;

    switch (pCmdBlock->DataPacket[2])
    {
    case VER_GET:
    {
        uint32_t nCheckSum = GetFwCheckSum();
        pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
        pReCmdBlock->DataPacket[1] = VERSION;
        pReCmdBlock->DataPacket[2] = VER_GET_RE;
        pReCmdBlock->DataPacket[3] = 19;
        pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];
        MemSet(&pReCmdBlock->DataPacket[5], 0xFF, 23);

        *(uint32_t*)&pReCmdBlock->DataPacket[8] = nCheckSum;
        if((pCmdBlock->eCmdSource == eUsb0Ep1Mode)||
                (pCmdBlock->eCmdSource == eUsb0Ep2Mode)||
                (pCmdBlock->eCmdSource == eUsb0Ep3Mode))
        {
            pReCmdBlock->DataPacket[18] = FN_BYTE(USB0_VID, 0);
            pReCmdBlock->DataPacket[19] = FN_BYTE(USB0_VID, 1);

            pReCmdBlock->DataPacket[20] = FN_BYTE(USB0_PID, 0);
            pReCmdBlock->DataPacket[21] = FN_BYTE(USB0_PID, 1);
        }
        else
        {
            pReCmdBlock->DataPacket[18] = FN_BYTE(g_pConfigData->nUsb1VID, 0);
            pReCmdBlock->DataPacket[19] = FN_BYTE(g_pConfigData->nUsb1VID, 1);

            pReCmdBlock->DataPacket[20] = FN_BYTE(g_pConfigData->nUsb1PID, 0);
            pReCmdBlock->DataPacket[21] = FN_BYTE(g_pConfigData->nUsb1PID, 1);

        }
        pReCmdBlock->DataPacket[22] = FN_BYTE(Firmware_Version, 0);
        pReCmdBlock->DataPacket[23] = FN_BYTE(Firmware_Version, 1);

        pReCmdBlock->nDataLen = 24;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case VER20_GET:
    {
        uint32_t nMasterFWCheckSum  = 0x0000;
        uint32_t nSlaveFWCheckSum   = 0x0000;

        uint16_t nMasterBootVersion = GetBootVersion();
        uint16_t nSlaveBootVersion  = 0x0000;

        uint32_t nMasterBootCRC     = GetBootDataCrc32();
        uint32_t nSlaveBootCRC      = 0x00;

        nSlaveFWCheckSum = 0;
        nSlaveFWCheckSum += pCmdBlock->DataPacket[8];
        nSlaveFWCheckSum += pCmdBlock->DataPacket[9]<<8;
        nSlaveFWCheckSum += pCmdBlock->DataPacket[10]<<16;
        nSlaveFWCheckSum += pCmdBlock->DataPacket[11]<<24;

        nSlaveBootVersion = pCmdBlock->DataPacket[12];
        nSlaveBootVersion += pCmdBlock->DataPacket[13]<<8;

        nSlaveBootCRC = pCmdBlock->DataPacket[14];
        nSlaveBootCRC += pCmdBlock->DataPacket[15]<<8;
        nSlaveBootCRC += pCmdBlock->DataPacket[16]<<16;
        nSlaveBootCRC += pCmdBlock->DataPacket[17]<<24;

        nMasterFWCheckSum = GetFwCheckSum();
        nMasterFWCheckSum += nSlaveFWCheckSum;
        nMasterFWCheckSum &= 0x0000FFFF;


        pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
        pReCmdBlock->DataPacket[1] = VERSION;
        pReCmdBlock->DataPacket[2] = VER20_GET_RE;
        pReCmdBlock->DataPacket[3] = 29;
        pReCmdBlock->DataPacket[4] = 1;
        MemSet(&pReCmdBlock->DataPacket[5], 0xFF, 50);

        //主机信息
        *(uint32_t *)(&pReCmdBlock->DataPacket[5]) = nMasterFWCheckSum;            //APP的checksum
        *(uint16_t *)(&pReCmdBlock->DataPacket[9]) = (uint16_t)Firmware_Version;   //APP的固件版本号
        *(uint16_t *)(&pReCmdBlock->DataPacket[11]) = nMasterBootVersion;          //BootLoader的固件版本号
        pReCmdBlock->DataPacket[13] = BOARD_DB;                                    //硬件系列识别号
        *(uint32_t *)(&pReCmdBlock->DataPacket[14]) = nMasterBootCRC;              //BootLoader的checksum

        pReCmdBlock->DataPacket[18] = (USB_VID % 0x100);
        pReCmdBlock->DataPacket[19] = (USB_VID / 0x100);
        pReCmdBlock->DataPacket[20] = (USB_PID % 0x100);
        pReCmdBlock->DataPacket[21] = (USB_PID / 0x100);

        pReCmdBlock->nDataLen = 35;

        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    // 获取触摸框型号
    case VER_BOARD_TYPE_GET:
    {
        pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
        pReCmdBlock->DataPacket[1] = VERSION;
        pReCmdBlock->DataPacket[2] = VER_BOARD_TYPE_GET_RE;
        pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];

        MemCpy(&pReCmdBlock->DataPacket[5], g_pConfigData->strDeviceDesc, FN_MIN(CMD_QUEUE_DATA_MAX_SIZE - 5, g_pConfigData->nDeviceDescLen));
        MemCpy(&pReCmdBlock->DataPacket[5 + FN_MIN(CMD_QUEUE_DATA_MAX_SIZE - 5, g_pConfigData->nDeviceDescLen)], g_pConfigData->strModelDesc, FN_MIN(CMD_QUEUE_DATA_MAX_SIZE - 5 - FN_MIN(CMD_QUEUE_DATA_MAX_SIZE - 5, g_pConfigData->nDeviceDescLen), g_pConfigData->nModelDescLen));
        pReCmdBlock->DataPacket[3] = FN_MIN(CMD_QUEUE_DATA_MAX_SIZE - 5, g_pConfigData->nDeviceDescLen + g_pConfigData->nModelDescLen);

        pReCmdBlock->nDataLen = pReCmdBlock->DataPacket[3] + 5;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;
    //SR整机获取触摸框唯一 KEY 值
    case GET_OTA_KEY:
    {
        const uint8_t arrKeyVal[] = "37ac70edc805ad7ec0caebab4c5d7a842fa1f4d8";

        pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
        pReCmdBlock->DataPacket[1] = VERSION;
        pReCmdBlock->DataPacket[2] = GET_OTA_KEY_RE;
        pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];

        MemCpy(&pReCmdBlock->DataPacket[5], arrKeyVal, FN_MIN(CMD_QUEUE_DATA_MAX_SIZE - 5, sizeof(arrKeyVal)));
        pReCmdBlock->DataPacket[3] = FN_MIN(CMD_QUEUE_DATA_MAX_SIZE - 5, sizeof(arrKeyVal)-1);
        pReCmdBlock->nDataLen = pReCmdBlock->DataPacket[3] + 5;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;
    case VER_DRIVE_GET:
    {
        MemCpy(pCmdBlock->DataPacket,pReCmdBlock->DataPacket,pCmdBlock->nDataLen);
        eRetVal  = CMD_ANSWER_DATA ;
    }
    break;

    case VER_DRIVE_GET_RE:
    {
        MemCpy(pCmdBlock->DataPacket,pReCmdBlock->DataPacket,pCmdBlock->nDataLen);
        eRetVal  = CMD_ANSWER_DATA ;
    }
    break;

    case 0x04:
    {
        eRetVal = CMD_ANSWER_NONE;
    }
    break;

    case VER_EEPROM_CHECK_GET:
    {
        #if 0
        pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
        pReCmdBlock->DataPacket[1] = VERSION;
        pReCmdBlock->DataPacket[2] = VER_EEPROM_CHECK_GET_RE;
        pReCmdBlock->DataPacket[4] = (int8_t)CheckEepromCrc2Flash();
        MemSet(&pReCmdBlock->DataPacket[5], 0xFF, CMD_QUEUE_DATA_MAX_SIZE-5);

        eRetVal  = CMD_ANSWER_DATA ;
        #else
        eRetVal  = CMD_ANSWER_UNKNOWN;
        #endif
    }
    break;

    default:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    break;
    }
    return eRetVal;
}



/*****************************************************************
Function:         CmdMaster
Description:      处理主机的相关指令
Parameter:        pCmdBlock: 通讯输入的内容
                  pReCmdBlock：通讯输出的内容
Return:           应答方式
*****************************************************************/
CmdAnswerType CmdMaster(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;

    switch (pCmdBlock->DataPacket[2])
    {
    case SLAVE_SET_SEND_DATA:   //强制发送ADC数据
    {
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = SLAVE;
        pReCmdBlock->DataPacket[2] = SLAVE_SET_SEND_DATA_RE;
        pReCmdBlock->DataPacket[3] = 3;

//        pReCmdBlock->DataPacket[5] = g_bSendTouchData;
//        pReCmdBlock->DataPacket[6] = g_eTouchData;
//        pReCmdBlock->DataPacket[8] = g_bSyncMode;
//
//        pReCmdBlock->nDataLen = 8;
//        AnswerCommand(pReCmdBlock->DataPacket, pReCmdBlock->nDataLen, pCmdBlock->eCmdSource);
//
//        //兼容免驱采集数据工具下发的指令
//        if (GetCRC32(pCmdBlock->DataPacket, 60) == FN_UINT32(&pCmdBlock->DataPacket[60]))
//        {
//            g_bSendTouchData = pCmdBlock->DataPacket[5];
//            g_eTouchData = pCmdBlock->DataPacket[6];
//            g_bSyncMode = pCmdBlock->DataPacket[8];
//
//            g_eUsbDevice = GetUsbDev(pCmdBlock->eCmdSource);
//            g_eUsbSendCfgType = (eUsbCfgBitType_t)pCmdBlock->DataPacket[9];
//            g_nEvalReportId = pCmdBlock->DataPacket[10];
//            if (pCmdBlock->DataPacket[11])
//            {
//                ScanAndOrgInit();
//            }
//        }
//        else
//        {
//            if(g_bSendTouchData != pCmdBlock->DataPacket[5] || g_eTouchData != pCmdBlock->DataPacket[6] || g_bSyncMode != pCmdBlock->DataPacket[8])
//            {
//                g_bSendTouchData = pCmdBlock->DataPacket[5];
//                g_eTouchData = pCmdBlock->DataPacket[6];
//                g_bSyncMode = pCmdBlock->DataPacket[8];
//                ScanAndOrgInit();
//            }
//        }

        /******** Modified by Roc ********/
        //printf("CMDMaster\r\n");
        //printf(" g_bSendTouchData:%x\r\n g_bSyncMode:%x\r\n",g_bSendTouchData,g_bSyncMode);
        /*********************************/
        eRetVal = CMD_ANSWER_NONE;
    }
    break;

    case SLAVE_UPGRADE:     //从机接收到升级命令
    {
        eRetVal = CMD_ANSWER_NONE;
    }
    break;

    case SLAVE_ADC_TEST:    //从机接收到
    {
//        int32_t nTestResult = 0;
//        if(pCmdBlock->DataPacket[FN_MIN(CMD_QUEUE_DATA_MAX_SIZE, 63)] == (uint8_t)GetCheckSum(&pCmdBlock->DataPacket[5], FN_MIN(pCmdBlock->DataPacket[3], CMD_QUEUE_DATA_MAX_SIZE - 5)))
//        {
//            //Byte5、6-T_B TEST_MAX_ADC, Byte7、8-T_B TEST_MIN_ADC
//            //Byte9、10-T_M TEST_MAX_ADC, Byte11、12-T_M TEST_MIN_ADC
//            g_nTEST_MAX_ADC = FN_UINT16(&pCmdBlock->DataPacket[9]);
//            g_nTEST_MIN_ADC = FN_UINT16(&pCmdBlock->DataPacket[11]);
//            //Byte13、14-T_F TEST_MAX_ADC, Byte15、16-T_F TEST_MIN_ADC
//
//            //Byte17、18-TEST_MAX_UNT_ADC
//            g_nTEST_MAX_UNT_ADC = FN_UINT16(&pCmdBlock->DataPacket[17]);
//
//            //Byte19、20-TEST_MAX_REVERSE_ADC, Byte21、22-TEST_MIN_REVERSE_ADC
//            g_nTEST_MAX_REVERSE_ADC = FN_UINT16(&pCmdBlock->DataPacket[19]);
//
//            //Byte23、24-T_B TEST_MAX_AGC, Byte25、26-T_B TEST_MIN_AGC
//            //Byte27、28-T_M TEST_MAX_AGC, Byte29、30-T_M TEST_MIN_AGC
//            g_nTEST_MAX_AGC = FN_UINT16(&pCmdBlock->DataPacket[27]);
//            g_nTEST_MIN_AGC = FN_UINT16(&pCmdBlock->DataPacket[29]);
//            //Byte31、32-T_F TEST_MAX_AGC, Byte33、34-T_F TEST_MIN_AGC
//
//            nTestResult = HardTest();   //第一位为1代表AGC异常，第二位为1代表ADC和波动值异常，第三位为1代表反向值
//
//            pReCmdBlock->DataPacket[0] = 0xfc;
//            pReCmdBlock->DataPacket[1] = SLAVE;
//            pReCmdBlock->DataPacket[2] = SLAVE_ADC_TEST_RE;
//            pReCmdBlock->DataPacket[3] = 4;
//            pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];
//            pReCmdBlock->DataPacket[5] = FN_BYTE(nTestResult, 0);
//            pReCmdBlock->DataPacket[6] = FN_BYTE(nTestResult, 1);
//            pReCmdBlock->DataPacket[7] = FN_BYTE(nTestResult, 2);
//            pReCmdBlock->DataPacket[8] = FN_BYTE(nTestResult, 3);
//            pReCmdBlock->nDataLen = 9;
//
//            eRetVal = CMD_ANSWER_DATA;
//        }
//        else
//        {
//            eRetVal = CMD_ANSWER_FAIL;
//        }
    }
    break;

    case SLAVE_QRIY_STATE_RESET:
    {

    }
    break;

    default:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    break;
    }
    return eRetVal;
}



/*****************************************************************
 * @Function : 获取点数的命令
 * @Parameter: pCmdBlock: 通讯输入的内容
               pReCmdBlock：通讯输出的内容
 * @return   : 应答方式
*****************************************************************/
CmdAnswerType CmdGetMaxPointNum(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
    pReCmdBlock->DataPacket[1] = GET_MAXPOINTNUM;
    pReCmdBlock->DataPacket[2] = 0x02;
    pReCmdBlock->DataPacket[3] = 0x01;
    pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];
    pReCmdBlock->DataPacket[5] = g_pConfigData->nUsePoint;
    pReCmdBlock->nDataLen = 6;
    return CMD_ANSWER_DATA;
}


/*****************************************************************
 * @Function : 处理信号测试相关命令
 * @Parameter: pCmdBlock: 通讯输入的内容
               pReCmdBlock：通讯输出的内容
 * @return   : 应答方式
*****************************************************************/
CmdAnswerType CmdSignalTest(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;
    uint8_t  i = 0;

    switch (pCmdBlock->DataPacket[2])
    {
    case ANDROIDTEST_GETINFO:
    {
        uint8_t nDir  = 2 - pCmdBlock->DataPacket[5];
        uint8_t nAxis = 1 - pCmdBlock->DataPacket[6];

        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = 0x86;
        pReCmdBlock->DataPacket[3] = 0x04;
        pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];

        pReCmdBlock->DataPacket[5] = FN_BYTE(g_pConfigData->nLedStart[nAxis][nDir], 0);
        pReCmdBlock->DataPacket[6] = FN_BYTE(g_pConfigData->nLedStart[nAxis][nDir], 1);

        pReCmdBlock->DataPacket[7] = FN_BYTE((g_pConfigData->nLedEnd[nAxis][nDir] + 1), 0);
        pReCmdBlock->DataPacket[8] = FN_BYTE((g_pConfigData->nLedEnd[nAxis][nDir] + 1), 1);
        pReCmdBlock->DataPacket[9] = FN_BYTE(g_pConfigData->nTestMaxAdc[nDir] * 4, 0);
        pReCmdBlock->DataPacket[10] = FN_BYTE(g_pConfigData->nTestMaxAdc[nDir] * 4, 1);
        pReCmdBlock->DataPacket[11] = FN_BYTE(g_pConfigData->nTestMinAdc[nDir] * 4, 0);
        pReCmdBlock->DataPacket[12] = FN_BYTE(g_pConfigData->nTestMinAdc[nDir] * 4, 1);
        pReCmdBlock->DataPacket[14] = FN_BYTE(g_pConfigData->nTestMaxAdcFactory[nDir] * 4, 0);
        pReCmdBlock->DataPacket[15] = FN_BYTE(g_pConfigData->nTestMaxAdcFactory[nDir] * 4, 1);
        pReCmdBlock->DataPacket[16] = FN_BYTE(g_pConfigData->nTestMinAdcFactory[nDir] * 4, 0);
        pReCmdBlock->DataPacket[17] = FN_BYTE(g_pConfigData->nTestMinAdcFactory[nDir] * 4, 1);

        pReCmdBlock->DataPacket[13] = FN_BYTE(g_pConfigData->nTestMaxUndulateAdc * 4, 0);
        pReCmdBlock->DataPacket[18] = FN_BYTE(g_pConfigData->nTestMaxUndulateAdc * 4, 0);
        pReCmdBlock->DataPacket[19] = FN_BYTE(g_pConfigData->nTestMaxReverseAdc * 4, 0);
        pReCmdBlock->DataPacket[20] = FN_BYTE(g_pConfigData->nTestMaxReverseAdc * 4, 1);
        pReCmdBlock->DataPacket[21] = FN_BYTE(g_pConfigData->nTestMinReverseAdc * 4, 0);
        pReCmdBlock->DataPacket[22] = FN_BYTE(g_pConfigData->nTestMinReverseAdc * 4, 1);
        pReCmdBlock->DataPacket[23] = FN_BYTE(g_pConfigData->nTestMaxAgc, 0);
        pReCmdBlock->DataPacket[24] = FN_BYTE(g_pConfigData->nTestMaxAgc, 1);
        pReCmdBlock->DataPacket[25] = FN_BYTE(g_pConfigData->nTestMinAgc, 0);
        pReCmdBlock->DataPacket[26] = FN_BYTE(g_pConfigData->nTestMinAgc, 1);
        pReCmdBlock->DataPacket[27] = FN_BYTE(g_pConfigData->nTestMaxAgcFactory, 0);
        pReCmdBlock->DataPacket[28] = FN_BYTE(g_pConfigData->nTestMaxAgcFactory, 1);
        pReCmdBlock->DataPacket[29] = FN_BYTE(g_pConfigData->nTestMinAgcFactory, 0);
        pReCmdBlock->DataPacket[30] = FN_BYTE(g_pConfigData->nTestMinAgcFactory, 1);
        pReCmdBlock->nDataLen = 31;

        if(g_stSingalTest.dataStep != 0)
        {
            MemCpy(&g_stSingalTest.stSingalData[g_stSingalTest.dataStep].DataPacket[0], &pReCmdBlock->DataPacket[0], pReCmdBlock->nDataLen);
            g_stSingalTest.stSingalData[g_stSingalTest.dataStep].nDataLen = pReCmdBlock->nDataLen;
            g_stSingalTest.stSingalData[g_stSingalTest.dataStep].eCmdSource = pReCmdBlock->eCmdSource;
            g_stSingalTest.dataStep ++;
        }

        if((nDir==2) && (nAxis == 1) && (g_stSingalTest.dataStep == 7))
        {
            for(i=0; i<g_stSingalTest.dataStep; i++)
            {
                AnswerCommand(g_stSingalTest.stSingalData[i].DataPacket, g_stSingalTest.stSingalData[i].nDataLen, g_stSingalTest.stSingalData[i].eCmdSource);
                //printf("singal test send response\r\n");
            }
            g_stSingalTest.dataStep = 0;

            //updata g_stTestStatus
//            g_stTestStatus.eStatus = eTestStart;
//            g_stTestStatus.eCmdSource = pReCmdBlock->eCmdSource;
//            g_stTestStatus.nStartTime = GetSystickTime();
        }
    }
    break;
    case TOUCH_BOX_CONTRO_PENETRATION:
    {
//        g_bTouchPenetration = 0;
    }
    break;
    case MCU_TOUCH_CONTRO_PENETRATION:
    {
//        g_bTouchPenetration = 1;
    }
    break;
    default:
    {
        eRetVal = CMD_ANSWER_NONE;
    }
    break;
    }

    return eRetVal;
}


ATTR_WEAK CmdAnswerType HwTonSet(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
//    if (1 == pCmdBlock->DataPacket[5])
//    {
//        ENABLE_T_On();    // 开启发射
//        g_bRunAlg = 1;    // 开启免驱算法
//    }
//    else
//    {
//        DISABLE_T_On();   // 关闭发射
//        g_bRunAlg = 0;    // 关闭免驱算法
//    }
//    ScanOnce();
    return CMD_ANSWER_OK;    
}

/*****************************************************************
 * @Function : 处理硬件的相关命令
 * @Parameter: pCmdBlock: 通讯输入的内容
               pReCmdBlock：通讯输出的内容
 * @return   : 应答方式
*****************************************************************/
CmdAnswerType CmdHardWare(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;

    switch (pCmdBlock->DataPacket[2])
    {
    case HW_RESET:
    {
        pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
        pReCmdBlock->DataPacket[1] = CMD_ANSWER;
        pReCmdBlock->DataPacket[2] = CMD_ANSWER_OK;
        pReCmdBlock->DataPacket[3] = 2;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->DataPacket[5] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[6] = pCmdBlock->DataPacket[2];
        pReCmdBlock->nDataLen = 7;

        AnswerCommand(pReCmdBlock->DataPacket, pReCmdBlock->nDataLen, pCmdBlock->eCmdSource);

        StopAllUsbDev();
        printf("\r\nhardware SystemReset \r\n");
        Delay_ms(700);
        SoftwareRest();
    }
    break;

    case HW_SCAN:
    {
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = HW;
        pReCmdBlock->DataPacket[2] = HW_SCAN_RE;
        pReCmdBlock->DataPacket[3] = 1;
        pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];
        pReCmdBlock->nDataLen = 5;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case HW_AGEING:
    {
        //printf("Aging Status:%d----->\r\n", g_stBurnAging.eMode);
//        g_stBurnAging.eCmdState = (pCmdBlock->DataPacket[5] == eAgingEnable) ? eAgingEnable : eAgingDisable;  //接入老化工具，使能老化模式
        eRetVal = CMD_ANSWER_OK;
        //printf("Aging Status:%d\r\n", g_stBurnAging.eMode);
    }
    break;

    /*LED进入老化的状态显示灯*/
    case HW_LED:
    {
//        if(pCmdBlock->DataPacket[5]==0)
//        {
//            LedOff();
//            g_bBoradLedEnable = 0;
//        }
//        else if(pCmdBlock->DataPacket[5]==1)
//        {
//            LedOn();
//            g_bBoradLedEnable = 0;
//        }
//        else
//        {
//            g_bBoradLedEnable = 1;
//        }
        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case HW_AGC_SET:
    {
//        if(pCmdBlock->DataPacket[5] == 1)
//        {
//
//        }
//        ScanOnce();
        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case HW_EDGE_GET:
    {
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = HW;
        pReCmdBlock->DataPacket[2] = HW_EDGE_GET_RE;
        pReCmdBlock->DataPacket[3] = 5;
        pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];

        pReCmdBlock->nDataLen = 10;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;
    //反向值测试
    case HW_TON_SET:
    {
        eRetVal = HwTonSet(pCmdBlock, pReCmdBlock);
    }
    break;

    case HW_TON_GET:    // 获取发射管发射状态
    {
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = HW;
        pReCmdBlock->DataPacket[2] = HW_TON_GET_RE;
        pReCmdBlock->DataPacket[3] = 1;    // 有效字节
        pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];
        pReCmdBlock->nDataLen = 6;

//        // 获取当前状态值
//        if(GET_T_STATUS())
//        {
//            pReCmdBlock->DataPacket[5] = 1; // 发射状态
//        }
//        else
//        {
//             pReCmdBlock->DataPacket[5] = 0; // 不发射状态
//        }
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case HW_F_MODE:
    {
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = HW;
        pReCmdBlock->DataPacket[2] = HW_F_MODE_RE;
        pReCmdBlock->DataPacket[3] = 1;
        pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];
        
#ifdef N32G457_ENV
        stFlashPersistentData_t  stFlashPersistentData;

        GetPersistentData(&stFlashPersistentData);
        if ((*(uint32_t *)(stFlashPersistentData.stFlashParaofOtherFunction.arrKey + 0) == 0xFFFFFFFF) && (*(uint32_t *)(stFlashPersistentData.stFlashParaofOtherFunction.arrKey + 4) == 0xFFFFFFFF)
            && (*(uint32_t *)(stFlashPersistentData.stFlashParaofOtherFunction.arrKey + 8) == 0xFFFFFFFF) && (*(uint32_t *)(stFlashPersistentData.stFlashParaofOtherFunction.arrKey + 12) == 0xFFFFFFFF))
        {
            pReCmdBlock->DataPacket[5] = 1;
        }
        else
        {
             pReCmdBlock->DataPacket[5] = 0;
        }
#else
        stStartBootPara_t stPara;
        GetBootPara(&stPara);
        if ((*(uint32_t *)(stPara.stBootFlashPara.nKey+ 0) == 0xFFFFFFFF) && (*(uint32_t *)(stPara.stBootFlashPara.nKey + 4) == 0xFFFFFFFF)
            && (*(uint32_t *)(stPara.stBootFlashPara.nKey + 8) == 0xFFFFFFFF) && (*(uint32_t *)(stPara.stBootFlashPara.nKey + 12) == 0xFFFFFFFF))
        {
            pReCmdBlock->DataPacket[5] = 1;
        }
        else
        {
             pReCmdBlock->DataPacket[5] = 0;
        }
#endif

        pReCmdBlock->nDataLen = 6;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case HW_SET_THRESHOLE:
    {
        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case HW_T_GET:
    {
        uint8_t nDir  = 2 - pCmdBlock->DataPacket[5];
        uint8_t nAxis = 1 - pCmdBlock->DataPacket[6];
        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = HW;
        pReCmdBlock->DataPacket[2] = HW_T_GET_RE;
        pReCmdBlock->DataPacket[3] = 0x04;
        pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];

        pReCmdBlock->DataPacket[5] = FN_BYTE(g_pConfigData->nLedStart[nAxis][nDir], 0);
        pReCmdBlock->DataPacket[6] = FN_BYTE(g_pConfigData->nLedStart[nAxis][nDir], 1);

        pReCmdBlock->DataPacket[7] = FN_BYTE((g_pConfigData->nLedEnd[nAxis][nDir] + 1), 0);
        pReCmdBlock->DataPacket[8] = FN_BYTE((g_pConfigData->nLedEnd[nAxis][nDir] + 1), 1);
        pReCmdBlock->DataPacket[9] = FN_BYTE(g_pConfigData->nTestMaxAdc[nDir] * 4, 0);
        pReCmdBlock->DataPacket[10] = FN_BYTE(g_pConfigData->nTestMaxAdc[nDir] * 4, 1);
        pReCmdBlock->DataPacket[11] = FN_BYTE(g_pConfigData->nTestMinAdc[nDir] * 4, 0);
        pReCmdBlock->DataPacket[12] = FN_BYTE(g_pConfigData->nTestMinAdc[nDir] * 4, 1);
        pReCmdBlock->DataPacket[14] = FN_BYTE(g_pConfigData->nTestMaxAdcFactory[nDir] * 4, 0);
        pReCmdBlock->DataPacket[15] = FN_BYTE(g_pConfigData->nTestMaxAdcFactory[nDir] * 4, 1);
        pReCmdBlock->DataPacket[16] = FN_BYTE(g_pConfigData->nTestMinAdcFactory[nDir] * 4, 0);
        pReCmdBlock->DataPacket[17] = FN_BYTE(g_pConfigData->nTestMinAdcFactory[nDir] * 4, 1);

        pReCmdBlock->DataPacket[13] = FN_BYTE(g_pConfigData->nTestMaxUndulateAdc * 4, 0);
        pReCmdBlock->DataPacket[18] = FN_BYTE(g_pConfigData->nTestMaxUndulateAdc * 4, 0);
        pReCmdBlock->DataPacket[19] = FN_BYTE(g_pConfigData->nTestMaxReverseAdc * 4, 0);
        pReCmdBlock->DataPacket[20] = FN_BYTE(g_pConfigData->nTestMaxReverseAdc * 4, 1);
        pReCmdBlock->DataPacket[21] = FN_BYTE(g_pConfigData->nTestMinReverseAdc * 4, 0);
        pReCmdBlock->DataPacket[22] = FN_BYTE(g_pConfigData->nTestMinReverseAdc * 4, 1);
        pReCmdBlock->DataPacket[23] = FN_BYTE(g_pConfigData->nTestMaxAgc, 0);
        pReCmdBlock->DataPacket[24] = FN_BYTE(g_pConfigData->nTestMaxAgc, 1);
        pReCmdBlock->DataPacket[25] = FN_BYTE(g_pConfigData->nTestMinAgc, 0);
        pReCmdBlock->DataPacket[26] = FN_BYTE(g_pConfigData->nTestMinAgc, 1);
        pReCmdBlock->DataPacket[27] = FN_BYTE(g_pConfigData->nTestMaxAgcFactory, 0);
        pReCmdBlock->DataPacket[28] = FN_BYTE(g_pConfigData->nTestMaxAgcFactory, 1);
        pReCmdBlock->DataPacket[29] = FN_BYTE(g_pConfigData->nTestMinAgcFactory, 0);
        pReCmdBlock->DataPacket[30] = FN_BYTE(g_pConfigData->nTestMinAgcFactory, 1);
        pReCmdBlock->nDataLen = 31;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case HW_TESTING:
    {
        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case HW_CHECKSIGNAL: //在进度条传送数据至PC前 先由固件自己验证
    {
        uint8_t bResult = 0;

        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = HW;
        pReCmdBlock->DataPacket[2] = HW_CHECKSIGNAL_RE;
        pReCmdBlock->DataPacket[3] = 0x02;
        pReCmdBlock->DataPacket[4] = 0x00;

        pReCmdBlock->DataPacket[5] = pCmdBlock->DataPacket[5];
        pReCmdBlock->DataPacket[6] = bResult;

        pReCmdBlock->nDataLen = 7;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case HW_LOCKAGC_SET:
    {
    }
    break;
    case HW_LOCKAGC_GET:
    {
    }
    break;
    case HW_LOCKAGC_RESET:
    {
    }
    break;

    case HW_ADC_INVERSE:
    {
    }
    break;

    case HW_ADC_INVERSE_V2:
    {
//#if (defined ATF403A_ENV || defined GD415_ENV)
//        uint32_t arrInverseVal[R_ADC_GROUP];
//
//        InitHwInverseVal(arrInverseVal);         //初始化硬件反向值
//        FeedWatchdog();
//
//        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
//        pReCmdBlock->DataPacket[1] = HW;
//        pReCmdBlock->DataPacket[2] = HW_ADC_INVERSE_V2_RE;
//        pReCmdBlock->DataPacket[3] = R_ADC_GROUP;
//
//        for (int32_t k = 0; k < R_ADC_GROUP; k++)
//        {
//            //从pReCmdBlock->DataPacket[4]开始存储各个通道的反向值
//            pReCmdBlock->DataPacket[4 + k] = (uint8_t)arrInverseVal[k];
//        }
//
//        pReCmdBlock->nDataLen = 64;
//
//        AnswerCommand(pReCmdBlock->DataPacket, pReCmdBlock->nDataLen, pCmdBlock->eCmdSource);
//        //测试完成后恢复原始信号
//        ADC_InitConfig();
//        FeedWatchdog();
//
//        ScanOnce();
//#else
//#error "No HW_ADC_INVERSE"
//#endif
    }
    break;
    case HW_TEST_CHANNEL:
    {
//        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
//        pReCmdBlock->DataPacket[1] = HW;
//        pReCmdBlock->DataPacket[2] = HW_TEST_CHANNEL_RE;
//        pReCmdBlock->DataPacket[3] = TestChannel();              //检查是否通道差异格外异常;
//        pReCmdBlock->nDataLen = 4;

        eRetVal = CMD_ANSWER_DATA;
    }
    break;
    
    case HW_GET_RAGC_MAX_GEAR:
    {
//        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
//        pReCmdBlock->DataPacket[1] = HW;
//        pReCmdBlock->DataPacket[2] = HW_GET_RAGC_MAX_GEAR_RE;
//        pReCmdBlock->DataPacket[3] = FN_BYTE(g_nRAgcMax + 1, 0);
//        pReCmdBlock->DataPacket[4] = FN_BYTE(g_nRAgcMax + 1, 1);
//
//        pReCmdBlock->nDataLen = 5;

        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case HW_GET_RAGC_MAPPING:
    {
//        uint16_t nStartIdx = pCmdBlock->DataPacket[3] + (pCmdBlock->DataPacket[4] << 8);
//        uint16_t nCnt = pCmdBlock->DataPacket[5];
//        nCnt = FN_MIN(nCnt, (64 - 3) / 2);
//
//        for (uint32_t i = 0, j = 0; i < nCnt; i++, j += 2)
//        {
//            uint8_t nVal = g_arrGearRAgc[FN_MIN(g_nRAgcMax, nStartIdx + i)];
//
//            pReCmdBlock->DataPacket[3 + j + 0] = FN_BYTE(nVal, 0);
//            pReCmdBlock->DataPacket[3 + j + 1] = 0;
//        }
//
//        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
//        pReCmdBlock->DataPacket[1] = HW;
//        pReCmdBlock->DataPacket[2] = HW_GET_RAGC_MAPPING_RE;
//
//        pReCmdBlock->nDataLen = 64;

        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    default:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    break;
    }

    return eRetVal;
}

/*****************************************************************
 * @Function : 固件升级复位与升级完成后设置升级标志位
 * @Parameter: pCmdBlock: 通讯输入的内容
               pReCmdBlock：通讯输出的内容
 * @return   :应答方式
 *****************************************************************/
CmdAnswerType CmdBootloader(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType  eRetVal = CMD_ANSWER_NONE;

    switch (pCmdBlock->DataPacket[2])
    {
    case BL_UPD_FLAG_SET:
    {
        eAppUpgradeFlag_t UpgradeFlag = GetUpgradeFlag();
//        printf("before upgrade flage %02X\r\n", UpgradeFlag);
        // 刚升级完成
        if (UpgradeFlag != eAppRunMode)
        {
            UpgradeFlag = eAppRunMode;
            SetUpgradeFlag(UpgradeFlag);
            Delay_ms(10);
            pReCmdBlock->DataPacket[0] = 0xfc;
            pReCmdBlock->DataPacket[1] = CMD_ANSWER;
            pReCmdBlock->DataPacket[2] = BL_UPD_FLAG_SET_RE;
            pReCmdBlock->DataPacket[3] = 2;
            pReCmdBlock->DataPacket[4] = 0x00;
            pReCmdBlock->DataPacket[5] = BL_UPD_FLAG_SET_OK;
            pReCmdBlock->DataPacket[6] = UpgradeFlag;
            eRetVal = CMD_ANSWER_DATA;
//            printf("finish upgrade. %02X\r\n", UpgradeFlag);
        }
        else
        {
            pReCmdBlock->DataPacket[0] = 0xfc;
            pReCmdBlock->DataPacket[1] = CMD_ANSWER;
            pReCmdBlock->DataPacket[2] = BL_UPD_FLAG_SET_RE;
            pReCmdBlock->DataPacket[3] = 2;
            pReCmdBlock->DataPacket[4] = 0x00;
            pReCmdBlock->DataPacket[5] = BL_UPD_FLAG_SET_NO_NEED;
            pReCmdBlock->DataPacket[6] = UpgradeFlag;
            eRetVal = CMD_ANSWER_DATA;
        }
        pReCmdBlock->nDataLen = 7;
    }
    break;

    case BL_UPD_FLAG_GET:
    {
         pReCmdBlock->DataPacket[0] = 0xfc;
         pReCmdBlock->DataPacket[1] = CMD_ANSWER;
         pReCmdBlock->DataPacket[2] = BL_UPD_FLAG_GET_RE;
         pReCmdBlock->DataPacket[3] = 2;
         pReCmdBlock->DataPacket[4] = 0x00;
         pReCmdBlock->DataPacket[5] = 0;
         pReCmdBlock->DataPacket[6] = GetUpgradeFlag();
         pReCmdBlock->nDataLen = 7;
         eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case BL_RUN_BL:
    {
        SoftwareRest();
    }
    break;

    case BL_UPD_START:
    {
        pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
        pReCmdBlock->DataPacket[1] = CMD_ANSWER;
        pReCmdBlock->DataPacket[2] = BL_UPD_START_RE;
        pReCmdBlock->DataPacket[3] = 2;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->DataPacket[5] = 0;
        pReCmdBlock->DataPacket[6] = GetUpgradeFlag();
        pReCmdBlock->nDataLen = 7;

        AnswerCommand(pReCmdBlock->DataPacket, pReCmdBlock->nDataLen, pCmdBlock->eCmdSource);

        printf("\r\nbootloader SystemReset\r\n");
        SetUpgradeFlag(eAppIAPMode);

        uint32_t nTick = GetCurrentTime();
        while (GetAllUsbQueueBusy() && (GetCurrentTime() - nTick) < 2000);
        StopAllUsbDev();

        printf("Reset!");
        Delay_ms(300);
        disable_global_irq(CSR_MSTATUS_MIE_MASK);        //关闭了中断，滴答定时器和喂狗中断都将会关闭
        SoftwareRest();
    }
    break;

    default:
        eRetVal = CMD_ANSWER_UNKNOWN;
        break;
    }

    return eRetVal;
}

/*****************************************************************
* @Function : 处理设备的相关配置信息
* @Parameter: [pCmdBlock] 通讯输入的内容
            : [pReCmdBlock]通讯输出的内容
* @return   :应答方式
*****************************************************************/
CmdAnswerType CmdDeviceConfig(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;

    switch(pCmdBlock->DataPacket[2])
    {
    case CONFIG_SET_DATA:
        {
            uint8_t nState = CONFIG_OK;
            uint16_t nPacketNo = 0;
            uint16_t nPacketLen = 0;
            static uint32_t s_nTotalLen = 0;
            static uint32_t s_nEachPacketLen = 0;
            static uint32_t s_nCrc = 0;
            static uint32_t s_nTotalPacketNum = 0;
            static uint32_t s_nType = 0;
            static uint8_t* s_pConfigBuf = NULL;

            nPacketLen = pCmdBlock->DataPacket[4];
            nPacketLen = (nPacketLen << 8) + pCmdBlock->DataPacket[3];

            if(pCmdBlock->DataPacket[5 + nPacketLen] == (uint8_t)(0xff & GetCheckSum(&pCmdBlock->DataPacket[5], nPacketLen)))
            {
                nPacketNo = pCmdBlock->DataPacket[6];
                nPacketNo = (nPacketNo << 8) + pCmdBlock->DataPacket[5];

                if(nPacketNo == 0)
                {
                    s_nType = pCmdBlock->DataPacket[10];
                    s_nType = (s_nType << 8) + pCmdBlock->DataPacket[9];
                    s_nType = (s_nType << 8) + pCmdBlock->DataPacket[8];
                    s_nType = (s_nType << 8) + pCmdBlock->DataPacket[7];

                    if(s_pConfigBuf != NULL)
                    {
                        //free(s_pConfigBuf);
                        s_pConfigBuf = NULL;
                    }

                    if(s_nType == 0)
                    {
//                        ClearFlashPara();
                        nState = CONFIG_FINISH;
                    }
                    else
                    {
                        s_nTotalLen = FN_UINT32(&pCmdBlock->DataPacket[11]);
                        s_nEachPacketLen = FN_UINT32(&pCmdBlock->DataPacket[15]);
                        s_nCrc = FN_UINT32(&pCmdBlock->DataPacket[19]);
                        s_nTotalPacketNum = FN_UINT32(&pCmdBlock->DataPacket[23]);
                        //s_pConfigBuf = malloc(s_nTotalLen);
                        if(s_pConfigBuf == NULL)
                        {
                            nState = CONFIG_OTHER_ERROR;
                        }
                    }
                }
                else if(nPacketNo < s_nTotalPacketNum && s_pConfigBuf != NULL)
                {
                    MemCpy(&s_pConfigBuf[s_nEachPacketLen * (nPacketNo - 1)], &pCmdBlock->DataPacket[7], nPacketLen - 2);
                    nState = CONFIG_OK;
                    if(nPacketNo == s_nTotalPacketNum - 1)
                    {
                        if(s_nCrc == GetCrc32_SW(s_pConfigBuf, s_nTotalLen))
                        {
                            nState = CONFIG_FINISH;
                            switch(s_nType)
                            {
                                case 1:
                                {
                                    //InitConfigData(s_pConfigBuf, s_nTotalLen, Host);
                                }
                                break;

                                default:
                                {
                                }
                                break;
                            }
                            //free(s_pConfigBuf);
                            s_pConfigBuf = NULL;
                        }
                        else
                        {
                            nState = CONFIG_OTHER_ERROR;
                        }
                    }
                }
                else
                {
                    nState = CONFIG_PACKETNO_ERROR;
                }
                eRetVal = CMD_ANSWER_DATA;
            }
            else
            {
                nState = CONFIG_CHECK_ERROR;
            }

            nPacketNo += 1;
            pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
            pReCmdBlock->DataPacket[1] = CMD_CONFIG;
            pReCmdBlock->DataPacket[2] = CONFIG_SET_DATA_RE;
            pReCmdBlock->DataPacket[3] = FN_BYTE(3, 0);
            pReCmdBlock->DataPacket[4] = FN_BYTE(3, 1);
            pReCmdBlock->DataPacket[5] = nState;
            pReCmdBlock->DataPacket[6] = FN_BYTE(nPacketNo, 0);
            pReCmdBlock->DataPacket[7] = FN_BYTE(nPacketNo, 1);
            pReCmdBlock->DataPacket[8] = GetCheckSum(&pReCmdBlock->DataPacket[5], 3);
            pReCmdBlock->nDataLen = 9;
            eRetVal = CMD_ANSWER_DATA;
        }
    break;

    case CONFIG_GET_DATA:
    {
        static uint32_t s_nTotalPacketNum = 0;
        static uint32_t s_nEachPacketLen = 0;
        static uint32_t s_nTotalLen = 0;
        static uint32_t s_nType = 0;
        static uint32_t s_nCrc = 0;

        static uint8_t* s_pBuf = 0;

        uint8_t nState = CONFIG_OK;
        uint16_t nPacketLen = 0;
        uint16_t nPacketNo = 0;

        nPacketLen = pCmdBlock->DataPacket[4];
        nPacketLen = (nPacketLen << 8) + pCmdBlock->DataPacket[3];

        if(pCmdBlock->DataPacket[5 + nPacketLen] == (uint8_t)(0xff & GetCheckSum(&pCmdBlock->DataPacket[5], nPacketLen)))
        {
            nPacketNo = FN_UINT16(&pCmdBlock->DataPacket[5]);

            if(nPacketNo == 0)
            {
                s_nType = FN_UINT32(&pCmdBlock->DataPacket[7]);

                switch(s_nType)
                {
                case 1:
                {
//                    nState = CONFIG_OK;
//                    s_pBuf = (uint8_t *)GetAlgTableCfgData();
//                    s_nTotalLen = GetAlgTableCfgLen();
//                    if ((uint32_t)s_pBuf < FLASH_BASE_ADDRESS || (uint32_t)s_pBuf > FLASH_MAX_ADDRESS)
//                    {
//                        return eRetVal;
//                    }
//                    s_nCrc = GetCrc32_SW(s_pBuf, s_nTotalLen);
//
//                    s_nEachPacketLen = (USB_PACKET_MAX_SIZE - 9)>(sizeof(pReCmdBlock->DataPacket)-9)?(sizeof(pReCmdBlock->DataPacket)-9):(USB_PACKET_MAX_SIZE - 9);
//                    s_nTotalPacketNum = (s_nTotalLen + s_nEachPacketLen - 1) / s_nEachPacketLen;
//
//                    nPacketLen = 16;
//                    g_stScanCoreVar.nScanCount = 0;
                }
                break;

                default:
                {
                    nPacketLen = 0;
                    nState = CONFIG_OTHER_ERROR;
                }
                break;
                }
                pReCmdBlock->DataPacket[8] = FN_BYTE(s_nTotalLen, 0);
                pReCmdBlock->DataPacket[9] = FN_BYTE(s_nTotalLen, 1);
                pReCmdBlock->DataPacket[10] = FN_BYTE(s_nTotalLen, 2);
                pReCmdBlock->DataPacket[11] = FN_BYTE(s_nTotalLen, 3);

                pReCmdBlock->DataPacket[12] = FN_BYTE(s_nCrc, 0);
                pReCmdBlock->DataPacket[13] = FN_BYTE(s_nCrc, 1);
                pReCmdBlock->DataPacket[14] = FN_BYTE(s_nCrc, 2);
                pReCmdBlock->DataPacket[15] = FN_BYTE(s_nCrc, 3);

                pReCmdBlock->DataPacket[16] = FN_BYTE(s_nEachPacketLen, 0);
                pReCmdBlock->DataPacket[17] = FN_BYTE(s_nEachPacketLen, 1);
                pReCmdBlock->DataPacket[18] = FN_BYTE(s_nEachPacketLen, 2);
                pReCmdBlock->DataPacket[19] = FN_BYTE(s_nEachPacketLen, 3);

                pReCmdBlock->DataPacket[20] = FN_BYTE(s_nTotalPacketNum, 0);
                pReCmdBlock->DataPacket[21] = FN_BYTE(s_nTotalPacketNum, 1);
                pReCmdBlock->DataPacket[22] = FN_BYTE(s_nTotalPacketNum, 2);
                pReCmdBlock->DataPacket[23] = FN_BYTE(s_nTotalPacketNum, 3);
            }
            else
            {
//                if(nPacketNo == s_nTotalPacketNum)
//                {
//                    nPacketLen = (s_nTotalLen - s_nEachPacketLen * (nPacketNo - 1));
//                    nState = CONFIG_FINISH;
//                    g_stScanCoreVar.nScanCount = 1;
//                }
//                else
//                {
//                    nPacketLen = s_nEachPacketLen;
//                    nState = CONFIG_OK;
//                }
//                MemCpy(&pReCmdBlock->DataPacket[8], (uint8_t*)s_pBuf + s_nEachPacketLen * (nPacketNo - 1), nPacketLen);
            }
        }
         else
        {
            nPacketLen = 0;
            nState = CONFIG_CHECK_ERROR;
        }
        nPacketLen += 3;
        pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
        pReCmdBlock->DataPacket[1] = CMD_CONFIG;
        pReCmdBlock->DataPacket[2] = CONFIG_GET_DATA_RE;
        pReCmdBlock->DataPacket[3] = FN_BYTE(nPacketLen, 0);
        pReCmdBlock->DataPacket[4] = FN_BYTE(nPacketLen, 1);
        pReCmdBlock->DataPacket[5] = nState;
        pReCmdBlock->DataPacket[6] = FN_BYTE(nPacketNo, 0);
        pReCmdBlock->DataPacket[7] = FN_BYTE(nPacketNo, 1);
        pReCmdBlock->DataPacket[5 + nPacketLen] = GetCheckSum(&pReCmdBlock->DataPacket[5], nPacketLen);
        pReCmdBlock->nDataLen = nPacketLen + 6;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case CONFIG_SET_QRCODE:
    {
        uint8_t nState = CONFIG_OK;
        uint8_t qrcode_data[64]= {0};
        uint8_t checksum = 0;
        uint8_t qrcode_len = pCmdBlock->DataPacket[3];

        qrcode_data[0] = qrcode_len;
        MemCpy(&qrcode_data[1], &pCmdBlock->DataPacket[4], qrcode_len);

        for(int i = 0;i<qrcode_len + 1;i++)
        {
            checksum += qrcode_data[i];
        }
        //printf(" %02X\n", checksum);
        if(checksum != pCmdBlock->DataPacket[4 + qrcode_len])
        {
            nState = CONFIG_CHECK_ERROR;
        }
        else
        {
//            qrcode_data[1 + qrcode_len] = checksum;
//            // 保存二维码信息到flash
//            FlashSaveQrCode(qrcode_data);
//            //printf("save QR code [%02X %02X %02X %02X %02X %02X %02X %02X]\n", qrcode_data[0],qrcode_data[1],qrcode_data[2],qrcode_data[3],qrcode_data[4],qrcode_data[5],qrcode_data[6],qrcode_data[7]);
        }

        pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
        pReCmdBlock->DataPacket[1] = CMD_CONFIG;
        pReCmdBlock->DataPacket[2] = CONFIG_SET_QRCODE_RE;
        pReCmdBlock->DataPacket[3] = nState;

        pReCmdBlock->nDataLen = 4;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case CONFIG_GET_QRCODE:
    {
        uint8_t nState = CONFIG_OK;
        uint8_t qrcode_data[64];
        uint8_t checksum = 0;
        uint8_t qrcode_len = 0;
        if(0x55 == pCmdBlock->DataPacket[3] && 0xAA == pCmdBlock->DataPacket[4] && 0xFF == pCmdBlock->DataPacket[5])
        {
//            qrcode_len = FlashGetQrCode(qrcode_data);
//            //printf("get QR code [%02X %02X %02X %02X %02X %02X %02X %02X]\n", qrcode_data[0],qrcode_data[1],qrcode_data[2],qrcode_data[3],qrcode_data[4],qrcode_data[5],qrcode_data[6],qrcode_data[7]);
//
//            if(qrcode_len > 59)
//            {
//                qrcode_len = 0;
//                nState = CONFIG_CHECK_ERROR;
//            }
//            else
//            {
//                for(int i = 0;i< 1+ qrcode_len;i++)
//                {
//                    checksum += qrcode_data[i];
//                }
//                if(checksum != qrcode_data[1 + qrcode_len])
//                {
//                    nState = CONFIG_CHECK_ERROR;
//                }
//            }

            //printf("state %d; qrcode len %d\n", nState, qrcode_len);
            pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
            pReCmdBlock->DataPacket[1] = CMD_CONFIG;
            pReCmdBlock->DataPacket[2] = CONFIG_GET_QRCODE_RE;
            pReCmdBlock->DataPacket[3] = nState;

            MemCpy(&pReCmdBlock->DataPacket[4], qrcode_data, qrcode_len+2);
            pReCmdBlock->nDataLen = qrcode_len + 6;
            eRetVal = CMD_ANSWER_DATA;
        }
    }
    break;

    default:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    }
    return eRetVal;
}

/*****************************************************************
 * @Function : 处理测试等相关的信号命令
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 应答方式
*****************************************************************/
#define CMD_PACKET_TEST_DATA_LEN    (uint8_t)(55)       //一包USB数据中包含有效测试数据最大长度
#define CMD_PACKET_INFO_DATA_LEN    (uint8_t)(9)        //一包USB数据中包含有效信息数据总长度
CmdAnswerType CmdData(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    uint32_t i = 0;
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;
    static uint8_t s_nCmdPackTestDataMaxLen = 0;    //USB数据中包含有效测试数据最大长度

    switch(pCmdBlock->DataPacket[2])
    {
    case DATA_GET_TOTAL:
    {
        //printf("Data_Total\r\n");
        pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
        pReCmdBlock->DataPacket[1] = CMD_DATA;
        pReCmdBlock->DataPacket[2] = DATA_GET_TOTAL_RE;
        pReCmdBlock->DataPacket[3] = 57;
        pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];
        pReCmdBlock->DataPacket[5] = 24;
        pReCmdBlock->nDataLen = 64;

//        for(i = 0; i < 12; i++)
//        {
//            pReCmdBlock->DataPacket[6 + 4 * i] = FN_BYTE(g_pConfigData->nTotal[X], 0);
//            pReCmdBlock->DataPacket[7 + 4 * i] = FN_BYTE(g_pConfigData->nTotal[X], 1);
//            pReCmdBlock->DataPacket[8 + 4 * i] = FN_BYTE(g_pConfigData->nTotal[Y], 0);
//            pReCmdBlock->DataPacket[9 + 4 * i] = FN_BYTE(g_pConfigData->nTotal[Y], 1);
//        }

        pReCmdBlock->DataPacket[6 + 4 * i] = 0;
        pReCmdBlock->DataPacket[7 + 4 * i] = 0;
        pReCmdBlock->DataPacket[8 + 4 * i] = 0;
        pReCmdBlock->DataPacket[9 + 4 * i] = 0;

        i++;
        pReCmdBlock->DataPacket[6 + 4 * i] = 0;
        pReCmdBlock->DataPacket[7 + 4 * i] = 0;
        pReCmdBlock->DataPacket[8 + 4 * i] = 0;
        pReCmdBlock->DataPacket[9 + 4 * i] = 0;

        i++;
        pReCmdBlock->DataPacket[6 + 4 * i] = 0;
        pReCmdBlock->DataPacket[7 + 4 * i] = 0;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case DATA_GET_BUF:
    {
        uint32_t nDir          = 2 - pCmdBlock->DataPacket[5] / 2 % 3 * (OBQ_COUNT / 2);  //根据结构体(NEW_TEST_BUF)区分左右正斜扫
        uint32_t nAxis         = pCmdBlock->DataPacket[5] % 2;                            //根据结构体(NEW_TEST_BUF)区分轴
        uint32_t nCmdGetOffset = FN_UINT16(&pCmdBlock->DataPacket[6]);                    //偏移量
        uint32_t nCmdGetLen    = FN_UINT16(&pCmdBlock->DataPacket[8]);                    //数据长度
        uint32_t nLedCnt       = 0;                                                       //nCmdGetOffset+nLedCnt(遍历灯的位置)

        pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = DATA_GET_BUF_RE;
        pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];
        pReCmdBlock->DataPacket[5] = pCmdBlock->DataPacket[5];
        pReCmdBlock->DataPacket[6] = pCmdBlock->DataPacket[6];
        pReCmdBlock->DataPacket[7] = pCmdBlock->DataPacket[7];

//        if ((pCmdBlock->DataPacket[5] < TEST_BUF_END))
//        {
//            //根据数据类型得到最大有效数据长度[E30/R30 RAGC/TAGC各占一个字节]
//            s_nCmdPackTestDataMaxLen = (pCmdBlock->DataPacket[5] >= T0_X_AGC && pCmdBlock->DataPacket[5] < T0_X_UNT) ? (uint8_t)(CMD_PACKET_TEST_DATA_LEN / 2) : CMD_PACKET_TEST_DATA_LEN;
//            //压缩发送需要的测试数据
//            pReCmdBlock->DataPacket[8] = FN_MIN(nCmdGetLen, s_nCmdPackTestDataMaxLen); //得到发送数据包的长度
//            pReCmdBlock->DataPacket[3] = pReCmdBlock->DataPacket[8] + 4; //未使用
//            for (nLedCnt = 0; nLedCnt < pReCmdBlock->DataPacket[8] && (CMD_PACKET_INFO_DATA_LEN + nLedCnt) < CMD_QUEUE_DATA_MAX_SIZE && (nCmdGetOffset + nLedCnt) < g_nTotal[nAxis]; nLedCnt++)
//            {
//                if (pCmdBlock->DataPacket[5] < T0_X_ORG)
//                {
//                    //打开测试工具后处理的事件
//                    OpenTestToolEventPro();
//                    if (g_stTestStatus.eStatus == eTestStart)
//                    {
//                        int16_t nAdc = g_stScanCoreVar.pADCCurrent[g_nTestAdcIndex[nAxis][nDir][nCmdGetOffset + nLedCnt]];
//                        if (((MIN_ADC_REQ-30) < nAdc) && (nAdc < MIN_ADC_REQ))
//                        {
//                            g_stScanCoreVar.pADCCurrent[g_nTestAdcIndex[nAxis][nDir][nCmdGetOffset + nLedCnt]] = (uint8_t)MIN_ADC_REQ; //限定波动最小值为80
//                        }
//                        else if (nAdc > (MIN_ADC_REQ+30))
//                        {
//                            g_stScanCoreVar.pADCCurrent[g_nTestAdcIndex[nAxis][nDir][nCmdGetOffset + nLedCnt]] = (uint8_t)(MIN_ADC_REQ+30);//限定波动最大值为110
//                        }
//                        pReCmdBlock->DataPacket[CMD_PACKET_INFO_DATA_LEN + nLedCnt] = g_stScanCoreVar.pADCCurrent[g_nTestAdcIndex[nAxis][nDir][nCmdGetOffset + nLedCnt]] ;
//                    }
//                    else
//                    {
//                        pReCmdBlock->DataPacket[CMD_PACKET_INFO_DATA_LEN + nLedCnt] = (uint8_t)(g_stScanCoreVar.pADCCurrent[g_nTestAdcIndex[nAxis][nDir][nCmdGetOffset + nLedCnt]]);
//                    }
//                }
//                else if (pCmdBlock->DataPacket[5] >= T0_X_ORG && pCmdBlock->DataPacket[5] < T0_X_AGC)
//                {
//                    pReCmdBlock->DataPacket[CMD_PACKET_INFO_DATA_LEN + nLedCnt] = (uint8_t)(g_ORG[g_nTestAdcIndex[nAxis][nDir][nCmdGetOffset + nLedCnt]] / THRESHOLD_RATE);
//                }
//                else if (pCmdBlock->DataPacket[5] >= T0_X_AGC && pCmdBlock->DataPacket[5] < T0_X_UNT)
//                {
//#if (SCAN_SOLUTION == SCAN_SOLUTION_6CH_E5HG)
//                    pReCmdBlock->DataPacket[CMD_PACKET_INFO_DATA_LEN + (2*nLedCnt)]    = FN_MAX(0, (uint8_t)(GET_RECE_AGC_GEAR_TAB(g_nTestAgcIndex[nAxis][nDir][nCmdGetOffset + nLedCnt], eRAgcLeft)));
//                    pReCmdBlock->DataPacket[CMD_PACKET_INFO_DATA_LEN + (2*nLedCnt+1)]  = FN_MAX(0, (uint8_t)(GET_RECE_AGC_GEAR_TAB(g_nTestAgcIndex[nAxis][nDir][nCmdGetOffset + nLedCnt], eRAgcRight)));
//#else
//                    pReCmdBlock->DataPacket[CMD_PACKET_INFO_DATA_LEN + (2*nLedCnt)]    = FN_MAX(0, (uint8_t)(GET_RECE_AGC_GEAR_TAB(g_nTestAgcIndex[nAxis][nDir][nCmdGetOffset + nLedCnt])));
//                    pReCmdBlock->DataPacket[CMD_PACKET_INFO_DATA_LEN + (2*nLedCnt+1)]  = FN_MAX(0, (uint8_t)(GET_TRAN_AGC_GEAR_TAB(g_nTestAgcIndex[nAxis][nDir][nCmdGetOffset + nLedCnt])));
//#endif
//                }
//                else if (pCmdBlock->DataPacket[5] >= T0_X_UNT && pCmdBlock->DataPacket[5] < TEST_BUF_END)
//                {
//                    pReCmdBlock->DataPacket[CMD_PACKET_INFO_DATA_LEN + nLedCnt] = 0;
//                }
//            }
//            //根据数据类型得到回传的数据长度
//            pReCmdBlock->nDataLen = (pCmdBlock->DataPacket[5] >= T0_X_AGC && pCmdBlock->DataPacket[5] < T0_X_UNT) ? (uint8_t)(CMD_PACKET_INFO_DATA_LEN + (2*nLedCnt)) : (uint8_t)(CMD_PACKET_INFO_DATA_LEN + nLedCnt);
//
//            AnswerCommand(pReCmdBlock->DataPacket, pReCmdBlock->nDataLen, pCmdBlock->eCmdSource);
//
//            eRetVal = CMD_ANSWER_NONE;
//        }
//        else
//        {
//            eRetVal = CMD_ANSWER_FAIL;
//        }
//        //updata g_stTestStatus
//        if (g_stTestStatus.eStatus == eTestStart
//            && g_stTestStatus.eCmdSource == pReCmdBlock->eCmdSource)
//        {
//            if(pCmdBlock->DataPacket[5] > T2_Y_ORG
//                || (uint32_t)(GetSystickTime() - g_stTestStatus.nStartTime) > ANDROID_UNTTEST_WAITTIME)
//            {
//                g_stTestStatus.eStatus = eTestFinish;
//                g_stTestStatus.nStartTime = GetSystickTime();
//                //printf("Test Finshed!\r\n");
//            }
//        }
    }
    break;

    case DATA_BACK:
    {
        pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
        pReCmdBlock->DataPacket[1] = DATANEW;
        pReCmdBlock->DataPacket[2] = DATA_BACK_RE;
        pReCmdBlock->DataPacket[3] = pCmdBlock->DataPacket[3];
        pReCmdBlock->DataPacket[4] = 0;

        if ((pCmdBlock->DataPacket[3] + 5) <= 64)
        {
            for(int i = 0; i < pCmdBlock->DataPacket[3]; i++)
            {
                pReCmdBlock->DataPacket[5 + i] = pCmdBlock->DataPacket[5 + i];
            }
            pReCmdBlock->nDataLen = pCmdBlock->DataPacket[3] + 5;
            AnswerCommand(pReCmdBlock->DataPacket, pReCmdBlock->nDataLen, eUsb0Ep1Mode);
            eRetVal = CMD_ANSWER_OK;
        }
        else
        {
            eRetVal = CMD_ANSWER_FAIL;
        }
    }
    break;

    case DATA_TO_UART:
    {
        pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
        pReCmdBlock->DataPacket[1] = DATANEW;
        pReCmdBlock->DataPacket[2] = DATA_TO_UART;
        pReCmdBlock->DataPacket[3] = pCmdBlock->DataPacket[3];
        pReCmdBlock->DataPacket[4] = 0;
        if ((pCmdBlock->DataPacket[3] + 5) <= 64)
        {
            for(int i=0; i<pCmdBlock->DataPacket[3]; i++)
            {
                pReCmdBlock->DataPacket[5 + i] = pCmdBlock->DataPacket[5 + i];
            }
            pReCmdBlock->nDataLen = pCmdBlock->DataPacket[3] + 5;
            AnswerCommand(pReCmdBlock->DataPacket, pReCmdBlock->nDataLen, eUsart3Mode);
            eRetVal = CMD_ANSWER_OK;
        }
        else
        {
            eRetVal = CMD_ANSWER_FAIL;
        }
    }
    break;

    case SCAN_INTERVAL:
    {
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = CMD_DATA;
        pReCmdBlock->DataPacket[2] = SCAN_INTERVAL_RE;
        pReCmdBlock->DataPacket[3] = 24;
        pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];

        for(i = 0; i < 3; i++)
        {
            //x轴的三个方向
            pReCmdBlock->DataPacket[5 + i * 4] = FN_BYTE(g_pConfigData->nLedQTStart[0][i], 0);
            pReCmdBlock->DataPacket[6 + i * 4] = FN_BYTE(g_pConfigData->nLedQTStart[0][i], 1);
            pReCmdBlock->DataPacket[7 + i * 4] = FN_BYTE(g_pConfigData->nLedQTEnd[0][i], 0);
            pReCmdBlock->DataPacket[8 + i * 4] = FN_BYTE(g_pConfigData->nLedQTEnd[0][i], 1);

            //y轴的三个方向
            pReCmdBlock->DataPacket[17 + i * 4] = FN_BYTE(g_pConfigData->nLedQTStart[1][i], 0);
            pReCmdBlock->DataPacket[18 + i * 4] = FN_BYTE(g_pConfigData->nLedQTStart[1][i], 1);
            pReCmdBlock->DataPacket[19 + i * 4] = FN_BYTE(g_pConfigData->nLedQTEnd[1][i], 0);
            pReCmdBlock->DataPacket[20 + i * 4] = FN_BYTE(g_pConfigData->nLedQTEnd[1][i], 1);
        }
        pReCmdBlock->nDataLen = 33;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case DATA_GET_OBLIQUE_PARA:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    break;

    default:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    break;
    }

    return eRetVal;
}

/*****************************************************************
 * @Function : 擦除Bootloader
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 处理应答的方式
*****************************************************************/
CmdAnswerType CmdEraseBootloaderKey(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;

    switch(pCmdBlock->DataPacket[2])
    {
    case ERASE_KEY_OPTION:
    {
//        uint8_t arrUniqueID[16] = {0};
//        int32_t i = 0;
//
//        GetUniqueID(arrUniqueID, sizeof(arrUniqueID));
//        for (i = 0; i < 16; i++)
//        {
//            arrUniqueID[i] = arrUniqueID[i] & pCmdBlock->DataPacket[5];
//            if (pCmdBlock->DataPacket[6 + i] != arrUniqueID[i])
//            {
//                break;
//            }
//        }
//
//        if(i == 16)
//        {
//            eRetVal = CMD_ANSWER_OK;
//            FlashEraseKey();
//        }
//        else
//        {
//            eRetVal = CMD_ANSWER_FAIL;
//        }
    }
    break;

    default:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    break;
    }
    return eRetVal;
}

/*****************************************************************
 * @Function : easymeeting免激活的通讯
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 处理应答的方式
*****************************************************************/
CmdAnswerType CmdEasymeetingRegister(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;
    pReCmdBlock->DataPacket[0] = 0xFC;
    pReCmdBlock->DataPacket[1] = EASY_MEETING;
    pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];

    switch(pCmdBlock->DataPacket[2])
    {
#if SEEWO == 1
    case EM_SEEWO:
    {
        pReCmdBlock->DataPacket[2] = EM_SEEWO_RE;
        pReCmdBlock->DataPacket[3] = 0x02;
        pReCmdBlock->DataPacket[5] = 0x01;
        pReCmdBlock->DataPacket[6] = ((u16)(pCmdBlock->DataPacket[5]) * pCmdBlock->DataPacket[6]) % pCmdBlock->DataPacket[7];
        pReCmdBlock->nDataLen = 7;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;
#endif
    case EM_EK:
    {
        pReCmdBlock->DataPacket[2] = EM_EK_RE;
        pReCmdBlock->DataPacket[3] = 0x02;
        pReCmdBlock->DataPacket[5] = 0x02;
        pReCmdBlock->DataPacket[6] = ((uint16_t)(pCmdBlock->DataPacket[6]) * pCmdBlock->DataPacket[7]) % pCmdBlock->DataPacket[5];
        pReCmdBlock->nDataLen = 7;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;


    default:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    break;
    }

    return eRetVal;
}


/*****************************************************************
 * @Function : 新协议参数设置的通讯
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 处理应答的方式
*****************************************************************/
CmdAnswerType CmdNewSetting(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;

    switch(pCmdBlock->DataPacket[2])
    {
    case NEW_SET_AGC_INIT:
    {
        // USB0=534ms USB1=2185ms
//        InitAGC();
//        ScanAndOrgInit();
        eRetVal = CMD_ANSWER_OK;
#if 1
        g_stSingalTest.stSingalData[0].DataPacket[0] = CMD_FC_REPORT_ID;
        g_stSingalTest.stSingalData[0].DataPacket[1] = CMD_ANSWER;
        g_stSingalTest.stSingalData[0].DataPacket[2] = CMD_ANSWER_OK;
        g_stSingalTest.stSingalData[0].DataPacket[3] = 2;
        g_stSingalTest.stSingalData[0].DataPacket[4] = pCmdBlock->DataPacket[4];
        g_stSingalTest.stSingalData[0].DataPacket[5] = pCmdBlock->DataPacket[1];
        g_stSingalTest.stSingalData[0].DataPacket[6] = pCmdBlock->DataPacket[2];
        g_stSingalTest.stSingalData[0].nDataLen = 7;
        g_stSingalTest.stSingalData[0].eCmdSource = pCmdBlock->eCmdSource;

        g_stSingalTest.dataStep = 1;
#endif

    }
    break;

    case NEW_SET_DRAW_TEST_EN:
    {
//        if(pCmdBlock->DataPacket[5] == 1)
//        {
//            if(pCmdBlock->DataPacket[3] == 2)
//            {
//                if(pCmdBlock->DataPacket[6] == 1)
//                {
//                    g_bSendHardWareTestResult = 1;      //关闭 :发送坐标的对与错
//                }
//                else
//                {
//                    g_bSendHardWareTestResult = 0;      //开启:1--开启测试模式，其它关闭测试模式
//                }
//            }
//            g_HardWareCheckResult = 0;                  //开启 :1--开启测试模式，其它关闭测试模式
//        }
//        else
//        {
//            g_HardWareCheckResult = 0x10000000;         //关闭:1--开启测试模式，其它关闭测试模式
//        }
//
//        g_eCheckHardWareComMode = pCmdBlock->eCmdSource;
        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case  NEW_SET_ALLRESET:
    {
//        g_bTouchEn = 1;
//        ResetCalibration();
//        ResetCoordFormat();
//        ResetSetting();
//        ResetRotation();
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = NEW_SET;
        pReCmdBlock->DataPacket[2] = NEW_SET_ALLRESET_RE;
        pReCmdBlock->nDataLen = 3;
        eRetVal = CMD_ANSWER_DATA;                   //与A8有点区别

        uint8_t SendBuff[64]={0xFC,CAL,CAL_GET};
#if ((USB1_DEVICE_CONFIG_TYPE & 0x02) == 0x02)
        AddCmdQueueBlock(SendBuff,sizeof(SendBuff),USB1_EP2_MODE);
#else
        AddCmdQueueBlock(SendBuff,sizeof(SendBuff),eUsb0Ep2Mode);
#endif
    }
    break ;

    case NEW_SET_SET:
    {
//        if(pCmdBlock->DataPacket[5] != 0xff) g_bTouchEn = pCmdBlock->DataPacket[5];
//        if(pCmdBlock->DataPacket[6] != 0xff) g_bRightBTNEn= pCmdBlock->DataPacket[6];
//        if(pCmdBlock->DataPacket[7] != 0xff || pCmdBlock->DataPacket[8] != 0xff) g_nRightBTNTime = pCmdBlock->DataPacket[7] + pCmdBlock->DataPacket[8] * 0x100;
//        if(pCmdBlock->DataPacket[9] != 0xff) g_nRightBTNRange = pCmdBlock->DataPacket[9];
//        if(pCmdBlock->DataPacket[10] != 0xff) g_bWheelEn= pCmdBlock->DataPacket[10];
//        if(pCmdBlock->DataPacket[11] != 0xff) g_nWheelDis = pCmdBlock->DataPacket[11];
//        if(pCmdBlock->DataPacket[12] != 0xff) g_bDragEn = pCmdBlock->DataPacket[12];
//        SaveSettingData();
//        SaveCoordFormat();
        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case NEW_SET_GET:
    {
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = NEW_SET;
//        pReCmdBlock->DataPacket[2] = NEW_SET_GET_RE;
//        pReCmdBlock->DataPacket[3] = 5;
//        pReCmdBlock->DataPacket[4] = 0;
//        pReCmdBlock->DataPacket[5] = g_bTouchEn;
//        pReCmdBlock->DataPacket[6] = g_bRightBTNEn;
//        pReCmdBlock->DataPacket[7] = FN_BYTE(g_nRightBTNTime, 0);
//        pReCmdBlock->DataPacket[8] = FN_BYTE(g_nRightBTNTime, 1);
//        pReCmdBlock->DataPacket[9] = g_nRightBTNRange;
//        pReCmdBlock->DataPacket[10] = g_bWheelEn;
//        pReCmdBlock->DataPacket[11] = g_nWheelDis;
//        pReCmdBlock->DataPacket[12] = g_bDragEn;
//        pReCmdBlock->nDataLen = 13;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case NEW_SET_SETRESET:
    {
//        g_bTouchEn = 1;
//        SaveCoordFormat();
//        ResetSetting();
        eRetVal = CMD_ANSWER_OK;
    }
    break;

    // 获取去污功能状态
    case NEW_SET_DEC_GET:
    {
#if 0
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = NEW_SET;
        pReCmdBlock->DataPacket[2] = NEW_SET_DEC_GET_RE;
        pReCmdBlock->DataPacket[3] = 1;    // 有效字节
        pReCmdBlock->DataPacket[5] = g_bDecontaminationEn;
        pReCmdBlock->nDataLen = 6;
        eRetVal = CMD_ANSWER_DATA;
#endif
        eRetVal = CMD_ANSWER_NONE;
#if ((USB1_DEVICE_CONFIG_TYPE & 0x02) == 0x02)
        AnswerCommand(&pCmdBlock->DataPacket[0],pCmdBlock->nDataLen,USB1_EP2_MODE);
#else
        AnswerCommand(&pCmdBlock->DataPacket[0],pCmdBlock->nDataLen,eUsb0Ep2Mode);
#endif
    }
    break;

    case NEW_SET_DEC_GET_RE:
    {
#if ((USB1_DEVICE_CONFIG_TYPE & 0x02) == 0x02)
        AnswerCommand(&pCmdBlock->DataPacket[0],pCmdBlock->nDataLen,USB1_EP1_MODE);
#else
        AnswerCommand(&pCmdBlock->DataPacket[0],pCmdBlock->nDataLen,eUsb0Ep1Mode);
#endif
        eRetVal = CMD_ANSWER_NONE;
    }
    break;

    //设置去污功能
    case NEW_SET_DEC_SET:    // 设置去污功能
    {
        eRetVal = CMD_ANSWER_OK;
        //将设置去污的命令转发到应用执行
#if ((USB1_DEVICE_CONFIG_TYPE & 0x02) == 0x02)
        AnswerCommand(&pCmdBlock->DataPacket[0],pCmdBlock->DataPacket[3]+5,USB1_EP2_MODE);
#else
        AnswerCommand(&pCmdBlock->DataPacket[0],pCmdBlock->DataPacket[3]+5,eUsb0Ep2Mode);
#endif
    }
    break;

    case NEW_SET_DRAW_TEST:
    {
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = NEW_SET;
        pReCmdBlock->DataPacket[2] = NEW_SET_DRAW_TEST_RE;
        pReCmdBlock->DataPacket[3] = 1;
        pReCmdBlock->DataPacket[4] = 0;
        pReCmdBlock->nDataLen = 6;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    default:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    break;
    }
    return eRetVal;
}

/*****************************************************************
 * @Function :处理配置的相关命令
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 处理应答的方式
*****************************************************************/
CmdAnswerType CmdConfigPara(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;

    switch (pCmdBlock->DataPacket[2])
    {
    case CONFIGPARA_INFO_READ:
    {
        eRetVal = CMD_ANSWER_NONE;
    }
    break;

    case CONFIGPARA_XY_LED_GET:
    {
        pReCmdBlock->DataPacket[0] = 0xFC;
        pReCmdBlock->DataPacket[1] = CONFIGPARA;
        pReCmdBlock->DataPacket[2] = CONFIGPARA_XY_LED_GET_RE;
        pReCmdBlock->DataPacket[3] = 16;
        pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];

        //X方向
        pReCmdBlock->DataPacket[5] = (uint8_t)(g_pConfigData->nXTLedNumForBoard[0]);
        pReCmdBlock->DataPacket[6] = (uint8_t)(g_pConfigData->nXTLedNumForBoard[1]);
        pReCmdBlock->DataPacket[7] = (uint8_t)(g_pConfigData->nXTLedNumForBoard[2]);
        pReCmdBlock->DataPacket[8] = (uint8_t)(g_pConfigData->nXTLedNumForBoard[3]);

        pReCmdBlock->DataPacket[9] = (uint8_t)(g_pConfigData->nXTLedNumForBoard[4]);
        pReCmdBlock->DataPacket[10] = (uint8_t)(g_pConfigData->nXTLedNumForBoard[5]);
        pReCmdBlock->DataPacket[11] = (uint8_t)(g_pConfigData->nXTLedNumForBoard[6]);
        pReCmdBlock->DataPacket[12] = (uint8_t)(g_pConfigData->nXTLedNumForBoard[7]);

        //Y方向
        pReCmdBlock->DataPacket[13] = (uint8_t)(g_pConfigData->nYTLedNumForBoard[0]);
        pReCmdBlock->DataPacket[14] = (uint8_t)(g_pConfigData->nYTLedNumForBoard[1]);
        pReCmdBlock->DataPacket[15] = (uint8_t)(g_pConfigData->nYTLedNumForBoard[2]);
        pReCmdBlock->DataPacket[16] = (uint8_t)(g_pConfigData->nYTLedNumForBoard[3]);

        pReCmdBlock->DataPacket[17] = (uint8_t)(g_pConfigData->nYTLedNumForBoard[4]);
        pReCmdBlock->DataPacket[18] = (uint8_t)(g_pConfigData->nYTLedNumForBoard[5]);
        pReCmdBlock->DataPacket[19] = (uint8_t)(g_pConfigData->nYTLedNumForBoard[6]);
        pReCmdBlock->DataPacket[20] = (uint8_t)(g_pConfigData->nYTLedNumForBoard[7]);
        pReCmdBlock->nDataLen = 21;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case CONFIGPARA_XY_RLED_GET:
    {
        pReCmdBlock->DataPacket[0] = 0xFC;
        pReCmdBlock->DataPacket[1] = CONFIGPARA;
        pReCmdBlock->DataPacket[2] = CONFIGPARA_XY_RLED_GET_RE;
        pReCmdBlock->DataPacket[3] = 16;
        pReCmdBlock->DataPacket[4] = pCmdBlock->DataPacket[4];

        //X方向
        pReCmdBlock->DataPacket[5] = (uint8_t)(g_pConfigData->nXRLedNumForBoard[0]);
        pReCmdBlock->DataPacket[6] = (uint8_t)(g_pConfigData->nXRLedNumForBoard[1]);
        pReCmdBlock->DataPacket[7] = (uint8_t)(g_pConfigData->nXRLedNumForBoard[2]);
        pReCmdBlock->DataPacket[8] = (uint8_t)(g_pConfigData->nXRLedNumForBoard[3]);

        pReCmdBlock->DataPacket[9] = (uint8_t)(g_pConfigData->nXRLedNumForBoard[4]);
        pReCmdBlock->DataPacket[10] = (uint8_t)(g_pConfigData->nXRLedNumForBoard[5]);
        pReCmdBlock->DataPacket[11] = (uint8_t)(g_pConfigData->nXRLedNumForBoard[6]);
        pReCmdBlock->DataPacket[12] = (uint8_t)(g_pConfigData->nXRLedNumForBoard[7]);

        //Y方向
        pReCmdBlock->DataPacket[13] = (uint8_t)(g_pConfigData->nYRLedNumForBoard[0]);
        pReCmdBlock->DataPacket[14] = (uint8_t)(g_pConfigData->nYRLedNumForBoard[1]);
        pReCmdBlock->DataPacket[15] = (uint8_t)(g_pConfigData->nYRLedNumForBoard[2]);
        pReCmdBlock->DataPacket[16] = (uint8_t)(g_pConfigData->nYRLedNumForBoard[3]);

        pReCmdBlock->DataPacket[17] = (uint8_t)(g_pConfigData->nYRLedNumForBoard[4]);
        pReCmdBlock->DataPacket[18] = (uint8_t)(g_pConfigData->nYRLedNumForBoard[5]);
        pReCmdBlock->DataPacket[19] = (uint8_t)(g_pConfigData->nYRLedNumForBoard[6]);
        pReCmdBlock->DataPacket[20] = (uint8_t)(g_pConfigData->nYRLedNumForBoard[7]);
        pReCmdBlock->nDataLen = 21;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case CONFIGPARA_READ:
    {
        //ConfigParaReadToPC(pCmdBlock,pReCmdBlock);
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case CONFIGPARA_WRITE:
    {
        //主机配置更新
        //eRetVal = ConfigParaWriteToTouchMaster(pCmdBlock,pReCmdBlock);
    }
    break;

    case CONFIGPARA_SLAVECRC32_GET_RE:
    {
        //主机配置更新
        //DoConfirmConfigParaCRC32(pCmdBlock);
        eRetVal =  CMD_ANSWER_NONE;
    }
    break;

    case CONFIGPARA_RESET:     //恢复出厂设置
    {
        eRetVal = CMD_ANSWER_NONE;
    }
    break;

    default:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    break;
    }

    return eRetVal;
}

/*****************************************************************
 * @Function: 处理用户相关的数据保存
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 处理应答的方式
*****************************************************************/
ATTR_WEAK CmdAnswerType CmdUserKey(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;
//    stObjCtrlCombin_t *pObjCombin = GetObjCombin();
//
//    switch(pCmdBlock->DataPacket[2])
//    {
//    case USERKEY_GET_ID:
//    {
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
//        pReCmdBlock->DataPacket[2] = USERKEY_GET_ID_RE;
//        pReCmdBlock->DataPacket[3] = 16;
//        pReCmdBlock->DataPacket[4] = 0;
//        GetUniqueID(&(pReCmdBlock->DataPacket[5]), 16);
//        pReCmdBlock->nDataLen = 21;
//        eRetVal = CMD_ANSWER_DATA;
//    }
//    break;
//
//    case USERKEY_GET_KEY:
//    {
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
//        pReCmdBlock->DataPacket[2] = USERKEY_GET_KEY_RE;
//        pReCmdBlock->DataPacket[3] = 56;
//        pReCmdBlock->DataPacket[4] = 0x00;
//        MemCpy(&(pReCmdBlock->DataPacket[5]), g_UserKey, 56);
//        pReCmdBlock->nDataLen = 64;
//        eRetVal = CMD_ANSWER_DATA;
//    }
//    break;
//
//    case USERKEY_SET_KEY:
//    {
//        MemCpy(g_UserKey, &(pCmdBlock->DataPacket[5]), 56);
//        eRetVal = CMD_ANSWER_OK;
//    }
//    break;
//
//    case USERKEY_RELOAD_KEY:
//    {
//        GetUserKey();
//        eRetVal = CMD_ANSWER_OK;
//    }
//    break;
//
//    case USERKEY_SAVE_KEY:
//    {
//        SaveUserKey();
//        eRetVal = CMD_ANSWER_OK;
//        //printf("Save Aging Time\r\n");
//    }
//    break;
//    default:
//    {
//        eRetVal = CMD_ANSWER_UNKNOWN;
//    }
//    break;
//    }
    return eRetVal;
}

/*****************************************************************
 * @Function: 配置是否发送坐标
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 处理应答的方式
*****************************************************************/
CmdAnswerType CmdSendCoordConfig(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;
    switch(pCmdBlock->DataPacket[2]) //根据命令散转
    {
    case CMD_SENDCOORD_EN:
    {
//        if(1==pCmdBlock->DataPacket[5])
//        {
//            g_bSendCoord = pCmdBlock->DataPacket[6];
//            pReCmdBlock->DataPacket[6] = pCmdBlock->DataPacket[6];
//
//        } else if(0==pCmdBlock->DataPacket[5])
//        {
//            pReCmdBlock->DataPacket[6] = g_bSendCoord ;
//        }
//        pReCmdBlock->DataPacket[0] = CMD_FC_REPORT_ID;
//        pReCmdBlock->DataPacket[1] = CMD_SENDCOORD_CONFIG;
//        pReCmdBlock->DataPacket[2] = CMD_CMD_SENDCOORD_EN_RE;
//        pReCmdBlock->DataPacket[3] = 0x03;
//        pReCmdBlock->DataPacket[4] = 0xCC;
//        pReCmdBlock->DataPacket[5] = pCmdBlock->DataPacket[5];
//        pReCmdBlock->nDataLen = 7 ;

        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    default:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    break;
    }

    return eRetVal;
}
/*****************************************************************
 * @Function : 是否使能 Ink 发送
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 处理应答的方式
*****************************************************************/
ATTR_WEAK CmdAnswerType CmdSendCoordInkEn(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    return CMD_ANSWER_NONE;
}
/*
 * 降半屏处理
 */
ATTR_WEAK CmdAnswerType ComDropScreen(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    eUsbDevice_t eUsbDev = GetUsbDev(pCmdBlock->eCmdSource);
    eUsbDevice_t eRUsbDev = eUsbDev == eUsbDev0 ? eUsbDev1 : eUsbDev0;
//    if (0 == pCmdBlock->DataPacket[5])
//    {
//        MemSet(&g_stDrop, 0, sizeof(g_stDrop));
//    }
//    else
//    {
//        MemSet(&g_stDrop, 0, sizeof(g_stDrop));
//        if (pCmdBlock->DataPacket[5] <= 3)
//        {
//            g_stDrop.arrOffset[Y] = MAX_LOGICAL_VALUE / (pCmdBlock->DataPacket[5] + 1);
//        }
//        else
//        {
//            g_stDrop.arrOffset[Y] = (int32_t)(MAX_LOGICAL_VALUE * (pCmdBlock->DataPacket[5] / 100.0f));
//        }
//        if(pCmdBlock->eCmdSource != eUsart3Mode)
//        {
//            g_stDrop.bUsbEnable[eRUsbDev] = 1;
//        }
//        else
//        {
//            g_stDrop.bUsbEnable[eUsbDev0] = 1;
//        }
//    }
    return CMD_ANSWER_NONE;
}
/*****************************************************************
 * @Function : 坐标发送格式设置
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 处理应答的方式
*****************************************************************/
ATTR_WEAK CmdAnswerType CmdFormat(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    eUsbDevice_t eUsbDev = GetUsbDev(pCmdBlock->eCmdSource);
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;

    switch(pCmdBlock->DataPacket[2]) //根据命令散转
    {

    case FMT_SET:
    {
//        if(pCmdBlock->DataPacket[5] == eFormatNone)
//        {
//            if(g_stUsbCoordCfg[eUsbDev].eFormat != eFormatNone)
//            {
//                g_stUsbCoordCfg[eUsbDev].eFormatRe = g_stUsbCoordCfg[eUsbDev].eFormat;
//            }
//            //change this to support USB0 send touch when only using USB0
//            //do not know why modify g_eUsb0Format here
//        }
//        else if(pCmdBlock->DataPacket[5] == FMT_RE)
//        {
//            g_stUsbCoordCfg[eUsbDev].eFormat = g_stUsbCoordCfg[eUsbDev].eFormatRe;
//        }
//        else
//        {
//            g_stUsbCoordCfg[eUsbDev].eFormat = (eTouchFormat_t)pCmdBlock->DataPacket[5];
//            g_stUsbCoordCfg[eUsbDev].eFormatRe = g_stUsbCoordCfg[eUsbDev].eFormat;
//        }

        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case FMT_GET:
    {
//        pReCmdBlock->nDataLen = 6;
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = FORMAT;
//        pReCmdBlock->DataPacket[2] = FMT_GET_RE;
//        pReCmdBlock->DataPacket[3] = 1;
//        pReCmdBlock->DataPacket[4] = 0;
//        pReCmdBlock->DataPacket[5] = g_stUsbCoordCfg[eUsbDev].eFormat;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case FMT_CVT:
    {
//        g_stUsbCoordCfg[eUsbDev].bTuioCoordEn = pCmdBlock->DataPacket[5];
        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case FMT_CAL:
    {
//        g_bCalEn = pCmdBlock->DataPacket[5];
        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case FMT_SMO:
    {
//        g_bSmoothEn = pCmdBlock->DataPacket[5];
        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case FMT_WIDTH_EN:
    {
//        if(pCmdBlock->DataPacket[5] == 0)
//        {
//            g_stUsbCoordCfg[eUsbDev].bTuioCoordWidthEn = 0;
//        }
//        else
//        {
//            g_stUsbCoordCfg[eUsbDev].bTuioCoordWidthEn = 1;
//        }

        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case FMT_USB_COORD:
    {
//        pReCmdBlock->nDataLen = 6;
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = FORMAT;
//        pReCmdBlock->DataPacket[2] = FMT_USB_COORD_RE;
//        pReCmdBlock->DataPacket[3] = 1;
//        pReCmdBlock->DataPacket[4] = 0;
//        pReCmdBlock->DataPacket[5] = g_stUsbCoordCfg[eUsbDev].bCoordEn;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case FMT_USB_COORD_EN:
    {
        eRetVal = CMD_ANSWER_NONE;
        switch(pCmdBlock->DataPacket[3])
        {
            case TOUCH_DATA_USB_TRAN:
            {
//                if (0 == pCmdBlock->DataPacket[5] || 1 == pCmdBlock->DataPacket[5])
//                {
//                    g_stUsbCoordCfg[eUsbDev].bCoordEn = pCmdBlock->DataPacket[5];
//                    pReCmdBlock->DataPacket[2] = 0x81;  //成功
//                }
//                else
//                {
//                    pReCmdBlock->DataPacket[2] = 0x82;  //失败
//                }
//                pReCmdBlock->DataPacket[0] = 0xfc;
//                pReCmdBlock->DataPacket[1] = 0xfe;
//
//                pReCmdBlock->DataPacket[3] = 0x02;
//                pReCmdBlock->DataPacket[4] = 0x00;
//                pReCmdBlock->DataPacket[5] = FORMAT;
//                pReCmdBlock->DataPacket[6] = FMT_USB_COORD_EN;
//                pReCmdBlock->nDataLen = 7;
                eRetVal = CMD_ANSWER_DATA;
            }
            break;
            case TOUCH_DATA_UART_TRAN:
            {
//                if (0 == pCmdBlock->DataPacket[5] || 1 == pCmdBlock->DataPacket[5])
//                {
//                    g_bUartCoordEn = pCmdBlock->DataPacket[5];
//                }
            }
            break;
            default:
            {
                eRetVal = CMD_ANSWER_NONE;
            }
            break;
        }

    }
    break;

    case FMT_MAC_10_SET:
    {
//        g_stUsbCoordCfg[eUsbDev].bUnderMac10 = pCmdBlock->DataPacket[5];
//        SaveCoordFormat();
        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case FMT_MAC_10_GET:
    {
//        pReCmdBlock->nDataLen = 6;
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = FORMAT;
//        pReCmdBlock->DataPacket[2] = FMT_MAC_10_GET_RE;
//        pReCmdBlock->DataPacket[3] = 1;
//        pReCmdBlock->DataPacket[4] = 0;
//        pReCmdBlock->DataPacket[5] = g_stUsbCoordCfg[eUsbDev].bUnderMac10;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case FMT_GET_ALL:
    {
//        pReCmdBlock->nDataLen = 12;
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = FORMAT;
//        pReCmdBlock->DataPacket[2] = FMT_GET_ALL_RE;
//        pReCmdBlock->DataPacket[3] = 7;
//        pReCmdBlock->DataPacket[4] = 0;
//        pReCmdBlock->DataPacket[5] = g_stUsbCoordCfg[eUsbDev].eFormat;
//        pReCmdBlock->DataPacket[6] = g_stUsbCoordCfg[eUsbDev].eFormatRe;
//        pReCmdBlock->DataPacket[7] = g_stUsbCoordCfg[eUsbDev].bTuioCoordEn;
//        pReCmdBlock->DataPacket[8] = g_bCalEn;
//        pReCmdBlock->DataPacket[9] = g_bSmoothEn;
//        pReCmdBlock->DataPacket[10] = g_stUsbCoordCfg[eUsbDev].bTuioCoordWidthEn;
//        pReCmdBlock->DataPacket[11] = g_stUsbCoordCfg[eUsbDev].bCoordEn;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case FMT_UART_COORD:
    {
//        pReCmdBlock->nDataLen = 6;
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = FORMAT;
//        pReCmdBlock->DataPacket[2] = FMT_UART_COORD_RE;
//        pReCmdBlock->DataPacket[3] = 1;
//        pReCmdBlock->DataPacket[4] = 0;
//        pReCmdBlock->DataPacket[5] = g_bUartCoordEn;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case FMT_UART_COORD_EN:
    {
//        g_bUartCoordEn = pCmdBlock->DataPacket[5];
        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case FMT_UART_POINT_NUM_SET:
    {
//        if (pCmdBlock->DataPacket[5] > 0 && pCmdBlock->DataPacket[5] <= USER_POINT)
//        {
//            g_nUartSendCoordNum = pCmdBlock->DataPacket[5];
//            eRetVal = CMD_ANSWER_OK;
//        }
//        else
//        {
//            eRetVal = CMD_ANSWER_FAIL;
//        }
    }
    break;

    case FMT_UART_POINT_NUM_GET:
    {
//        pReCmdBlock->nDataLen = 6;
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = FORMAT;
//        pReCmdBlock->DataPacket[2] = FMT_UART_POINT_NUM_GET_RE;
//        pReCmdBlock->DataPacket[3] = 1;
//        pReCmdBlock->DataPacket[4] = 0;
//        pReCmdBlock->DataPacket[5] = g_nUartSendCoordNum;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case FMT_UART_COORD_WIDTH_SET:
    {
//        if (pCmdBlock->DataPacket[5] == 0 || pCmdBlock->DataPacket[5] == 1)
//        {
//            g_bUartCoordWidthEn = pCmdBlock->DataPacket[5];
//            eRetVal = CMD_ANSWER_OK;
//        }
//        else
//        {
//            eRetVal = CMD_ANSWER_FAIL;
//        }
    }
    break;

    case FMT_UART_COORD_WIDTH_GET:
    {
//        pReCmdBlock->nDataLen = 6;
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = FORMAT;
//        pReCmdBlock->DataPacket[2] = FMT_UART_COORD_WIDTH_GET_RE;
//        pReCmdBlock->DataPacket[3] = 1;
//        pReCmdBlock->DataPacket[4] = 0;
//        pReCmdBlock->DataPacket[5] = g_bUartCoordWidthEn;
        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case FMT_PHYSICAL_SIZE_GET:
    {
        pReCmdBlock->nDataLen = 9;
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = FORMAT;
        pReCmdBlock->DataPacket[2] = FMT_PHYSICAL_SIZE_GET_RE;
        pReCmdBlock->DataPacket[3] = 4;
        pReCmdBlock->DataPacket[4] = 0;
        pReCmdBlock->DataPacket[5] = 0;
        //Modified -> For android
        pReCmdBlock->DataPacket[6] = FN_BYTE(g_pConfigData->nPhysicalX, 0);
        pReCmdBlock->DataPacket[7] = FN_BYTE(g_pConfigData->nPhysicalX, 1);
        pReCmdBlock->DataPacket[8] = FN_BYTE(g_pConfigData->nPhysicalY, 0);
        pReCmdBlock->DataPacket[9] = FN_BYTE(g_pConfigData->nPhysicalY, 1);
        eRetVal = CMD_ANSWER_DATA;
    }
    break;
    case FMT_CMD_0B_RES:
    {
        eRetVal = CmdSendCoordInkEn(pCmdBlock, pReCmdBlock);
    }
    break;
    case FMT_CMD_32_RES:
    {
        eRetVal = ComDropScreen(pCmdBlock, pReCmdBlock);
    }
    break;
    default:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    break;
    }
    return eRetVal;
}

/*****************************************************************
 * @Function : 设置旋转参数
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 处理应答的方式
*****************************************************************/
CmdAnswerType CmdRotation(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;

    switch(pCmdBlock->DataPacket[2]) //根据命令散转
    {
    case ROTATION_SET:
    {
//        if(pCmdBlock->DataPacket[3] != 0x02)
//        {
//            g_nDeRotation = (pCmdBlock->DataPacket[5] + g_pConfigData->eRotationDef) % 4;
//            g_nDeRotationOS = 0xcc;
//            ResetRotation();
//        }
//        else
//        {
//            g_nRotation = (pCmdBlock->DataPacket[5] + g_pConfigData->eRotationDef) % 4;
//            SaveRotationData();
//        }

        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case ROTATION_OS_SET:
    {
//        if(pCmdBlock->DataPacket[3] != 0x02)
//        {
//            g_nDeRotationOS = pCmdBlock->DataPacket[5];
//        }
//        else
//        {
//            g_nRotationOS = pCmdBlock->DataPacket[5];
//        }
        eRetVal = CMD_ANSWER_OK;
    }
    break;

    case ROTATION_DEF_GET:
    {
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = ROTATION;
//        pReCmdBlock->DataPacket[2] = ROTATION_DEF_GET_RE;
//        pReCmdBlock->DataPacket[3] = 1;
//        pReCmdBlock->DataPacket[4] = 0;
//        if(g_nDeRotation != 0xcc)
//        {
//            pReCmdBlock->DataPacket[5] = g_nDeRotation;
//        }
//        else
//        {
//            pReCmdBlock->DataPacket[5] = g_pConfigData->eRotationDef;
//        }
//        pReCmdBlock->nDataLen = 6;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case ROTATION_GET:
    {
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = ROTATION;
//        pReCmdBlock->DataPacket[2] = ROTATION_GET_RE;
//        pReCmdBlock->DataPacket[3] = 8;
//        pReCmdBlock->DataPacket[4] = 0;
//        pReCmdBlock->DataPacket[5] = g_nRotation;
//        pReCmdBlock->DataPacket[6] = g_nRotationOS;
//        pReCmdBlock->DataPacket[7] = g_nDeRotation;
//        pReCmdBlock->DataPacket[8] = g_nDeRotationOS;
//        pReCmdBlock->DataPacket[9] = g_bReverse;
//        pReCmdBlock->DataPacket[10] = g_pConfigData->eRotationDef;
//        pReCmdBlock->DataPacket[11] = g_pConfigData->eRotationOsDef;
//        pReCmdBlock->DataPacket[12] = g_pConfigData->bReverse;
//        pReCmdBlock->nDataLen = 13;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    default:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    break;
    }

    return eRetVal;
}

/*****************************************************************
 * @Function : 旧校准命令处理
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 处理应答的方式
*****************************************************************/
CmdAnswerType CmdOldCalibration(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType nRetVal = CMD_ANSWER_NONE;
    uint32_t nDeviation = 0;

//    g_stCalibrationData.nStep= pCmdBlock->DataPacket[2];
//    switch (pCmdBlock->DataPacket[2]) //根据校准步骤散转
//    {
//    case CAL_BEGIN: //启动校准，返回确认
//    {
//        g_bCalEn = 0;
//        g_bInCalibrating = 1;
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = CAL;
//        pReCmdBlock->DataPacket[2] = CAL_STEP1;
//        pReCmdBlock->nDataLen = 3;
//        nRetVal = CMD_ANSWER_DATA;
//    }
//    break;
//
//    case CAL_STEP1:
//    case CAL_STEP2:
//    case CAL_STEP3:
//    case CAL_STEP4:
//    {
//        g_bInCalibrating = 1;
//        nRetVal = CMD_ANSWER_NONE;
//    }
//    break;
//
//    case CAL_CHECK:
//    {
//        //如果上位机下发系统旋转参数则使用，如果没有下发则计算出默认的
//        g_bCalEn = 1;
//        //参数不能在此处保存。在校准时随便点四下，再重启机器会有触摸偏差很大，造成无触摸的假象
//        //SaveCalibrationData(); //保存参数
//        nDeviation = GetDeviation(); //校验校准是否OK
//
//        pReCmdBlock->nDataLen = 19;
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = CAL;
//        pReCmdBlock->DataPacket[2] = CAL_END;
//        pReCmdBlock->DataPacket[3] = (nDeviation >= 5) ? 0x03 : 0x00;
//
//        pReCmdBlock->DataPacket[4] = nDeviation;
//        pReCmdBlock->DataPacket[5] = nDeviation;
//        pReCmdBlock->DataPacket[11] = (g_stCalibrationData.arrCoord[0].nX + g_stCalibrationData.arrCoord[3].nX) / 2 / 0x100;
//        pReCmdBlock->DataPacket[12] = (g_stCalibrationData.arrCoord[0].nX + g_stCalibrationData.arrCoord[3].nX) / 2 % 0x100;
//        pReCmdBlock->DataPacket[13] = (g_stCalibrationData.arrCoord[0].nY + g_stCalibrationData.arrCoord[1].nY) / 2 / 0x100;
//        pReCmdBlock->DataPacket[14] = (g_stCalibrationData.arrCoord[0].nY + g_stCalibrationData.arrCoord[1].nY) / 2 % 0x100;
//        pReCmdBlock->DataPacket[15] = (g_stCalibrationData.arrCoord[1].nX + g_stCalibrationData.arrCoord[2].nX) / 2 / 0x100;
//        pReCmdBlock->DataPacket[16] = (g_stCalibrationData.arrCoord[1].nX + g_stCalibrationData.arrCoord[2].nX) / 2 % 0x100;
//        pReCmdBlock->DataPacket[17] = (g_stCalibrationData.arrCoord[2].nY + g_stCalibrationData.arrCoord[3].nY) / 2 / 0x100;
//        pReCmdBlock->DataPacket[18] = (g_stCalibrationData.arrCoord[2].nY + g_stCalibrationData.arrCoord[3].nY) / 2 % 0x100;
//
//        nRetVal = CMD_ANSWER_DATA;
//#if (1 == CALIB_INFO_DUG)
//        printf("check cal %04x %04x %04x %04x\r\n", g_stCalibrationData.nREOX1, g_stCalibrationData.nREOX2, g_stCalibrationData.nREOY1, g_stCalibrationData.nREOY2);
//#endif
//    }
//    break;
//
//    case CAL_END:
//    {
//        pReCmdBlock->DataPacket[0] = 0xFC;
//        pReCmdBlock->DataPacket[1] = 0x01;
//        pReCmdBlock->DataPacket[2] = 0x08;
//        pReCmdBlock->nDataLen = 3;
//        nRetVal = CMD_ANSWER_DATA;
//#if (1 == CALIB_INFO_DUG)
//        printf("cal end\r\n");
//#endif
//    }
//    break;
//
//    case CAL_RESET:
//    {
//        g_bSendCoord = 0;   //解决进入校准功能异常操作[不保存校准参数就不进入SendCoord函数]
//        ResetCalibration(); // 由于校准软件校准完毕后，未决定OK就已经保存数据，复位只能复位到出厂参数。
//        ResetDefaultPointTransform();
//        nRetVal = CMD_ANSWER_OK;
//    }
//    break;
//
//    case ALL_RESET:
//    {
//        ResetCalibration();
//        ResetDefaultPointTransform();
//        ResetCoordFormat();
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = CAL;
//        pReCmdBlock->DataPacket[2] = ALL_RESET;
//        pReCmdBlock->nDataLen = 3;
//
//        nRetVal = CMD_ANSWER_DATA;
//    }
//    break;
//
//    case CAL_GET:
//    {
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = CAL;
//        pReCmdBlock->DataPacket[2] = CAL_GET_RE;
//        pReCmdBlock->DataPacket[3] = 21;
//        pReCmdBlock->DataPacket[4] = 0;
//
//        *((uint32_t*)(&pReCmdBlock->DataPacket[5]))   = g_stCalibrationData.nREOX1; //nREOX1;
//        *((uint32_t*)(&pReCmdBlock->DataPacket[9]))   = g_stCalibrationData.nREOX2; //nREOX2;
//        *((uint32_t*)(&pReCmdBlock->DataPacket[13]))  = g_stCalibrationData.nREOY1; //nREOY1;
//        *((uint32_t*)(&pReCmdBlock->DataPacket[17]))  = g_stCalibrationData.nREOY2; //nREOY2;
//        SaveCalibrationData(); //保存参数
//
//        pReCmdBlock->DataPacket[21] = g_pConfigData->eRotationParameter;
//
//        pReCmdBlock->nDataLen = 22 ;
//        nRetVal = CMD_ANSWER_DATA;
//#if (1 == CALIB_INFO_DUG)
//        printf("get cal %04x %04x %04x %04x\r\n", g_stCalibrationData.nREOX1, g_stCalibrationData.nREOX2, g_stCalibrationData.nREOY1, g_stCalibrationData.nREOY2);
//#endif
//    }
//    break;
//    default:
//    {
//    }
//    break;
//    }
    return nRetVal;
}

/*****************************************************************
 * @Function : 处理多点坐标数据的转发
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 处理应答的方式
*****************************************************************/
int32_t CmdMultPoints(uint8_t* pCmdData,  int16_t Len, eCmdSource_t eMode)
{
    return 1;
}

/*****************************************************************************
 函 数 名  : ShowIndicatorLed
 功能描述  : 闪灯
 输入参数  : uint8_t nTouchPointCount
 输出参数  : 无
 返 回 值  :
*****************************************************************************/
int8_t ShowIndicatorLed(uint8_t nTouchPointCount)
{
    int8_t bLedOn = 0;
//    if(g_bBoradLedEnable == 1)
//    {
//        if (nTouchPointCount > 0)
//        {
//            LedOn();
//            bLedOn = 1;
//        }
//        else
//        {
//            LedOff();
//            bLedOn = 0;
//        }
//    }
    return bLedOn;
}

/*****************************************************************************
 函 数 名  : HandleTime2Event
 功能描述  : 处理Time2的事件
 输入参数  : void
 输出参数  : 无
 返 回 值  :
*****************************************************************************/
void HandleTime2Event(void)
{
    while(1)
    {
        break ;
    }
}

/*****************************************************************
 * @Function : 参数设置
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 处理应答的方式
*****************************************************************/
CmdAnswerType CmdSetting(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;

    switch (pCmdBlock->DataPacket[1])
    {
    case SET_TIME:
    {
//        g_nRightBTNTime = pCmdBlock->DataPacket[2] + (((uint16_t)pCmdBlock->DataPacket[3]) << 8);
//        if (g_nRightBTNTime > 2500 || g_nRightBTNTime < 500)
//        {
//            g_nRightBTNTime = 1500;
//        }
//        g_nRightBTNRange = pCmdBlock->DataPacket[6];
//        if (g_nRightBTNRange > 5 || g_nRightBTNRange < 1)
//        {
//            g_nRightBTNRange = 1;
//        }
//        g_nDoubleClickDbRange = pCmdBlock->DataPacket[7];
//        SaveSettingData(); //保存参数*/
        pReCmdBlock->DataPacket[0] = 0xFC;
        pReCmdBlock->DataPacket[1] = SET_TIME;
        pReCmdBlock->nDataLen = 2;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case GET_TIME:   //获取时间
    {
//        pReCmdBlock->DataPacket[0] = 0xFC;
//        pReCmdBlock->DataPacket[1] = GET_TIME;
//        pReCmdBlock->DataPacket[2] = FN_BYTE(g_nRightBTNTime, 0);
//        pReCmdBlock->DataPacket[3] = FN_BYTE(g_nRightBTNTime, 1);
//        pReCmdBlock->DataPacket[4] = 0;
//        pReCmdBlock->DataPacket[5] = 0;
//        pReCmdBlock->DataPacket[6] = g_nRightBTNRange;
//        pReCmdBlock->DataPacket[7] = g_nDoubleClickDbRange;
//        pReCmdBlock->DataPacket[8] = g_bRightBTNEn;
//        pReCmdBlock->DataPacket[9] = 0;
//        pReCmdBlock->nDataLen = 10;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case SET_RB_EN: //设置右键功能以及鼠标功能
    {
//        g_bRightBTNEn = pCmdBlock->DataPacket[2];
//        if (g_bRightBTNEn > 1)
//        {
//            g_bRightBTNEn = 1;
//        }
//        SaveSettingData(); //保存参数*/
        pReCmdBlock->DataPacket[0] = 0xFC;
        pReCmdBlock->DataPacket[1] = SET_RB_EN;
        pReCmdBlock->nDataLen = 2;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case SET_ALL_TOUCH_ENABLE:  //设置禁止所有通道的触摸
    {
//        g_bTouchEn = pCmdBlock->DataPacket[2];
//        if (g_bTouchEn > 1)
//        {
//            g_bTouchEn = 1;
//        }
        eRetVal = CMD_ANSWER_NONE;
    }
    break;

    case GET_ALL_TOUCH_ENABLE:  //获取所有通道的触摸状态
    {
//        pReCmdBlock->DataPacket[0] = 0xFC;
//        pReCmdBlock->DataPacket[1] = GET_ALL_TOUCH_ENABLE;
//        pReCmdBlock->DataPacket[2] = g_bTouchEn;
//        pReCmdBlock->nDataLen = 3;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case ALL_RESET:  //设置鼠标是否使能
    {
//        g_bTouchEn = 1;
//        SaveCoordFormat();
//        ResetSetting();
//        ResetRotation();
//        ResetCalibration();
        pReCmdBlock->DataPacket[0] = 0xFC;
        pReCmdBlock->DataPacket[1] = ALL_RESET;
        pReCmdBlock->DataPacket[2] = 00;
        pReCmdBlock->DataPacket[3] = 00;
        pReCmdBlock->DataPacket[4] = 00;
        pReCmdBlock->nDataLen = 5;

        /*发送复位命令*/
#if ((USB1_DEVICE_CONFIG_TYPE & 0x02) == 0x02)
        AnswerCommand(&pCmdBlock->DataPacket[0],pCmdBlock->nDataLen,USB1_EP2_MODE);
#else
        AnswerCommand(&pCmdBlock->DataPacket[0],pCmdBlock->nDataLen,eUsb0Ep2Mode);
#endif
        /*发送格式*/
            uint8_t SendBuff1[64] = {0xFC, FORMAT, FMT_GET_ALL};
#if ((USB1_DEVICE_CONFIG_TYPE & 0x02) == 0x02)
        AddCmdQueueBlock(SendBuff1, sizeof(SendBuff1), USB1_EP2_MODE);
#else
            AddCmdQueueBlock(SendBuff1, sizeof(SendBuff1), eUsb0Ep2Mode);
#endif
        /*发送旋转参数*/
            uint8_t SendBuff2[64]= {0xFC, ROTATION, ROTATION_GET};
#if ((USB1_DEVICE_CONFIG_TYPE & 0x02) == 0x02)
        AddCmdQueueBlock(SendBuff2, sizeof(SendBuff2), USB1_EP2_MODE);
#else
            AddCmdQueueBlock(SendBuff2, sizeof(SendBuff2), eUsb0Ep2Mode);
#endif
        /*发送校准参数*/
            uint8_t SendBuff3[64] = {0xFC, CAL,CAL_GET};
#if ((USB1_DEVICE_CONFIG_TYPE & 0x02) == 0x02)
        AddCmdQueueBlock(SendBuff3, sizeof(SendBuff3), USB1_EP2_MODE);
#else
            AddCmdQueueBlock(SendBuff3, sizeof(SendBuff3), eUsb0Ep2Mode);
#endif

        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    case GET_CAL_COEFF:
    {
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = GET_CAL_COEFF;
        pReCmdBlock->nDataLen = 2;
        eRetVal = CMD_ANSWER_DATA;
    }
    break;

    default:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    break;
    }

    return eRetVal;
}

/*****************************************************************
 * @Function : 校准命令处理
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 处理应答的方式
*****************************************************************/
CmdAnswerType CmdCalibration(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
//    eUsbDevice_t eUsbDev = GetUsbDev(pCmdBlock->eCmdSource);
//    static uint8_t s_bIsCalibrteBegin = 0;
//    static eTouchFormat_t s_eFormatSave;

    CmdAnswerType eRetVal = CMD_ANSWER_NONE;
//    int32_t i = 0;
//    int8_t CalibraData[4] = {0};
//
//    g_stCalibrationData.nStep= pCmdBlock->DataPacket[2];
//    switch(g_stCalibrationData.nStep) //根据校准步骤散转
//    {
//    case NEW_CAL_BEGIN:  //启动校准，返回确认
//    {
//        if(!s_bIsCalibrteBegin)
//        {
//            s_eFormatSave = g_stUsbCoordCfg[eUsbDev].eFormat;
//            //do not know the reason modify g_eUsb0Format here
//            //g_eUsb0Format = eFormatNone;
//            g_bCalEn = 0;
//
//            s_bIsCalibrteBegin = 1; // 设置为真，表示已经启动
//        }
//
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = NEW_CAL;
//        pReCmdBlock->DataPacket[2] = NEW_CAL_BEGIN_RE;
//
//        pReCmdBlock->nDataLen = 3;
//        eRetVal = CMD_ANSWER_DATA;
//    }
//    break;
//
//    case NEW_CAL_END:  // 接收校准数据
//    {
//        if(s_bIsCalibrteBegin)  // 判断是否已经启动了校准
//        {
//            g_stUsbCoordCfg[eUsbDev].eFormat = s_eFormatSave;
//            g_bCalEn = 1;
//            if(pCmdBlock->DataPacket[5] == 1)
//            {
//                for(i=0; i<4; i++)
//                {
//                    g_stCalibrationData.arrCoord[i].nX = (uint16_t)(pCmdBlock->DataPacket[7 + i * 4]) + (uint16_t)(pCmdBlock->DataPacket[6 + i * 4]) * 0x100;
//                    g_stCalibrationData.arrCoord[i].nY = (uint16_t)(pCmdBlock->DataPacket[9 + i * 4]) + (uint16_t)(pCmdBlock->DataPacket[8 + i * 4]) * 0x100;
//                }
//                CalibrationInit();
//            }
//            s_bIsCalibrteBegin = 0;
//        }
//
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = NEW_CAL;
//        pReCmdBlock->DataPacket[2] = NEW_CAL_END_RE;
//
//        for(i = 3; i < 64; i++)
//        {
//            pReCmdBlock->DataPacket[i] = pCmdBlock->DataPacket[i];
//        }
//
//        pReCmdBlock->nDataLen = 64;
//        eRetVal = CMD_ANSWER_DATA;
//    }
//    break;
//
//    case NEW_CAL_SAVE:    // 保存校准数据
//    {
//        pReCmdBlock->DataPacket[5] = pCmdBlock->DataPacket[5];
//        if(pCmdBlock->DataPacket[5] == 1)
//        {
//            SaveCalibrationData();
//        }
//        else
//        {
//            ResetCalibration();
//        }
//
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = NEW_CAL;
//        pReCmdBlock->DataPacket[2] = NEW_CAL_SAVE_RE;
//        pReCmdBlock->DataPacket[3] = 1;
//        pReCmdBlock->DataPacket[5] = 1;
//        pReCmdBlock->nDataLen = 6;
//        eRetVal = CMD_ANSWER_DATA;
//    }
//    break;
//
//    case NEW_CAL_GET:
//    {
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = NEW_CAL;
//        pReCmdBlock->DataPacket[2] = NEW_CAL_GET_RE;
//
//        pReCmdBlock->DataPacket[3] = 17;
//        pReCmdBlock->DataPacket[4] = 0;
//        pReCmdBlock->DataPacket[5] = 1;
//        pReCmdBlock->DataPacket[6] = FN_BYTE(g_stCalibrationData.arrCoord[0].nX, 1);
//        pReCmdBlock->DataPacket[7] = FN_BYTE(g_stCalibrationData.arrCoord[0].nX, 0);
//        pReCmdBlock->DataPacket[8] = FN_BYTE(g_stCalibrationData.arrCoord[0].nY, 1);
//        pReCmdBlock->DataPacket[9] = FN_BYTE(g_stCalibrationData.arrCoord[0].nY, 0);
//        pReCmdBlock->DataPacket[10] = FN_BYTE(g_stCalibrationData.arrCoord[1].nX, 1);
//        pReCmdBlock->DataPacket[11] = FN_BYTE(g_stCalibrationData.arrCoord[1].nX, 0);
//        pReCmdBlock->DataPacket[12] = FN_BYTE(g_stCalibrationData.arrCoord[1].nY, 1);
//        pReCmdBlock->DataPacket[13] = FN_BYTE(g_stCalibrationData.arrCoord[1].nY, 0);
//        pReCmdBlock->DataPacket[14] = FN_BYTE(g_stCalibrationData.arrCoord[2].nX, 1);
//        pReCmdBlock->DataPacket[15] = FN_BYTE(g_stCalibrationData.arrCoord[2].nX, 0);
//        pReCmdBlock->DataPacket[16] = FN_BYTE(g_stCalibrationData.arrCoord[2].nY, 1);
//        pReCmdBlock->DataPacket[17] = FN_BYTE(g_stCalibrationData.arrCoord[2].nY, 0);
//        pReCmdBlock->DataPacket[18] = FN_BYTE(g_stCalibrationData.arrCoord[3].nX, 1);
//        pReCmdBlock->DataPacket[19] = FN_BYTE(g_stCalibrationData.arrCoord[3].nX, 0);
//        pReCmdBlock->DataPacket[20] = FN_BYTE(g_stCalibrationData.arrCoord[3].nY, 1);
//        pReCmdBlock->DataPacket[21] = FN_BYTE(g_stCalibrationData.arrCoord[3].nY, 0);
//        pReCmdBlock->nDataLen = 22;
//        eRetVal = CMD_ANSWER_DATA;
//    }
//    break;
//
//    case NEW_CAL_RESET:
//    {
//        ResetRotation();
//        ResetCalibration();
//        eRetVal = CMD_ANSWER_OK;
//    }
//    break;
//
//    case NEW_CAL_SAVE_DEFAULT:  // 将校准数据保存为默认校准数据
//    {
//        SaveCalibrationData();
//        eRetVal = CMD_ANSWER_OK;
//    }
//    break;
//
//    case NEW_CAL_RESET_DEFAULT:     // 将校准数据重置为出厂值
//    {
//        ResetCalibration();
//        eRetVal = CMD_ANSWER_OK;
//    }
//    break;
//    case NEW_CAL_APP_SET:
//    {
//        /*  0x80:左边偏右        0x40:左边偏左
//            0x20:右边偏右        0x10:右边偏左
//            0x08:上边偏下        0x04:上边偏上
//            0x02:下边偏下        0x01:下边偏上
//        */
//        CalibraData[0] = 0;
//        CalibraData[1] = 0;
//        CalibraData[2] = 0;
//        CalibraData[3] = 0;
//        switch(pCmdBlock->DataPacket[4] & 0xFF)
//        {
//        case 0x80:
//            CalibraData[0] = 1;
//            break;
//        case 0x40:
//            CalibraData[0] = -1;
//            break;
//        case 0x20:
//            CalibraData[1] = 1;
//            break;
//        case 0x10:
//            CalibraData[1] = -1;
//            break;
//        case 0x08:
//            CalibraData[2] = 1;
//            break;
//        case 0x04:
//            CalibraData[2] = -1;
//            break;
//        case 0x02:
//            CalibraData[3] = 1;
//            break;
//        case 0x01:
//            CalibraData[3] = -1;
//            break;
//        default :
//            break;
//        }
//        if (pCmdBlock->DataPacket[4] == 0xff) //0xff表示即将启动校准，将鼠标格式的数据设置为touch0发送
//        {
//            g_stUsbEpsInfo[eUsbDev].arrInterfaceIdxMap[eUsbCfgMouseBit] = g_stUsbEpsInfo[eUsbDev].arrInterfaceIdxMap[eUsbCfgTouch0CommBit];
//        }
//        //注意：g_stCalibrationData.nREOX等4个参数是unsigned类型，经过下面的加减之后，不能为负数，否则会变得很大，导致触摸偏移，导致无触摸假象
//        g_stCalibrationData.nREOX1 = g_stCalibrationData.nREOX1 + CalibraData[0];
//        g_stCalibrationData.nREOX2 = g_stCalibrationData.nREOX2 + CalibraData[1];
//        g_stCalibrationData.nREOY1 = g_stCalibrationData.nREOY1 + CalibraData[2];
//        g_stCalibrationData.nREOY2 = g_stCalibrationData.nREOY2 + CalibraData[3];
//
//        SetCalibrationData();
//        
//        pReCmdBlock->DataPacket[0] = 0xfc;
//        pReCmdBlock->DataPacket[1] = NEW_CAL;
//        pReCmdBlock->DataPacket[2] = NEW_CAL_APP_SET_RE;
//        *((uint32_t*)(&pReCmdBlock->DataPacket[5]))   = g_stCalibrationData.nREOX1; //nREOX1;
//        *((uint32_t*)(&pReCmdBlock->DataPacket[9]))   = g_stCalibrationData.nREOX2; //nREOX2;
//        *((uint32_t*)(&pReCmdBlock->DataPacket[13]))  = g_stCalibrationData.nREOY1; //nREOY1;
//        *((uint32_t*)(&pReCmdBlock->DataPacket[17]))  = g_stCalibrationData.nREOY2; //nREOY2;
//
//        pReCmdBlock->DataPacket[21] = g_pConfigData->eRotationParameter;
//        pReCmdBlock->nDataLen       = 22 ;
//
//        eRetVal = CMD_ANSWER_DATA;
//    }
//    break;
//    default:
//    {
//        eRetVal = CMD_ANSWER_UNKNOWN;
//    }
//    break;
//    }

    return eRetVal;
}

/********************************************************************************************************
 * @Function :擦除操作
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 返回应答的方式
 *******************************************************************************************************/
CmdAnswerType CmdHandleFlash(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_DATA;

    pReCmdBlock->DataPacket[0] = 0xfc ;
    pReCmdBlock->DataPacket[1] = FLASH_COMMAND ;

    switch (pCmdBlock->DataPacket[2])
    {
    case ERASE_BOOT:
//        ClearBootloader();
        pReCmdBlock->DataPacket[2] = ERASE_BOOT_RE;
        break;

    case ERASE_APP :
//        ClearApp();
        pReCmdBlock->DataPacket[2] = ERASE_APP_RE;
        break;

    default:
        pReCmdBlock->DataPacket[2] = 0x83;
        break;
    }
    pReCmdBlock->nDataLen = 3;
    return eRetVal;
}

/********************************************************************************************************
 * @Function :调试操作
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 返回应答的方式
 *******************************************************************************************************/
ATTR_WEAK CmdAnswerType CmdDebug(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    eUsbDevice_t eUsbDev = GetUsbDev(pCmdBlock->eCmdSource);
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;

    MemCpy(pReCmdBlock, pCmdBlock, sizeof(CMD_QUEUE_BLOCK));
    switch (pCmdBlock->DataPacket[2])
    {
        case DEBUG_GET_USB_CONFIG_TYPE:
        {
            pReCmdBlock->DataPacket[5] = g_eUsbCfgType[eUsbDev];
            eRetVal = CMD_ANSWER_DATA;
        }
        break;

        case DEBUG_SET_USB_CONFIG_TYPE:
        {
//            stUsbEnumInfo_t stUsbTouchDevice = {USB0_VID, USB0_PID, Firmware_Version, (eUsbCfgType_t)pCmdBlock->DataPacket[5],
//            g_pConfigData->nPhysicalX, g_pConfigData->nPhysicalY, g_pConfigData->bTouchCoordWidthEn};
//            stUsbTouchDevice.eUsbDev = eUsbDev;
//            StartUsbDev(stUsbTouchDevice);    //开启USB枚举
//            g_stUsbCoordCfg[eUsbDev].eTouchItf = stUsbTouchDevice.eUsbCfgType & eUsbCfgTouch1Bit ? eUsbCfgTouch1Bit : eUsbCfgTouch0CommBit;
        }
        break;

        case DEBUG_REG_USB_PRINTF_REPORT_ID:
        {
//            g_nUsbPrintReportId = pCmdBlock->DataPacket[6];
//            UsbDevCallbackRegister(pCmdBlock->DataPacket[5], Evaluation);
        }
        break;
        case DEBUG_PRINTF_INFO:
        {
            pReCmdBlock->DataPacket[0] = 0xfc;
            pReCmdBlock->DataPacket[1] = DEBUG_COMMAND;
            pReCmdBlock->DataPacket[2] = 0x84;
//            pReCmdBlock->DataPacket[3] = FN_BYTE(g_bRunAlg , 0);
//            pReCmdBlock->DataPacket[4] = FN_BYTE(g_eAlgInit, 0);
//            pReCmdBlock->DataPacket[5] = FN_BYTE(g_pAlgCoreVar->nRealPoint  , 0);
//            pReCmdBlock->DataPacket[6] = FN_BYTE(g_pAlgCoreVar->nValidRayCnt, 1);
//            pReCmdBlock->DataPacket[7] = FN_BYTE(g_pAlgCoreVar->nValidRayCnt, 0);
//            pReCmdBlock->DataPacket[8] = FN_BYTE(g_pAlgCoreVar->nMaskLineCnt, 1);
//            pReCmdBlock->DataPacket[9] = FN_BYTE(g_pAlgCoreVar->nMaskLineCnt, 0);
//            pReCmdBlock->DataPacket[10] = g_pAlgCoreVar->nActiveState;
//            pReCmdBlock->DataPacket[11] = FN_BYTE(g_nDownLight, 1);
//            pReCmdBlock->DataPacket[12] = FN_BYTE(g_nDownLight, 0);
//            pReCmdBlock->DataPacket[13] = FN_BYTE(g_nHaveLight, 1);
//            pReCmdBlock->DataPacket[14] = FN_BYTE(g_nHaveLight, 0);
//            pReCmdBlock->DataPacket[15] = g_nTouchCount;
//            pReCmdBlock->DataPacket[16] = g_stAmbienceVar.eEnvmPattern;
//            pReCmdBlock->DataPacket[17] = FN_BYTE(g_bRecvQTData, 0);
//            pReCmdBlock->DataPacket[18] = FN_BYTE(g_nFrame, 1);
//            pReCmdBlock->DataPacket[19] = FN_BYTE(g_nFrame, 0);
            pReCmdBlock->nDataLen = 20;

            eRetVal = CMD_ANSWER_DATA;
        }
        break;
        case DEBUG_AUTO_AGC_EN:
        {
//            if (0 == pCmdBlock->DataPacket[5] || 1 == pCmdBlock->DataPacket[5])
//            {
//                g_bAutoAgcEn = pCmdBlock->DataPacket[5];
//                g_bAutoAGC = (!pCmdBlock->DataPacket[5]);
//            }
        }
        break;
        default:
        break;
    }
    pReCmdBlock->nDataLen = 3;
    return eRetVal;
}

/*****************************************************************
Function:       ComPenetratingRegion
Description:    触摸穿透
Parameter:
Return:            void
*****************************************************************/
ATTR_WEAK CmdAnswerType ComPenetratingRegion(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;
//    stRegionStruct_t stpRegion;
//    uint8_t nRegionIDTotal = 0;
//    uint8_t nRet = 0;
//    uint8_t i = 0;
//
//    switch (pCmdBlock->DataPacket[2])
//    {
//        case CMD_ADD_REGION:
//        {
//            if(pCmdBlock->DataPacket[3] == 9)
//            {
//                stpRegion.nId = pCmdBlock->DataPacket[5];
//                stpRegion.nXStart = pCmdBlock->DataPacket[6] + (pCmdBlock->DataPacket[7] << 8);
//                stpRegion.nYStart = pCmdBlock->DataPacket[8] + (pCmdBlock->DataPacket[9] << 8);
//                stpRegion.nXEnd = pCmdBlock->DataPacket[10] + (pCmdBlock->DataPacket[11] << 8);
//                stpRegion.nYEnd = pCmdBlock->DataPacket[12] + (pCmdBlock->DataPacket[13] << 8);
//
//                if((stpRegion.nXStart < stpRegion.nXEnd) && (stpRegion.nYStart < stpRegion.nYEnd)
//                    &&(stpRegion.nXStart >= 0) && (stpRegion.nXEnd <= MAX_LOGICAL_VALUE)
//                    &&(stpRegion.nYStart >= 0) && (stpRegion.nYEnd <= MAX_LOGICAL_VALUE))
//                {
//                       nRet = AddRegionArea(&stpRegion);
//                }
//                else
//                {
//                    nRet = 0x03;
//                }
//            }
//            else
//            {
//                nRet = 0x03;
//            }
//
//            pReCmdBlock->DataPacket[0] = 0xfc;
//            pReCmdBlock->DataPacket[1] = CMD_NON_PENETRAING_REGION;
//            pReCmdBlock->DataPacket[2] = CMD_ADD_REGION_RE;
//            pReCmdBlock->DataPacket[3] = 2;
//            pReCmdBlock->DataPacket[4] = 0;
//            pReCmdBlock->DataPacket[5] = nRet;
//            pReCmdBlock->DataPacket[6] = stpRegion.nId;
//            pReCmdBlock->nDataLen = 7;
//            eRetVal = CMD_ANSWER_DATA;
//        }
//        break;
//
//        case CMD_DELETE_REGION:
//        {
//            stpRegion.nId = pCmdBlock->DataPacket[5];
//            nRet = DeleteRegionArea(stpRegion.nId);
//
//            pReCmdBlock->DataPacket[0] = 0xfc;
//            pReCmdBlock->DataPacket[1] = CMD_NON_PENETRAING_REGION;
//            pReCmdBlock->DataPacket[2] = CMD_DELETE_REGION_RE;
//            pReCmdBlock->DataPacket[3] = 2;
//            pReCmdBlock->DataPacket[4] = 0;
//            pReCmdBlock->DataPacket[5] = nRet;
//            pReCmdBlock->DataPacket[6] = stpRegion.nId;
//
//            pReCmdBlock->nDataLen = 7;
//            eRetVal = CMD_ANSWER_DATA;
//        }
//        break;
//
//        case CMD_QUERY_REGION:
//        {
//            stpRegion.nId = pCmdBlock->DataPacket[5];
//            nRet = GetRegionArea(stpRegion.nId, &stpRegion);
//
//            pReCmdBlock->DataPacket[0] = 0xfc;
//            pReCmdBlock->DataPacket[1] = CMD_NON_PENETRAING_REGION;
//            pReCmdBlock->DataPacket[2] = CMD_QUERY_REGION_RE;
//            pReCmdBlock->DataPacket[3] = 10;
//            pReCmdBlock->DataPacket[4] = 0;
//
//            pReCmdBlock->DataPacket[5] = nRet;
//            pReCmdBlock->DataPacket[6] = stpRegion.nId;
//
//            pReCmdBlock->DataPacket[7] = (stpRegion.nXStart & 0xff);
//            pReCmdBlock->DataPacket[8] = (stpRegion.nXStart >> 8);
//
//            pReCmdBlock->DataPacket[9] = (stpRegion.nYStart & 0xff);
//            pReCmdBlock->DataPacket[10]= (stpRegion.nYStart >> 8);
//
//            pReCmdBlock->DataPacket[11]= (stpRegion.nXEnd & 0xff);
//            pReCmdBlock->DataPacket[12]= (stpRegion.nXEnd >> 8);
//
//            pReCmdBlock->DataPacket[13]= (stpRegion.nYEnd & 0xff);
//            pReCmdBlock->DataPacket[14]= (stpRegion.nYEnd >> 8);
//
//            pReCmdBlock->nDataLen = 15;
//            eRetVal = CMD_ANSWER_DATA;
//        }
//        break;
//
//        case CMD_QUERY_REGION_ID:
//        {
//             nRegionIDTotal = 0;
//
//             for(i = 0; i < MAXIMUM_NUMBER_OF_REGIONS; i++)
//             {
//                 if(g_stRegArray.stRegion[i].nId < MAXIMUM_NUMBER_OF_REGIONS)
//                 {
//                    nRegionIDTotal++;
//                    pReCmdBlock->DataPacket[6+nRegionIDTotal] = g_stRegArray.stRegion[i].nId;
//                 }
//             }
//
//            pReCmdBlock->DataPacket[0] = 0xfc;
//            pReCmdBlock->DataPacket[1] = CMD_NON_PENETRAING_REGION;
//            pReCmdBlock->DataPacket[2] = CMD_QUERY_REGION_ID_RE;
//            pReCmdBlock->DataPacket[3] = 2 + nRegionIDTotal;
//            pReCmdBlock->DataPacket[4] = 0;
//            pReCmdBlock->DataPacket[5] = 0;
//            pReCmdBlock->DataPacket[6] = nRegionIDTotal;
//
//            pReCmdBlock->nDataLen = 7 + nRegionIDTotal;
//            eRetVal = CMD_ANSWER_DATA;
//        }
//        break;
//
//        default:
//        {
//            eRetVal = CMD_ANSWER_UNKNOWN;
//
//        }
//        break;
//    }
    return eRetVal;
}

/*
 * USB 转发模拟键盘的数据
 */
CmdAnswerType ComKeyboard(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;

    switch (pCmdBlock->DataPacket[2])
    {
        case KEYBOARD_SENDKEY:
        {
//            SendKeyboardData(eUsbDev0, pCmdBlock->DataPacket[5], pCmdBlock->DataPacket[6]);
//            MemCpy(pReCmdBlock, pCmdBlock, pCmdBlock->nDataLen);
//            pReCmdBlock->DataPacket[2] = KEYBOARD_SENDKEY_RE;
//            pReCmdBlock->nDataLen = pCmdBlock->nDataLen;
            eRetVal = CMD_ANSWER_DATA;
        }
        break;

        case KEYBOARD_KEY_PRESS:
        {
//            pCmdBlock->DataPacket[2] = KEYBOARD_KEY_PRESS_RE;
//            SendKeyboardAKey(eUsbDev0, pCmdBlock->DataPacket[5], pCmdBlock->DataPacket[6]);
//            pReCmdBlock->nDataLen = pCmdBlock->nDataLen;
            eRetVal = CMD_ANSWER_OK;
        }
        break;

        case KEYBOARD_KEY_RELEASE:
        {
//            pCmdBlock->DataPacket[2] = KEYBOARD_KEY_RELEASE_RE;
//            pCmdBlock->DataPacket[5] = 0;
//            pCmdBlock->DataPacket[6] = 0;
//            SendKeyboardAKey(eUsbDev0, pCmdBlock->DataPacket[5], pCmdBlock->DataPacket[6]);
//            pReCmdBlock->nDataLen = pCmdBlock->nDataLen;
            eRetVal = CMD_ANSWER_OK;
        }
        break;

        default:
        {
            eRetVal = CMD_ANSWER_UNKNOWN;
        }
        break;
    }
    return eRetVal;
}


/*
 * 缩放屏幕处理
 */
ATTR_WEAK CmdAnswerType ComZoomScreen(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    eUsbDevice_t eUsbDev = GetUsbDev(pCmdBlock->eCmdSource);
    eUsbDevice_t eRUsbDev = eUsbDev == eUsbDev0 ? eUsbDev1 : eUsbDev0;
    CmdAnswerType eRetVal = CMD_ANSWER_NONE;

    switch(pCmdBlock->DataPacket[2]) //根据命令散转
    {
    case SCREEN_ZOOM_SETTING:
    {
//        //屏幕缩放设置
//        MemSet(&g_stZoom, 0, sizeof(g_stZoom));
//
//        g_stZoom.x = pCmdBlock->DataPacket[5] + (pCmdBlock->DataPacket[6] << 8);
//        g_stZoom.y = pCmdBlock->DataPacket[7] + (pCmdBlock->DataPacket[8] << 8);
//        g_stZoom.w = pCmdBlock->DataPacket[9] + (pCmdBlock->DataPacket[10] << 8);
//        g_stZoom.h = pCmdBlock->DataPacket[11] + (pCmdBlock->DataPacket[12] << 8);
//
//        if((g_stZoom.x + g_stZoom.y + g_stZoom.w + g_stZoom.h) != 0)
//        {
//            if(pCmdBlock->eCmdSource != eUsart3Mode)
//            {
//                g_stZoom.bUsbEnable[eUsbDev] = 0;
//                g_stZoom.bUsbEnable[eRUsbDev] = 1;
//            }
//            else
//            {
//                g_stZoom.bUsbEnable[eUsbDev0] = 1;
//            }
//        }
    }
    break;

    default:
    {
        eRetVal = CMD_ANSWER_UNKNOWN;
    }
    break;
    }
    return eRetVal;
}
/*
 * 硬件自检
 */
ATTR_WEAK CmdAnswerType ComHwCheck(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    return CMD_ANSWER_NONE;
}



CmdAnswerType Communication(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{

    CmdAnswerType eRetVal = CMD_ANSWER_DATA;
    pReCmdBlock->eCmdSource = pCmdBlock->eCmdSource;
    
    switch(pCmdBlock->DataPacket[1])
    {
        case VERSION:
        {
            eRetVal = CmdVersion(pCmdBlock, pReCmdBlock);
        }
        break;

        case ANDROIDTEST:
        {
            eRetVal = CmdSignalTest(pCmdBlock, pReCmdBlock);
        }
        break;

        case HW:
        {
            eRetVal = CmdHardWare(pCmdBlock, pReCmdBlock);
        }
        break;

        case GET_MAXPOINTNUM:
        {
            eRetVal = CmdGetMaxPointNum(pCmdBlock, pReCmdBlock);
        }
        break;

        case CMD_CONFIG:
        {
            eRetVal = CmdDeviceConfig(pCmdBlock, pReCmdBlock);
        }
        break;

        case CMD_DATA:
        case DATANEW:
        {
            eRetVal = CmdData(pCmdBlock, pReCmdBlock);
        }
        break;

        case SLAVE:
        {
            eRetVal = CmdMaster(pCmdBlock, pReCmdBlock);
        }
        break;

        case NEW_SET:
        {
            eRetVal = CmdNewSetting(pCmdBlock, pReCmdBlock);
        }
        break;
        /*
         * Includes aging and Android drive-free protocols
         */
        case USERKEY:
        case USERKEYNEW:
        {
            eRetVal = CmdUserKey(pCmdBlock, pReCmdBlock);
        }
        break;

        case EASY_MEETING:  //和应用软件对接
        {
            eRetVal = CmdEasymeetingRegister(pCmdBlock, pReCmdBlock);
        }
        break;

        case ERASE_KEY:
        {
            eRetVal = CmdEraseBootloaderKey(pCmdBlock, pReCmdBlock);
        }
        break;

        case FORMAT:
        {
            eRetVal = CmdFormat(pCmdBlock, pReCmdBlock);
        }
        break;

        /**处理旋转参数**/
        case ROTATION:
        {
            eRetVal = CmdRotation(pCmdBlock, pReCmdBlock);
        }
        break;

        /**处理旧的校准命令**/
        case CAL:
        {
            eRetVal = CmdOldCalibration(pCmdBlock, pReCmdBlock);
        }
        break;

        /**处理新的校准命令**/
        case NEW_CAL:
        {
            eRetVal = CmdCalibration(pCmdBlock, pReCmdBlock);
        }
        break;

        /*处理配置发点命令*/
        case CMD_SENDCOORD_CONFIG:
        {
            eRetVal = CmdSendCoordConfig(pCmdBlock, pReCmdBlock);
        }
        break;

        case SET_TIME:
        case GET_TIME:
        case SET_RB_EN:
        case GET_CAL_COEFF:
        case SET_ALL_TOUCH_ENABLE:
        case GET_ALL_TOUCH_ENABLE:
        case ALL_RESET:
        {
            eRetVal = CmdSetting(pCmdBlock, pReCmdBlock);
        }
        break;

        case CONFIGPARA:
        {
            eRetVal = CmdConfigPara(pCmdBlock, pReCmdBlock);
        }
        break;

        case BL:
        case BLNEW:
        {
            eRetVal = CmdBootloader(pCmdBlock, pReCmdBlock);
        }
        break;

        case FLASH_COMMAND:
        {
            eRetVal = CmdHandleFlash(pCmdBlock, pReCmdBlock);
        }
        break;

        case DEBUG_COMMAND:
        {
            eRetVal = CmdDebug(pCmdBlock, pReCmdBlock);
        }
        break;
        case CMD_NON_PENETRAING_REGION:
        {
            eRetVal = ComPenetratingRegion(pCmdBlock, pReCmdBlock);
        }break;
        case KEYBOARD:
        {
            eRetVal = ComKeyboard(pCmdBlock, pReCmdBlock);
        }break;
        case CMD_C3_RES:
        {
            eRetVal = ComDropScreen(pCmdBlock, pReCmdBlock);
        }
        break;
        case CMD_6D_RES:
        {
            eRetVal = ComZoomScreen(pCmdBlock, pReCmdBlock);
        }
        break;
        case CMD_B0_RES:
        {
            eRetVal = ComHwCheck(pCmdBlock, pReCmdBlock);
        }
        break;
        default:
        {
            //Usb0RemoteHost();
            eRetVal = CMD_ANSWER_UNKNOWN;
        }
        break;
    }
    
    return eRetVal;;
}
