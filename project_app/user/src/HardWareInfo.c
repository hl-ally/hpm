#include "HardWareInfo.h"
#include "GlobalVariables.h"
#include "TouchSignal.h"
#include "Api_Math.h"
//#include "scan.h"
#include "stdio.h"

const stPosF_t g_arrBottomPosF[] = BOTTOM_LED_POS;
const stPosF_t g_arrTopPosF[] = TOP_LED_POS;
const stPosF_t g_arrLeftPosF[] = LEFT_LED_POS;
const stPosF_t g_arrRightPosF[] = RIGHT_LED_POS;


// 为了编译通过，临时函数
int32_t GetADCIndex(eAxis_t eAxis, int32_t nTLed, int32_t nRLed)
{
    int32_t nLedADCOffset = 0;
//    //confirm nTLed exist
//    if (nTLed >= g_stScanSeg[eAxis][nRLed / R_ADC_GROUP].nStart && nTLed <= g_stScanSeg[eAxis][nRLed / R_ADC_GROUP].nEnd)
//    {
//        nLedADCOffset = g_nBlockStartTLedTimes[eAxis][nRLed / R_ADC_GROUP] + nTLed - g_stScanSeg[eAxis][nRLed / R_ADC_GROUP].nStart;
//
//        nLedADCOffset = (nRLed % R_ADC_GROUP) + nLedADCOffset * ADC_CH_TOTAL;
//    }

    return nLedADCOffset;
}


/********************************************************************************************************
 * @Function :初始化四边的Led位置
 * @Parameter:[stLedPosArray] LED位置数组
              [eEdge] 为某一边，以主板为参考，主板所在边为下边，主板相临边为左边
              [nMaxCount] arrPos buffer存储LED最大个数
 * @return 返回对应轴LED数量
 ********************************************************************************************************/
int32_t InitLedPosition(stPosF_t* stLedPosArray, int32_t nMaxCount, eEdge_t eEdge)
{
    int32_t nCount = 0;
    int32_t i = 0;
    const stPosF_t *arrPosTmp = NULL;

    switch(eEdge)
    {
    case eBottom:
    {
        //Read Poistion array from Hardwareinfo.h
        arrPosTmp = g_arrBottomPosF;
        nCount = FN_MIN(nMaxCount, sizeof(g_arrBottomPosF) / sizeof(stPosF_t));
    }
    break;

    case eTop:
    {
        arrPosTmp = g_arrTopPosF;
        nCount = FN_MIN(nMaxCount, sizeof(g_arrTopPosF) / sizeof(stPosF_t));
    }
    break;

    case eLeft:
    {
        arrPosTmp = g_arrLeftPosF;
        nCount = FN_MIN(nMaxCount, sizeof(g_arrLeftPosF) / sizeof(stPosF_t));
    }
    break;

    case eRight:
    {
        arrPosTmp = g_arrRightPosF;
        nCount = FN_MIN(nMaxCount, sizeof(g_arrRightPosF) / sizeof(stPosF_t));
    }
    break;

    default:
    {
    }
    break;
    }

    for (i = 0; arrPosTmp != NULL && i < nCount; i++)
    {
        stLedPosArray[i] = arrPosTmp[i];
    }

    return nCount;
}

/********************************************************************************************************
 * @Function :初始化光线灯管之间的关系，每条光线的发射与接收灯编号
 * @Parameter:[nIndexArray]: 数量、发射灯索引、该发射所有接数灯索引、数据4字节补齐
              nRLedStart: 在当前边中，当前灯的索引
              eEdge: 边缘信息，主板为参考，主板是下边，主板相临边为左边
              nMaxCount: arrIndex的最大长度
 * @return 返回光线数量
 ********************************************************************************************************/
