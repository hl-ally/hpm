#include "AlgCustomer.h"

#if (CUSTOMER_ID == CUSTOMER_SEEWO)
#include "AlgDefaultDefined.h"
#include "AlgTouchPoints.h"
#include "AlgApiMath.h"
#include "AlgInfraredVector.h"
#include "Evaluation.h"
#include "Command.h"
//通用(中间区域)的触摸高度优化 过程自适应参数
#define DEFAULT_GENERAL_DOWN_MIN_DEPTH            (20)  //默认落点的深度值要求
#define DEFAULT_GENERAL_UP_MIN_DEPTH              (35)  //默认抬点的深度值要求
#define DYNAMIC_GENERAL_DOWN_ADJ_DEPTH            (13)  //动态参数调整Down上浮值
#define DYNAMIC_GENERAL_UP_ADJ_DEPTH              (18)  //动态参数调整Up上浮值

//底边的触摸高度优化 过程自适应参数
#define DEFAULT_BOTTOM_NEAR_DOWN_MIN_DEPTH        (35)  //默认落点的深度值要求
#define DEFAULT_BOTTOM_NEAR_UP_MIN_DEPTH          (28)  //默认抬点的深度值要求
#define DYNAMIC_BOTTOM_NEAR_DOWN_ADJ_DEPTH        (17)  //动态参数调整Down上浮值
#define DYNAMIC_BOTTOM_NEAR_UP_ADJ_DEPTH          (11)  //动态参数调整Up上浮值

#define DEFAULT_BOTTOM_MIDDLE_DOWN_MIN_DEPTH      (35)  //默认落点的深度值要求
#define DEFAULT_BOTTOM_MIDDLE_UP_MIN_DEPTH        (26)  //默认抬点的深度值要求
#define DYNAMIC_BOTTOM_MIDDLE_DOWN_ADJ_DEPTH      (16)  //动态参数调整Down上浮值
#define DYNAMIC_BOTTOM_MIDDLE_UP_ADJ_DEPTH        (20)  //动态参数调整Up上浮值

#define DEFAULT_BOTTOM_FAR_DOWN_MIN_DEPTH         (35)  //默认落点的深度值要求
#define DEFAULT_BOTTOM_FAR_UP_MIN_DEPTH           (28)  //默认抬点的深度值要求
#define DYNAMIC_BOTTOM_FAR_DOWN_ADJ_DEPTH         (15)  //动态参数调整Down上浮值
#define DYNAMIC_BOTTOM_FAR_UP_ADJ_DEPTH           (11)  //动态参数调整Up上浮值

//顶边的触摸高度优化 过程自适应参数
#define DEFAULT_TOP_NEAR_DOWN_MIN_DEPTH           (35)  //默认落点的深度值要求
#define DEFAULT_TOP_NEAR_UP_MIN_DEPTH             (40)  //默认抬点的深度值要求
#define DYNAMIC_TOP_NEAR_DOWN_ADJ_DEPTH           (13)  //动态参数调整Down上浮值
#define DYNAMIC_TOP_NEAR_UP_ADJ_DEPTH             (15)  //动态参数调整Up上浮值

#define DEFAULT_TOP_MIDDLE_DOWN_MIN_DEPTH         (40)  //默认落点的深度值要求
#define DEFAULT_TOP_MIDDLE_UP_MIN_DEPTH           (50)  //默认抬点的深度值要求
#define DYNAMIC_TOP_MIDDLE_DOWN_ADJ_DEPTH         (20)  //动态参数调整Down上浮值
#define DYNAMIC_TOP_MIDDLE_UP_ADJ_DEPTH           (33)  //动态参数调整Up上浮值

#define DEFAULT_TOP_FAR_DOWN_MIN_DEPTH            (35)  //默认落点的深度值要求
#define DEFAULT_TOP_FAR_UP_MIN_DEPTH              (35)  //默认抬点的深度值要求
#define DYNAMIC_TOP_FAR_DOWN_ADJ_DEPTH            (10)  //动态参数调整Down上浮值
#define DYNAMIC_TOP_FAR_UP_ADJ_DEPTH              (13)  //动态参数调整Up上浮值

//左边的触摸高度优化 过程自适应参数
#define DEFAULT_LEFT_NEAR_DOWN_MIN_DEPTH          (35)  //默认落点的深度值要求
#define DEFAULT_LEFT_NEAR_UP_MIN_DEPTH            (40)  //默认抬点的深度值要求
#define DYNAMIC_LEFT_NEAR_DOWN_ADJ_DEPTH          (10)  //动态参数调整Down上浮值
#define DYNAMIC_LEFT_NEAR_UP_ADJ_DEPTH            (18)  //动态参数调整Up上浮值

#define DEFAULT_LEFT_MIDDLE_DOWN_MIN_DEPTH        (35)  //默认落点的深度值要求
#define DEFAULT_LEFT_MIDDLE_UP_MIN_DEPTH          (40)  //默认抬点的深度值要求
#define DYNAMIC_LEFT_MIDDLE_DOWN_ADJ_DEPTH        (10)  //动态参数调整Down上浮值
#define DYNAMIC_LEFT_MIDDLE_UP_ADJ_DEPTH          (25)  //动态参数调整Up上浮值

#define DEFAULT_LEFT_FAR_DOWN_MIN_DEPTH           (35)  //默认落点的深度值要求
#define DEFAULT_LEFT_FAR_UP_MIN_DEPTH             (40)  //默认抬点的深度值要求
#define DYNAMIC_LEFT_FAR_DOWN_ADJ_DEPTH           (10)  //动态参数调整Down上浮值
#define DYNAMIC_LEFT_FAR_UP_ADJ_DEPTH             (18)  //动态参数调整Up上浮值

//右边的触摸高度优化 过程自适应参数
#define DEFAULT_RIGHT_NEAR_DOWN_MIN_DEPTH         (35)  //默认落点的深度值要求
#define DEFAULT_RIGHT_NEAR_UP_MIN_DEPTH           (40)  //默认抬点的深度值要求
#define DYNAMIC_RIGHT_NEAR_DOWN_ADJ_DEPTH         (10)  //动态参数调整Down上浮值
#define DYNAMIC_RIGHT_NEAR_UP_ADJ_DEPTH           (22)  //动态参数调整Up上浮值

#define DEFAULT_RIGHT_MIDDLE_DOWN_MIN_DEPTH       (35)  //默认落点的深度值要求
#define DEFAULT_RIGHT_MIDDLE_UP_MIN_DEPTH         (40)  //默认抬点的深度值要求
#define DYNAMIC_RIGHT_MIDDLE_DOWN_ADJ_DEPTH       (10)  //动态参数调整Down上浮值
#define DYNAMIC_RIGHT_MIDDLE_UP_ADJ_DEPTH         (22)  //动态参数调整Up上浮值

#define DEFAULT_RIGHT_FAR_DOWN_MIN_DEPTH          (35)  //默认落点的深度值要求
#define DEFAULT_RIGHT_FAR_UP_MIN_DEPTH            (40)  //默认抬点的深度值要求
#define DYNAMIC_RIGHT_FAR_DOWN_ADJ_DEPTH          (10)  //动态参数调整Down上浮值
#define DYNAMIC_RIGHT_FAR_UP_ADJ_DEPTH            (22)  //动态参数调整Up上浮值

#pragma arm section code =".code.fast"

#if (CHECK_TOUCH_HEIGHT_EN && USE_CUSTOMER_CHECKHEIGHT)

#if (30 == CHECK_TOUCH_HEIGHT_MM)               //MAXHUB 笔使用触摸高度
/*
 * 对触摸点进行触摸高度优化 尽量去掉远离触摸表面的物体
 * @para arrPointF: 触摸物数组
 * @para nCount: 点数
 * @return: 剩余点数
 */
int32_t CheckTouchHeight(stPointF_t *arrPointF, uint32_t nCount)
{
    eActivedLevel_t arrActived[MAX_POINT_SFX];  //0:不处于活动区域  1:处于未曾到有抬点记录的活动区域  2:处于记录到曾有抬点的活动区域
    stTouch_t **arrTouch = g_pAlgCoreVar->arrTouch;
    stTrack_t *pTrack = &g_pAlgCoreVar->stTrack;

    static stTouchHPointF_t s_arrPoint[MAX_POINT_SFX];
    static stPropertyTrack_t s_stProperty[MAX_POINT_SFX];
    static uint8_t s_arrDepDownDef[eAreaALLTypeTotal] = {0};  //默认落点的深度值要求
    static uint8_t s_arrDepUpDef[eAreaALLTypeTotal] = {0};    //默认抬点的深度值要求
    static uint8_t s_arrDyDown[eAreaALLTypeTotal] = {0};      //根据动态收集的深度参数调整的上浮值
    static uint8_t s_arrDyUp[eAreaALLTypeTotal] = {0};        //根据动态收集的深度参数调整的上浮值
    static uint8_t s_bCheckTouchHeightInit = 0;
    static uint8_t s_arrDownTryAgain[MAX_POINT_SFX] = {0};
    static stCoordF_t s_arrDownTryAgainCoordF[MAX_POINT_SFX];

    if (!s_bCheckTouchHeightInit)  //参数初始化...
    {
        const uint8_t arrDepDownDefInit[eAreaALLTypeTotal] = {0,
                                                        DEFAULT_BOTTOM_NEAR_DOWN_MIN_DEPTH,DEFAULT_BOTTOM_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_BOTTOM_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_TOP_NEAR_DOWN_MIN_DEPTH,DEFAULT_TOP_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_TOP_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_LEFT_NEAR_DOWN_MIN_DEPTH,DEFAULT_LEFT_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_LEFT_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_RIGHT_NEAR_DOWN_MIN_DEPTH,DEFAULT_RIGHT_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_RIGHT_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_GENERAL_DOWN_MIN_DEPTH};

        const uint8_t arrDepUpDefInit[eAreaALLTypeTotal] = {0,
                                                      DEFAULT_BOTTOM_NEAR_UP_MIN_DEPTH,DEFAULT_BOTTOM_MIDDLE_UP_MIN_DEPTH,DEFAULT_BOTTOM_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_TOP_NEAR_UP_MIN_DEPTH,DEFAULT_TOP_MIDDLE_UP_MIN_DEPTH,DEFAULT_TOP_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_LEFT_NEAR_UP_MIN_DEPTH,DEFAULT_LEFT_MIDDLE_UP_MIN_DEPTH,DEFAULT_LEFT_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_RIGHT_NEAR_UP_MIN_DEPTH,DEFAULT_RIGHT_MIDDLE_UP_MIN_DEPTH,DEFAULT_RIGHT_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_GENERAL_UP_MIN_DEPTH};

        const uint8_t arrDyDownInit[eAreaALLTypeTotal] = {0,
                                                    DYNAMIC_BOTTOM_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_BOTTOM_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_BOTTOM_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_TOP_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_TOP_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_TOP_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_LEFT_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_LEFT_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_LEFT_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_RIGHT_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_RIGHT_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_RIGHT_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_GENERAL_DOWN_ADJ_DEPTH};

        const uint8_t arrDyUpInit[eAreaALLTypeTotal] = {0,
                                                  DYNAMIC_BOTTOM_NEAR_UP_ADJ_DEPTH,DYNAMIC_BOTTOM_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_BOTTOM_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_TOP_NEAR_UP_ADJ_DEPTH,DYNAMIC_TOP_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_TOP_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_LEFT_NEAR_UP_ADJ_DEPTH,DYNAMIC_LEFT_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_LEFT_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_RIGHT_NEAR_UP_ADJ_DEPTH,DYNAMIC_RIGHT_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_RIGHT_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_GENERAL_UP_ADJ_DEPTH};
        for (uint8_t i = 0; i < eAreaALLTypeTotal; i++)
        {
            s_arrDepDownDef[i] = arrDepDownDefInit[i];
            s_arrDepUpDef[i] = arrDepUpDefInit[i];
            s_arrDyDown[i] = arrDyDownInit[i];
            s_arrDyUp[i] = arrDyUpInit[i];
        }
        s_bCheckTouchHeightInit = 1;
    }
    //检查是否在画线/写字/点击
    for (int32_t i = 0; i < MAX_POINT_SFX; i++)
    {
        arrActived[i] = eActivedNone;
        if (arrPointF[i].eStatus != eBtnNone)
        {
            //仅在活动区域内进行触摸高度优化，有可能在点击/写字
            arrActived[i] = arrPointF[i].eActived;
            if (eLeftBtnMove == arrPointF[i].eStatus)
            {
                stKFMiscPara_t *pKFMiscPara = GetKFMiscPara(eKFPtTracker, arrPointF[i].nId);
                float fMaxMovement = FN_MAX_SFX(FN_ABS_SUB_SFX(arrPointF[i].stCoordF.fX, pKFMiscPara->stDownCoordF.fX),
                                                FN_ABS_SUB_SFX(arrPointF[i].stCoordF.fY, pKFMiscPara->stDownCoordF.fY));
                if (!(fMaxMovement < MAX_CHECK_HEIGHT_DOWN_DIST))
                {
                    arrActived[i] = eActivedNone;
                }
                else if (eActivedNone == arrActived[i] &&
                         (GetSystickTime()-pKFMiscPara->nDownTick) < MAX_CHECK_HEIGHT_DOWN_TICK)
                {
                    arrActived[i] = eActivedL0;
                }
            }
        }
    }
    int32_t nMinLineCnt = 0;
    int32_t nMID = 0, nAID = 0;
    stTouchH_t *stTouchH = NULL;
    eAreaType_Edge_t eAreaType = eNoneAllType;
    eAreaType_Edge_t eBlindAreaType = eNoneAllType;
    for(int32_t i = 0; i < MAX_POINT_SFX; i++)
    {
        if (arrPointF[i].eStatus != eBtnNone)
        {
            nMID = arrPointF[i].nId;
            nAID = arrPointF[i].nAId;
            stTouchH_t stTouchHUp;
            stTouchH = &stTouchHUp;
            if (arrPointF[i].eStatus != eLeftBtnUp || nAID < g_pAlgCoreVar->nTouchCount)
            {
                stTouchH = &arrTouch[nAID]->stBp.stTouchH; //取出对应的物理信息
            }
            else //解决超多点后可能内存不足的问题
            {
                MemSet(&stTouchHUp, 0, sizeof(stTouchHUp));
                stTouchHUp.nMaxDepth[X] = MIN_DEPATH_VALUE;
                stTouchHUp.nMaxDepth[Y] = MIN_DEPATH_VALUE;
                nAID = -1;
            }

            nMinLineCnt = FN_MIN_SFX(stTouchH->nLineCnt[X], stTouchH->nLineCnt[Y]);
            eAreaType = GetAreaTypeFromEdge(arrPointF[i].stCoordF, eSideTypeP);
            eBlindAreaType = GetAreaTypeFromEdge(arrPointF[i].stCoordF, eSideTypeBMax);

            switch (arrPointF[i].eStatus)
            {
                case eLeftBtnDown:
                {
                    s_stProperty[nMID].nDepathCacheLen = 0;
                    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                    {
                        s_stProperty[nMID].nDepathDownReq[eAxis] = s_arrDepDownDef[eAreaType];
                        s_stProperty[nMID].nDepathUpReq[eAxis] = s_arrDepUpDef[eAreaType];
                    }
                    s_stProperty[nMID].nAreaCacheLen = 0;
                    s_stProperty[nMID].nAreaAver = (int16_t)arrPointF[i].fAreaSize;
                    arrPointF[i].eStatus = eBtnNone;
                    nCount--;
                }
                break;

                case eLeftBtnMove:
                {
                    if (s_arrPoint[i].eStatus == eBtnNone || s_arrPoint[i].eStatus == eLeftBtnUp)
                    {
                        int32_t bAreaIncing = (arrPointF[i].fAreaSize > (s_arrPoint[i].fAreaSize*AREA_INC_RATE));
                        stKFMiscPara_t *pKFMiscPara = GetKFMiscPara(eKFPtTracker, nMID);

                        if (pTrack->arrCheckHOpt[nMID]) //曾经被触摸高度优化抬起过
                        {
                            if (s_arrDownTryAgain[i])
                            {
                                if (stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                        stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y])
                                {
                                    if (++s_arrDownTryAgain[i] > 3 ||
                                            FN_MAX_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < FN_MIN_SFX(s_stProperty[nMID].nDepathDownReq[X], s_stProperty[nMID].nDepathDownReq[Y]))
                                    {
                                        arrPointF[i].stCoordF = s_arrDownTryAgainCoordF[i];
                                        arrPointF[i].eStatus = eLeftBtnDown;
                                    }
                                }
                                else
                                {
                                    s_arrDownTryAgain[i] = 0;
                                }
                            }
                            else if (!bAreaIncing &&
                                        stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                        stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y] &&
                                        (nAID < 0 || arrTouch[nAID]->eAreaType != eFillMaskingType))
                            {
                                s_arrDownTryAgain[i]++;
                                s_arrDownTryAgainCoordF[i] = arrPointF[i].stCoordF;
                            }
                        }
                        else
                        {
                            if (nMinLineCnt != 0)
                            {
                                if ((!bAreaIncing &&    //1.面积不再持续增长的前提下，深度满足要求，出点
                                        stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                        stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y]) ||
                                        (((FN_MAX_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < 20 &&  //2.最小深度低于一定值时，同时移动的距离已经满足一定条件，出点
                                            FN_GET_COORDF_DIST_POW2_SFX(arrPointF[i].stCoordF, pKFMiscPara->stDownCoordF) > DISTANCE_DOWN_LIMIT_POW2) ||
                                        (eActivedL2 == arrPointF[i].eActived && //3.写字状态下，最小深度低于一定值时，出点
                                            FN_MAX_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < 10)) &&
                                         (arrPointF[i].fAreaSize < s_arrPoint[i].fAreaSize*1.3f)))
                                {
                                    arrPointF[i].stCoordF = s_arrPoint[i].stCoordF;
                                    arrPointF[i].eStatus = eLeftBtnDown;
                                }
                            }
                        }
                        if (arrPointF[i].eStatus != eLeftBtnDown)
                        {
                            arrPointF[i].eStatus = eBtnNone;
                            nCount--;
                        }
                        else
                        {
                            CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnDown);
                        }
                    }
                    else if (arrActived[i] != eActivedNone)  //是否需要进行高度优化
                    {
                        if (nMinLineCnt == 0 ||
                                ((int16_t)arrPointF[i].fAreaSize < (s_stProperty[nMID].nAreaAver*90/100) &&
                                 ((stTouchH->nMaxDepth[X] > s_stProperty[nMID].nDepathUpReq[X]) || (stTouchH->nMaxDepth[Y] > s_stProperty[nMID].nDepathUpReq[Y]))))
                        {
                            if (eLeftBtnUp == (arrPointF[i].eStatus = CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnUp)))
                            {
                                arrPointF[i].stCoordF = s_arrPoint[i].stCoordF;
                                s_arrDownTryAgain[i] = 0;
                            }
                        }
                    }
                    if (eLeftBtnMove == arrPointF[i].eStatus)
                    {
                        CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnMove);
                    }
                }
                break;

                case eLeftBtnUp:
                {
                    CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnUp);
                    if (s_arrPoint[i].eStatus == eBtnNone || s_arrPoint[i].eStatus == eLeftBtnUp)
                    {
                        arrPointF[i].eStatus = eBtnNone;
                        nCount--;
                    }
                }
                break;

                default:
                break;
            }
            if (eLeftBtnMove == arrPointF[i].eStatus) //Move点状态下，以窗口的方式更新点属性
            {
                int32_t nCurIdx = s_stProperty[nMID].nDepathCacheLen;
                for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                {
                    s_stProperty[nMID].arrDepath[eAxis][nCurIdx] = stTouchH->nMaxDepth[eAxis];
                }
                if (++s_stProperty[nMID].nDepathCacheLen == MAX_DEPATH_CACHE) //深度缓存满了，更新一次阈值
                {
                    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                    {
                        uint8_t  nAverDepath = 0;
                        uint32_t nDepathTotal = 0;
                        for (int32_t j = 0; j < s_stProperty[nMID].nDepathCacheLen; j++)
                        {
                            nDepathTotal += s_stProperty[nMID].arrDepath[eAxis][j];
                        }
                        nAverDepath = nDepathTotal/MAX_DEPATH_CACHE;
                        s_stProperty[nMID].nDepathAver[eAxis] = nAverDepath;

                        if (eBlindAreaType > eNoneAllType && eBlindAreaType < eGeneralAllType)
                        {
                            s_stProperty[nMID].nDepathDownReq[eAxis] = ((nAverDepath+s_arrDyDown[eAreaType])*2) < 65 ? ((nAverDepath+s_arrDyDown[eAreaType])*2) : 65;          //Down上浮值
                            s_stProperty[nMID].nDepathUpReq[eAxis] = ((nAverDepath+s_arrDepUpDef[eAreaType])*2) < 65 ? ((nAverDepath+s_arrDepUpDef[eAreaType])*2) : 65;       //Up上浮值
                        }
                        else
                        {
                            s_stProperty[nMID].nDepathDownReq[eAxis] = FN_MIN_SFX(s_arrDepDownDef[eAreaType], nAverDepath+s_arrDyDown[eAreaType]); //Down上浮值
                            s_stProperty[nMID].nDepathUpReq[eAxis] = FN_MIN_SFX(s_arrDepUpDef[eAreaType], nAverDepath+s_arrDyUp[eAreaType]);       //Up上浮值
                        }
                    }
                    s_stProperty[nMID].nDepathCacheLen = 0;
                }
                nCurIdx = s_stProperty[nMID].nAreaCacheLen;
                s_stProperty[nMID].arrArea[nCurIdx] = (int16_t)arrPointF[i].fAreaSize;
                if (++s_stProperty[nMID].nAreaCacheLen == MAX_AREA_CACHE) //面积缓存满了，更新一次阈值
                {
                    uint32_t nAreaTotal = 0;
                    for (int32_t j = 0; j < s_stProperty[nMID].nAreaCacheLen; j++)
                    {
                        nAreaTotal += s_stProperty[nMID].arrArea[j];
                    }
                    s_stProperty[nMID].nAreaAver = nAreaTotal/MAX_AREA_CACHE;
                    s_stProperty[nMID].nAreaCacheLen = 0;
                }
            }
        }
        s_arrPoint[i].nId = arrPointF[i].nId;
        s_arrPoint[i].eStatus = arrPointF[i].eStatus;
        s_arrPoint[i].fAreaSize = arrPointF[i].fAreaSize;
        s_arrPoint[i].stCoordF = arrPointF[i].stCoordF;
    }
    return nCount;
}

