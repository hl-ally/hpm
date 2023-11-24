#ifndef __API_CMD_QUEUE_H__
#define __API_CMD_QUEUE_H__

#include "stdint.h"
#include "GlobalDataStructure.h"

#define CMD_QUEUE_DEPTH                 32
#define CMD_QUEUE_DATA_MAX_SIZE         64

#define CMD_FC_REPORT_ID                0xFC
#define CMD_FE_REPORT_ID                0xFE
#define CMD_FD_REPORT_ID                0xFD
#define CMD_EA_REPORT_ID                0xEA

typedef struct _CMD_QUEUE_BLOCK
{
    eCmdSource_t eCmdSource;                        // 命令通道
    uint8_t DataPacket[CMD_QUEUE_DATA_MAX_SIZE];  // 命令内容
    int16_t nDataLen;                             // 命令的实际长度
} CMD_QUEUE_BLOCK;

extern int32_t AddCmdQueueBlock(uint8_t *pBuf, int16_t nLen, eCmdSource_t eCmdSoutce);
extern uint8_t GetCmdQueueBlock(CMD_QUEUE_BLOCK *cbk);
extern void ClearCmdQueueBlock(void);
extern CMD_QUEUE_BLOCK  g_stCmdQueue[CMD_QUEUE_DEPTH];
#endif//__API_CMD_QUEUE_H__