int32_t InitTLedRLedRelation(uint16_t* nTRLedRelationIndex, int32_t nMaxCount, eEdge_t eEdge, int32_t nRLedStart)
{
    uint16_t* pCount = nTRLedRelationIndex;
    int32_t nRLedEnd = 0;
    int32_t i = 0, j = 0;
    int32_t nCount = 0;
    stSgmH_t sgm = {0, 0};

    switch(eEdge)
    {
    case eBottom:
    {
        sgm.nStart = 0;
        sgm.nEnd = g_nTotal[X] - 1;
        //Get the End of Receive array
        //if start.x was not in the range of certain scan angle,then move to next
        //use the same way to get end.x
        nRLedEnd = FN_MIN(nRLedStart + ADC_CH_TOTAL, g_nTotal[X]) - 1;

        while(!(FN_ABS(g_fMaxRate[X] * (g_arrPos[eTop][nRLedStart].fY - g_arrPos[eBottom][sgm.nStart].fY)) -
                FN_ABS(g_arrPos[eTop][nRLedStart].fX - g_arrPos[eBottom][sgm.nStart].fX) > INFINITE_MIN))
        {
            sgm.nStart++;
        }
        while(!(FN_ABS(g_fMaxRate[X] * (g_arrPos[eTop][nRLedEnd].fY - g_arrPos[eBottom][sgm.nEnd].fY)) -
                FN_ABS(g_arrPos[eTop][nRLedEnd].fX - g_arrPos[eBottom][sgm.nEnd].fX) > INFINITE_MIN))
        {
            sgm.nEnd--;
        }

        for(i = sgm.nStart; i <= sgm.nEnd && nCount < nMaxCount - 2; i++)
        {
            //Each segment format[light num][LED Index][PD Index0][PD Index1]...
            pCount = &nTRLedRelationIndex[nCount];
            nTRLedRelationIndex[nCount++] = 0;
            nTRLedRelationIndex[nCount++] = i;
            for(j = nRLedStart; j <= nRLedEnd && nCount < nMaxCount; j++)
            {
                if(FN_ABS(g_fMaxRate[X] * (g_arrPos[eTop][j].fY - g_arrPos[eBottom][i].fY)) -
                        FN_ABS(g_arrPos[eTop][j].fX - g_arrPos[eBottom][i].fX) > INFINITE_MIN)
                {
                    if(i < EDGE_COUNT_X && j < EDGE_COUNT_X || i >= g_nTotal[X] - EDGE_COUNT_X && j >= g_nTotal[X] - EDGE_COUNT_X
                            || j % SIGNAL_INTERVAL_X == i % SIGNAL_INTERVAL_X)
                    {
                        //consider edge LED may not support 1T-MR,1R-MT??
                        //Light Num ++
                        nTRLedRelationIndex[nCount++] = j + g_nTotal[X];
                        (*pCount)++;
                    }
                }
            }
            nCount += ((*pCount) % 2);
        }
    }

    break;

    case eLeft:
    {
        sgm.nStart = 0;
        sgm.nEnd = g_nTotal[Y] - 1;
        nRLedEnd = FN_MIN(nRLedStart + ADC_CH_TOTAL, g_nTotal[Y]) - 1;
        while(!(FN_ABS(g_fMaxRate[Y] * (g_arrPos[eLeft][nRLedStart].fX - g_arrPos[eRight][sgm.nStart].fX)) -
                FN_ABS(g_arrPos[eLeft][nRLedStart].fY - g_arrPos[eRight][sgm.nStart].fY) > INFINITE_MIN))
        {
            sgm.nStart++;
        }

        while(!(FN_ABS(g_fMaxRate[Y] * (g_arrPos[eLeft][nRLedEnd].fX - g_arrPos[eRight][sgm.nEnd].fX)) -
                FN_ABS(g_arrPos[eLeft][nRLedEnd].fY - g_arrPos[eRight][sgm.nEnd].fY) > INFINITE_MIN))
        {
            sgm.nEnd--;
        }
        //printf("[%3d--%3d:\t\t%3d--%3d]:\n",nRLedStart,nRLedEnd,sgm.nStart,sgm.nEnd);
        for(i = sgm.nStart; i <= sgm.nEnd && nCount < nMaxCount - 2; i++)
        {
            pCount = &nTRLedRelationIndex[nCount];
            nTRLedRelationIndex[nCount++] = 0;
            nTRLedRelationIndex[nCount++] = i + g_nTotal[X] * 2 + g_nTotal[Y];
            for(j = nRLedStart; j <= nRLedEnd && nCount < nMaxCount; j++)
            {
                if(FN_ABS(g_fMaxRate[Y] * (g_arrPos[eLeft][j].fX - g_arrPos[eRight][i].fX)) - FN_ABS(g_arrPos[eLeft][j].fY - g_arrPos[eRight][i].fY) > INFINITE_MIN)
                {
                    if(i < EDGE_COUNT_Y && j < EDGE_COUNT_Y || i >= g_nTotal[Y] - EDGE_COUNT_Y && j >= g_nTotal[Y] - EDGE_COUNT_Y
                            || j % SIGNAL_INTERVAL_Y == i % SIGNAL_INTERVAL_Y)
                    {
                        nTRLedRelationIndex[nCount++] = j  + g_nTotal[X] * 2;
                        (*pCount)++;
                    }
                }
            }
            //printf("\t%d:%d\n",i,(*pCount));
            nCount += ((*pCount) % 2);
        }
    }

    break;

    case eTop:
    {
    }
    break;

    case eRight:
    {
    }
    break;

    default:
    {
    }
    break;
    }

    return nCount;
}