#elif (25 == CHECK_TOUCH_HEIGHT_MM)         //海外供客笔使用触摸高度
/*
 * 对触摸点进行触摸高度优化 尽量去掉远离触摸表面的物体
 * @para arrPointF: 触摸物数组
 * @para nCount: 点数
 * @return: 剩余点数
 */
int32_t CheckTouchHeight(stPointF_t *arrPointF, uint32_t nCount)
{
    eActivedLevel_t arrActived[MAX_POINT_SFX];  //0:不处于活动区域  1:处于未曾到有抬点记录的活动区域  2:处于记录到曾有抬点的活动区域
    stTouch_t **arrTouch = g_pAlgCoreVar->arrTouch;
    stTrack_t *pTrack = &g_pAlgCoreVar->stTrack;

    static stTouchHPointF_t s_arrPoint[MAX_POINT_SFX];
    static stPropertyTrack_t s_stProperty[MAX_POINT_SFX];
    static uint8_t s_arrDepDownDef[eAreaALLTypeTotal] = {0};  //默认落点的深度值要求
    static uint8_t s_arrDepUpDef[eAreaALLTypeTotal] = {0};    //默认抬点的深度值要求
    static uint8_t s_arrDyDown[eAreaALLTypeTotal] = {0};      //根据动态收集的深度参数调整的上浮值
    static uint8_t s_arrDyUp[eAreaALLTypeTotal] = {0};        //根据动态收集的深度参数调整的上浮值
    static uint8_t s_bCheckTouchHeightInit = 0;
    static uint8_t s_arrDownTryAgain[MAX_POINT_SFX] = {0};
    static stCoordF_t s_arrDownTryAgainCoordF[MAX_POINT_SFX];
    const float fLV1AreaSize = 30.0f;

    if (!s_bCheckTouchHeightInit)  //参数初始化...
    {
        const uint8_t arrDepDownDefInit[eAreaALLTypeTotal] = {0,
                                                        DEFAULT_BOTTOM_NEAR_DOWN_MIN_DEPTH,DEFAULT_BOTTOM_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_BOTTOM_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_TOP_NEAR_DOWN_MIN_DEPTH,DEFAULT_TOP_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_TOP_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_LEFT_NEAR_DOWN_MIN_DEPTH,DEFAULT_LEFT_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_LEFT_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_RIGHT_NEAR_DOWN_MIN_DEPTH,DEFAULT_RIGHT_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_RIGHT_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_GENERAL_DOWN_MIN_DEPTH};

        const uint8_t arrDepUpDefInit[eAreaALLTypeTotal] = {0,
                                                      DEFAULT_BOTTOM_NEAR_UP_MIN_DEPTH,DEFAULT_BOTTOM_MIDDLE_UP_MIN_DEPTH,DEFAULT_BOTTOM_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_TOP_NEAR_UP_MIN_DEPTH,DEFAULT_TOP_MIDDLE_UP_MIN_DEPTH,DEFAULT_TOP_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_LEFT_NEAR_UP_MIN_DEPTH,DEFAULT_LEFT_MIDDLE_UP_MIN_DEPTH,DEFAULT_LEFT_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_RIGHT_NEAR_UP_MIN_DEPTH,DEFAULT_RIGHT_MIDDLE_UP_MIN_DEPTH,DEFAULT_RIGHT_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_GENERAL_UP_MIN_DEPTH};

        const uint8_t arrDyDownInit[eAreaALLTypeTotal] = {0,
                                                    DYNAMIC_BOTTOM_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_BOTTOM_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_BOTTOM_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_TOP_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_TOP_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_TOP_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_LEFT_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_LEFT_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_LEFT_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_RIGHT_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_RIGHT_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_RIGHT_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_GENERAL_DOWN_ADJ_DEPTH};

        const uint8_t arrDyUpInit[eAreaALLTypeTotal] = {0,
                                                  DYNAMIC_BOTTOM_NEAR_UP_ADJ_DEPTH,DYNAMIC_BOTTOM_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_BOTTOM_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_TOP_NEAR_UP_ADJ_DEPTH,DYNAMIC_TOP_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_TOP_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_LEFT_NEAR_UP_ADJ_DEPTH,DYNAMIC_LEFT_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_LEFT_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_RIGHT_NEAR_UP_ADJ_DEPTH,DYNAMIC_RIGHT_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_RIGHT_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_GENERAL_UP_ADJ_DEPTH};
        for (uint8_t i = 0; i < eAreaALLTypeTotal; i++)
        {
            s_arrDepDownDef[i] = arrDepDownDefInit[i];
            s_arrDepUpDef[i] = arrDepUpDefInit[i];
            s_arrDyDown[i] = arrDyDownInit[i];
            s_arrDyUp[i] = arrDyUpInit[i];
        }
        s_bCheckTouchHeightInit = 1;
    }
    //检查是否在画线/写字/点击
    for (int32_t i = 0; i < MAX_POINT_SFX; i++)
    {
        arrActived[i] = eActivedNone;
        if (arrPointF[i].eStatus != eBtnNone)
        {
            //仅在活动区域内进行触摸高度优化，有可能在点击/写字
            arrActived[i] = arrPointF[i].eActived;
            if (eLeftBtnMove == arrPointF[i].eStatus)
            {
                stKFMiscPara_t *pKFMiscPara = GetKFMiscPara(eKFPtTracker, arrPointF[i].nId);
                float fMaxMovement = FN_MAX_SFX(FN_ABS_SUB_SFX(arrPointF[i].stCoordF.fX, pKFMiscPara->stDownCoordF.fX),
                                                FN_ABS_SUB_SFX(arrPointF[i].stCoordF.fY, pKFMiscPara->stDownCoordF.fY));
                if (!(fMaxMovement < MAX_CHECK_HEIGHT_DOWN_DIST))
                {
                    arrActived[i] = eActivedNone;
                }
                else if (eActivedNone == arrActived[i] &&
                         (GetSystickTime()-pKFMiscPara->nDownTick) < MAX_CHECK_HEIGHT_DOWN_TICK)
                {
                    arrActived[i] = eActivedL0;
                }
            }
        }
    }
    int32_t nMinLineCnt = 0;
    int32_t nMID = 0, nAID = 0;
    stTouchH_t *stTouchH = NULL;
    eAreaType_Edge_t eAreaType = eNoneAllType;
    eAreaType_Edge_t eBlindAreaType = eNoneAllType;
    for(int32_t i = 0; i < MAX_POINT_SFX; i++)
    {
        if (arrPointF[i].eStatus != eBtnNone)
        {
            nMID = arrPointF[i].nId;
            nAID = arrPointF[i].nAId;
            stTouchH_t stTouchHUp;
            stTouchH = &stTouchHUp;
            if (arrPointF[i].eStatus != eLeftBtnUp || nAID < g_pAlgCoreVar->nTouchCount)
            {
                stTouchH = &arrTouch[nAID]->stBp.stTouchH; //取出对应的物理信息
            }
            else //解决超多点后可能内存不足的问题
            {
                MemSet(&stTouchHUp, 0, sizeof(stTouchHUp));
                stTouchHUp.nMaxDepth[X] = MIN_DEPATH_VALUE;
                stTouchHUp.nMaxDepth[Y] = MIN_DEPATH_VALUE;
                nAID = -1;
            }

            nMinLineCnt = FN_MIN_SFX(stTouchH->nLineCnt[X], stTouchH->nLineCnt[Y]);
            eAreaType = GetAreaTypeFromEdge(arrPointF[i].stCoordF, eSideTypeP);
            eBlindAreaType = GetAreaTypeFromEdge(arrPointF[i].stCoordF, eSideTypeBMax);
            if (GetAreaType(arrPointF[i].stCoordF, eSideTypeP) < eGeneralType && arrPointF[i].fAreaSize < fLV1AreaSize)
            {
                stTouchH->nMaxDepth[X] = FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]);
                stTouchH->nMaxDepth[Y] = stTouchH->nMaxDepth[X];
            }
            switch (arrPointF[i].eStatus)
            {
                case eLeftBtnDown:
                {
                    s_stProperty[nMID].nDepathCacheLen = 0;
                    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                    {
                        s_stProperty[nMID].nDepathDownReq[eAxis] = s_arrDepDownDef[eAreaType];
                        s_stProperty[nMID].nDepathUpReq[eAxis] = s_arrDepUpDef[eAreaType];
                    }
                    s_stProperty[nMID].nAreaCacheLen = 0;
                    s_stProperty[nMID].nAreaAver = (int16_t)arrPointF[i].fAreaSize;
                    arrPointF[i].eStatus = eBtnNone;
                    nCount--;
                }
                break;

                case eLeftBtnMove:
                {
                    if (s_arrPoint[i].eStatus == eBtnNone || s_arrPoint[i].eStatus == eLeftBtnUp)
                    {
                        int32_t bAreaIncing = (arrPointF[i].fAreaSize > (s_arrPoint[i].fAreaSize*AREA_INC_RATE));
                        stKFMiscPara_t *pKFMiscPara = GetKFMiscPara(eKFPtTracker, nMID);

                        if (pTrack->arrCheckHOpt[nMID]) //曾经被触摸高度优化抬起过
                        {
                            if (s_arrDownTryAgain[i])
                            {
                                if ((stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                        stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y]) ||
                                        (arrPointF[i].fAreaSize < fLV1AreaSize &&
                                         FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < FN_MAX_SFX(s_stProperty[nMID].nDepathDownReq[X], s_stProperty[nMID].nDepathDownReq[Y])))
                                {
                                    if (++s_arrDownTryAgain[i] > 3 ||
                                            FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < FN_MAX_SFX(s_stProperty[nMID].nDepathDownReq[X], s_stProperty[nMID].nDepathDownReq[Y]))
                                    {
                                        arrPointF[i].stCoordF = s_arrDownTryAgainCoordF[i];
                                        arrPointF[i].eStatus = eLeftBtnDown;
                                    }
                                }
                                else
                                {
                                    s_arrDownTryAgain[i] = 0;
                                }
                            }
                            else if (!bAreaIncing &&
                                        stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                        stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y] &&
                                        (nAID < 0 || arrTouch[nAID]->eAreaType != eFillMaskingType))
                            {
                                s_arrDownTryAgain[i]++;
                                s_arrDownTryAgainCoordF[i] = arrPointF[i].stCoordF;
                            }
                        }
                        else
                        {
                            if (nMinLineCnt != 0)
                            {
                                if ((!bAreaIncing &&    //1.面积不再持续增长的前提下，深度满足要求，出点
                                        stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                        stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y]) ||
                                        (FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < 20 &&  //2.最小深度低于一定值时，同时移动的距离已经满足一定条件，出点
                                            FN_GET_COORDF_DIST_POW2_SFX(arrPointF[i].stCoordF, pKFMiscPara->stDownCoordF) > DISTANCE_DOWN_LIMIT_POW2) ||
                                        (eActivedL2 == arrPointF[i].eActived && //3.写字状态下，最小深度低于一定值时，出点
                                            FN_MAX_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < 10))
                                {
                                    arrPointF[i].stCoordF = s_arrPoint[i].stCoordF;
                                    arrPointF[i].eStatus = eLeftBtnDown;
                                }
                            }
                        }
                        if (arrPointF[i].eStatus != eLeftBtnDown)
                        {
                            arrPointF[i].eStatus = eBtnNone;
                            nCount--;
                        }
                        else
                        {
                            CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnDown);
                        }
                    }
                    else if (arrActived[i] != eActivedNone)  //是否需要进行高度优化
                    {
                        if (nMinLineCnt == 0 ||
                                ((int16_t)arrPointF[i].fAreaSize < (s_stProperty[nMID].nAreaAver*90/100) &&
                                 ((stTouchH->nMaxDepth[X] > s_stProperty[nMID].nDepathUpReq[X]) || (stTouchH->nMaxDepth[Y] > s_stProperty[nMID].nDepathUpReq[Y]))))
                        {
                            if (eLeftBtnUp == (arrPointF[i].eStatus = CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnUp)))
                            {
                                arrPointF[i].stCoordF = s_arrPoint[i].stCoordF;
                                s_arrDownTryAgain[i] = 0;
                            }
                        }
                    }
                    if (eLeftBtnMove == arrPointF[i].eStatus)
                    {
                        CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnMove);
                    }
                }
                break;

                case eLeftBtnUp:
                {
                    CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnUp);
                    if (s_arrPoint[i].eStatus == eBtnNone || s_arrPoint[i].eStatus == eLeftBtnUp)
                    {
                        arrPointF[i].eStatus = eBtnNone;
                        nCount--;
                    }
                }
                break;

                default:
                break;
            }
            if (eLeftBtnMove == arrPointF[i].eStatus) //Move点状态下，以窗口的方式更新点属性
            {
                int32_t nCurIdx = s_stProperty[nMID].nDepathCacheLen;
                for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                {
                    s_stProperty[nMID].arrDepath[eAxis][nCurIdx] = stTouchH->nMaxDepth[eAxis];
                }
                if (++s_stProperty[nMID].nDepathCacheLen == MAX_DEPATH_CACHE) //深度缓存满了，更新一次阈值
                {
                    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                    {
                        uint8_t  nAverDepath = 0;
                        uint32_t nDepathTotal = 0;
                        for (int32_t j = 0; j < s_stProperty[nMID].nDepathCacheLen; j++)
                        {
                            nDepathTotal += s_stProperty[nMID].arrDepath[eAxis][j];
                        }
                        nAverDepath = nDepathTotal/MAX_DEPATH_CACHE;
                        s_stProperty[nMID].nDepathAver[eAxis] = nAverDepath;

                        if (eBlindAreaType > eNoneAllType && eBlindAreaType < eGeneralAllType)
                        {
                            s_stProperty[nMID].nDepathDownReq[eAxis] = ((nAverDepath+s_arrDyDown[eAreaType])*2) < 65 ? ((nAverDepath+s_arrDyDown[eAreaType])*2) : 65;          //Down上浮值
                            s_stProperty[nMID].nDepathUpReq[eAxis] = ((nAverDepath+s_arrDepUpDef[eAreaType])*2) < 65 ? ((nAverDepath+s_arrDepUpDef[eAreaType])*2) : 65;       //Up上浮值
                        }
                        else
                        {
                            s_stProperty[nMID].nDepathDownReq[eAxis] = FN_MIN_SFX(s_arrDepDownDef[eAreaType], nAverDepath+s_arrDyDown[eAreaType]); //Down上浮值
                            s_stProperty[nMID].nDepathUpReq[eAxis] = FN_MIN_SFX(s_arrDepUpDef[eAreaType], nAverDepath+s_arrDyUp[eAreaType]);       //Up上浮值
                        }
                    }
                    s_stProperty[nMID].nDepathCacheLen = 0;
                }
                nCurIdx = s_stProperty[nMID].nAreaCacheLen;
                s_stProperty[nMID].arrArea[nCurIdx] = (int16_t)arrPointF[i].fAreaSize;
                if (++s_stProperty[nMID].nAreaCacheLen == MAX_AREA_CACHE) //面积缓存满了，更新一次阈值
                {
                    uint32_t nAreaTotal = 0;
                    for (int32_t j = 0; j < s_stProperty[nMID].nAreaCacheLen; j++)
                    {
                        nAreaTotal += s_stProperty[nMID].arrArea[j];
                    }
                    s_stProperty[nMID].nAreaAver = nAreaTotal/MAX_AREA_CACHE;
                    s_stProperty[nMID].nAreaCacheLen = 0;
                }
            }
        }
        s_arrPoint[i].nId = arrPointF[i].nId;
        s_arrPoint[i].eStatus = arrPointF[i].eStatus;
        s_arrPoint[i].fAreaSize = arrPointF[i].fAreaSize;
        s_arrPoint[i].stCoordF = arrPointF[i].stCoordF;
    }
    return nCount;
}
#elif (2502 == CHECK_TOUCH_HEIGHT_MM)         //SW13F 使用触摸高度
/*
 * 对触摸点进行触摸高度优化 尽量去掉远离触摸表面的物体
 * @para arrPointF: 触摸物数组
 * @para nCount: 点数
 * @return: 剩余点数
 */
