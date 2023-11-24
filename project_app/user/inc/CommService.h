#ifndef __COMMDSERVICE_H__
#define __COMMDSERVICE_H__

#include "Api_CommandQueue.h"

#define CMD_ANSWER              0xFE

//通讯模块的返回值
typedef enum _CMD_CMD_ANSWER_TYPE_
{
    CMD_ANSWER_NONE,
    CMD_ANSWER_UNKNOWN,
    CMD_ANSWER_FAIL,
    CMD_ANSWER_OK,
    CMD_ANSWER_DATA,
    CMD_ANSWER_ROLLBACK,
}CmdAnswerType;

extern void CmdService(void);
extern void AnswerCommand(uint8_t pAnsCmdData[], uint32_t nlength, eCmdSource_t eCmdSource);




#endif