/*********************************************************************************************************
 * @Function :初始化ADC Buffer 中用于用于测试ADC的索引值
 * @Parameter:[]nTestAdcIndex] 存储方式为先数量、发射灯索引、接数灯索引、数据4字节补齐
             :[nMaxCount]: arrIndex的最大长度
             :[eAxis] 坐标轴
             :[eEdge]: 为某一边，以主板为参考，主板所在边为下边，主板相临边为左边
 * @return 返回索引的数量
 ********************************************************************************************************/
int32_t InitTestAdcIndex(uint16_t* nTestAdcIndex, int32_t nMaxTestAdcIndexCount, eAxis_t eAxis, int32_t eDir)
{
    int32_t nRLed = 0;
    int32_t i = 0;

    nMaxTestAdcIndexCount = FN_MIN(nMaxTestAdcIndexCount, g_nTotal[eAxis]);
    for(i = 0; i < nMaxTestAdcIndexCount; i++)
    {
        nRLed = FN_MAX(FN_MIN(g_nTotal[eAxis] - 1, i - g_nInterVal[eAxis][eDir]), 0);
        nTestAdcIndex[i] = g_nAllAdcIndexIndex[(uint16_t)GetADCIndex(eAxis, i, nRLed)];
    }

    return nMaxTestAdcIndexCount;
}

/*********************************************************************************************************
 * @Function :初始化AGC Buffer 中用于测试的AGC的索引
 * @Parameter:[nTestAgcIndex]: 存储方式为先数量、发射灯索引、接数灯索引、数据4字节补齐
             :[nMaxCount]: arrIndex的最大长度
             :[eAxis] 坐标轴
             :[eEdge]: 为某一边，以主板为参考，主板所在边为下边，主板相临边为左边
 * @return 返回索引的数量
 ********************************************************************************************************/
int32_t InitTestAgcIndex(uint16_t* nTestAgcIndex, int32_t nMaxCount, eAxis_t eAxis, int32_t eDir)
{
    int32_t nRLed = 0;
    int32_t i = 0;

    nMaxCount = FN_MIN(nMaxCount, g_nTotal[eAxis]);
    for(i = 0; i < nMaxCount; i++)
    {
        nRLed = FN_MAX(FN_MIN(g_nTotal[eAxis] - 1, i - g_nInterVal[eAxis][eDir]), 0);
//        nTestAgcIndex[i] = (uint16_t)GetAGCIndex(eAxis, i, nRLed);
    }

    return nMaxCount;
}

/*********************************************************************************************************
 * @Function :初始化AGC Buffer 中用于测试的AGC的索引
 * @Parameter:[nTestRIdIndex]: 存储方式为先数量、发射灯索引、接数灯索引、数据4字节补齐
             :[nMaxCount]: arrIndex的最大长度
             :[eAxis] 坐标轴
             :[eEdge]: 为某一边，以主板为参考，主板所在边为下边，主板相临边为左边
 * @return 返回索引的数量
 ********************************************************************************************************/
int32_t InitTestRIdIndex(uint16_t* nTestRIdIndex, int32_t nMaxCount, eAxis_t eAxis, int32_t eDir)
{
    int32_t nRLed = 0;
    int32_t i = 0;

    nMaxCount = FN_MIN(nMaxCount, g_nTotal[eAxis]);
    for(i = 0; i < nMaxCount; i++)
    {
        nRLed = FN_MAX(FN_MIN(g_nTotal[eAxis] - 1, i - g_nInterVal[eAxis][eDir]), 0);
        nTestRIdIndex[i] = nRLed;
    }

    return nMaxCount;
}