int32_t CheckTouchHeight(stPointF_t *arrPointF, uint32_t nCount)
{
    eActivedLevel_t arrActived[MAX_POINT_SFX];  //0:不处于活动区域  1:处于未曾到有抬点记录的活动区域  2:处于记录到曾有抬点的活动区域
    stTouch_t **arrTouch = g_pAlgCoreVar->arrTouch;
    stTrack_t *pTrack = &g_pAlgCoreVar->stTrack;

    static stTouchHPointF_t s_arrPoint[MAX_POINT_SFX];
    static stPropertyTrack_t s_stProperty[MAX_POINT_SFX];
    static uint8_t s_arrDepDownDef[eAreaALLTypeTotal] = {0};  //默认落点的深度值要求
    static uint8_t s_arrDepUpDef[eAreaALLTypeTotal] = {0};    //默认抬点的深度值要求
    static uint8_t s_arrDyDown[eAreaALLTypeTotal] = {0};      //根据动态收集的深度参数调整的上浮值
    static uint8_t s_arrDyUp[eAreaALLTypeTotal] = {0};        //根据动态收集的深度参数调整的上浮值
    static uint8_t s_bCheckTouchHeightInit = 0;
    static uint8_t s_arrDownTryAgain[MAX_POINT_SFX] = {0};
    static stCoordF_t s_arrDownTryAgainCoordF[MAX_POINT_SFX];
    const float fLV1AreaSize = 30.0f;
    static uint8_t s_arrBufferingUp[MAX_POINT_SFX] = {0};
    stActiveArea_t *pActiveArea = &g_pAlgCoreVar->stActiveArea;
    int32_t nEasyTHCount = 3;

    if (!s_bCheckTouchHeightInit)  //参数初始化...
    {
        const uint8_t arrDepDownDefInit[eAreaALLTypeTotal] = {0,
                                                        DEFAULT_BOTTOM_NEAR_DOWN_MIN_DEPTH,DEFAULT_BOTTOM_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_BOTTOM_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_TOP_NEAR_DOWN_MIN_DEPTH,DEFAULT_TOP_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_TOP_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_LEFT_NEAR_DOWN_MIN_DEPTH,DEFAULT_LEFT_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_LEFT_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_RIGHT_NEAR_DOWN_MIN_DEPTH,DEFAULT_RIGHT_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_RIGHT_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_GENERAL_DOWN_MIN_DEPTH};

        const uint8_t arrDepUpDefInit[eAreaALLTypeTotal] = {0,
                                                      DEFAULT_BOTTOM_NEAR_UP_MIN_DEPTH,DEFAULT_BOTTOM_MIDDLE_UP_MIN_DEPTH,DEFAULT_BOTTOM_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_TOP_NEAR_UP_MIN_DEPTH,DEFAULT_TOP_MIDDLE_UP_MIN_DEPTH,DEFAULT_TOP_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_LEFT_NEAR_UP_MIN_DEPTH,DEFAULT_LEFT_MIDDLE_UP_MIN_DEPTH,DEFAULT_LEFT_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_RIGHT_NEAR_UP_MIN_DEPTH,DEFAULT_RIGHT_MIDDLE_UP_MIN_DEPTH,DEFAULT_RIGHT_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_GENERAL_UP_MIN_DEPTH};

        const uint8_t arrDyDownInit[eAreaALLTypeTotal] = {0,
                                                    DYNAMIC_BOTTOM_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_BOTTOM_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_BOTTOM_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_TOP_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_TOP_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_TOP_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_LEFT_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_LEFT_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_LEFT_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_RIGHT_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_RIGHT_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_RIGHT_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_GENERAL_DOWN_ADJ_DEPTH};

        const uint8_t arrDyUpInit[eAreaALLTypeTotal] = {0,
                                                  DYNAMIC_BOTTOM_NEAR_UP_ADJ_DEPTH,DYNAMIC_BOTTOM_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_BOTTOM_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_TOP_NEAR_UP_ADJ_DEPTH,DYNAMIC_TOP_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_TOP_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_LEFT_NEAR_UP_ADJ_DEPTH,DYNAMIC_LEFT_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_LEFT_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_RIGHT_NEAR_UP_ADJ_DEPTH,DYNAMIC_RIGHT_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_RIGHT_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_GENERAL_UP_ADJ_DEPTH};
        for (uint8_t i = 0; i < eAreaALLTypeTotal; i++)
        {
            s_arrDepDownDef[i] = arrDepDownDefInit[i];
            s_arrDepUpDef[i] = arrDepUpDefInit[i];
            s_arrDyDown[i] = arrDyDownInit[i];
            s_arrDyUp[i] = arrDyUpInit[i];
        }
        s_bCheckTouchHeightInit = 1;
    }
    //检查是否在画线/写字/点击
    for (int32_t i = 0; i < MAX_POINT_SFX; i++)
    {
        arrActived[i] = eActivedNone;
        if (arrPointF[i].eStatus != eBtnNone)
        {
            //仅在活动区域内进行触摸高度优化，有可能在点击/写字
            arrActived[i] = arrPointF[i].eActived;
            if (eLeftBtnMove == arrPointF[i].eStatus)
            {
                stKFMiscPara_t *pKFMiscPara = GetKFMiscPara(eKFPtTracker, arrPointF[i].nId);
                float fMaxMovement = FN_MAX_SFX(FN_ABS_SUB_SFX(arrPointF[i].stCoordF.fX, pKFMiscPara->stDownCoordF.fX),
                                                FN_ABS_SUB_SFX(arrPointF[i].stCoordF.fY, pKFMiscPara->stDownCoordF.fY));
                if (!(fMaxMovement < MAX_CHECK_HEIGHT_DOWN_DIST) ||
                        (!(pActiveArea->nActiveCnt < nEasyTHCount) &&
                         arrPointF[i].fAreaSize < PEN_8_AREA_POW2 &&
                         g_pAlgCoreVar->fLstMaxArea < GIRLS_FINGERS_AREA_POW2))
                {
                    arrActived[i] = eActivedNone;
                }
                else if (eActivedNone == arrActived[i] &&
                         (GetSystickTime()-pKFMiscPara->nDownTick) < MAX_CHECK_HEIGHT_DOWN_TICK)
                {
                    arrActived[i] = eActivedL0;
                }
            }
        }
    }
    int32_t nMinLineCnt = 0;
    int32_t nMID = 0, nAID = 0;
    stTouchH_t *stTouchH = NULL;
    eAreaType_Edge_t eAreaType = eNoneAllType;
    eAreaType_Edge_t eBlindAreaType = eNoneAllType;
    for(int32_t i = 0; i < MAX_POINT_SFX; i++)
    {
        if (arrPointF[i].eStatus != eBtnNone)
        {
            nMID = arrPointF[i].nId;
            nAID = arrPointF[i].nAId;
            stTouchH_t stTouchHUp;
            stTouchH = &stTouchHUp;
            if (arrPointF[i].eStatus != eLeftBtnUp || nAID < g_pAlgCoreVar->nTouchCount)
            {
                stTouchH = &arrTouch[nAID]->stBp.stTouchH; //取出对应的物理信息
            }
            else //解决超多点后可能内存不足的问题
            {
                MemSet(&stTouchHUp, 0, sizeof(stTouchHUp));
                stTouchHUp.nMaxDepth[X] = MIN_DEPATH_VALUE;
                stTouchHUp.nMaxDepth[Y] = MIN_DEPATH_VALUE;
                nAID = -1;
            }

            nMinLineCnt = FN_MIN_SFX(stTouchH->nLineCnt[X], stTouchH->nLineCnt[Y]);
            eAreaType = GetAreaTypeFromEdge(arrPointF[i].stCoordF, eSideTypeP);
            eBlindAreaType = GetAreaTypeFromEdge(arrPointF[i].stCoordF, eSideTypeBMax);
            if (GetAreaType(arrPointF[i].stCoordF, eSideTypeP) < eGeneralType && arrPointF[i].fAreaSize < fLV1AreaSize)
            {
                stTouchH->nMaxDepth[X] = FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]);
                stTouchH->nMaxDepth[Y] = stTouchH->nMaxDepth[X];
            }
            switch (arrPointF[i].eStatus)
            {
                case eLeftBtnDown:
                {
                    s_stProperty[nMID].nDepathCacheLen = 0;
                    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                    {
                        s_stProperty[nMID].nDepathDownReq[eAxis] = s_arrDepDownDef[eAreaType];
                        s_stProperty[nMID].nDepathUpReq[eAxis] = s_arrDepUpDef[eAreaType];
                    }
                    s_stProperty[nMID].nAreaCacheLen = 0;
                    s_stProperty[nMID].nAreaAver = (int16_t)arrPointF[i].fAreaSize;
                    arrPointF[i].eStatus = eBtnNone;
                    s_arrBufferingUp[nMID] = 0;
                    nCount--;
                }
                break;

                case eLeftBtnMove:
                {
                    if (s_arrPoint[i].eStatus == eBtnNone || s_arrPoint[i].eStatus == eLeftBtnUp)
                    {
                        stKFMiscPara_t *pKFMiscPara = GetKFMiscPara(eKFPtTracker, nMID);

                        //面积不再增长
                        int32_t bAreaIncing = (arrPointF[i].fAreaSize > (s_arrPoint[i].fAreaSize*1.1f));

                        if (pTrack->arrCheckHOpt[nMID]) //曾经被触摸高度优化抬起过
                        {
                            if (s_arrDownTryAgain[i])
                            {
                                if ((stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                        stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y]) ||
                                        (arrPointF[i].fAreaSize < fLV1AreaSize &&
                                         FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < FN_MAX_SFX(s_stProperty[nMID].nDepathDownReq[X], s_stProperty[nMID].nDepathDownReq[Y])))
                                {
                                    if (++s_arrDownTryAgain[i] > 3 ||
                                            FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < FN_MAX_SFX(s_stProperty[nMID].nDepathDownReq[X], s_stProperty[nMID].nDepathDownReq[Y]))
                                    {
                                        arrPointF[i].stCoordF = s_arrDownTryAgainCoordF[i];
                                        arrPointF[i].eStatus = eLeftBtnDown;
                                    }
                                }
                                else
                                {
                                    s_arrDownTryAgain[i] = 0;
                                }
                            }
                            else if (!bAreaIncing &&
                                        (stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                        stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y] &&
                                        (nAID < 0 || arrTouch[nAID]->eAreaType != eFillMaskingType)) ||
                                        (g_pAlgCoreVar->nMaskLineCnt > 128 &&
                                        (GetSystickTime()-pKFMiscPara->nDownTick) > 20 &&
                                        (FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < 10)))
                            {
                                s_arrDownTryAgain[i]++;
                                s_arrDownTryAgainCoordF[i] = arrPointF[i].stCoordF;
                            }
                        }
                        else
                        {
                            if (nMinLineCnt != 0)
                            {
                                int32_t bDown = 0;
                                if (arrPointF[i].fAreaSize < PEN_6_AREA_POW2)
                                {
                                    if ((!bAreaIncing &&    //1.面积不再持续增长的前提下，深度满足要求，出点
                                            ((stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] && stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y]) ||
                                             ((GetSystickTime()-pKFMiscPara->nDownTick) > 20 && (FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < 10 ||
                                             (FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < 30 &&
                                              FN_GET_COORDF_DIST_POW2_SFX(arrPointF[i].stCoordF, pKFMiscPara->stDownCoordF) > SLOPING_DISTANCE_DOWN_LIMIT_POW2))))) ||
                                            (!(pActiveArea->nActiveCnt < nEasyTHCount) &&
                                             FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < FN_MAX_SFX(s_stProperty[nMID].nDepathDownReq[X], s_stProperty[nMID].nDepathDownReq[Y]) &&
                                             g_pAlgCoreVar->fLstMaxArea < GIRLS_FINGERS_AREA_POW2))
                                    {
                                        bDown = 1;
                                    }
                                }
                                else if (arrPointF[i].fAreaSize < BOYS_FINGERS_AREA_POW2)
                                {
                                    if (!bAreaIncing &&    //1.面积不再持续增长的前提下，深度满足要求，出点
                                            stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                            stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y] &&
                                            ((GetSystickTime()-pKFMiscPara->nDownTick) > 20 ||
                                                FN_GET_COORDF_DIST_POW2_SFX(arrPointF[i].stCoordF, pKFMiscPara->stDownCoordF) > DISTANCE_DOWN_LIMIT_POW2))
                                    {
                                        bDown = 1;
                                    }
                                }
                                else
                                {
                                    if (!bAreaIncing &&    //1.面积不再持续增长的前提下，深度满足要求，出点
                                            stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                            stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y])
                                    {
                                        bDown = 1;
                                    }
                                }

                                if (bDown)
                                {
                                    arrPointF[i].stCoordF = s_arrPoint[i].stCoordF;
                                    arrPointF[i].eStatus = eLeftBtnDown;
                                }
                            }
                        }
                        if (arrPointF[i].eStatus != eLeftBtnDown)
                        {
                            arrPointF[i].eStatus = eBtnNone;
                            nCount--;
                        }
                        else
                        {
                            CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnDown);
                        }
                    }
                    else if (arrActived[i] != eActivedNone)  //是否需要进行高度优化
                    {
                        if (nMinLineCnt == 0 ||
                                ((int16_t)arrPointF[i].fAreaSize < (s_stProperty[nMID].nAreaAver*90/100) &&
                                ((stTouchH->nMaxDepth[X] > s_stProperty[nMID].nDepathUpReq[X] && (arrPointF[i].fAreaSize > PEN_6_AREA_POW2 || stTouchH->nLineCnt[X] < 12)) ||
                                (stTouchH->nMaxDepth[Y] > s_stProperty[nMID].nDepathUpReq[Y] && (arrPointF[i].fAreaSize > PEN_6_AREA_POW2 || stTouchH->nLineCnt[Y] < 12)))))
                        {
                            if (eLeftBtnUp == (arrPointF[i].eStatus = CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnUp)))
                            {
                                if (s_arrBufferingUp[nMID]++ < 2)
                                {
                                    arrPointF[i].eStatus = eLeftBtnMove;
                                }
                                else
                                {
                                    s_arrDownTryAgain[i] = 0;
                                }
                                arrPointF[i].stCoordF = s_arrPoint[i].stCoordF;
                            }
                        }
                        if (FN_MAX_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < 10)
                        {
                            s_arrBufferingUp[nMID] = 0;
                        }
                    }
                    if (eLeftBtnMove == arrPointF[i].eStatus)
                    {
                        CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnMove);
                    }
                }
                break;

                case eLeftBtnUp:
                {
                    CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnUp);
                    if (s_arrPoint[i].eStatus == eBtnNone || s_arrPoint[i].eStatus == eLeftBtnUp)
                    {
                        arrPointF[i].eStatus = eBtnNone;
                        nCount--;
                    }
                }
                break;

                default:
                break;
            }
            if (eLeftBtnMove == arrPointF[i].eStatus) //Move点状态下，以窗口的方式更新点属性
            {
                int32_t nCurIdx = s_stProperty[nMID].nDepathCacheLen;
                for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                {
                    s_stProperty[nMID].arrDepath[eAxis][nCurIdx] = stTouchH->nMaxDepth[eAxis];
                }
                if (++s_stProperty[nMID].nDepathCacheLen == MAX_DEPATH_CACHE) //深度缓存满了，更新一次阈值
                {
                    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                    {
                        uint8_t  nAverDepath = 0;
                        uint32_t nDepathTotal = 0;
                        for (int32_t j = 0; j < s_stProperty[nMID].nDepathCacheLen; j++)
                        {
                            nDepathTotal += s_stProperty[nMID].arrDepath[eAxis][j];
                        }
                        nAverDepath = nDepathTotal/MAX_DEPATH_CACHE;
                        s_stProperty[nMID].nDepathAver[eAxis] = nAverDepath;

                        if (eBlindAreaType > eNoneAllType && eBlindAreaType < eGeneralAllType)
                        {
                            s_stProperty[nMID].nDepathDownReq[eAxis] = ((nAverDepath+s_arrDyDown[eAreaType])*2) < 65 ? ((nAverDepath+s_arrDyDown[eAreaType])*2) : 65;          //Down上浮值
                            s_stProperty[nMID].nDepathUpReq[eAxis] = ((nAverDepath+s_arrDepUpDef[eAreaType])*2) < 65 ? ((nAverDepath+s_arrDepUpDef[eAreaType])*2) : 65;       //Up上浮值
                        }
                        else
                        {
                            s_stProperty[nMID].nDepathDownReq[eAxis] = FN_MIN_SFX(s_arrDepDownDef[eAreaType], nAverDepath+s_arrDyDown[eAreaType]); //Down上浮值
                            s_stProperty[nMID].nDepathUpReq[eAxis] = FN_MIN_SFX(s_arrDepUpDef[eAreaType], nAverDepath+s_arrDyUp[eAreaType]);       //Up上浮值
                        }
                    }
                    s_stProperty[nMID].nDepathCacheLen = 0;
                }
                nCurIdx = s_stProperty[nMID].nAreaCacheLen;
                s_stProperty[nMID].arrArea[nCurIdx] = (int16_t)arrPointF[i].fAreaSize;
                if (++s_stProperty[nMID].nAreaCacheLen == MAX_AREA_CACHE) //面积缓存满了，更新一次阈值
                {
                    uint32_t nAreaTotal = 0;
                    for (int32_t j = 0; j < s_stProperty[nMID].nAreaCacheLen; j++)
                    {
                        nAreaTotal += s_stProperty[nMID].arrArea[j];
                    }
                    s_stProperty[nMID].nAreaAver = nAreaTotal/MAX_AREA_CACHE;
                    s_stProperty[nMID].nAreaCacheLen = 0;
                }
            }
        }
        s_arrPoint[i].nId = arrPointF[i].nId;
        s_arrPoint[i].eStatus = arrPointF[i].eStatus;
        s_arrPoint[i].fAreaSize = arrPointF[i].fAreaSize;
        s_arrPoint[i].stCoordF = arrPointF[i].stCoordF;
        MemCpy(s_arrPoint[i].nMaxDepth, arrPointF[i].nMaxDepth, sizeof(arrPointF[i].nMaxDepth));
    }
    return nCount;
}
#elif (2503 == CHECK_TOUCH_HEIGHT_MM)         //SW13F 使用触摸高度
/*
 * 对触摸点进行触摸高度优化 尽量去掉远离触摸表面的物体
 * @para arrPointF: 触摸物数组
 * @para nCount: 点数
 * @return: 剩余点数
 */
