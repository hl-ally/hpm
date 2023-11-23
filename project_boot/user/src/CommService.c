#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
//#include "SystemDefine.h"
#include "Api_CommandQueue.h"
#include "Api_UsbDevice.h"
#include "Api_Flash.h"
//#include "Api_Gpio.h"
#include "Command.h"
#include "CommService.h"
#include "Communication.h"


/*************************************************************************************************

 *************************************************************************************************/
void AnswerCommand(uint8_t pAnsCmdBuf[], uint32_t nlength, eCmdSource_t eCmdSource)
{
    if ((nlength == 0) || (nlength > CMD_QUEUE_DATA_MAX_SIZE) )
    {
        printf("\r\n boot Send Max Length !nlength = %d,MAX:%d\r\n",nlength,CMD_QUEUE_DATA_MAX_SIZE);
        return;
    }

    printf("source:%d,send:%d,%x,%x ",eCmdSource,nlength,pAnsCmdBuf[0],pAnsCmdBuf[1]);
    switch(eCmdSource)
    {
        case eUsb0Ep1Mode:
        case eUsb0Ep2Mode:
        case eUsb0Ep3Mode:
        case eUsb0Ep4Mode:
        case eUsb0Ep5Mode:
        case eUsb0Ep6Mode:
        case eUsb0Ep7Mode:
        {
            USBEPSendPacket(eUsbDev0, (uint8_t)(eCmdSource-eUsb0Ep1Mode), pAnsCmdBuf, nlength);
        }
        break;

        case eUsb1Ep1Mode:
        case eUsb1Ep2Mode:
        case eUsb1Ep3Mode:
        case eUsb1Ep4Mode:
        case eUsb1Ep5Mode:
        case eUsb1Ep6Mode:
        case eUsb1Ep7Mode:
        {
            USBEPSendPacket(eUsbDev1, (uint8_t)(eCmdSource-eUsb1Ep1Mode), pAnsCmdBuf, nlength);
        }
        break;

        default:
        break;
    }
}

void CmdService(void)
{
    CMD_QUEUE_BLOCK  stCmdBlock;
    CMD_QUEUE_BLOCK  stReCmdBlock;
    CmdAnswerType    eResult;

    if (GetCmdQueueBlock(&stCmdBlock))
    {
        memset(&stReCmdBlock, 0, sizeof(CMD_QUEUE_BLOCK));
        switch (stCmdBlock.DataPacket[0])
        {
            case CMD_FE_REPORT_ID:
            {
                stReCmdBlock.eCmdSource = stCmdBlock.eCmdSource;
                eResult = Communication(&stCmdBlock, &stReCmdBlock);
                switch (eResult)
                {
                    case CMD_ANSWER_DATA:
                    {
                        AnswerCommand(stReCmdBlock.DataPacket, stReCmdBlock.nDataLen, stCmdBlock.eCmdSource);
                    }
                    break;
                    case CMD_ANSWER_UNKNOWN:
                    {
                        stReCmdBlock.DataPacket[0] = CMD_FD_REPORT_ID;
                        stReCmdBlock.DataPacket[1] = CMD_ANSWER;
                        stReCmdBlock.DataPacket[2] = CMD_ANSWER_UNKNOWN;
                        stReCmdBlock.DataPacket[3] = 2;
                        stReCmdBlock.DataPacket[4] = stCmdBlock.DataPacket[4];
                        stReCmdBlock.DataPacket[5] = stCmdBlock.DataPacket[1];
                        stReCmdBlock.DataPacket[6] = stCmdBlock.DataPacket[2];
                        stReCmdBlock.nDataLen = 7;
                        AnswerCommand(stReCmdBlock.DataPacket, stReCmdBlock.nDataLen, stReCmdBlock.eCmdSource);
                    }
                    break;
                    case CMD_ANSWER_FAIL:
                    {
                        stReCmdBlock.DataPacket[0] = CMD_FD_REPORT_ID;
                        stReCmdBlock.DataPacket[1] = CMD_ANSWER;
                        stReCmdBlock.DataPacket[2] = CMD_ANSWER_FAIL;
                        stReCmdBlock.DataPacket[3] = 2;
                        stReCmdBlock.DataPacket[4] = stCmdBlock.DataPacket[4];
                        stReCmdBlock.DataPacket[5] = stCmdBlock.DataPacket[1];
                        stReCmdBlock.DataPacket[6] = stCmdBlock.DataPacket[2];
                        stReCmdBlock.nDataLen = 7;
                        AnswerCommand(stReCmdBlock.DataPacket, stReCmdBlock.nDataLen, stReCmdBlock.eCmdSource);
                    }
                    break;
                    case CMD_ANSWER_OK:
                    {
                        stReCmdBlock.DataPacket[0] = CMD_FD_REPORT_ID;
                        stReCmdBlock.DataPacket[1] = CMD_ANSWER;
                        stReCmdBlock.DataPacket[2] = CMD_ANSWER_OK;
                        stReCmdBlock.DataPacket[3] = 2;
                        stReCmdBlock.DataPacket[4] = stCmdBlock.DataPacket[4];
                        stReCmdBlock.DataPacket[5] = stCmdBlock.DataPacket[1];
                        stReCmdBlock.DataPacket[6] = stCmdBlock.DataPacket[2];
                        stReCmdBlock.nDataLen = 7;
                        AnswerCommand(stReCmdBlock.DataPacket, stReCmdBlock.nDataLen, stReCmdBlock.eCmdSource);
                    }
                    break;
                    default:
                    {
                        
                    }
                    break;
                }
            }
            break;
            
            case CMD_FD_REPORT_ID:
            {

            }
            break;
            case CMD_FC_REPORT_ID:
            {
                
            }
            break;
            
            default:
            {
                
            }
            break;
        }    
    }
}