/*********************************************************************************************************
 * @Function :根据扫描角度，除边缘外，斜扫相对灯间隔最小值
 * @Parameter:无参数
 * @return 返回光线数量
 ********************************************************************************************************/
int32_t InitLedMaxInterval(void)
{
    int32_t i = 0, j = 0;
    int32_t nDir = 0;

    for(nDir = 0; nDir < OBQ_COUNT; nDir++)
    {
        for(i = 0; i < g_nTotal[X]; i++)
        {
            j = FN_MIN(g_nTotal[X] - 1, FN_MAX(0, i - g_nInterVal[X][nDir]));
            if(!(FN_ABS(g_fMaxRate[X] * (g_arrPos[eTop][j].fY - g_arrPos[eBottom][i].fY))
                    - FN_ABS(g_arrPos[eTop][j].fX - g_arrPos[eBottom][i].fX) > INFINITE_MIN)
                    || !(i < EDGE_COUNT_X
                         && j < EDGE_COUNT_X
                         || i >= g_nTotal[X] - EDGE_COUNT_X
                         && j >= g_nTotal[X] - EDGE_COUNT_X
                         || j % SIGNAL_INTERVAL_X == i % SIGNAL_INTERVAL_X))
            {
                if(g_nInterVal[X][nDir] > 0)
                {
                    g_nInterVal[X][nDir]--;
                    i--;
                }
                else if(g_nInterVal[X][nDir] < 0)
                {
                    g_nInterVal[X][nDir]++;
                    i--;
                }
            }
        }
    }

    for(nDir = 0; nDir < OBQ_COUNT; nDir++)
    {
        for(i = 0; i < g_nTotal[Y]; i++)
        {
            j = FN_MIN(g_nTotal[Y] - 1, FN_MAX(0, i - g_nInterVal[Y][nDir]));
            if(!(FN_ABS(g_fMaxRate[Y] * (g_arrPos[eRight][i].fX - g_arrPos[eLeft][j].fX)) - FN_ABS(g_arrPos[eRight][i].fY - g_arrPos[eLeft][j].fY) > INFINITE_MIN)
                    || !(i < EDGE_COUNT_Y && j < EDGE_COUNT_Y || i >= g_nTotal[Y] - EDGE_COUNT_Y && j >= g_nTotal[Y] - EDGE_COUNT_Y || j % SIGNAL_INTERVAL_Y == i % SIGNAL_INTERVAL_Y))
            {
                if(g_nInterVal[Y][nDir] > 0)
                {
                    g_nInterVal[Y][nDir]--;
                    i--;
                }
                else if(g_nInterVal[Y][nDir] < 0)
                {
                    g_nInterVal[Y][nDir]++;
                    i--;
                }
            }
        }
    }

    return 0;
}

/*********************************************************************************************************
 * @Function :初始化数据Index用于找出有使用的灯线索引
 * @Parameter:[nAdcIndexArray] 存储所有有效信号在ADC Buffer中的索引
             :[nMaxCount] arrIndex的最大长度
             :[eEdge] 为某一边，以主板为参考，主板所在边为下边，主板相临边为左边
             :[nRLedStart] 坐标轴
 * @return 返回光线数量
 ******************************************************************************************************/