int32_t CheckTouchHeight(stPointF_t *arrPointF, uint32_t nCount)
{
    eActivedLevel_t arrActived[MAX_POINT_SFX];  //0:不处于活动区域  1:处于未曾到有抬点记录的活动区域  2:处于记录到曾有抬点的活动区域
    stTouch_t **arrTouch = g_pAlgCoreVar->arrTouch;
    stTrack_t *pTrack = &g_pAlgCoreVar->stTrack;

    static stTouchHPointF_t s_arrPoint[MAX_POINT_SFX];
    static stPropertyTrack_t s_stProperty[MAX_POINT_SFX];
    static uint8_t s_arrDepDownDef[eAreaALLTypeTotal] = {0};  //默认落点的深度值要求
    static uint8_t s_arrDepUpDef[eAreaALLTypeTotal] = {0};    //默认抬点的深度值要求
    static uint8_t s_arrDyDown[eAreaALLTypeTotal] = {0};      //根据动态收集的深度参数调整的上浮值
    static uint8_t s_arrDyUp[eAreaALLTypeTotal] = {0};        //根据动态收集的深度参数调整的上浮值
    static uint8_t s_bCheckTouchHeightInit = 0;
    static uint8_t s_arrDownTryAgain[MAX_POINT_SFX] = {0};
    static stCoordF_t s_arrDownTryAgainCoordF[MAX_POINT_SFX];
    const float fLV1AreaSize = 30.0f;
    static uint8_t s_arrBufferingUp[MAX_POINT_SFX] = {0};
    stActiveArea_t *pActiveArea = &g_pAlgCoreVar->stActiveArea;
    int32_t nEasyTHCount = 3;
    stObjCtrlCombin_t *pObjCombin = GetObjCombin();

    if (!s_bCheckTouchHeightInit)  //参数初始化...
    {
        const uint8_t arrDepDownDefInit[eAreaALLTypeTotal] = {0,
                                                        DEFAULT_BOTTOM_NEAR_DOWN_MIN_DEPTH,DEFAULT_BOTTOM_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_BOTTOM_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_TOP_NEAR_DOWN_MIN_DEPTH,DEFAULT_TOP_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_TOP_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_LEFT_NEAR_DOWN_MIN_DEPTH,DEFAULT_LEFT_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_LEFT_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_RIGHT_NEAR_DOWN_MIN_DEPTH,DEFAULT_RIGHT_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_RIGHT_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_GENERAL_DOWN_MIN_DEPTH};

        const uint8_t arrDepUpDefInit[eAreaALLTypeTotal] = {0,
                                                      DEFAULT_BOTTOM_NEAR_UP_MIN_DEPTH,DEFAULT_BOTTOM_MIDDLE_UP_MIN_DEPTH,DEFAULT_BOTTOM_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_TOP_NEAR_UP_MIN_DEPTH,DEFAULT_TOP_MIDDLE_UP_MIN_DEPTH,DEFAULT_TOP_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_LEFT_NEAR_UP_MIN_DEPTH,DEFAULT_LEFT_MIDDLE_UP_MIN_DEPTH,DEFAULT_LEFT_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_RIGHT_NEAR_UP_MIN_DEPTH,DEFAULT_RIGHT_MIDDLE_UP_MIN_DEPTH,DEFAULT_RIGHT_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_GENERAL_UP_MIN_DEPTH};

        const uint8_t arrDyDownInit[eAreaALLTypeTotal] = {0,
                                                    DYNAMIC_BOTTOM_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_BOTTOM_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_BOTTOM_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_TOP_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_TOP_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_TOP_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_LEFT_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_LEFT_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_LEFT_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_RIGHT_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_RIGHT_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_RIGHT_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_GENERAL_DOWN_ADJ_DEPTH};

        const uint8_t arrDyUpInit[eAreaALLTypeTotal] = {0,
                                                  DYNAMIC_BOTTOM_NEAR_UP_ADJ_DEPTH,DYNAMIC_BOTTOM_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_BOTTOM_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_TOP_NEAR_UP_ADJ_DEPTH,DYNAMIC_TOP_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_TOP_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_LEFT_NEAR_UP_ADJ_DEPTH,DYNAMIC_LEFT_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_LEFT_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_RIGHT_NEAR_UP_ADJ_DEPTH,DYNAMIC_RIGHT_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_RIGHT_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_GENERAL_UP_ADJ_DEPTH};
        for (uint8_t i = 0; i < eAreaALLTypeTotal; i++)
        {
            s_arrDepDownDef[i] = arrDepDownDefInit[i];
            s_arrDepUpDef[i] = arrDepUpDefInit[i];
            s_arrDyDown[i] = arrDyDownInit[i];
            s_arrDyUp[i] = arrDyUpInit[i];
        }
        s_bCheckTouchHeightInit = 1;
    }
    //检查是否在画线/写字/点击
    for (int32_t i = 0; i < MAX_POINT_SFX; i++)
    {
        arrActived[i] = eActivedNone;
        if (arrPointF[i].eStatus != eBtnNone)
        {
            //仅在活动区域内进行触摸高度优化，有可能在点击/写字
            arrActived[i] = arrPointF[i].eActived;
            if (eLeftBtnMove == arrPointF[i].eStatus)
            {
                stKFMiscPara_t *pKFMiscPara = GetKFMiscPara(eKFPtTracker, arrPointF[i].nId);
                float fMaxMovement = FN_MAX_SFX(FN_ABS_SUB_SFX(arrPointF[i].stCoordF.fX, pKFMiscPara->stDownCoordF.fX),
                                                FN_ABS_SUB_SFX(arrPointF[i].stCoordF.fY, pKFMiscPara->stDownCoordF.fY));
                if (!(fMaxMovement < MAX_CHECK_HEIGHT_DOWN_DIST) ||
                        (!(pActiveArea->nActiveCnt < nEasyTHCount) &&
                         arrPointF[i].fAreaSize < PEN_8_AREA_POW2 &&
                         g_pAlgCoreVar->fLstMaxArea < GIRLS_FINGERS_AREA_POW2))
                {
                    arrActived[i] = eActivedNone;
                }
                else if (eActivedNone == arrActived[i] &&
                         (GetSystickTime()-pKFMiscPara->nDownTick) < MAX_CHECK_HEIGHT_DOWN_TICK)
                {
                    arrActived[i] = eActivedL0;
                }
            }
        }
    }
    int32_t nMinLineCnt = 0;
    int32_t nMID = 0, nAID = 0;
    stTouchH_t *stTouchH = NULL;
    eAreaType_Edge_t eAreaType = eNoneAllType;
    eAreaType_Edge_t eBlindAreaType = eNoneAllType;
    for(int32_t i = 0; i < MAX_POINT_SFX; i++)
    {
        if (arrPointF[i].eStatus != eBtnNone)
        {
            nMID = arrPointF[i].nId;
            nAID = arrPointF[i].nAId;
            stTouchH_t stTouchHUp;
            stTouchH = &stTouchHUp;
            if (arrPointF[i].eStatus != eLeftBtnUp || nAID < g_pAlgCoreVar->nTouchCount)
            {
                stTouchH = &arrTouch[nAID]->stBp.stTouchH; //取出对应的物理信息
            }
            else //解决超多点后可能内存不足的问题
            {
                MemSet(&stTouchHUp, 0, sizeof(stTouchHUp));
                stTouchHUp.nMaxDepth[X] = MIN_DEPATH_VALUE;
                stTouchHUp.nMaxDepth[Y] = MIN_DEPATH_VALUE;
                nAID = -1;
            }

            nMinLineCnt = FN_MIN_SFX(stTouchH->nLineCnt[X], stTouchH->nLineCnt[Y]);
            eAreaType = GetAreaTypeFromEdge(arrPointF[i].stCoordF, eSideTypeP);
            eBlindAreaType = GetAreaTypeFromEdge(arrPointF[i].stCoordF, eSideTypeBMax);
            if (GetAreaType(arrPointF[i].stCoordF, eSideTypeP) < eGeneralType && arrPointF[i].fAreaSize < fLV1AreaSize)
            {
                stTouchH->nMaxDepth[X] = FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]);
                stTouchH->nMaxDepth[Y] = stTouchH->nMaxDepth[X];
            }
            switch (arrPointF[i].eStatus)
            {
                case eLeftBtnDown:
                {
                    s_stProperty[nMID].nDepathCacheLen = 0;
                    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                    {
                        s_stProperty[nMID].nDepathDownReq[eAxis] = s_arrDepDownDef[eAreaType];
                        s_stProperty[nMID].nDepathUpReq[eAxis] = s_arrDepUpDef[eAreaType];
                    }
                    s_stProperty[nMID].nAreaCacheLen = 0;
                    s_stProperty[nMID].nAreaAver = (int16_t)arrPointF[i].fAreaSize;
                    arrPointF[i].eStatus = eBtnNone;
                    s_arrBufferingUp[nMID] = 0;
                    nCount--;
                }
                break;

                case eLeftBtnMove:
                {
                    if (s_arrPoint[i].eStatus == eBtnNone || s_arrPoint[i].eStatus == eLeftBtnUp)
                    {
                        stKFMiscPara_t *pKFMiscPara = GetKFMiscPara(eKFPtTracker, nMID);

                        //面积不再增长
                        int32_t bAreaIncing = (arrPointF[i].fAreaSize > (s_arrPoint[i].fAreaSize*1.1f));

                        if (pTrack->arrCheckHOpt[nMID]) //曾经被触摸高度优化抬起过
                        {
                            if (s_arrDownTryAgain[i])
                            {
                                if ((stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                        stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y]) ||
                                        (arrPointF[i].fAreaSize < fLV1AreaSize &&
                                         FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < FN_MAX_SFX(s_stProperty[nMID].nDepathDownReq[X], s_stProperty[nMID].nDepathDownReq[Y])))
                                {
                                    if (++s_arrDownTryAgain[i] > 3 ||
                                            FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < FN_MAX_SFX(s_stProperty[nMID].nDepathDownReq[X], s_stProperty[nMID].nDepathDownReq[Y]))
                                    {
                                        arrPointF[i].stCoordF = s_arrDownTryAgainCoordF[i];
                                        arrPointF[i].eStatus = eLeftBtnDown;
                                    }
                                }
                                else
                                {
                                    s_arrDownTryAgain[i] = 0;
                                }
                            }
                            else if (!bAreaIncing &&
                                        stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                        stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y] &&
                                        (nAID < 0 || arrTouch[nAID]->eAreaType != eFillMaskingType))
                            {
                                s_arrDownTryAgain[i]++;
                                s_arrDownTryAgainCoordF[i] = arrPointF[i].stCoordF;
                            }
                        }
                        else
                        {
                            if (nMinLineCnt != 0)
                            {
                                int32_t bDown = 0;
                                float  fCoordDistPow2 = ((FN_MAX_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) >
                                                            FN_MAX_SFX(s_stProperty[nMID].nDepathDownReq[X], s_stProperty[nMID].nDepathDownReq[Y])) &&
                                                            !(pActiveArea->nActiveCnt > 2)) ? 5.0f * 5.0f : 0;

                                if (arrPointF[i].fAreaSize < PEN_6_AREA_POW2)
                                {
                                    if ((!bAreaIncing &&    //1.面积不再持续增长的前提下，深度满足要求，出点
                                            ((stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] && stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y]) ||
                                             (!bAreaIncing && (GetSystickTime()-pKFMiscPara->nDownTick) > 20 && FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < 10 &&
                                             FN_GET_COORDF_DIST_POW2_SFX(arrPointF[i].stCoordF, pKFMiscPara->stDownCoordF) > fCoordDistPow2))) ||
                                            (!(pActiveArea->nActiveCnt < nEasyTHCount) &&
                                             FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < FN_MAX_SFX(s_stProperty[nMID].nDepathDownReq[X], s_stProperty[nMID].nDepathDownReq[Y]) &&
                                             g_pAlgCoreVar->fLstMaxArea < GIRLS_FINGERS_AREA_POW2))
                                    {
                                        bDown = 1;
                                    }
                                }
                                else if (arrPointF[i].fAreaSize < BOYS_FINGERS_AREA_POW2)
                                {
                                    if (!bAreaIncing &&    //1.面积不再持续增长的前提下，深度满足要求，出点
                                            stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                            stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y] &&
                                            ((GetSystickTime()-pKFMiscPara->nDownTick) > 20 ||
                                                FN_GET_COORDF_DIST_POW2_SFX(arrPointF[i].stCoordF, pKFMiscPara->stDownCoordF) > DISTANCE_DOWN_LIMIT_POW2))
                                    {
                                        bDown = 1;
                                    }
                                }
                                else
                                {
                                    if (!bAreaIncing &&    //1.面积不再持续增长的前提下，深度满足要求，出点
                                            stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                            stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y])
                                    {
                                        bDown = 1;
                                    }
                                }

                                if (bDown)
                                {
                                    arrPointF[i].stCoordF = s_arrPoint[i].stCoordF;
                                    arrPointF[i].eStatus = eLeftBtnDown;
                                }
                            }
                        }
                        if (arrPointF[i].eStatus != eLeftBtnDown)
                        {
                            arrPointF[i].eStatus = eBtnNone;
                            nCount--;
                        }
                        else
                        {
                            CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnDown);
                        }
                    }
                    else if (arrActived[i] != eActivedNone)  //是否需要进行高度优化
                    {
                        if (nMinLineCnt == 0 ||
                                ((int16_t)arrPointF[i].fAreaSize < (s_stProperty[nMID].nAreaAver*90/100) &&
                                ((stTouchH->nMaxDepth[X] > s_stProperty[nMID].nDepathUpReq[X] && (arrPointF[i].fAreaSize > PEN_6_AREA_POW2 || stTouchH->nLineCnt[X] < 12)) ||
                                (stTouchH->nMaxDepth[Y] > s_stProperty[nMID].nDepathUpReq[Y] && (arrPointF[i].fAreaSize > PEN_6_AREA_POW2 || stTouchH->nLineCnt[Y] < 12)))))
                        {
                            if (eLeftBtnUp == (arrPointF[i].eStatus = CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnUp)))
                            {
                                if (s_arrBufferingUp[nMID]++ < 2)
                                {
                                    arrPointF[i].eStatus = eLeftBtnMove;
                                }
                                else
                                {
                                    s_arrDownTryAgain[i] = 0;
                                }
                                arrPointF[i].stCoordF = s_arrPoint[i].stCoordF;
                            }
                        }
                        if (FN_MAX_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < 10)
                        {
                            s_arrBufferingUp[nMID] = 0;
                        }
                    }
                    if (eLeftBtnMove == arrPointF[i].eStatus)
                    {
                        CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnMove);
                    }
                }
                break;

                case eLeftBtnUp:
                {
                    CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnUp);
                    if (s_arrPoint[i].eStatus == eBtnNone || s_arrPoint[i].eStatus == eLeftBtnUp)
                    {
                        arrPointF[i].eStatus = eBtnNone;
                        nCount--;
                    }
                }
                break;

                default:
                break;
            }
            if (eLeftBtnMove == arrPointF[i].eStatus) //Move点状态下，以窗口的方式更新点属性
            {
                int32_t nCurIdx = s_stProperty[nMID].nDepathCacheLen;
                for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                {
                    s_stProperty[nMID].arrDepath[eAxis][nCurIdx] = stTouchH->nMaxDepth[eAxis];
                }
                if (++s_stProperty[nMID].nDepathCacheLen == MAX_DEPATH_CACHE) //深度缓存满了，更新一次阈值
                {
                    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                    {
                        uint8_t  nAverDepath = 0;
                        uint32_t nDepathTotal = 0;
                        for (int32_t j = 0; j < s_stProperty[nMID].nDepathCacheLen; j++)
                        {
                            nDepathTotal += s_stProperty[nMID].arrDepath[eAxis][j];
                        }
                        nAverDepath = nDepathTotal/MAX_DEPATH_CACHE;
                        s_stProperty[nMID].nDepathAver[eAxis] = nAverDepath;

                        if (eBlindAreaType > eNoneAllType && eBlindAreaType < eGeneralAllType)
                        {
                            s_stProperty[nMID].nDepathDownReq[eAxis] = ((nAverDepath+s_arrDyDown[eAreaType])*2) < 65 ? ((nAverDepath+s_arrDyDown[eAreaType])*2) : 65;          //Down上浮值
                            s_stProperty[nMID].nDepathUpReq[eAxis] = ((nAverDepath+s_arrDepUpDef[eAreaType])*2) < 65 ? ((nAverDepath+s_arrDepUpDef[eAreaType])*2) : 65;       //Up上浮值
                        }
                        else
                        {
                            s_stProperty[nMID].nDepathDownReq[eAxis] = FN_MIN_SFX(s_arrDepDownDef[eAreaType], nAverDepath+s_arrDyDown[eAreaType]); //Down上浮值
                            s_stProperty[nMID].nDepathUpReq[eAxis] = FN_MIN_SFX(s_arrDepUpDef[eAreaType], nAverDepath+s_arrDyUp[eAreaType]);       //Up上浮值
                        }
                    }
                    s_stProperty[nMID].nDepathCacheLen = 0;
                }
                nCurIdx = s_stProperty[nMID].nAreaCacheLen;
                s_stProperty[nMID].arrArea[nCurIdx] = (int16_t)arrPointF[i].fAreaSize;
                if (++s_stProperty[nMID].nAreaCacheLen == MAX_AREA_CACHE) //面积缓存满了，更新一次阈值
                {
                    uint32_t nAreaTotal = 0;
                    for (int32_t j = 0; j < s_stProperty[nMID].nAreaCacheLen; j++)
                    {
                        nAreaTotal += s_stProperty[nMID].arrArea[j];
                    }
                    s_stProperty[nMID].nAreaAver = nAreaTotal/MAX_AREA_CACHE;
                    s_stProperty[nMID].nAreaCacheLen = 0;
                }
            }
        }
        s_arrPoint[i].nId = arrPointF[i].nId;
        s_arrPoint[i].eStatus = arrPointF[i].eStatus;
        s_arrPoint[i].fAreaSize = arrPointF[i].fAreaSize;
        s_arrPoint[i].stCoordF = arrPointF[i].stCoordF;
    }
    return nCount;
}
#endif

