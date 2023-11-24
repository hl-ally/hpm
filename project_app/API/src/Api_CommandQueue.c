#include <stdio.h>
#include <stdint.h>
#include <Api_CommandQueue.h>
#include "hpm_soc.h"
#include "Api_Math.h"


//通信队列
volatile uint8_t g_nCmdQueueCurrentIndex = 0;
volatile uint8_t g_nCmdQueueLength = 0;
CMD_QUEUE_BLOCK  g_stCmdQueue[CMD_QUEUE_DEPTH];

/*********************************************************************************************************
 * @Function : 添加命令数据到CmdQueue中
 * @Parameter: [pnCmdData]数据Buffer
             : [nLen]有效数据长度
             : [eCmdSoutce] 数据源
 * @return   : 成功返回0，错误返回负数
 *********************************************************************************************************/
int32_t AddCmdQueueBlock(uint8_t *pnCmdData, int16_t nLen, eCmdSource_t eCmdSoutce)
{
    CMD_QUEUE_BLOCK cmdBlock;
    uint32_t emptyIndex;

    if (pnCmdData == 0) //数据异常退出
    {
        return -1;
    }
    if (g_nCmdQueueLength >= CMD_QUEUE_DEPTH) //队列异常退出
    {
    	printf("cmd queue full %d\r\n", g_nCmdQueueLength);
        return -2;
    }
    if((nLen < 0) || (nLen > CMD_QUEUE_DATA_MAX_SIZE)) //长度异常退出
    {
        printf("nLen Max = %d,%d\r\n",CMD_QUEUE_DATA_MAX_SIZE,nLen);
        nLen = CMD_QUEUE_DATA_MAX_SIZE;
        //return -3;
    }

    MemSet(cmdBlock.DataPacket, 0, CMD_QUEUE_DATA_MAX_SIZE);
    MemCpy(cmdBlock.DataPacket, pnCmdData, nLen);
    cmdBlock.nDataLen = nLen;
    cmdBlock.eCmdSource = eCmdSoutce;

    emptyIndex = (g_nCmdQueueCurrentIndex + g_nCmdQueueLength) % CMD_QUEUE_DEPTH;
    g_stCmdQueue[emptyIndex] = cmdBlock;
//    __disable_irq();
    disable_global_irq(CSR_MSTATUS_MIE_MASK);
    g_nCmdQueueLength++;
//    __enable_irq();
    enable_global_irq(CSR_MSTATUS_MIE_MASK);

    return 0;
}

/*********************************************************************************************************
 * @Function : 获取命令
 * @Parameter: [pCmdBlock]用于存储信命令的结构体指针
 * @return   : 正确返回1，错误返回0
 *********************************************************************************************************/
uint8_t GetCmdQueueBlock(CMD_QUEUE_BLOCK *pCmdBlock)
{
    if (g_nCmdQueueLength > 0)
    {
        MemCpy(pCmdBlock, &g_stCmdQueue[g_nCmdQueueCurrentIndex], sizeof(CMD_QUEUE_BLOCK));        
//        __disable_irq();
        disable_global_irq(CSR_MSTATUS_MIE_MASK);
        g_nCmdQueueCurrentIndex = (g_nCmdQueueCurrentIndex + 1) % CMD_QUEUE_DEPTH;
        g_nCmdQueueLength--;        
//        __enable_irq();
        enable_global_irq(CSR_MSTATUS_MIE_MASK);
        return 1;
    }
    else
    {
        return 0;
    }
}

/*********************************************************************************************************
 * @Function :清空Queue中的所有命令
 * @Parameter: 无参数
 * @return   : 无返回值
 *********************************************************************************************************/
void ClearCmdQueueBlock(void)
{
    MemSet(g_stCmdQueue, 0, sizeof(CMD_QUEUE_BLOCK)*CMD_QUEUE_DEPTH);
    g_nCmdQueueCurrentIndex = 0;
    g_nCmdQueueLength = 0;
}

