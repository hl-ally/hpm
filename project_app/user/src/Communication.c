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
#include "GlobalVariables.h"
#include "Api_UsbDevice.h"
#include "hpm_common.h"
#include "hpm_gpio_drv.h"


static volatile int32_t sg_bAdminAccess = 0;  //是否进入管理员模式

CmdAnswerType Communication(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{

    CmdAnswerType eRetVal = CMD_ANSWER_DATA;

    static uint32_t snTotalPacketNum = 0;
    static uint32_t sn_CurPacketNum = 0;
    static uint8_t s_nCmdDownAck = CMD_DOWNLOAD;

    uint32_t nCurPacketNum = 0;
    uint32_t nDataCrc32  = 0;
    uint32_t i = 0;

    pReCmdBlock->DataPacket[0] = CMD_FD_REPORT_ID;
    switch(pCmdBlock->DataPacket[1])
    {
        

        default:
        {
            pReCmdBlock->DataPacket[1] = CMD_NACK;
            pReCmdBlock->DataPacket[2] = NACK_ERROR_CMD;
            pReCmdBlock->nDataLen = 3;
        }
        break;
    }
    
    return eRetVal;;
}