#if(PALM_REJECTION_ENABLE)
int32_t CheckTouchHeight_ForPalm(stPointF_t *arrPointF, uint32_t nCount)
{
    eActivedLevel_t arrActived[MAX_POINT_SFX];  //0:不处于活动区域  1:处于未曾到有抬点记录的活动区域  2:处于记录到曾有抬点的活动区域
    stTouch_t **arrTouch = g_pAlgCoreVar->arrTouch;
    stTrack_t *pTrack = &g_pAlgCoreVar->stTrack;

    static stTouchHPointF_t s_arrPoint[MAX_POINT_SFX];
    static stPropertyTrack_t s_stProperty[MAX_POINT_SFX];
    static uint8_t s_arrDepDownDef[eAreaALLTypeTotal] = {0};  //默认落点的深度值要求
    static uint8_t s_arrDepUpDef[eAreaALLTypeTotal] = {0};    //默认抬点的深度值要求
    static uint8_t s_arrDyDown[eAreaALLTypeTotal] = {0};      //根据动态收集的深度参数调整的上浮值
    static uint8_t s_arrDyUp[eAreaALLTypeTotal] = {0};        //根据动态收集的深度参数调整的上浮值
    static uint8_t s_bCheckTouchHeightInit = 0;
    static uint8_t s_arrDownTryAgain[MAX_POINT_SFX] = {0};
    //static stCoordF_t s_arrDownTryAgainCoordF[MAX_POINT_SFX];
    const float fLV1AreaSize = 30.0f;
    static uint8_t s_arrBufferingUp[MAX_POINT_SFX] = {0};
    stActiveArea_t *pActiveArea = &g_pAlgCoreVar->stActiveArea;
    int32_t nEasyTHCount = 3;

    if (!s_bCheckTouchHeightInit)  //参数初始化...
    {
        const uint8_t arrDepDownDefInit[eAreaALLTypeTotal] = {0,
                                                        DEFAULT_BOTTOM_NEAR_DOWN_MIN_DEPTH,DEFAULT_BOTTOM_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_BOTTOM_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_TOP_NEAR_DOWN_MIN_DEPTH,DEFAULT_TOP_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_TOP_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_LEFT_NEAR_DOWN_MIN_DEPTH,DEFAULT_LEFT_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_LEFT_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_RIGHT_NEAR_DOWN_MIN_DEPTH,DEFAULT_RIGHT_MIDDLE_DOWN_MIN_DEPTH,DEFAULT_RIGHT_FAR_DOWN_MIN_DEPTH,
                                                        DEFAULT_GENERAL_DOWN_MIN_DEPTH};

        const uint8_t arrDepUpDefInit[eAreaALLTypeTotal] = {0,
                                                      DEFAULT_BOTTOM_NEAR_UP_MIN_DEPTH,DEFAULT_BOTTOM_MIDDLE_UP_MIN_DEPTH,DEFAULT_BOTTOM_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_TOP_NEAR_UP_MIN_DEPTH,DEFAULT_TOP_MIDDLE_UP_MIN_DEPTH,DEFAULT_TOP_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_LEFT_NEAR_UP_MIN_DEPTH,DEFAULT_LEFT_MIDDLE_UP_MIN_DEPTH,DEFAULT_LEFT_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_RIGHT_NEAR_UP_MIN_DEPTH,DEFAULT_RIGHT_MIDDLE_UP_MIN_DEPTH,DEFAULT_RIGHT_FAR_UP_MIN_DEPTH,
                                                      DEFAULT_GENERAL_UP_MIN_DEPTH};

        const uint8_t arrDyDownInit[eAreaALLTypeTotal] = {0,
                                                    DYNAMIC_BOTTOM_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_BOTTOM_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_BOTTOM_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_TOP_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_TOP_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_TOP_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_LEFT_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_LEFT_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_LEFT_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_RIGHT_NEAR_DOWN_ADJ_DEPTH,DYNAMIC_RIGHT_MIDDLE_DOWN_ADJ_DEPTH,DYNAMIC_RIGHT_FAR_DOWN_ADJ_DEPTH,
                                                    DYNAMIC_GENERAL_DOWN_ADJ_DEPTH};

        const uint8_t arrDyUpInit[eAreaALLTypeTotal] = {0,
                                                  DYNAMIC_BOTTOM_NEAR_UP_ADJ_DEPTH,DYNAMIC_BOTTOM_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_BOTTOM_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_TOP_NEAR_UP_ADJ_DEPTH,DYNAMIC_TOP_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_TOP_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_LEFT_NEAR_UP_ADJ_DEPTH,DYNAMIC_LEFT_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_LEFT_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_RIGHT_NEAR_UP_ADJ_DEPTH,DYNAMIC_RIGHT_MIDDLE_UP_ADJ_DEPTH,DYNAMIC_RIGHT_FAR_UP_ADJ_DEPTH,
                                                  DYNAMIC_GENERAL_UP_ADJ_DEPTH};
        for (uint8_t i = 0; i < eAreaALLTypeTotal; i++)
        {
            s_arrDepDownDef[i] = arrDepDownDefInit[i];
            s_arrDepUpDef[i] = arrDepUpDefInit[i];
            s_arrDyDown[i] = arrDyDownInit[i];
            s_arrDyUp[i] = arrDyUpInit[i];
        }
        s_bCheckTouchHeightInit = 1;
    }
    //检查是否在画线/写字/点击
    for (int32_t i = 0; i < MAX_POINT_SFX; i++)
    {
        arrActived[i] = eActivedNone;
        if (arrPointF[i].eStatus != eBtnNone)
        {
            //仅在活动区域内进行触摸高度优化，有可能在点击/写字
            arrActived[i] = arrPointF[i].eActived;
            if (eLeftBtnMove == arrPointF[i].eStatus)
            {
                stKFMiscPara_t *pKFMiscPara = GetKFMiscPara(eKFPtTracker, arrPointF[i].nId);
                float fMaxMovement = FN_MAX_SFX(FN_ABS_SUB_SFX(arrPointF[i].stCoordF.fX, pKFMiscPara->stDownCoordF.fX),
                                                FN_ABS_SUB_SFX(arrPointF[i].stCoordF.fY, pKFMiscPara->stDownCoordF.fY));
                if (!(fMaxMovement < MAX_CHECK_HEIGHT_DOWN_DIST) ||
                        (!(pActiveArea->nActiveCnt < nEasyTHCount) &&
                         arrPointF[i].fAreaSize < PEN_8_AREA_POW2 &&
                         g_pAlgCoreVar->fLstMaxArea < GIRLS_FINGERS_AREA_POW2))
                {
                    arrActived[i] = eActivedNone;
                }
                else if (eActivedNone == arrActived[i] &&
                         (GetSystickTime()-pKFMiscPara->nDownTick) < MAX_CHECK_HEIGHT_DOWN_TICK)
                {
                    arrActived[i] = eActivedL0;
                }
            }
        }
    }
    int32_t nMinLineCnt = 0;
    int32_t nMID = 0, nAID = 0;
    stTouchH_t *stTouchH = NULL;
    eAreaType_Edge_t eAreaType = eNoneAllType;
    eAreaType_Edge_t eBlindAreaType = eNoneAllType;
    for(int32_t i = 0; i < MAX_POINT_SFX; i++)
    {
        if (arrPointF[i].eStatus != eBtnNone)
        {
            nMID = arrPointF[i].nId;
            nAID = arrPointF[i].nAId;
            stTouchH_t stTouchHUp;
            stTouchH = &stTouchHUp;
            if (arrPointF[i].eStatus != eLeftBtnUp || nAID < g_pAlgCoreVar->nTouchCount)
            {
                stTouchH = &arrTouch[nAID]->stBp.stTouchH; //取出对应的物理信息
            }
            else //解决超多点后可能内存不足的问题
            {
                MemSet(&stTouchHUp, 0, sizeof(stTouchHUp));
                stTouchHUp.nMaxDepth[X] = MIN_DEPATH_VALUE;
                stTouchHUp.nMaxDepth[Y] = MIN_DEPATH_VALUE;
                nAID = -1;
            }

            nMinLineCnt = FN_MIN_SFX(stTouchH->nLineCnt[X], stTouchH->nLineCnt[Y]);
            eAreaType = GetAreaTypeFromEdge(arrPointF[i].stCoordF, eSideTypeP);
            eBlindAreaType = GetAreaTypeFromEdge(arrPointF[i].stCoordF, eSideTypeBMax);
            if (GetAreaType(arrPointF[i].stCoordF, eSideTypeP) < eGeneralType && arrPointF[i].fAreaSize < fLV1AreaSize)
            {
                stTouchH->nMaxDepth[X] = FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]);
                stTouchH->nMaxDepth[Y] = stTouchH->nMaxDepth[X];
            }
            switch (arrPointF[i].eStatus)
            {
                case eLeftBtnDown:
                {
                    s_stProperty[nMID].nDepathCacheLen = 0;
                    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                    {
                        s_stProperty[nMID].nDepathDownReq[eAxis] = s_arrDepDownDef[eAreaType];
                        s_stProperty[nMID].nDepathUpReq[eAxis] = s_arrDepUpDef[eAreaType];
                    }
                    s_stProperty[nMID].nAreaCacheLen = 0;
                    s_stProperty[nMID].nAreaAver = (int16_t)arrPointF[i].fAreaSize;
                    arrPointF[i].eStatus = eBtnNone;
                    s_arrBufferingUp[nMID] = 0;
                    nCount--;
//                    CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnDown);
                }
                break;

                case eLeftBtnMove:
                {
                    if (s_arrPoint[i].eStatus == eBtnNone || s_arrPoint[i].eStatus == eLeftBtnUp)
                    {
                        int32_t bAreaIncing = (arrPointF[i].fAreaSize > (s_arrPoint[i].fAreaSize*1.1f));
                        bAreaIncing = 0;/*手笔分离尽量落点条件放宽*/
                        stKFMiscPara_t *pKFMiscPara = GetKFMiscPara(eKFPtTracker, nMID);

                        if (pTrack->arrCheckHOpt[nMID]) //曾经被触摸高度优化抬起过
                        {
                            if (s_arrDownTryAgain[i])
                            {
                                if ((stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                        stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y]) ||
                                        (arrPointF[i].fAreaSize < fLV1AreaSize &&
                                         FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < FN_MAX_SFX(s_stProperty[nMID].nDepathDownReq[X], s_stProperty[nMID].nDepathDownReq[Y])))
                                {
                                    if (++s_arrDownTryAgain[i] > 3 ||
                                            FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < FN_MAX_SFX(s_stProperty[nMID].nDepathDownReq[X], s_stProperty[nMID].nDepathDownReq[Y]))
                                    {
                                        //arrPointF[i].stCoordF = s_arrDownTryAgainCoordF[i];
                                        arrPointF[i].eStatus = eLeftBtnDown;
                                    }
                                }
                                else
                                {
                                    s_arrDownTryAgain[i] = 0;
                                }
                            }
                            else if (!bAreaIncing &&
                                        stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                        stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y] &&
                                        (nAID < 0 || arrTouch[nAID]->eAreaType != eFillMaskingType))
                            {
                                s_arrDownTryAgain[i]++;
                                //s_arrDownTryAgainCoordF[i] = arrPointF[i].stCoordF;
                            }
                        }
                        else
                        {
                            if (nMinLineCnt != 0)
                            {
                                int32_t bDown = 0;
                                if (arrPointF[i].fAreaSize < PEN_6_AREA_POW2)
                                {
                                    if ((!bAreaIncing &&    //1.面积不再持续增长的前提下，深度满足要求，出点
                                            ((stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] && stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y]) ||
                                             ((GetSystickTime()-pKFMiscPara->nDownTick) > 20 && FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < 10))) ||
                                            (!(pActiveArea->nActiveCnt < nEasyTHCount) &&
                                             FN_MIN_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < FN_MAX_SFX(s_stProperty[nMID].nDepathDownReq[X], s_stProperty[nMID].nDepathDownReq[Y]) &&
                                             g_pAlgCoreVar->fLstMaxArea < GIRLS_FINGERS_AREA_POW2))
                                    {
                                        bDown = 1;
                                    }
                                }
                                else if (arrPointF[i].fAreaSize < BOYS_FINGERS_AREA_POW2)
                                {
                                    if (!bAreaIncing &&    //1.面积不再持续增长的前提下，深度满足要求，出点
                                            stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                            stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y] &&
                                            ((GetSystickTime()-pKFMiscPara->nDownTick) > 20 ||
                                                FN_GET_COORDF_DIST_POW2_SFX(arrPointF[i].stCoordF, pKFMiscPara->stDownCoordF) > DISTANCE_DOWN_LIMIT_POW2))
                                    {
                                        bDown = 1;
                                    }
                                }
                                else
                                {
                                    if (!bAreaIncing &&    //1.面积不再持续增长的前提下，深度满足要求，出点
                                            stTouchH->nMaxDepth[X] < s_stProperty[nMID].nDepathDownReq[X] &&
                                            stTouchH->nMaxDepth[Y] < s_stProperty[nMID].nDepathDownReq[Y])
                                    {
                                        bDown = 1;
                                    }
                                }

                                if (bDown)
                                {
                                    //arrPointF[i].stCoordF = s_arrPoint[i].stCoordF;
                                    arrPointF[i].eStatus = eLeftBtnDown;
                                }
                            }
                        }
                        if (arrPointF[i].eStatus != eLeftBtnDown)
                        {
                            arrPointF[i].eStatus = eBtnNone;
                            nCount--;
                        }
                        else
                        {
                            CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnDown);
                        }
                    }
                    else if (arrActived[i] != eActivedNone)  //是否需要进行高度优化
                    {
                        if (nMinLineCnt == 0 ||
                                ((int16_t)arrPointF[i].fAreaSize < (s_stProperty[nMID].nAreaAver*90/100) &&
                                ((stTouchH->nMaxDepth[X] > s_stProperty[nMID].nDepathUpReq[X] && (arrPointF[i].fAreaSize > PEN_6_AREA_POW2 || stTouchH->nLineCnt[X] < 12)) ||
                                (stTouchH->nMaxDepth[Y] > s_stProperty[nMID].nDepathUpReq[Y] && (arrPointF[i].fAreaSize > PEN_6_AREA_POW2 || stTouchH->nLineCnt[Y] < 12)))))
                        {
                            if (eLeftBtnUp == (arrPointF[i].eStatus = CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnUp)))
                            {
                                if (s_arrBufferingUp[nMID]++ < 2)
                                {
                                    arrPointF[i].eStatus = eLeftBtnMove;
                                }
                                else
                                {
                                    s_arrDownTryAgain[i] = 0;
                                }
                                //arrPointF[i].stCoordF = s_arrPoint[i].stCoordF;
                            }
                        }
                        if (FN_MAX_SFX(stTouchH->nMaxDepth[X], stTouchH->nMaxDepth[Y]) < 10)
                        {
                            s_arrBufferingUp[nMID] = 0;
                        }
                    }
                    if (eLeftBtnMove == arrPointF[i].eStatus)
                    {
                        CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnMove);
                    }
                }
                break;

                case eLeftBtnUp:
                {
                    CheckTouchHCallBack(&arrPointF[i], arrPointF[i].eStatus, eLeftBtnUp);
                    if (s_arrPoint[i].eStatus == eBtnNone || s_arrPoint[i].eStatus == eLeftBtnUp)
                    {
                        arrPointF[i].eStatus = eBtnNone;
                        nCount--;
                    }
                }
                break;

                default:
                break;
            }
            if (eLeftBtnMove == arrPointF[i].eStatus) //Move点状态下，以窗口的方式更新点属性
            {
                int32_t nCurIdx = s_stProperty[nMID].nDepathCacheLen;
                for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                {
                    s_stProperty[nMID].arrDepath[eAxis][nCurIdx] = stTouchH->nMaxDepth[eAxis];
                }
                if (++s_stProperty[nMID].nDepathCacheLen == MAX_DEPATH_CACHE) //深度缓存满了，更新一次阈值
                {
                    for (eAxis_t eAxis = X; eAxis < eAxisCount; eAxis++)
                    {
                        uint8_t  nAverDepath = 0;
                        uint32_t nDepathTotal = 0;
                        for (int32_t j = 0; j < s_stProperty[nMID].nDepathCacheLen; j++)
                        {
                            nDepathTotal += s_stProperty[nMID].arrDepath[eAxis][j];
                        }
                        nAverDepath = nDepathTotal/MAX_DEPATH_CACHE;
                        s_stProperty[nMID].nDepathAver[eAxis] = nAverDepath;

                        if (eBlindAreaType > eNoneAllType && eBlindAreaType < eGeneralAllType)
                        {
                            s_stProperty[nMID].nDepathDownReq[eAxis] = ((nAverDepath+s_arrDyDown[eAreaType])*2) < 65 ? ((nAverDepath+s_arrDyDown[eAreaType])*2) : 65;          //Down上浮值
                            s_stProperty[nMID].nDepathUpReq[eAxis] = ((nAverDepath+s_arrDepUpDef[eAreaType])*2) < 65 ? ((nAverDepath+s_arrDepUpDef[eAreaType])*2) : 65;       //Up上浮值
                        }
                        else
                        {
                            s_stProperty[nMID].nDepathDownReq[eAxis] = FN_MIN_SFX(s_arrDepDownDef[eAreaType], nAverDepath+s_arrDyDown[eAreaType]); //Down上浮值
                            s_stProperty[nMID].nDepathUpReq[eAxis] = FN_MIN_SFX(s_arrDepUpDef[eAreaType], nAverDepath+s_arrDyUp[eAreaType]);       //Up上浮值
                        }
                    }
                    s_stProperty[nMID].nDepathCacheLen = 0;
                }
                nCurIdx = s_stProperty[nMID].nAreaCacheLen;
                s_stProperty[nMID].arrArea[nCurIdx] = (int16_t)arrPointF[i].fAreaSize;
                if (++s_stProperty[nMID].nAreaCacheLen == MAX_AREA_CACHE) //面积缓存满了，更新一次阈值
                {
                    uint32_t nAreaTotal = 0;
                    for (int32_t j = 0; j < s_stProperty[nMID].nAreaCacheLen; j++)
                    {
                        nAreaTotal += s_stProperty[nMID].arrArea[j];
                    }
                    s_stProperty[nMID].nAreaAver = nAreaTotal/MAX_AREA_CACHE;
                    s_stProperty[nMID].nAreaCacheLen = 0;
                }
            }
        }
        s_arrPoint[i].nId = arrPointF[i].nId;
        s_arrPoint[i].eStatus = arrPointF[i].eStatus;
        s_arrPoint[i].fAreaSize = arrPointF[i].fAreaSize;
        s_arrPoint[i].stCoordF = arrPointF[i].stCoordF;
    }
    return nCount;
}
#endif
#pragma arm section

#endif

#ifndef QT_ENV
#include <string.h>
#include <stdio.h>
#include "Calibration.h"
#include "PointTransform.h"
#include "Api_CommandQueue.h"
#include "Api_Math.h"
#include "Command.h"
#include "CommService.h"
#include "Communication.h"
#include "SendCoord.h"
#include "FlashPara.h"
#include "ProtocolAnalysis.h"
#include "Timer.h"
#include "Api_Flash.h"
#include "OtherFunction.h"
#include "Api_Gpio.h"
#include "Gesture.h"
#include "PenetratingRegion.h"
#include "GlobalVariables.h"
#include "scan.h"

//seewo 私有存储KEY命令
#define PRODUCT_SERIAL_ID_SIZE                  56
#define USERKEY_GET_PRODUCT_SERIAL_ID           0x05
#define USERKEY_GET_PRODUCT_SERIAL_ID_RE        0x85
#define USERKEY_SAVE_PRODUCT_SERIAL_ID          0x06
#define USERKEY_SAVE_PRODUCT_SERIAL_ID_RE       0x86

//seewo 免驱数据获取与设置子命令, 主命令是 0xA9
#define ANDR_DRIVE_FREE_PEN_SIZE_SET                0x10//笔径设置
#define ANDR_DRIVE_FREE_SMOOTH_LEVEL_SET            0x20//平滑等级设置
#define ANDR_DRIVE_FREE_AREA_SET                    0x30//面积需求设置
#define ANDROIDT_DRIVE_FREE_SET_OK_ACK              0Xf1//设置成功回复
#define ANDROIDT_DRIVE_FREE_SET_ERR_ACK             0xf2//设置失败回复
#define ANDROIDT_DRIVE_FREE_PEN_SIZE_QUERY          0x11//笔径查询
#define ANDROIDT_DRIVE_FREE_PEN_SIZE_QUERY_ACK      0x12//笔径查询回复
#define ANDR_DRIVE_FREE_SMOOTH_LEVEL_QUERY          0x21//平滑等级查询
#define ANDR_DRIVE_FREE_SMOOTH_LEVEL_ACK            0x22//平滑等级查询回复
#define ANDR_DRIVE_FREE_AREA_QUERY                  0x31//面积需求查询
#define ANDR_DRIVE_FREE_AREA_QUERY_ACK              0x32//面积需求查询回复
#define ANDR_DRIVE_FREE_MAX_POINT_SET               0x40//最大点数设置
#define ANDR_DRIVE_FREE_SPLICE_MODE_SET             0x50//双拼模式设置
#define ANDR_DRIVE_FREE_REPORT_AREA_SET             0x60//面积上报设置
#define ANDR_DRIVE_FREE_MAX_POINT_QUERY             0x41//最大点数查询
#define ANDR_DRIVE_FREE_MAX_POINT_QUERY_ACK         0x42//最大点数查询回复
#define ANDR_DRIVE_FREE_SPLICE_MODE_QUERY           0x51//双拼模式查询
#define ANDR_DRIVE_FREE_SPLICE_MODE_QUERY_ACK       0x52//双拼模式查询回复
#define ANDR_DRIVE_FREE_REPORT_AREA_QUERY           0x61//面积上报查询
#define ANDR_DRIVE_FREE_REPORT_AREA_QUERY_ACK       0x62//面积上报查询回复
#define ANDR_DRIVE_FREE_PALM_REJECTION_SET          0x70//防掌触设置
#define ANDR_DRIVE_FREE_PALM_REJECTION_QUERY        0x71//防掌触开关查询
#define ANDR_DRIVE_FREE_PALM_REJECTION_QUERY_ACK    0x72//防掌触开关查询回复
#define ANDR_DRIVE_FREE_TOUCH_PROTOCOL_SET          0x80//触控协议设置(新旧协议)
#define ANDR_DRIVE_FREE_TOUCH_PROTOCOL_QUERY        0x81//触控协议查询
#define ANDR_DRIVE_FREE_TOUCH_PROTOCOL_QUERY_ACK    0x82//触控协议查询回复
#define ANDR_DRIVE_FREE_SN_NUM_QUERY                0x91//触摸框SN序列号查询
#define ANDR_DRIVE_FREE_SN_NUM_QUERY_ACK            0x92//触摸框SN序列号查询回复
#define ANDR_DRIVE_FREE_TAKE_UP_TO_WRITE_SET        0xA0//提笔即写功能设置
#define ANDR_DRIVE_FREE_TAKE_UP_TO_WRITE_QUERY      0xA1//提笔即写功能设置查询
#define ANDR_DRIVE_FREE_TAKE_UP_TO_WRITE_QUERY_ACK  0xA2//提笔即写功能设置查询回复
#define ANDR_DRIVE_FREE_TAKE_UP_TO_WRITE_CHECK      0xA3//提笔即写握手检查

static volatile int32_t sg_nUserPoint = USER_POINT;

//客户私有USB协议数据保存
typedef struct
{
    uint32_t nVid;
    uint32_t nPid;
    uint32_t nFwVer;
    uint32_t nCrc;
} stUsbPrivData_t;

//客户私有数据保存
typedef struct
{
    uint8_t  arrSerialID[PRODUCT_SERIAL_ID_SIZE];
    stUsbPrivData_t stUsbPrivData[eUsbDevCount];
} stCusPrivData_t;

/*
 * 获取产品序列号ID
 * @return: 返回产品序列号ID数据长度
 */