int32_t InitAllAdcIndex(uint16_t* nAdcIndexArray, int32_t nMaxCount, eEdge_t eEdge, int32_t nRLedStart)
{
    int32_t nStartT, nEndT;
    int32_t nRLedEnd = 0;
    int32_t i = 0, j = 0;
    int32_t nCount = 0;

    switch(eEdge)
    {
    case eBottom:
    {
        nStartT = g_stScanSeg[X][nRLedStart / R_ADC_GROUP].nStart;
        nEndT = g_stScanSeg[X][nRLedStart / R_ADC_GROUP].nEnd;
        nRLedEnd = FN_MIN(nRLedStart + ADC_CH_TOTAL, g_nTotal[X]) - 1;

        for(i = nStartT; i <= nEndT && nCount < nMaxCount; i++)
        {
            for(j = nRLedStart; j <= nRLedEnd && nCount < nMaxCount; j++)
            {
                if(FN_ABS(g_fMaxRate[X] * (g_arrPos[eTop][j].fY - g_arrPos[eBottom][i].fY)) - FN_ABS(g_arrPos[eTop][j].fX - g_arrPos[eBottom][i].fX) > INFINITE_MIN)
                {
                    if(i < EDGE_COUNT_X && j < EDGE_COUNT_X || i >= g_nTotal[X] - EDGE_COUNT_X && j >= g_nTotal[X] - EDGE_COUNT_X
                            || j % SIGNAL_INTERVAL_X == i % SIGNAL_INTERVAL_X)
                    {
                        nAdcIndexArray[nCount++] = (uint16_t)GetADCIndex(X, i, j);
                    }
                }
            }
        }
    }
    break;

    case eLeft:
    {
        nStartT = g_stScanSeg[Y][nRLedStart / R_ADC_GROUP].nStart;
        nEndT = g_stScanSeg[Y][nRLedStart / R_ADC_GROUP].nEnd;
        nRLedEnd = FN_MIN(nRLedStart + ADC_CH_TOTAL, g_nTotal[Y]) - 1;

        for(i = nStartT; i <= nEndT && nCount < nMaxCount; i++)
        {
            for(j = nRLedStart; j <= nRLedEnd && nCount < nMaxCount; j++)
            {
                if(FN_ABS(g_fMaxRate[Y] * (g_arrPos[eRight][i].fX - g_arrPos[eLeft][j].fX)) - FN_ABS(g_arrPos[eRight][i].fY - g_arrPos[eLeft][j].fY) > INFINITE_MIN)
                {
                    if(i < EDGE_COUNT_Y && j < EDGE_COUNT_Y || i >= g_nTotal[Y] - EDGE_COUNT_Y && j >= g_nTotal[Y] - EDGE_COUNT_Y
                            || j % SIGNAL_INTERVAL_Y == i % SIGNAL_INTERVAL_Y)
                    {
                        nAdcIndexArray[nCount++] = (uint16_t)GetADCIndex(Y, i, j);
                    }
                }
            }
        }
    }
    break;

    case eTop:
    {
    }
    break;

    case eRight:
    {
    }
    break;

    default:
    {
    }
    break;
    }

    return nCount;
}

/*********************************************************************************************************
 * @Function :初始化制定接收灯在ADC Buffer列表中，对应的所有接收灯ADC索引的索引下标的标号
 * @Parameter:nIndexIndexArray: 存储前n个一发多收中有效信号的数量
              nRLedStart: 在当前边中，当前灯的索引
              eEdge: 为某一边，以主板为参考，主板所在边为下边，主板相临边为左边
              nMaxCount: arrIndex的最大长度
              nIndex: 起始的下标
 * @return 返回该接收灯组使用块的数量，相当对应发送灯的数量
 ********************************************************************************************************/