uint32_t FlashGetProductSerialID(uint8_t *SerialID, int32_t nLen)
{
    stCusPrivData_t stCusPrivData;
    GetDataList(eCustomerPrivateData, (uint8_t *)&stCusPrivData, sizeof(stCusPrivData_t));
    memcpy(SerialID, stCusPrivData.arrSerialID, FN_MIN(PRODUCT_SERIAL_ID_SIZE, nLen));
    return FN_MIN(PRODUCT_SERIAL_ID_SIZE, nLen);
}

/*
 * 写入产品序列号ID
 */
void FlashSetProductSerialID(uint8_t *SerialID, int32_t nLen)
{
    stCusPrivData_t stCusPrivData;
    GetDataList(eCustomerPrivateData, (uint8_t *)&stCusPrivData, sizeof(stCusPrivData_t));
    memcpy(stCusPrivData.arrSerialID, SerialID, FN_MIN(PRODUCT_SERIAL_ID_SIZE, nLen));
    SaveDataList(eCustomerPrivateData, (uint8_t *)&stCusPrivData, sizeof(stCusPrivData_t));
}

/*
 * 获取私有USB协议数据
 * @return: 返回私有USB协议数据
 */
uint32_t FlashGetUsbPrivData(eUsbDevice_t eUsbDev, stUsbPrivData_t *pUsbPrivData, int32_t nLen)
{
    stCusPrivData_t stCusPrivData;
    GetDataList(eCustomerPrivateData, (uint8_t *)&stCusPrivData, sizeof(stCusPrivData_t));
    if (stCusPrivData.stUsbPrivData[eUsbDev].nCrc !=
            GetCRC32((uint8_t *)&stCusPrivData.stUsbPrivData[eUsbDev], sizeof(stUsbPrivData_t)-4))
    {
        if (eUsbDev == eUsbDev0)
        {
            stCusPrivData.stUsbPrivData[eUsbDev].nVid = USB0_VID;
            stCusPrivData.stUsbPrivData[eUsbDev].nPid = USB0_PID;
            stCusPrivData.stUsbPrivData[eUsbDev].nFwVer = Firmware_Version;
        }
        else if (eUsbDev == eUsbDev1)
        {
            stCusPrivData.stUsbPrivData[eUsbDev].nVid = USB1_VID;
            stCusPrivData.stUsbPrivData[eUsbDev].nPid = USB1_PID;
            stCusPrivData.stUsbPrivData[eUsbDev].nFwVer = Firmware_Version;
        }
        stCusPrivData.stUsbPrivData[eUsbDev].nCrc = GetCRC32((uint8_t *)&stCusPrivData.stUsbPrivData[eUsbDev], sizeof(stUsbPrivData_t)-4);
        SaveDataList(eCustomerPrivateData, (uint8_t *)&stCusPrivData, sizeof(stCusPrivData_t));
    }
    *pUsbPrivData = stCusPrivData.stUsbPrivData[eUsbDev];
    return FN_MIN(sizeof(stUsbPrivData_t), nLen);
}

/*
 * 写入私有USB协议数据
 */
void FlashSetUsbPrivData(eUsbDevice_t eUsbDev, stUsbPrivData_t *pUsbPrivData, int32_t nLen)
{
    stCusPrivData_t stCusPrivData;
    GetDataList(eCustomerPrivateData, (uint8_t *)&stCusPrivData, sizeof(stCusPrivData_t));
    stCusPrivData.stUsbPrivData[eUsbDev] = *pUsbPrivData;
    stCusPrivData.stUsbPrivData[eUsbDev].nCrc = GetCRC32((uint8_t *)&stCusPrivData.stUsbPrivData[eUsbDev], sizeof(stUsbPrivData_t)-4);
    SaveDataList(eCustomerPrivateData, (uint8_t *)&stCusPrivData, sizeof(stCusPrivData_t));
}

/*****************************************************************
 * @Function: 处理用户相关的数据保存
 * @Parameter: [pCmdBlock] 通讯输入的内容
             : [pReCmdBlock]通讯输出的内容
 * @return   : 处理应答的方式
*****************************************************************/
eCmdAnswerType_t CmdUserKey(stCmdQueueBlock_t* pCmdBlock, stCmdQueueBlock_t* pReCmdBlock)
{
    eCmdAnswerType_t eRetVal = eCmdAnswerNone;
    stObjCtrlCombin_t *pObjCombin = GetObjCombin();
    static uint8_t s_TakeUpToWriteStep = 0;

    switch(pCmdBlock->DataPacket[2])
    {
    case USERKEY_GET_ID:
    {
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = USERKEY_GET_ID_RE;
        pReCmdBlock->DataPacket[3] = 16;
        pReCmdBlock->DataPacket[4] = 0;
        GetUniqueID(&(pReCmdBlock->DataPacket[5]), 16);
        pReCmdBlock->nDataLen = 21;
        eRetVal = eCmdAnswerData;
    }
    break;

    case USERKEY_GET_KEY:
    {
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = USERKEY_GET_KEY_RE;
        pReCmdBlock->DataPacket[3] = 56;
        pReCmdBlock->DataPacket[4] = 0x00;
        memcpy(&(pReCmdBlock->DataPacket[5]), g_UserKey, 56);
        pReCmdBlock->nDataLen = 64;
        eRetVal = eCmdAnswerData;
    }
    break;

    case USERKEY_SET_KEY:
    {
        memcpy(g_UserKey, &(pCmdBlock->DataPacket[5]), 56);
        eRetVal = eCmdAnswerOK;
    }
    break;

    case USERKEY_RELOAD_KEY:
    {
        GetUserKey();
        eRetVal = eCmdAnswerOK;
    }
    break;

    case USERKEY_SAVE_KEY:
    {
        SaveUserKey();
        eRetVal = eCmdAnswerOK;
        //printf("Save Aging Time\r\n");
    }
    break;
/*
 * The following is the Android drive-free agreement
 */
    case ANDR_DRIVE_FREE_PEN_SIZE_SET:
    {
        //获取该版本下的Base值
        eObjRecTypeGp_t eBase = (eObjRecTypeGp_t)(RECOGNISE_OBJ_DEFAULT_GROUP - DEFAULT_PENSIZE_TYP);
        if (0 != RecogniseObjectInit((eObjRecTypeGp_t)(eBase+pCmdBlock->DataPacket[5])))
        {
            printf("RecogniseObjectInit error\n");
        }

        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_OK_ACK;
        pReCmdBlock->DataPacket[3] = 0x00;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->nDataLen = 5;
        eRetVal = eCmdAnswerData;
    }
    break;
    case ANDR_DRIVE_FREE_SMOOTH_LEVEL_SET:
    {
        g_eSmoothLevel = pCmdBlock->DataPacket[5];
        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_OK_ACK;
        pReCmdBlock->DataPacket[3] = 0x00;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->nDataLen = 5;
        eRetVal = eCmdAnswerData;
    }
    break;
    case ANDR_DRIVE_FREE_AREA_SET:
    {
        pObjCombin->bRecoObjEnable = pCmdBlock->DataPacket[5];
        if (1)
        {
            pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_OK_ACK;
        }
        else
        {
            pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_ERR_ACK;
        }
        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[3] = 0x00;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->nDataLen = 5;
        eRetVal = eCmdAnswerData;
    }
    break;
    case ANDROIDT_DRIVE_FREE_PEN_SIZE_QUERY:
    {
        //获取该版本下的Base值
        eObjRecTypeGp_t eBase = (eObjRecTypeGp_t)(RECOGNISE_OBJ_DEFAULT_GROUP - DEFAULT_PENSIZE_TYP);
        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_PEN_SIZE_QUERY_ACK;
        pReCmdBlock->DataPacket[3] = 0x01;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->DataPacket[5] = (uint8_t)(pObjCombin->eObjTypeGroup-eBase);
        pReCmdBlock->nDataLen = 6;
        eRetVal = eCmdAnswerData;
    }
    break;
    case ANDR_DRIVE_FREE_SMOOTH_LEVEL_QUERY:
    {
        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = ANDR_DRIVE_FREE_SMOOTH_LEVEL_ACK;
        pReCmdBlock->DataPacket[3] = 0x01;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->DataPacket[5] = g_eSmoothLevel;
        pReCmdBlock->nDataLen = 6;
        eRetVal = eCmdAnswerData;
    }
    break;

    case ANDR_DRIVE_FREE_AREA_QUERY:
    {
        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = ANDR_DRIVE_FREE_AREA_QUERY_ACK;
        pReCmdBlock->DataPacket[3] = 0x01;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->DataPacket[5] = pObjCombin->bRecoObjEnable;
        pReCmdBlock->nDataLen = 6;
        eRetVal = eCmdAnswerData;
    }
    break;

    case ANDR_DRIVE_FREE_MAX_POINT_SET:         //最大点数设置
    {
        sg_nUserPoint = FN_MIN(MAX_POINT, pCmdBlock->DataPacket[5]);

        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_OK_ACK;
        pReCmdBlock->DataPacket[3] = 0x00;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->nDataLen = 5;
        eRetVal = eCmdAnswerData;
    }
    break;

    case ANDR_DRIVE_FREE_MAX_POINT_QUERY:       //最大触摸点数查询
    {
        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = ANDR_DRIVE_FREE_MAX_POINT_QUERY_ACK;
        pReCmdBlock->DataPacket[3] = 0x01;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->DataPacket[5] = sg_nUserPoint;
        pReCmdBlock->nDataLen = 6;
        eRetVal = eCmdAnswerData;
    }
    break;

    case ANDR_DRIVE_FREE_SPLICE_MODE_SET:       //双拼模式设置
    {
        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_OK_ACK;
        pReCmdBlock->DataPacket[3] = 0x00;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->nDataLen = 5;
        eRetVal = eCmdAnswerData;
    }
    break;

    case ANDR_DRIVE_FREE_SPLICE_MODE_QUERY:     //双拼模式查询
    {
        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = ANDR_DRIVE_FREE_SPLICE_MODE_QUERY_ACK;
        pReCmdBlock->DataPacket[3] = 0x01;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->DataPacket[5] = 0x00;
        pReCmdBlock->nDataLen = 6;
        eRetVal = eCmdAnswerData;
    }
    break;

    case ANDR_DRIVE_FREE_REPORT_AREA_SET:       //上报面积设置(物理值/逻辑值)
    {
        if(pCmdBlock->DataPacket[5] <= 1)
        {
            g_eTouchSizeType = 3 - 3 * pCmdBlock->DataPacket[5];
            pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_OK_ACK;
        }
        else
        {
            pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_ERR_ACK;
        }

        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[3] = 0x00;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->nDataLen = 5;
        eRetVal = eCmdAnswerData;
    }
    break;

    case ANDR_DRIVE_FREE_REPORT_AREA_QUERY:     //上报面积查询
    {
        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = ANDR_DRIVE_FREE_REPORT_AREA_QUERY_ACK;
        pReCmdBlock->DataPacket[3] = 0x01;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->DataPacket[5] = g_eTouchSizeType ? 0x00 : 0x01;
        pReCmdBlock->nDataLen = 6;
        eRetVal = eCmdAnswerData;
    }
    break;

    case USERKEY_GET_PRODUCT_SERIAL_ID:
    {
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = USERKEYNEW;
        pReCmdBlock->DataPacket[2] = USERKEY_GET_PRODUCT_SERIAL_ID_RE;
        pReCmdBlock->DataPacket[3] = PRODUCT_SERIAL_ID_SIZE;
        pReCmdBlock->nDataLen = pCmdBlock->nDataLen;
        FlashGetProductSerialID(&pReCmdBlock->DataPacket[5], PRODUCT_SERIAL_ID_SIZE);
        eRetVal = eCmdAnswerData;
    }
    break;

    case USERKEY_SAVE_PRODUCT_SERIAL_ID:
    {
        FlashSetProductSerialID(&pCmdBlock->DataPacket[5], pCmdBlock->DataPacket[3]);
        eRetVal = eCmdAnswerOK;
    }
    break;

    case ANDR_DRIVE_FREE_PALM_REJECTION_SET:    //防掌触设置
    {
        if(pCmdBlock->DataPacket[5] <= ePalmProcTypeOnlyHidePalm)
        {
            g_ePalmRejectionType = (ePalmProcType_t)pCmdBlock->DataPacket[5];
            pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_OK_ACK;
        }
        else
        {
            pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_ERR_ACK;
        }

        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[3] = 0x00;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->nDataLen = 5;
        eRetVal = eCmdAnswerData;
    }
    break;

    case ANDR_DRIVE_FREE_PALM_REJECTION_QUERY:
    {
        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = ANDR_DRIVE_FREE_PALM_REJECTION_QUERY_ACK;
        pReCmdBlock->DataPacket[3] = 0x01;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->DataPacket[5] = g_ePalmRejectionType;
        pReCmdBlock->nDataLen = 6;
        eRetVal = eCmdAnswerData;
    }
    break;

    case ANDR_DRIVE_FREE_TOUCH_PROTOCOL_SET:
    {
        extern volatile uint8_t g_bUsbCoordVersion; 
        extern stUsbCoordCfg_t g_stUsbCoordCfg[eUsbDevCount];
        uint8_t nUsbReportVersion = 0;
        //开启了eUsbCfgTouch2Bit才支持切换功能
        if(pCmdBlock->DataPacket[5] == 0 && g_eUsbCfgType[eUsbDev0] & eUsbCfgTouch2Bit)
        {
            g_stUsbCoordCfg[eUsbDev0].eTouchItf = eUsbCfgTouch1Bit;
            nUsbReportVersion = pCmdBlock->DataPacket[5];
            pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_OK_ACK;
        }
        else if(pCmdBlock->DataPacket[5] == 1 && g_eUsbCfgType[eUsbDev0] & eUsbCfgTouch2Bit)
        {
            g_stUsbCoordCfg[eUsbDev0].eTouchItf = eUsbCfgTouch2Bit;
            nUsbReportVersion = pCmdBlock->DataPacket[5];
            pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_OK_ACK;
        }
        else
        {
            pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_ERR_ACK;
        }
        g_bUsbCoordVersion = nUsbReportVersion;
        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[3] = 0x00;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->nDataLen = 5;
        eRetVal = eCmdAnswerData;
    }
    break;

    case ANDR_DRIVE_FREE_TOUCH_PROTOCOL_QUERY:
    {
        extern volatile uint8_t g_bUsbCoordVersion; 
        
        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = ANDR_DRIVE_FREE_TOUCH_PROTOCOL_QUERY_ACK;
        pReCmdBlock->DataPacket[3] = 0x01;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->DataPacket[5] = g_bUsbCoordVersion;
        pReCmdBlock->nDataLen = 6;
        eRetVal = eCmdAnswerData;

    }
    break;

    case ANDR_DRIVE_FREE_SN_NUM_QUERY:
    {
        uint8_t nQrCodeData[64];
        uint8_t nChecksum = 0;
        uint8_t nQrCodeLen = FlashGetQrCode(nQrCodeData);

        for(int i = 0; i< (1 + nQrCodeLen); i++)
        {
            nChecksum += nQrCodeData[i];
        }
        
        if(nChecksum != nQrCodeData[1 + nQrCodeLen])
        {
            printf("Get QrCode Checksum Err[%x,%x]!!!\n",nChecksum,nQrCodeData[1 + nQrCodeLen]);
            eRetVal = eCmdAnswerFail;
        }
        else
        {
            pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
            pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
            pReCmdBlock->DataPacket[2] = ANDR_DRIVE_FREE_SN_NUM_QUERY_ACK;
            pReCmdBlock->DataPacket[3] = nQrCodeLen;
            pReCmdBlock->DataPacket[4] = 0;

            MemCpy(&pReCmdBlock->DataPacket[5], nQrCodeData, nQrCodeLen+2);
            pReCmdBlock->nDataLen = (nQrCodeLen + 5 + 2);
            eRetVal = eCmdAnswerData;
        }
    }
    break;

    case ANDR_DRIVE_FREE_TAKE_UP_TO_WRITE_SET:
    {
        if(pCmdBlock->DataPacket[5] <= 1 && pCmdBlock->DataPacket[6] <= 1)
        {
            s_TakeUpToWriteStep = pCmdBlock->DataPacket[5];
            if(0 == s_TakeUpToWriteStep)
            {
                g_pAlgCoreVar->stTakeUp2Write.bEnable = 0;
            }
            #if 0
            g_pAlgCoreVar->stTakeUp2Write.bEnable = pCmdBlock->DataPacket[5];
            if(g_pAlgCoreVar->stTakeUp2Write.bEnable)
            {
                g_pAlgCoreVar->stTakeUp2Write.bStatus = pCmdBlock->DataPacket[6];
            }
            else
            {
                g_pAlgCoreVar->stTakeUp2Write.bStatus = 0;
            }
            #endif
            pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_OK_ACK;
        }
        else
        {
            pReCmdBlock->DataPacket[2] = ANDROIDT_DRIVE_FREE_SET_ERR_ACK;
        }

        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[3] = 0x00;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->nDataLen = 5;
        eRetVal = eCmdAnswerData;
    }
    break;

    case ANDR_DRIVE_FREE_TAKE_UP_TO_WRITE_QUERY:
    {
        pReCmdBlock->DataPacket[0] = pCmdBlock->DataPacket[0];
        pReCmdBlock->DataPacket[1] = pCmdBlock->DataPacket[1];
        pReCmdBlock->DataPacket[2] = ANDR_DRIVE_FREE_TAKE_UP_TO_WRITE_QUERY_ACK;
        pReCmdBlock->DataPacket[3] = 0x02;
        pReCmdBlock->DataPacket[4] = 0x00;
        pReCmdBlock->DataPacket[5] = g_pAlgCoreVar->stTakeUp2Write.bEnable;
        pReCmdBlock->DataPacket[6] = g_pAlgCoreVar->stTakeUp2Write.bStatus;
        pReCmdBlock->nDataLen = 7;
        eRetVal = eCmdAnswerData;
    }
    break;

    case ANDR_DRIVE_FREE_TAKE_UP_TO_WRITE_CHECK:
    {
        if(1 == s_TakeUpToWriteStep && pCmdBlock->DataPacket[5] == 1)
        {
            g_pAlgCoreVar->stTakeUp2Write.bEnable = 1;
        }
        else
        {
            g_pAlgCoreVar->stTakeUp2Write.bEnable = 0;
        }

        eRetVal = eCmdAnswerNone;
    }
    
    default:
    {
        eRetVal = eCmdAnswerUnknown;
    }
    break;
    }
    return eRetVal;
}

/*
 * 设置升级标记
 */
int32_t RegCusEventAlgInit(void)
{
    eAppUpgradeFlag_t UpgradeFlag = GetUpgradeFlag();
    //printf("before upgrade flage %02X\r\n", UpgradeFlag);
    // 刚升级完成
    if (UpgradeFlag != eAppRunMode)
    {
        UpgradeFlag = eAppRunMode;
        SetUpgradeFlag(UpgradeFlag);
        Delay_ms(10);
        //printf("finish upgrade. %02X\r\n", UpgradeFlag);
    }

    return 0;
}

eCmdAnswerType_t CmdSendCoordInkEn(stCmdQueueBlock_t* pCmdBlock, stCmdQueueBlock_t* pReCmdBlock)
{
    eCmdAnswerType_t eRetVal = eCmdAnswerOK;

    g_bSendRealUartLength = pCmdBlock->DataPacket[5];
    if(g_bSendRealUartLength > 1)
    {

        g_bSendRealUartLength = 1;
    }

    return eRetVal;
}


/********************************************************************************************************
 * @Function : 获取发送USB数据格式
 * @return   : 发送USB数据格式
********************************************************************************************************/
eTouchFormat_t GetTouchSendFormat(stPoint_t *arrPoint, uint32_t *pTouchCount, eUsbDevice_t eUsbDev)
{
    stUsbCoordCfg_t *pCoordCfg = &g_stUsbCoordCfg[eUsbDev];
    eTouchFormat_t eTouchFormat = pCoordCfg->eFormat;
    stInkDataSend_t *pInkData = &pCoordCfg->stInkDataSend;
    uint32_t nTouchCount = *pTouchCount;

    //SR要求超过最大InkPen点数默认报touch
    //没有枚举完成，不能主动切换报点格式。防止MAC下，没有装UPDD情况下，把Mouse切为MultiTouch后，在MAC体现为无触摸
    if(GetUsbReportStatus(eUsbDev) && nTouchCount > MAX_INK_PENS && eTouchFormat != eFormatCstMultiTouch) 
    {
//        pCoordCfg->eFormat = eFormatMultiTouch;   //注释该行代码: 不修改报点格式，进改变当前帧的报点格式
        return eFormatMultiTouch;
    }
    
    if (pInkData->bInkEnable && eTouchFormat != eFormatMouse && eTouchFormat != eFormatCstMultiTouch && g_bIsWinSys)
    {
        int32_t nPenIdOfs = 0;
        for (int32_t i = 0; i < USER_POINT; i++)
        {
            int32_t nId = arrPoint[i].nId;
            if (arrPoint[i].eStatus == eLeftBtnDown)
            {
                int32_t nMaxWH = FN_MAX(arrPoint[i].nWidth, arrPoint[i].nHeight);
                eTipReportType_t eRetTip = MatchObj_GetReportTipId(arrPoint[i].nId);
                if (eRetTip == eTipSmall || 
                    eRetTip == eTipBig || 
                    (eRetTip == eTipFinger && nMaxWH > INK_ERASER_SIZE))
                {
                    nPenIdOfs = GetAvbPenIdOfs(eUsbDev);
                    if (!(nPenIdOfs < 0))
                    {
                        pInkData->arrInk[nId].nPenIdOffset = nPenIdOfs;
                        pInkData->arrInk[nId].nTipSwitch = (eRetTip == eTipBig || 
                                                           (eRetTip == eTipFinger && nMaxWH > INK_ERASER_SIZE))  ?
                                                            ((0x01 << 1) | (0x01 << 2)) : ((0x01 << 0) | (0x01 << 1));
                        pInkData->arrInk[nId].nDownTipSwitch = (eRetTip == eTipBig || 
                                                           (eRetTip == eTipFinger && nMaxWH > INK_ERASER_SIZE)) ?
                                                            ((0x01 << 1) | (0x01 << 3)) : (0x01 << 1);  //In Range
                        pInkData->arrCurPenIdLst[nPenIdOfs] = nId;
                        pInkData->nCurInkPens++;

                        pCoordCfg->eFormat = eFormatInk;
                        eTouchFormat = pCoordCfg->eFormat;
                        pInkData->bInkPenEnable = 1;
                    }
                }
                else if (!pInkData->bInkPenEnable)
                {
                    pCoordCfg->eFormat = eFormatMultiTouch;
                    eTouchFormat = pCoordCfg->eFormat;
                    pInkData->bMultiTouchEnable = 1;
                }
                else
                {
                    arrPoint[i].eStatus = eBtnNone;
                    nTouchCount--;
                }
            }
            else if (arrPoint[i].eStatus & eBtnBitUp)
            {
                for (int32_t j = 0; j < MAX_INK_PENS; j++)
                {
                    if (nId == pInkData->arrCurPenIdLst[j])
                    {
                        pInkData->arrCurPenIdLst[j] = -1;
                        nPenIdOfs = pInkData->arrInk[nId].nPenIdOffset;
                        pInkData->nCurInkPens = FN_MAX(0, pInkData->nCurInkPens-1);
                    }
                }
            }
        }
        //eFormatMultiTouch切换到Ink, 抬起所有touch数据
        if (pInkData->bMultiTouchEnable && eTouchFormat == eFormatInk)
        {
            stPoint_t arrPointTmp[MAX_POINT];
            int32_t nTouchCountTmp = *pTouchCount;

            MemCpy(arrPointTmp, arrPoint, sizeof(arrPointTmp));
            for (int32_t i = 0; i < USER_POINT; i++)
            {
                if (arrPointTmp[i].eStatus & eBtnBitDown)
                {
                    arrPointTmp[i].eStatus = eBtnNone;
                    nTouchCountTmp--;
                }
                else if (arrPointTmp[i].eStatus & eBtnBitMove)
                {
                    arrPointTmp[i].eStatus &= (~eBtnBitMove);
                    arrPointTmp[i].eStatus |= eBtnBitUp;
                }
            }
            SendUsbTouchCoord(arrPointTmp, nTouchCountTmp, 0, eUsbDev);
            pInkData->bMultiTouchEnable = 0;
        }
        if (pInkData->nCurInkPens == 0 && pInkData->bInkPenEnable)
        {
            pInkData->bMultiTouchEnable = 0;
            pInkData->bInkPenEnable = 0;
            pInkData->bInkEraserEnable = 0;

            SendUsbInkCoord(arrPoint, nTouchCount, eUsbDev);

            int32_t nMoveTouchCnt = 0;
            for (int32_t i = 0; i < USER_POINT; i++)
            {
                if ((arrPoint[i].eStatus & eBtnBitMove) ||
                        (arrPoint[i].eStatus & eBtnBitDown))
                {
                    arrPoint[i].eStatus &= (~eBtnBitMove);
                    arrPoint[i].eStatus |= eBtnBitDown;
                    nMoveTouchCnt++;
                }
                else if (arrPoint[i].eStatus & eBtnBitUp)
                {
                    arrPoint[i].eStatus = eBtnNone;
                    nTouchCount--;
                }
            }
            if (nMoveTouchCnt)
            {
                pCoordCfg->eFormat = eFormatMultiTouch;
                eTouchFormat = pCoordCfg->eFormat;
                pInkData->bMultiTouchEnable = 1;
            }
            else
            {
                eTouchFormat = eFormatNone;
            }
        }
    }
    *pTouchCount = nTouchCount;
    return eTouchFormat;
}


//根据客户规则生成指定的字符串 可重定义
int32_t GetProductString(uint8_t *pStr, eUsbDevice_t eUsbDev)
{
    int32_t nLen = FN_MIN(MAX_STRING_LENGTH, sizeof(USB_PRODUCT_STRING) - 1);
    char pModelStr[] = USB_PRODUCT_STRING;
    char arrDesc[] = "Touch Device";

    if(0 == (strcmp(pModelStr, arrDesc)))
    {
        sprintf((char *)pStr, "%s", pModelStr);
    }
    else
    {
        int32_t nTouchSize = GetDeviceSize(pModelStr, nLen);
        char *pMcuVendor = "ATF403A";
        char *pUsbDevType = eUsbDev == eUsbDev0 ? "FS" : "HS";
        char arrCompileTime[sizeof(USB_DESC5_COMPILE_TIME)] = {0};
    
        memset(arrCompileTime, 0, sizeof(arrCompileTime));
        memcpy(arrCompileTime, USB_DESC5_COMPILE_TIME, 8);
        
#ifdef GD405_ENV
        pMcuVendor = "GD405";
#elif defined GD415_ENV
        pMcuVendor = "GD415";
#elif defined ST750_ENV
        pMcuVendor = "STH750";
#endif
        sprintf((char *)pStr, "%s_%d_0x%X_%s_S%s_%s_U%s", pModelStr,
                                                    g_pConfigData->nUsePoint,
                                                    GetFwCheckSum(), pMcuVendor,
                                                    USB_DESC4_SVN_ID, arrCompileTime, pUsbDevType);
    }

    return nLen;
}

/*
 * 降半屏处理
 */
eCmdAnswerType_t ComDropScreen(stCmdQueueBlock_t* pCmdBlock, stCmdQueueBlock_t* pReCmdBlock)
{
    eUsbDevice_t eUsbDev = GetUsbDev(pCmdBlock->eCmdSource);
    eUsbDevice_t eRUsbDev = eUsbDev == eUsbDev0 ? eUsbDev1 : eUsbDev0;
    if (0 == pCmdBlock->DataPacket[5])
    {
        MemSet(&g_stDrop, 0, sizeof(g_stDrop));
    }
    else
    {
        MemSet(&g_stDrop, 0, sizeof(g_stDrop));
        if (pCmdBlock->DataPacket[5] <= 3)
        {
            g_stDrop.arrOffset[Y] = MAX_LOGICAL_VALUE / (pCmdBlock->DataPacket[5] + 1);
        }
        else
        {
            g_stDrop.arrOffset[Y] = (int32_t)(MAX_LOGICAL_VALUE * (pCmdBlock->DataPacket[5] / 100.0f));
        }
        if(pCmdBlock->eCmdSource != eUsart3Mode)
        {
            g_stDrop.bUsbEnable[eRUsbDev] = 1;
        }
        else
        {
            g_stDrop.bUsbEnable[eUsbDev0] = 1;
        }
    }
    return eCmdAnswerNone;
}


eCmdAnswerType_t CmdFormat(stCmdQueueBlock_t* pCmdBlock, stCmdQueueBlock_t* pReCmdBlock)
{
    eUsbDevice_t eUsbDev = GetUsbDev(pCmdBlock->eCmdSource);
    eCmdAnswerType_t eRetVal = eCmdAnswerNone;

    switch(pCmdBlock->DataPacket[2]) //根据命令散转
    {

    case FMT_SET:
    {
        if(pCmdBlock->DataPacket[5] == eFormatNone)
        {
            if(g_stUsbCoordCfg[eUsbDev].eFormat != eFormatNone)
            {
                g_stUsbCoordCfg[eUsbDev].eFormatRe = g_stUsbCoordCfg[eUsbDev].eFormat;
            }
            //change this to support USB0 send touch when only using USB0
            //do not know why modify g_eUsb0Format here
            eRetVal = eCmdAnswerOK;
        }
        else if(pCmdBlock->DataPacket[5] == FMT_RE)
        {
            g_stUsbCoordCfg[eUsbDev].eFormat = g_stUsbCoordCfg[eUsbDev].eFormatRe;
            eRetVal = eCmdAnswerOK;
        }
        else
        {
            g_stUsbCoordCfg[eUsbDev].eFormat = (eTouchFormat_t)pCmdBlock->DataPacket[5];
            g_stUsbCoordCfg[eUsbDev].eFormatRe = g_stUsbCoordCfg[eUsbDev].eFormat;
            if(g_stUsbCoordCfg[eUsbDev].eFormat == eFormatInk)
            {
                g_bIsWinSys = 1;//如果通信命令主动要INK数据，则跟着命令走
            }
            g_eUsbSendFormat[eUsbDev] = g_stUsbCoordCfg[eUsbDev].eFormat;
            eRetVal = eCmdAnswerData;
        }
        pReCmdBlock->nDataLen = 7;
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = FORMAT;
        pReCmdBlock->DataPacket[2] = FMT_SET;
        pReCmdBlock->DataPacket[3] = 2;
        pReCmdBlock->DataPacket[4] = 0;
        pReCmdBlock->DataPacket[5] = g_stUsbCoordCfg[eUsbDev].eFormat;
        pReCmdBlock->DataPacket[6] = (uint8_t)g_bIsWinSys;
        
    }
    break;

    case FMT_GET:
    {
        pReCmdBlock->nDataLen = 7;
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = FORMAT;
        pReCmdBlock->DataPacket[2] = FMT_GET_RE;
        pReCmdBlock->DataPacket[3] = 2;
        pReCmdBlock->DataPacket[4] = 0;
        pReCmdBlock->DataPacket[5] = g_stUsbCoordCfg[eUsbDev].eFormat;
        pReCmdBlock->DataPacket[6] = (uint8_t)g_bIsWinSys;
        eRetVal = eCmdAnswerData;
    }
    break;

    case FMT_CVT:
    {
        g_stUsbCoordCfg[eUsbDev].bTuioCoordEn = pCmdBlock->DataPacket[5];
        eRetVal = eCmdAnswerOK;
    }
    break;

    case FMT_CAL:
    {
        g_bCalEn = pCmdBlock->DataPacket[5];
        eRetVal = eCmdAnswerOK;
    }
    break;

    case FMT_SMO:
    {
        g_bSmoothEn = pCmdBlock->DataPacket[5];
        eRetVal = eCmdAnswerOK;
    }
    break;

    case FMT_WIDTH_EN:
    {
        if(pCmdBlock->DataPacket[5] == 0)
        {
            g_stUsbCoordCfg[eUsbDev].bTuioCoordWidthEn = 0;
        }
        else
        {
            g_stUsbCoordCfg[eUsbDev].bTuioCoordWidthEn = 1;
        }

        eRetVal = eCmdAnswerOK;
    }
    break;

    case FMT_USB_COORD:
    {
        pReCmdBlock->nDataLen = 6;
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = FORMAT;
        pReCmdBlock->DataPacket[2] = FMT_USB_COORD_RE;
        pReCmdBlock->DataPacket[3] = 1;
        pReCmdBlock->DataPacket[4] = 0;
        pReCmdBlock->DataPacket[5] = g_stUsbCoordCfg[eUsbDev].bCoordEn;
        eRetVal = eCmdAnswerData;
    }
    break;

    case FMT_USB_COORD_EN:
    {
        eRetVal = eCmdAnswerNone;
        switch(pCmdBlock->DataPacket[3])
        {
            case TOUCH_DATA_USB_TRAN:
            {
                if (0 == pCmdBlock->DataPacket[5] || 1 == pCmdBlock->DataPacket[5])
                {
                    g_stUsbCoordCfg[eUsbDev].bCoordEn = pCmdBlock->DataPacket[5];
                    g_UsbReportPtEnable = pCmdBlock->DataPacket[5];
                    pReCmdBlock->DataPacket[2] = 0x81;  //成功
                }
                else
                {
                    pReCmdBlock->DataPacket[2] = 0x82;  //失败
                }
                pReCmdBlock->DataPacket[0] = 0xfc;
                pReCmdBlock->DataPacket[1] = 0xfe;

                pReCmdBlock->DataPacket[3] = 0x02;
                pReCmdBlock->DataPacket[4] = 0x00;
                pReCmdBlock->DataPacket[5] = FORMAT;
                pReCmdBlock->DataPacket[6] = FMT_USB_COORD_EN;
                pReCmdBlock->nDataLen = 7;
                eRetVal = eCmdAnswerData;
            }
            break;
            case TOUCH_DATA_UART_TRAN:
            {
                if (0 == pCmdBlock->DataPacket[5] || 1 == pCmdBlock->DataPacket[5])
                {
                    g_bUartCoordEn = pCmdBlock->DataPacket[5];
                    g_UsbReportPtEnable = pCmdBlock->DataPacket[5];
                }
            }
            break;
            default:
            {
                eRetVal = eCmdAnswerNone;
            }
            break;
        }

    }
    break;

    case FMT_MAC_10_SET:
    {
        g_stUsbCoordCfg[eUsbDev].bUnderMac10 = pCmdBlock->DataPacket[5];
        SaveCoordFormat();
        eRetVal = eCmdAnswerOK;
    }
    break;

    case FMT_MAC_10_GET:
    {
        pReCmdBlock->nDataLen = 6;
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = FORMAT;
        pReCmdBlock->DataPacket[2] = FMT_MAC_10_GET_RE;
        pReCmdBlock->DataPacket[3] = 1;
        pReCmdBlock->DataPacket[4] = 0;
        pReCmdBlock->DataPacket[5] = g_stUsbCoordCfg[eUsbDev].bUnderMac10;
        eRetVal = eCmdAnswerData;
    }
    break;

    case FMT_GET_ALL:
    {
        pReCmdBlock->nDataLen = 12;
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = FORMAT;
        pReCmdBlock->DataPacket[2] = FMT_GET_ALL_RE;
        pReCmdBlock->DataPacket[3] = 7;
        pReCmdBlock->DataPacket[4] = 0;
        pReCmdBlock->DataPacket[5] = g_stUsbCoordCfg[eUsbDev].eFormat;
        pReCmdBlock->DataPacket[6] = g_stUsbCoordCfg[eUsbDev].eFormatRe;
        pReCmdBlock->DataPacket[7] = g_stUsbCoordCfg[eUsbDev].bTuioCoordEn;
        pReCmdBlock->DataPacket[8] = g_bCalEn;
        pReCmdBlock->DataPacket[9] = g_bSmoothEn;
        pReCmdBlock->DataPacket[10] = g_stUsbCoordCfg[eUsbDev].bTuioCoordWidthEn;
        pReCmdBlock->DataPacket[11] = g_stUsbCoordCfg[eUsbDev].bCoordEn;
        eRetVal = eCmdAnswerData;
    }
    break;

    case FMT_UART_COORD:
    {
        pReCmdBlock->nDataLen = 6;
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = FORMAT;
        pReCmdBlock->DataPacket[2] = FMT_UART_COORD_RE;
        pReCmdBlock->DataPacket[3] = 1;
        pReCmdBlock->DataPacket[4] = 0;
        pReCmdBlock->DataPacket[5] = g_bUartCoordEn;
        eRetVal = eCmdAnswerData;
    }
    break;

    case FMT_UART_COORD_EN:
    {
        g_bUartCoordEn = pCmdBlock->DataPacket[5];
        eRetVal = eCmdAnswerOK;
    }
    break;

    case FMT_UART_POINT_NUM_SET:
    {
        if (pCmdBlock->DataPacket[5] > 0 && pCmdBlock->DataPacket[5] <= USER_POINT)
        {
            g_nUartSendCoordNum = pCmdBlock->DataPacket[5];
            eRetVal = eCmdAnswerOK;
        }
        else
        {
            eRetVal = eCmdAnswerFail;
        }
    }
    break;

    case FMT_UART_POINT_NUM_GET:
    {
        pReCmdBlock->nDataLen = 6;
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = FORMAT;
        pReCmdBlock->DataPacket[2] = FMT_UART_POINT_NUM_GET_RE;
        pReCmdBlock->DataPacket[3] = 1;
        pReCmdBlock->DataPacket[4] = 0;
        pReCmdBlock->DataPacket[5] = g_nUartSendCoordNum;
        eRetVal = eCmdAnswerData;
    }
    break;

    case FMT_UART_COORD_WIDTH_SET:
    {
        if (pCmdBlock->DataPacket[5] == 0 || pCmdBlock->DataPacket[5] == 1)
        {
            g_bUartCoordWidthEn = pCmdBlock->DataPacket[5];
            eRetVal = eCmdAnswerOK;
        }
        else
        {
            eRetVal = eCmdAnswerFail;
        }
    }
    break;

    case FMT_UART_COORD_WIDTH_GET:
    {
        pReCmdBlock->nDataLen = 6;
        pReCmdBlock->DataPacket[0] = 0xfc;
        pReCmdBlock->DataPacket[1] = FORMAT;
        pReCmdBlock->DataPacket[2] = FMT_UART_COORD_WIDTH_GET_RE;
        pReCmdBlock->DataPacket[3] = 1;
        pReCmdBlock->DataPacket[4] = 0;
        pReCmdBlock->DataPacket[5] = g_bUartCoordWidthEn;
        eRetVal = eCmdAnswerOK;
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
        if(pCmdBlock->eCmdSource > eUsb1Ep7Mode && pCmdBlock->eCmdSource < eCmdModeCount)
        {
            pReCmdBlock->DataPacket[4] = FN_BYTE(g_pConfigData->nPhysicalX, 0);
            pReCmdBlock->DataPacket[5] = FN_BYTE(g_pConfigData->nPhysicalX, 1);
            pReCmdBlock->DataPacket[6] = FN_BYTE(g_pConfigData->nPhysicalY, 0);
            pReCmdBlock->DataPacket[9] = FN_BYTE(g_pConfigData->nPhysicalY, 1);
        }
        else
        {
            //Modified -> For android
            pReCmdBlock->DataPacket[6] = FN_BYTE(g_pConfigData->nPhysicalX, 0);
            pReCmdBlock->DataPacket[7] = FN_BYTE(g_pConfigData->nPhysicalX, 1);
            pReCmdBlock->DataPacket[8] = FN_BYTE(g_pConfigData->nPhysicalY, 0);
            pReCmdBlock->DataPacket[9] = FN_BYTE(g_pConfigData->nPhysicalY, 1);
        }

        eRetVal = eCmdAnswerData;
    }
    break;
    case FMT_CMD_0B_RES:
    {
        //SR 客户是用于控制串口发送不定长数据开关
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
        eRetVal = eCmdAnswerUnknown;
    }
    break;
    }
    return eRetVal;
}