int32_t InitTLedRLedLableCountSum(stTONIdxInfo_t* nIndexIndexArray, int32_t nMaxCount, eEdge_t eEdge, int32_t nRLedStart, int32_t nStartIndex)
{
    int32_t nTLedStart;
    int32_t nTLedEnd;
    int32_t nRLedEnd = 0;
    int32_t i = 0, j = 0;
    int32_t nCount = 0;
    int32_t nIndex = nStartIndex;

    switch(eEdge)
    {
    case eBottom:
    {
        nTLedStart = g_stScanSeg[X][nRLedStart / R_ADC_GROUP].nStart;
        nTLedEnd = g_stScanSeg[X][nRLedStart / R_ADC_GROUP].nEnd;
        nRLedEnd = FN_MIN(nRLedStart + ADC_CH_TOTAL, g_nTotal[X]) - 1;

        for(i = nTLedStart; i <= nTLedEnd && nCount < nMaxCount; i++)
        {
            int16_t nCh = (-1);
            for(j = nRLedStart; j <= nRLedEnd && nCount < nMaxCount; j++)
            {
                if(FN_ABS(g_fMaxRate[X] * (g_arrPos[eTop][j].fY - g_arrPos[eBottom][i].fY)) - FN_ABS(g_arrPos[eTop][j].fX - g_arrPos[eBottom][i].fX) > INFINITE_MIN)
                {
                    //valid Lights
                    if(i < EDGE_COUNT_X && j < EDGE_COUNT_X || i >= g_nTotal[X] - EDGE_COUNT_X && j >= g_nTotal[X] - EDGE_COUNT_X
                            || j % SIGNAL_INTERVAL_X == i % SIGNAL_INTERVAL_X)
                    {
                        if((-1) == nCh)
                        {
                            nCh = j % 9;
                        }
                        nIndex++;
                    }
                }
            }
//            printf("count: %4d  Ch:%3d  Idx:%5d\n",nCount,nCh,nIndex);
            nIndexIndexArray[nCount].nCH = nCh;
            nIndexIndexArray[nCount++].nIndex = nIndex;
        }
    }
    break;

    case eLeft:
    {
        nTLedStart = g_stScanSeg[Y][nRLedStart / R_ADC_GROUP].nStart;
        nTLedEnd = g_stScanSeg[Y][nRLedStart / R_ADC_GROUP].nEnd;
        nRLedEnd = FN_MIN(nRLedStart + ADC_CH_TOTAL, g_nTotal[Y]) - 1;

        for(i = nTLedStart; i <= nTLedEnd && nCount < nMaxCount; i++)
        {
            int16_t nCh = (-1);
            for(j = nRLedStart; j <= nRLedEnd && nCount < nMaxCount; j++)
            {
                if(FN_ABS(g_fMaxRate[Y] * (g_arrPos[eRight][i].fX - g_arrPos[eLeft][j].fX)) - FN_ABS(g_arrPos[eRight][i].fY - g_arrPos[eLeft][j].fY) > INFINITE_MIN)
                {
                    if(i < EDGE_COUNT_Y && j < EDGE_COUNT_Y || i >= g_nTotal[Y] - EDGE_COUNT_Y && j >= g_nTotal[Y] - EDGE_COUNT_Y
                            || j % SIGNAL_INTERVAL_Y == i % SIGNAL_INTERVAL_Y)
                    {
                        if((-1) == nCh)
                        {
                            nCh = j % 9;
                        }
                        nIndex++;
                    }
                }
            }
//            printf("count: %4d  Ch:%3d  Idx:%5d\n",nCount,nCh,nIndex);
            nIndexIndexArray[nCount].nCH = nCh;
            nIndexIndexArray[nCount++].nIndex = nIndex;
			
        }		
    }
    break;

    case eTop:
    {
    }
    break;

    case eRight:
    {
    }
    break;

    default:
    {
    }
    break;
    }

    return nCount;
}

/*********************************************************************************************************
 * @Function :初始化每组一发多收所依据的索引值
 * @Parameter:nRBlockStartIndexArray: 存储前n组一发多收中每组AGC所依据的Adc的索引
              nRLedStart: 在当前边中，当前灯的索引
              eEdge: 为某一边，以主板为参考，主板所在边为下边，主板相临边为左边
              nMaxCount: arrIndex的最大长度
 * @return 返回光线数量
 ********************************************************************************************************/
int32_t InitRBlockAdcStartIndex(uint16_t* nRBlockStartIndexArray, int32_t nIndexArrayMaxCount, eEdge_t eEdge, int32_t nRLedStart)
{
    int32_t nStart = 0, nEnd = 0;
    int32_t nStartT, nEndT;
    int32_t nRLedEnd = 0;
    int32_t i = 0, j = 0;
    int32_t nCount = 0;

    switch(eEdge)
    {
    case eBottom:
    {
        nStartT = g_stScanSeg[X][nRLedStart / R_ADC_GROUP].nStart;
        nEndT = g_stScanSeg[X][nRLedStart / R_ADC_GROUP].nEnd;
        nRLedEnd = FN_MIN(nRLedStart + ADC_CH_TOTAL, g_nTotal[X]) - 1;

        for(i = nStartT; i <= nEndT && nCount < nIndexArrayMaxCount; i++)
        {
            for(j = nRLedStart; j <= nRLedEnd; j++)
            {
                if(FN_ABS(g_fMaxRate[X] * (g_arrPos[eTop][j].fY - g_arrPos[eBottom][i].fY))
                        - FN_ABS(g_arrPos[eTop][j].fX - g_arrPos[eBottom][i].fX) > INFINITE_MIN)
                {
                    nStart = j;
                    break;
                }
            }

            for(j = nRLedEnd; j >= nRLedStart; j--)
            {
                if(FN_ABS(g_fMaxRate[X] * (g_arrPos[eTop][j].fY - g_arrPos[eBottom][i].fY))
                        - FN_ABS(g_arrPos[eTop][j].fX - g_arrPos[eBottom][i].fX) > INFINITE_MIN)
                {
                    nEnd = j;
                    break;
                }
            }

            if(nStart == nEnd)
            {
                nRBlockStartIndexArray[nCount++] = (uint16_t)GetADCIndex(X, i, nStart);
            }
            else
            {
                //compare slop,if Start_Slop > End_Slop => then write corresponding index of Start PD
                if(FN_ABS((g_arrPos[eTop][nStart].fX - g_arrPos[eBottom][i].fX) / (g_arrPos[eTop][nStart].fY - g_arrPos[eBottom][i].fY))
                        - FN_ABS((g_arrPos[eTop][nEnd].fX - g_arrPos[eBottom][i].fX) / (g_arrPos[eTop][nEnd].fY - g_arrPos[eBottom][i].fY)) > INFINITE_MIN)
                {
                    nRBlockStartIndexArray[nCount++] = (uint16_t)GetADCIndex(X, i, nStart);
                }
                else
                {
                    nRBlockStartIndexArray[nCount++] = (uint16_t)GetADCIndex(X, i, nEnd);
                }
            }
        }
    }
    break;

    case eLeft:
    {
        nStartT = g_stScanSeg[Y][nRLedStart / R_ADC_GROUP].nStart;
        nEndT = g_stScanSeg[Y][nRLedStart / R_ADC_GROUP].nEnd;
        nRLedEnd = FN_MIN(nRLedStart + ADC_CH_TOTAL, g_nTotal[Y]) - 1;

        for(i = nStartT; i <= nEndT && nCount < nIndexArrayMaxCount; i++)
        {
            for(j = nRLedStart; j <= nRLedEnd; j++)
            {
                if(FN_ABS(g_fMaxRate[Y] * (g_arrPos[eRight][i].fX - g_arrPos[eLeft][j].fX)) - FN_ABS(g_arrPos[eRight][i].fY - g_arrPos[eLeft][j].fY) > INFINITE_MIN)
                {
                    nStart = j;
                    break;
                }
            }

            for(j = nRLedEnd; j >= nRLedStart; j--)
            {
                if(FN_ABS(g_fMaxRate[Y] * (g_arrPos[eRight][i].fX - g_arrPos[eLeft][j].fX)) - FN_ABS(g_arrPos[eRight][i].fY - g_arrPos[eLeft][j].fY) > INFINITE_MIN)
                {
                    nEnd = j;
                    break;
                }
            }

            if(nStart == nEnd)
            {
                nRBlockStartIndexArray[nCount++] = (uint16_t)GetADCIndex(Y, i, nStart);
            }
            else
            {
                if(FN_ABS((g_arrPos[eRight][nStart].fY - g_arrPos[eLeft][j].fY) / (g_arrPos[eRight][nStart].fX - g_arrPos[eLeft][j].fX))
                        - FN_ABS((g_arrPos[eRight][nEnd].fY - g_arrPos[eLeft][j].fY) / (g_arrPos[eRight][nEnd].fX - g_arrPos[eLeft][j].fX)) > INFINITE_MIN)
                {
                    nRBlockStartIndexArray[nCount++] = (uint16_t)GetADCIndex(Y, i, nStart);
                }
                else
                {
                    nRBlockStartIndexArray[nCount++] = (uint16_t)GetADCIndex(Y, i, nEnd);
                }
            }
        }
    }
    break;

    case eTop:
    {
    }
    break;

    case eRight:
    {
    }
    break;

    default:
    {
    }
    break;
    }

    return nCount;
}

/*******************************************************************************************************
 * @Function :初始化每一组接收灯对应的发射灯的范围
 * @Parameter: stScanSegment: 存放每组接收灯的起始发射与结束发射灯标号
               nMaxCount: stScanSegment的最单位数量
               eAxis: 坐标数量
 * @return   : 返回实际的数量
 ********************************************************************************************************/