#endif //QT_ENV

#if (USE_CUSTOMER_CLICK_EVENT)
/*********************** 单击/双击/长按事务 参数 ***********************/
#ifndef FOR_CLICK_MIN_AREA
#define FOR_CLICK_MIN_AREA                 (PEN_8_AREA_POW2)              //针对8mm直径及以上的触摸物做右键
#endif

#ifndef FOR_RIGHT_KEY_MIN_AREA
#define FOR_RIGHT_KEY_MIN_AREA             (GIRLS_FINGERS_AREA_POW2)      //针对手指及以上的触摸物做右键，避免手指触控难出右键的问题
#endif

#ifndef RIGHT_KEY_DISTANCE
#define RIGHT_KEY_DISTANCE                 (7)        //右键距离
#endif
#define RIGHT_KEY_DISTANCE_POW2            (RIGHT_KEY_DISTANCE*RIGHT_KEY_DISTANCE)

#ifndef CLICK_DISTANCE
#define CLICK_DISTANCE                     (6)        //点击允许抖动距离
#endif
#define CLICK_DISTANCE_POW2                (CLICK_DISTANCE*CLICK_DISTANCE)

#ifndef DUAL_CLICK_DISTANCE
#define DUAL_CLICK_DISTANCE                (5)        //双击允许抖动距离
#endif
#define DUAL_CLICK_DISTANCE_POW2           (DUAL_CLICK_DISTANCE*DUAL_CLICK_DISTANCE)

#ifndef EXTEND_CLICK_DISTANCE
#define EXTEND_CLICK_DISTANCE              (3)        //去污外扩点击距离
#endif
#define EXTEND_CLICK_DISTANCE_POW2         (EXTEND_CLICK_DISTANCE * EXTEND_CLICK_DISTANCE)

#define MAX_CLICK_INTERVAL                 (96)     //96ms最长一次点击时间
#define MAX_DOUBLE_CLICK_INTERVAL_BETWEEN  (300)    //300ms双击间隔时间


#if 1
/*
 * 对单击/双击/长按事件进行优化处理, R30S系列提笔即写功能合入此处
 * @para arrPointF: 触摸物数组
 * @para nCount: 点数
 * @return: 剩余点数
 */
int32_t ForClickEvent(stPointF_t *arrPointF, uint32_t nCount)
{
    static stCoordF_t s_stActiveCoordF = {0};
    static uint32_t s_nActiveTick = 0;
    static int32_t  s_nIDLock = -1;
    static int32_t  s_bDualClickLock = 0;
    static int32_t  s_bSingleClickLock = 0;
    static int32_t  s_bRightKeyProcessing = 0;
    static eTipReportType_t eLastRetTip = eTipFinger;
    uint32_t nExtendRange;         //点击外扩范围

    nExtendRange = (g_pAlgCoreVar->nMaskLineCnt > 0) ? EXTEND_CLICK_DISTANCE_POW2 : 0;
    g_pAlgCoreVar->stTakeUp2Write.bPtInsert = 0;

    if (nCount != 1)
    {
        if (nCount > 1 || (GetSystickTime()-s_nActiveTick) > MAX_DOUBLE_CLICK_INTERVAL_BETWEEN)
        {
            s_bSingleClickLock = 0;
            s_bDualClickLock = 0;
            s_nIDLock = -1;
        }
        s_bRightKeyProcessing = 0;
#if 1
        for (int32_t i = 0; i < MAX_POINT_SFX; i++)
        {
            if(arrPointF[i].eStatus != eBtnNone)
            {
                if(g_pAlgCoreVar->stTakeUp2Write.bEnable)
                {
                    eTipReportType_t eRetTip = (arrPointF[i].eStatus != eLeftBtnUp) ? MatchObj_GetReportTipId(arrPointF[i].nId) : eLastRetTip;
                    eLastRetTip = eRetTip;
                    if(eRetTip == eTipSmall)
                    {
                        g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendUartOnly;
                        break;
                    }
                    else
                    {
                        g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendAll;
                    }
                }
                else
                {
                    g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendAll;
                }
            }
        }

#else
        if(0 == g_pAlgCoreVar->stTakeUp2Write.bStatus) //非提笔即写模式
        {
            g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendAll;
        }
        else
        {
            g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendUartOnly;
        }
#endif
        return nCount;
    }
    for (int32_t i = 0, bBreak = 0; !bBreak && i < MAX_POINT_SFX; i++)
    {
        eTipReportType_t eRetTip = (arrPointF[i].eStatus != eLeftBtnUp) ? MatchObj_GetReportTipId(arrPointF[i].nId) : eLastRetTip;
        eLastRetTip = eRetTip;
        switch (arrPointF[i].eStatus)
        {
            case eLeftBtnDown:
            {
                if (s_nActiveTick != 0 &&
                        s_bSingleClickLock &&
                        (GetSystickTime()-s_nActiveTick) < MAX_DOUBLE_CLICK_INTERVAL_BETWEEN &&
                        FN_GET_COORDF_DIST_POW2_SFX(s_stActiveCoordF, arrPointF[i].stCoordF) < (DUAL_CLICK_DISTANCE_POW2 + nExtendRange))
                {
                    arrPointF[i].stCoordF = s_stActiveCoordF;
                    s_bDualClickLock = 1;
                }
                s_nIDLock = arrPointF[i].nId;
                s_stActiveCoordF = arrPointF[i].stCoordF;
                s_nActiveTick = GetSystickTime();
                s_bRightKeyProcessing = 1;
                g_pAlgCoreVar->stTakeUp2Write.bFillDownPt = 1;
            }
            break;

            case eLeftBtnMove:
            {
                if (s_nIDLock != arrPointF[i].nId)
                {
                    break;
                }

                stKFMiscPara_t *pKFMiscPara = GetKFMiscPara(eKFPtTracker, arrPointF[i].nId);
                s_bSingleClickLock = 0;
                int32_t nDistance = FN_GET_COORDF_DIST_POW2_SFX(s_stActiveCoordF, arrPointF[i].stCoordF);
                int32_t nDownDistance = FN_GET_COORDF_DIST_POW2_SFX(pKFMiscPara->stDownCoordF, arrPointF[i].stCoordF);
                uint32_t nTickTime = GetSystickTime()-s_nActiveTick;
                if (nTickTime < MAX_CLICK_INTERVAL &&   //处理单击事务
                        (nDistance < (CLICK_DISTANCE_POW2 + nExtendRange) ||
                        (nDownDistance < (CLICK_DISTANCE_POW2 + nExtendRange) && (nDistance < (15.0f * 15.0f)))) &&
                        arrPointF[i].fAreaSize > FOR_CLICK_MIN_AREA)
                {
                    arrPointF[i].stCoordF = s_stActiveCoordF;
                    s_bSingleClickLock = 1;
                }
                else
                {
                    s_nIDLock = -1;
                }
                if (s_bDualClickLock) //处理双击事务
                {
                    if (nDistance < (DUAL_CLICK_DISTANCE_POW2 + nExtendRange) ||
                       (nDownDistance < (DUAL_CLICK_DISTANCE_POW2 + nExtendRange) && (nDistance < (15.0f * 15.0f))))
                    {
                        arrPointF[i].stCoordF = s_stActiveCoordF;
                    }
                    else
                    {
                        s_bDualClickLock = 0;
                        s_nIDLock = -1;
                    }
                }
                if (!(arrPointF[i].fAreaSize < FOR_RIGHT_KEY_MIN_AREA) &&
                        s_bRightKeyProcessing == 1)   //处理手指等大物体长按事务
                {
                    if (!(nDistance < RIGHT_KEY_DISTANCE_POW2))
                    {
                        s_bRightKeyProcessing = 0;
                    }
                    else
                    {
                        arrPointF[i].stCoordF = s_stActiveCoordF;
                    }
                }
                bBreak = 1;
            }
            break;

            case eLeftBtnUp:
            {
                if ((s_bSingleClickLock || s_bDualClickLock) && s_nIDLock == arrPointF[i].nId)
                {
                    arrPointF[i].stCoordF = s_stActiveCoordF;
                    s_nIDLock = -1;
                }
                bBreak = 1;
                s_bDualClickLock = 0;
            }
            break;
            default:
            break;
        }

        if(arrPointF[i].eStatus != eBtnNone)
        {
            if(g_pAlgCoreVar->stTakeUp2Write.bEnable)
            {
                eTipReportType_t eRetTip = (arrPointF[i].eStatus != eLeftBtnUp) ? MatchObj_GetReportTipId(arrPointF[i].nId) : eLastRetTip;
                eLastRetTip = eRetTip;
                if(eRetTip == eTipSmall)
                {
                    g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendUartOnly;
                    break;
                }
                else
                {
                    g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendAll;
                }
            }
            else
            {
                g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendAll;
            }
        }

        if(arrPointF[i].eStatus == eLeftBtnUp)
        {
            s_nActiveTick = GetSystickTime();
            s_stActiveCoordF = arrPointF[i].stCoordF;
        }
        
    }
//    TRACE_LOG("Frme[%d] Sing:%d  Dual: %d  Right: %d\n",GetCurFrame(), s_bSingleClickLock, s_bDualClickLock, s_bRightKeyProcessing);
//    TRACE_LOG("_____Frme:[%d] PtInsert: %d  FillDown: %d PtSendCtrl: %d  Enable: %d\n",GetCurFrame(),g_pAlgCoreVar->stTakeUp2Write.bPtInsert, g_pAlgCoreVar->stTakeUp2Write.bFillDownPt, g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl,g_pAlgCoreVar->stTakeUp2Write.bStatus);
    return nCount;
}

#else
/*
 * 对单击/双击/长按事件进行优化处理, R30S系列提笔即写功能合入此处
 * @para arrPointF: 触摸物数组
 * @para nCount: 点数
 * @return: 剩余点数
 */
int32_t ForClickEvent(stPointF_t *arrPointF, uint32_t nCount)
{
    static stCoordF_t s_stActiveCoordF = {0};
    static uint32_t s_nActiveTick = 0;
    static int32_t  s_nIDLock = -1;
    static int32_t  s_bDualClickLock = 0;
    static int32_t  s_bSingleClickLock = 0;
    static int32_t  s_bRightKeyProcessing = 0;
    static eTipReportType_t eLastRetTip = eTipFinger;
    uint32_t nExtendRange;         //点击外扩范围

    nExtendRange = (g_pAlgCoreVar->nMaskLineCnt > 0) ? EXTEND_CLICK_DISTANCE_POW2 : 0;
    g_pAlgCoreVar->stTakeUp2Write.bPtInsert = 0;

    if (nCount != 1)
    {
        if (nCount > 1 || (GetSystickTime()-s_nActiveTick) > MAX_DOUBLE_CLICK_INTERVAL_BETWEEN)
        {
            s_bSingleClickLock = 0;
            s_bDualClickLock = 0;
            s_nIDLock = -1;
        }
        s_bRightKeyProcessing = 0;

        if(0 == g_pAlgCoreVar->stTakeUp2Write.bStatus) //非提笔即写模式
        {
            g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendAll;
        }
        else
        {
            g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendUartOnly;
        }
        return nCount;
    }
    for (int32_t i = 0, bBreak = 0; !bBreak && i < MAX_POINT_SFX; i++)
    {
        eTipReportType_t eRetTip = (arrPointF[i].eStatus != eLeftBtnUp) ? MatchObj_GetReportTipId(arrPointF[i].nId) : eLastRetTip;
        eLastRetTip = eRetTip;
        switch (arrPointF[i].eStatus)
        {
            case eLeftBtnDown:
            {
                if (s_nActiveTick != 0 &&
                        s_bSingleClickLock &&
                        (GetSystickTime()-s_nActiveTick) < MAX_DOUBLE_CLICK_INTERVAL_BETWEEN &&
                        FN_GET_COORDF_DIST_POW2_SFX(s_stActiveCoordF, arrPointF[i].stCoordF) < (DUAL_CLICK_DISTANCE_POW2 + nExtendRange))
                {
                    arrPointF[i].stCoordF = s_stActiveCoordF;
                    s_bDualClickLock = 1;
                }
                s_nIDLock = arrPointF[i].nId;
                s_stActiveCoordF = arrPointF[i].stCoordF;
                s_nActiveTick = GetSystickTime();
                s_bRightKeyProcessing = 1;
                g_pAlgCoreVar->stTakeUp2Write.bFillDownPt = 1;
            }
            break;

            case eLeftBtnMove:
            {
                if (s_nIDLock != arrPointF[i].nId)
                {
                    break;
                }

                stKFMiscPara_t *pKFMiscPara = GetKFMiscPara(eKFPtTracker, arrPointF[i].nId);
                s_bSingleClickLock = 0;
                int32_t nDistance = FN_GET_COORDF_DIST_POW2_SFX(s_stActiveCoordF, arrPointF[i].stCoordF);
                int32_t nDownDistance = FN_GET_COORDF_DIST_POW2_SFX(pKFMiscPara->stDownCoordF, arrPointF[i].stCoordF);
                uint32_t nTickTime = GetSystickTime()-s_nActiveTick;
                if (nTickTime < MAX_CLICK_INTERVAL &&   //处理单击事务
                        (nDistance < (CLICK_DISTANCE_POW2 + nExtendRange) ||
                        (nDownDistance < (CLICK_DISTANCE_POW2 + nExtendRange) && (nDistance < (15.0f * 15.0f)))) &&
                        arrPointF[i].fAreaSize > FOR_CLICK_MIN_AREA)
                {
                    arrPointF[i].stCoordF = s_stActiveCoordF;
                    s_bSingleClickLock = 1;
                }
                else
                {
                    s_nIDLock = -1;
                }
                if (s_bDualClickLock) //处理双击事务
                {
                    if (nDistance < (DUAL_CLICK_DISTANCE_POW2 + nExtendRange) ||
                       (nDownDistance < (DUAL_CLICK_DISTANCE_POW2 + nExtendRange) && (nDistance < (15.0f * 15.0f))))
                    {
                        arrPointF[i].stCoordF = s_stActiveCoordF;
                    }
                    else
                    {
                        s_bDualClickLock = 0;
                        s_nIDLock = -1;
                    }
                }
                if (!(arrPointF[i].fAreaSize < FOR_RIGHT_KEY_MIN_AREA) &&
                        s_bRightKeyProcessing == 1)   //处理手指等大物体长按事务
                {
                    if (!(nDistance < RIGHT_KEY_DISTANCE_POW2))
                    {
                        s_bRightKeyProcessing = 0;
                    }
                    else
                    {
                        arrPointF[i].stCoordF = s_stActiveCoordF;
                    }
                }
                bBreak = 1;
            }
            break;

            case eLeftBtnUp:
            {
                if ((s_bSingleClickLock || s_bDualClickLock) && s_nIDLock == arrPointF[i].nId)
                {
                    arrPointF[i].stCoordF = s_stActiveCoordF;
                    s_nIDLock = -1;
                }
                bBreak = 1;
                s_bDualClickLock = 0;
            }
            break;
            default:
            break;
        }


        if(arrPointF[i].eStatus != eBtnNone)
        {
            if(g_pAlgCoreVar->stTakeUp2Write.bEnable)
            {
                if(0 == g_pAlgCoreVar->stTakeUp2Write.bStatus ) //非提笔即写模式
                {
                    if(s_bDualClickLock)        //双击
                    {
                        g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendUsbOnly;
                    }
                    else if(s_bSingleClickLock || s_bRightKeyProcessing)
                    {
                        if(eRetTip == eTipSmall)    //细笔，需要检测是否进入提笔即写模式，并且识别出来之前要禁用所有触摸
                        {
                            uint32_t nTickTime = GetSystickTime()-s_nActiveTick;
                            int32_t nDistance = FN_GET_COORDF_DIST_POW2_SFX(s_stActiveCoordF, arrPointF[i].stCoordF);
                            if(s_bRightKeyProcessing && 
                                (nDistance  > (CLICK_DISTANCE_POW2 + nExtendRange)))    //非单击或右键场景，进入提笔即写模式，仅用串口
                            {
                                g_pAlgCoreVar->stTakeUp2Write.bStatus = 1;
#ifndef QT_ENV
                                //MCU环境下, 需要告知安卓进入提笔即写功能
                                uint8_t arrSend[64];
                                uint8_t nLen = 0;
                                arrSend[0] = 0xFC;
                                arrSend[1] = USERKEYNEW;
                                arrSend[2] = ANDR_DRIVE_FREE_TAKE_UP_TO_WRITE_QUERY_ACK;
                                arrSend[3] = 0x02;
                                arrSend[4] = 0x00;
                                arrSend[5] = g_pAlgCoreVar->stTakeUp2Write.bEnable;
                                arrSend[6] = g_pAlgCoreVar->stTakeUp2Write.bStatus;
                                nLen = 7;
                                AnswerCommand(arrSend, nLen, eUsart3Mode);
#endif
                                //Down点补报
                                if(1 == g_pAlgCoreVar->stTakeUp2Write.bFillDownPt)
                                {
                                    g_pAlgCoreVar->stTakeUp2Write.bFillDownPt = 0;
                                    arrPointF[i].eStatus = eLeftBtnDown;
                                }
                                g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendUartOnly;
                                
                            }
                            else if(s_bRightKeyProcessing &&
                                    !(nTickTime < MAX_CLICK_INTERVAL))  //长按时间太久，继续USB报点
                            {
                                //Down点补报
                                if(1 == g_pAlgCoreVar->stTakeUp2Write.bFillDownPt)
                                {
                                    g_pAlgCoreVar->stTakeUp2Write.bFillDownPt = 0;
                                    arrPointF[i].eStatus = eLeftBtnDown;
                                }
                                g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendUsbOnly;
                            }
                            else
                            {
                                //压住点不报
                                if(s_bSingleClickLock && arrPointF[i].eStatus == eLeftBtnUp)
                                {
                                    //单击, 需要补回单击效果
                                    g_pAlgCoreVar->stTakeUp2Write.bPtInsert = 1;
                                    g_pAlgCoreVar->stTakeUp2Write.stInsertPt = arrPointF[i];
                                    g_pAlgCoreVar->stTakeUp2Write.stInsertPt.stCoordF = s_stActiveCoordF;
                                }
                                arrPointF[i].eStatus = eBtnNone;
                                nCount = 0;
                                g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendUsbOnly;
                            }
                        }
                        else
                        {
                            g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendAll;
                        }
                    }
                }
                else
                {
                    //提笔即写模式，细笔串口报点, 粗笔USB报点
                    if(eRetTip == eTipSmall)
                    {
                        g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendUartOnly;
                    }
                    else
                    {
                        g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl = eSendAll;
                    }
                }
            }
            
        }
        
        if(arrPointF[i].eStatus == eLeftBtnUp)
        {
            s_nActiveTick = GetSystickTime();
            s_stActiveCoordF = arrPointF[i].stCoordF;
        }
        
    }
//    TRACE_LOG("Frme[%d] Sing:%d  Dual: %d  Right: %d\n",GetCurFrame(), s_bSingleClickLock, s_bDualClickLock, s_bRightKeyProcessing);
//    TRACE_LOG("_____Frme:[%d] PtInsert: %d  FillDown: %d PtSendCtrl: %d  Enable: %d\n",GetCurFrame(),g_pAlgCoreVar->stTakeUp2Write.bPtInsert, g_pAlgCoreVar->stTakeUp2Write.bFillDownPt, g_pAlgCoreVar->stTakeUp2Write.ePtSendCtrl,g_pAlgCoreVar->stTakeUp2Write.bStatus);
    return nCount;
}
#endif
#endif

#endif //CUSTOMER_SEEWO