int32_t InitScanBlockRange(stSgmH_t* stScanSegment, int32_t nMaxCount, eAxis_t eAxis)
{
    int32_t nRLedEnd = 0;
    int32_t nCount = 0;
    int32_t i = 0;

    switch(eAxis)
    {
    case X:
    {
        for(i = 0; i < g_nTotal[eAxis] && nCount < nMaxCount; i += R_ADC_GROUP)
        {
            stScanSegment[nCount].nStart = 0;
            stScanSegment[nCount].nEnd = g_nTotal[eAxis] - 1;
            nRLedEnd = FN_MIN(i + R_ADC_GROUP, g_nTotal[eAxis]) - 1;
            while(!(FN_ABS(g_fMaxRate[X] * (g_arrPos[eTop][i].fY - g_arrPos[eBottom][stScanSegment[nCount].nStart].fY)) -
                    FN_ABS(g_arrPos[eTop][i].fX - g_arrPos[eBottom][stScanSegment[nCount].nStart].fX) > INFINITE_MIN))
            {
                stScanSegment[nCount].nStart++;
            }

            while(!(FN_ABS(g_fMaxRate[X] * (g_arrPos[eTop][nRLedEnd].fY - g_arrPos[eBottom][stScanSegment[nCount].nEnd].fY)) -
                    FN_ABS(g_arrPos[eTop][nRLedEnd].fX - g_arrPos[eBottom][stScanSegment[nCount].nEnd].fX) > INFINITE_MIN))
            {
                stScanSegment[nCount].nEnd--;
            }
            nCount++;
        }
    }
    break;

    case Y:
    {
        for(i = 0; i < g_nTotal[eAxis] && nCount < nMaxCount; i += R_ADC_GROUP)
        {
            stScanSegment[nCount].nStart = 0;
            stScanSegment[nCount].nEnd = g_nTotal[eAxis] - 1;
            nRLedEnd = FN_MIN(i + R_ADC_GROUP, g_nTotal[eAxis]) - 1;
            while(!(FN_ABS(g_fMaxRate[Y] * (g_arrPos[eLeft][i].fX - g_arrPos[eRight][stScanSegment[nCount].nStart].fX)) -
                    FN_ABS(g_arrPos[eLeft][i].fY - g_arrPos[eRight][stScanSegment[nCount].nStart].fY) > INFINITE_MIN))
            {
                stScanSegment[nCount].nStart++;
            }

            while(!(FN_ABS(g_fMaxRate[Y] * (g_arrPos[eLeft][nRLedEnd].fX - g_arrPos[eRight][stScanSegment[nCount].nEnd].fX)) -
                    FN_ABS(g_arrPos[eLeft][nRLedEnd].fY - g_arrPos[eRight][stScanSegment[nCount].nEnd].fY) > INFINITE_MIN))
            {
                stScanSegment[nCount].nEnd--;
            }
            nCount++;

        }
    }
    break;

    default:
    {
    }
    break;
    }

    return nCount;
}

/*******************************************************************************************************
 * @Function :初始化每组接收灯的第一个发射灯是全局扫描中的第多少次发射
 * @Parameter: [nStartTimesArray]: 存放序号结果
               nMaxCount: arrIndex的最大数量
               eAxis: 坐标数量
 * @return   : 返回实际的数量
 *******************************************************************************************************/
int32_t InitBlockStartTLedTimes(uint16_t* nStartTimesArray, int32_t nMaxCount, eAxis_t eAxis)
{
    int32_t nEnd = 0;
    int32_t i = 0;

    if(eAxis == X)
    {
        nStartTimesArray[0] = 0;
    }
    else
    {
        nEnd = (g_nTotal[X] - 1) / R_ADC_GROUP;
        nStartTimesArray[0] = g_nBlockStartTLedTimes[X][nEnd] + g_stScanSeg[X][nEnd].nEnd - g_stScanSeg[X][nEnd].nStart + 1;
    }

    nEnd = (g_nTotal[eAxis] - 1) / R_ADC_GROUP + 1;
    nEnd = FN_MIN(nMaxCount, nEnd);

    for(i = 1; i < nEnd; i++)
    {
        nStartTimesArray[i] = nStartTimesArray[i - 1] + g_stScanSeg[eAxis][i - 1].nEnd - g_stScanSeg[eAxis][i - 1].nStart + 1;
    }

    return nEnd;
}



