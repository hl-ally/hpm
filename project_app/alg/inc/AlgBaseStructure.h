#ifndef _ALG_BASE_STRUCTURE_
#define _ALG_BASE_STRUCTURE_

#include "AlgCustomer.h"
#include "AlgApiStdint.h"

/*
 * 产品类型
 */
typedef enum
{
    eFourSides,
    eTwoSides,
    eProductTypeCount
} eProductType_t;

/*
 * 组包时的包类型
 */
typedef enum
{
    eInk,
    eMouse,
    eTouch,
    eTuio,
    eUartTouch,
    eFigTouch,          // Multi-touch support angle and press 
    ePacketTypeCount
} eTouchPacketType_t;

/*
 * 方向轴定义
 */
typedef enum
{
    X,
    Y,
    eAxisCount,
    eErrorDir     //错误方向
} eAxis_t;

/*
 * 以主板所在位置定义上下左右，主板所在边为下边，邻边的左边，对边分别为上边和右边
 */
typedef enum
{
    eBottom,
    eTop,
    eLeft,
    eRight,
    eEdgeCount
} eEdge_t;

/*
 * Led灯的类型定义
 */
typedef enum
{
    eRLedType = 0,
    eTLedType,
    eLedTypeTotal
} eLedType_t;

/*
 * 段范围定义
 */
typedef enum
{
    eSgmStart = 0,
    eSgmEnd,
    eSgmCount
} eSgms_t;

typedef enum
{
    eBtnBitNone = 0x00,
    eBtnBitDown = 0x01,
    eBtnBitMove = 0x02,
    eBtnBitUp = 0x04
} eBtnBitStatus_t;

//点状态的枚举
typedef enum
{
    eBtnNone = 0x00,
    eBtnTouch = 0xff,
    
    eLeftBtnDown = 0x11,
    eLeftBtnMove = 0x12,
    eLeftBtnUp = 0x14,

    eRightBtnDown = 0x21,
    eRightBtnMove = 0x22,
    eRightBtnUp = 0x24,

    eMidBtnDown = 0x41,
    eMidBtnMove = 0x42,
    eMidBtnUp = 0x44,

    eWheelDown = 0x81,
    eWheelMove = 0x82,
    eWheelUp = 0x84
} eBtnStatus_t;

typedef enum
{
    eActivedNone = 0,  //不处于活动区域
    eActivedL0,        //活动级别0, 处于未曾到有抬点记录(但有Down点)的活动区域
    eActivedL1,        //活动级别1, 处于记录到曾有抬点过1次的活动区域
    eActivedL2         //活动级别2, 处于记录到曾有抬点超过1次的活动区域
} eActivedLevel_t;

// 光线状态
typedef enum
{
    eLineUnCover = 0x00,  //未遮挡
    eLineCover   = 0x01,  //遮挡的
    eLineRemoved = 0x02,  //标记移除使用
    eLineFrozen  = 0x04,  //标记光线暂时冻结
    eLineSpot    = 0x08,  //标记光线已被去污
    eLineFilter  = 0x10,  //标记光线已被动态过滤
} eLineStatus_t;

//可疑区域状态
typedef enum
{
    eCertainFake  = -2,
    eProbablyFake = -1,
    eNewAddIn     = 0,
    eNewAddInFrozen,
    eProbablyReal,
    eCertainReal,
} eAreaStatus_t;

//预测点使用状态
typedef enum
{
    ePreUnuse  = -1,  //未使用预测ID
    ePreUsable = 0,   //预测ID可使用
    ePreInUse,        //预测ID正在使用
    ePreSyncWriting,  //预测ID进入写字同步处理
    ePreSyncDrawing,  //预测ID进入画线同步处理
    ePreAbandon,      //预测ID已使用过，处于抛弃状态，不可用
} ePreStatus_t;

//计算结果类型
typedef enum
{
    TYPE1 = 1,              //斜扫四边形与待扫多边形不相交，但在其左方或下方
    TYPE2,                  //斜扫四边形的左边或下边与待切多边形不相交，但右边或上边与之相交
    TYPE3,                  //斜扫四边形的两条边都与多边形形相交
    TYPE4,                  //斜扫四多形的右边或上边与待切多边形不相交，但左边或下边相交
    TYPE5,                  //待切多边形与斜扫四边形的两条边都不相交，但在其中间
    TYPE6,                  //斜扫四边形与待扫多边形不相交，但在其右方或上方
    TYPE_ERR = 0xFF
} ePCutRes_t;

//平滑的级别选择
typedef enum
{
    eSmoothNone = 0,            //关闭平滑  无延迟
    eSmoothLevel1,              //平滑等级1 延迟大
    eSmoothLevel2,              //平滑等级2 延迟中
    eSmoothLevel3,              //平滑等级3 延迟小
    eSmoothDefaultLevel,        //默认的平滑等级
    eSmoothLevelCount
} eSmoothLevel_t;

//段数据结构
typedef struct
{
    int32_t nStart;
    int32_t nEnd;
} stSgm_t;

#pragma pack(1)  //强制单字节对齐 节省空间
//段数据结构 - Byte
typedef struct
{
    uint8_t nStart;
    uint8_t nEnd;
} stSgmB_t;
#pragma pack()

//段数据结构 - HalfWord
typedef struct
{
    int16_t nStart;
    int16_t nEnd;
} stSgmH_t;

//段数据结构 - float
typedef struct
{
    float fStart;
    float fEnd;
} stSgmF_t;

/*
 * 二维坐标的结构体 - 整型
 */
typedef struct
{
    int32_t nX;
    int32_t nY;
} stCoord_t;

/*
 * 二维坐标的结构体 - 浮点型
 */
typedef struct
{
    float fX;
    float fY;
} stCoordF_t;

typedef struct
{
    fix13_t theta;          // theta
    fix13_t rho;            // rho
}stPosPolar_t;

/*
 * 二维坐标的结构体 - 快速强制转换访问
 */
typedef struct
{
    float arrCoord[eAxisCount];
} stAxCoordF_t;

/*
 * 触摸物的宽高信息
 */
typedef struct
{
    float fWidth;   //触摸宽度
    float fHeight;  //触摸高度
} stAreaSizeF_t;

#if !USE_CUSTOMER_RECOGNISE
/*
 * 触摸物预设类型种类
 */
typedef enum
{
    ePenTypeSeewo3mm = 0, //seewo的3mm笔径
    ePenTypeSeewo5mm,
    ePenTypeSeewo7mm,
    ePenTypeSeewo8mm,
    ePenTypeSeewo11mm,
    ePenTypeSeewo5mmV2,
    ePenTypeSeewo8mmV2,
    ePenTypeSeewo3mmV3, //seewo的3mm笔径
    ePenTypeSeewo5mmV3,
    ePenTypeSeewo7mmV3,
    ePenTypeSeewo8mmV3,
    ePenTypeSeewo11mmV3,
    ePenTypeSeewo3mm_30,
    ePenTypeSeewo10mm_30,
    eObjTypeTotal,
    eObjTypeNone,       //默认空类型
    eObjTypeUndefined,  //未定义的类型，即报实际值
    eObjTypeCntTotal
} eObjRecType_t;

/*
 * 触摸物预设类型组合数量
 */
typedef enum
{
    eObjGpSeewo3_7_11 = 0,   //SEEWO的3/7/11mm笔径组合
    eObjGpSeewo5_8,          //SEEWO的5/8mm笔径组合
    eObjGpSeewo5_8_V2,       //SEEWO的5/8mm笔径组合    报告0.1mm单位的物理宽高值
    eObjGpSeewo3_7_11_30,    //SEEWO的3/7/11mm笔径组合, 七代机需要下发选择笔的类型, 故七代机都要3种连续的笔
    eObjGpSeewo5_8_30,       //SEEWO的5/8mm笔径组合
    eObjGpSeewo3_7_11_V3,    //SEEWO的3/7/11mm笔径组合, 七代机需要下发选择笔的类型, 故七代机都要3种连续的笔
    eObjGpSeewo5_8_V3,       //SEEWO的5/8mm笔径组合
    eObjGpSeewo3_10_30,      //SEEWO的3/8mm笔径组合    报告0.1mm单位的物理宽高值
    eObjGroupTotal,
    eObjGroupNone
} eObjRecTypeGp_t;
#endif
/*
 * 不同笔尖上报Id
 */
typedef enum
{
    eTipFinger = 0,     //默认是手指
    eTipSmall = 1,      //细笔尖
    eTipBig = 2,        //粗笔尾
    eTipAllCnt
}eTipReportType_t;

typedef struct
{
    float fWidthfactor;
    float fHeightfactor;
    float fAreafactor;      // make sure fZ > 0, if not reverse all sign digital
    float fConst;           // when fZ > 0, fConst always < 0
    float fReviseVal;     //预测正确修正值
} stSVMPara_t;

/*
 * 预设物体的属性
 */
typedef struct
{
    eObjRecType_t eObjType;             //触摸物预设类型种类
    float         fReportWidth;         //上报触摸宽度
    float         fReportHeight;        //上报触摸高度
    float         fReportWidthTypeV2;   //上报触摸宽度V2
    float         fReportHeightTypeV2;  //上报触摸高度V2
    stSgmF_t      stRecoArea;           //识别的面积范围
    stSgmF_t      stRecoWH;             //识别的宽高范围
    stSVMPara_t   stSVMPara;            // SVM 参数
} stObjProperty_t;

/*
 * 预设物体的组合
 */
typedef struct
{
    int32_t                bRecoObjEnable;    //是否开启物体识别
    stObjProperty_t const *pObjProperty;      //触摸物的属性
    eObjRecTypeGp_t        eObjTypeGroup;     //使用的触摸组合序号
    float                  fMaxAreaLimit;     //最大识别的面积限制
    float                  fMaxWHLimit;       //最大识别的宽高限制
    eObjRecType_t         *arrObjGroup;       //当前正使用的组合类型
    int32_t                nObjPropertyCount; //触摸物组合的数量
    eObjRecType_t         *arrObjTypeIdLock;  //已锁定的ID类型
    uint8_t                bDefaultWHType;    //默认使用的宽高类型(逻辑/物理)
} stObjCtrlCombin_t;

/*
 * 点的区域所属标签
 */
typedef enum
{
    eNoneType    = 0x00,
    eBottomType  = 0x01,
    eTopType     = 0x02,
    eLeftType    = 0x04,
    eRightType   = 0x08,
    eGeneralType = 0x10,  //通用区域的点标签
    eSyncDrawingType,     //画线处理扫描同步手动合成的点
    eSyncWritingType,     //写字处理扫描同步手动合成的点
    eFillLastType,        //通过上一帧的点数据补回来的点
    eFillPreType,         //通过预测点数据补回来的点
    eFillMaskingType,     //去污情况下，通过上一帧的点数据补回来的点
    eBrushType,           //排笔特殊处理的点
    eAdjustedType,        //微调后的点
    ePullOverType,        //单点快速向边沿画线的补点行为
    eFillHoughType,       //通过Hough点数据补回来的点
    eBBlackAdjType,       //通过黑边内微调反射回来的点
    eInclineType,         //倾斜慢速画线的点
    ePalmPygType,         //手笔分离找到的点
    ePalmRjtFillType,     //手笔分离补点找到的点
    eMultiPalmPenType,    //多人擦写找到的点
    eMultiPalmFillType,   //多人擦写补点找到的点
    eBBoardRefFillType,   //黑板反射KB坐标补回来的点
    eAreaTypeTotal        //点类型的总数
} eAreaType_t;

typedef enum
{
	eNoneAllType         = 0x00,
    eBottomNearType      = 0x01,
	eBottomMiddleType,
	eBottomFarType,    
	eTopNearType,      
	eTopMiddleType,    
	eTopFarType,       
	eLeftNearType,     
	eLeftMiddleType,   
	eLeftFarType,      
	eRightNearType,    
	eRightMiddleType,  
	eRightFarType,  
	eGeneralAllType,
	eAreaALLTypeTotal
} eAreaType_Edge_t;

/*
 * 特殊区域制定
 */
typedef enum
{
    eSideTypeH = 0,  //一般用于触摸高度优化
    eSideTypeF,      //一般用于手指多点在边沿画线
    eSideTypeB,      //盲区和AA区的中界线，一般用于小物体在盲区的优化
    eSideTypeBMax,   //最大盲区，一般用于细笔径同步画线处理优化
    eSideTypeP,      //一般用于快速画线拉边优化
    eSideTypePressure, //用于压感检测优化
    eSideTypeSrchIRRay, //用于系数矢量区域
    eSideTypeTotal
} eSideType_t;

/*
 * 预留光线的类型种类
 */
typedef enum
{
    eRectMaxStartSide        = 0,   //最大可视区域（包含不可检测区域）
    eRectMaxEndSide          = 2,
    eRectDetectableStartSide = 4,   //最大可检测区域
    eRectDetectableEndSide   = 6,
    eReserveLineTotal        = 8
} eReserveLine_t;

/*
 * 矩形数据结构
 */
typedef struct
{
    int32_t nStartX;
    int32_t nStartY;
    int32_t nEndX;
    int32_t nEndY;
} stRect_t;

/*
 * 矩形数据结构
 */
typedef struct
{
    float fStartX;
    float fStartY;
    float fEndX;
    float fEndY;
} stRectF_t;

/*
 * 2D平面位置的数据结构
 */
typedef struct
{
    int32_t fX;
    int32_t fY;
} stPos_t;

/*
 * 2D平面位置的数据结构
 */
typedef struct
{
    float fX;
    float fY;
} stPosF_t;

/*
 * 一条abc光线的信息
 */
typedef struct {
    float a;
    float b;
    float c;
} stLiAbcF_t;

/*
 * 一条斜截式光线的信息
 */
typedef struct {
    float s;  //斜率
    float i;  //截距
} stLiSloIcpF_t;

/*
 * 多条光线的信息集合
 */
typedef struct {
    uint16_t  *arrIdx;
    uint8_t    nTotal;        //光线数量
    float      fPos;          //X/Y的偏移位置
} stLisF_t;

/*
 * 每一个矢量射线的信息
 * 斜率、截距、矢量射线深度
 * 在找矢量射线模块中和多点算法的模块中使用
 */
typedef struct
{
    int16_t   nStartIdx;      //矢量射线开始的光线序号
    int16_t   nEndIdx;        //矢量射线结束的光线序号
    int8_t    nDSIdx;         //矢量射线开始的光线序号
    int8_t    nDEIdx;         //矢量射线结束的光线序号
    uint8_t   nDepth;         //当前平面中第i个矢量射线的深度 用百分比表示
    uint8_t   eState;         //矢量射线状态
} stIRRayInf_t;

/*
 * 每一个方向的矢量射线及矢量射线的数量 -- 指针形式
 */
typedef struct
{
    uint8_t        nRayCount;      //当前平面的矢量射线数据总数量
    uint8_t        nValidRayCount; //当前平面的矢量射线数据有效数量(非removed状态) 置为removed后，此数值对应减一
    uint8_t        bBaseRaySet;    //是否已作过基础矢量使用
    stIRRayInf_t  *arrIRRay;       //矢量射线数据
} stIRRaysInf_t;

/*
 * 数据管道类型定义
 */
typedef enum
{
    eUartComMode,
    eUsbEP1Mode,
    eUsbEP2Mode,
    eUsbEP3Mode,
    eUsbHostMode,
    eModeCount
} eTransferMode_t;

/*
 * 各模块处理通讯接口的返回值
 */
typedef enum
{
    eReplyNone,
    eReplyUnknown,
    eReplyFailed,
    eReplyOK,
    eReplyData
} eCmdReplyType_t;

/*
 * 发点格式
 */
typedef enum
{
    eFormatMouse = 0x01,
    eFormatTouch = 0x02,
    eFormatMultiTouch = 0x03,
    eFormatMac = 0x04,
    eFormatInk = 0x05,
    eFormatNone = 0xcc
} eTouchFormat_t;

/*
 * 触摸点的结构体
 * 找到的点必须存在此数据结构的数组中，
 * 进行后续的手势、平滑、校准等操作
 */
typedef struct
{
    uint8_t   nId;             //点ID
    uint8_t   nAId;            //点在算法内存中的索引序号
    uint8_t   bDegReport;      //上报坐标点
    uint8_t   nGeoFigureId;    //几何图形Id
    stCoord_t stCoord;         //触摸位置
    int32_t   nWidth;          //触摸宽度
    int32_t   nHeight;         //触摸高度
    int32_t   nAreaSize;       //面积
    int16_t   nAzimuth;        //角度: 单位为0.1Deg
    uint16_t  nPressure;       //压感值
    eBtnStatus_t eStatus;      //按键状态
} stPoint_t;

/*
 * 触摸点的结构体
 * 找到的点必须存在此数据结构的数组中，
 * 进行后续的手势、平滑、校准等操作
 */
typedef struct
{
    uint8_t         nId;          //点ID
    uint8_t         nAId;         //点在算法内存中的索引序号
    eBtnStatus_t    eStatus;      //按键状态
    eActivedLevel_t eActived;     //点的活动状态
    int8_t          nActivedID;   //点的活动状态
    stCoordF_t      stCoordF;     //触摸位置
    float           fWidth;       //触摸宽度
    float           fHeight;      //触摸高度
    float           fAreaSize;    //面积
    float           fAngle;       //degree
    uint16_t        nPressure;    //压感值
    
    int32_t         nLineCnt[eAxisCount];  //穿过的X/Y遮挡光线数量
    int32_t         nDepLineCnts; //深遮挡光线比例
    uint8_t         nMaxDepth[eAxisCount]; //触摸物的最大深度
    uint8_t         nAvgDepth[eAxisCount]; //触摸框的平均深度
} stPointF_t;

/*
 * 触摸点的结构体
 * 找到的点必须存在此数据结构的数组中，
 * 进行后续的手势、平滑、校准等操作
 */
typedef struct
{
    uint8_t    nId;             //点ID
    stCoordF_t stCoordF;        //触摸位置
} stPointLiteF_t;

//下位机需要同步上传到上位机的特殊位数据
typedef struct
{
    uint32_t nTimestamp:20;          //时间戳
    uint32_t nReserve:5;             //保留位
    uint32_t bInitMask:1;            //是否更新背景值
    uint32_t bReflectAction:1;       //反射信号上升动作记录
    uint32_t bNmOrgUpdateAction:1;   //正常Org更新动作
    uint32_t bBBOrgUpdateAction:1;   //黑板Org更新动作
    uint32_t bSlOrgUpdateAction:1;   //慢速轻微落下Org更新动作
    uint32_t bRlAgcInitAction:1;     //实时Agc更新动作
    uint32_t bFlAgcInitAction:1;     //全部Agc更新动作
} stSlvSpecBits_t;


typedef enum
{
    eFifo_Success,
    eFifo_Null,
    eFifo_Full,
    eFifo_Empty
}eFifoStatus;
    
typedef struct
{
    uint8_t*        pBuff;  //fifo according to trace turns invalid
    uint32_t        nHead;
    uint32_t        nRear;
    uint32_t        nLen;
    uint32_t        nMaxLen;
}stFifo_t;


/*************************************************************************************
 * @Function : Fifo_Init
 * @Desc     :
 * @Parameter:
 * @return   :
 *************************************************************************************/
static inline eFifoStatus Fifo_Init(stFifo_t* pFifo, uint8_t* pBuff, uint32_t QueueMaxLen)
{
    pFifo->nMaxLen      = QueueMaxLen;
    pFifo->nLen         = 0;
    pFifo->nHead        = 0;
    pFifo->nRear        = QueueMaxLen-1;
    pFifo->pBuff        = pBuff;
    return eFifo_Success;
}

/*************************************************************************************
 * @Function : Fifo_In
 * @Desc     : recycle one trace's memory
 * @Parameter:
 * @return   :
 *************************************************************************************/
static inline eFifoStatus Fifo_In(stFifo_t* pFifo, uint8_t* pData)
{
    if(pFifo->nLen >= pFifo->nMaxLen)
    {
        return eFifo_Full;
    }
    pFifo->nLen++;
    pFifo->nRear = (pFifo->nRear+1) % pFifo->nMaxLen;
    pFifo->pBuff[pFifo->nRear] = *pData;
    return eFifo_Success;
}

/*************************************************************************************
 * @Function :  Fifo_Out
 * @Desc     :  provide free memory for one trace
 * @Parameter:
 * @return   :
 *************************************************************************************/
static inline eFifoStatus Fifo_Out(stFifo_t* pFifo, uint8_t* pData)
{
    if(0 == pFifo->nLen)
    {
        return eFifo_Empty;
    }
    pFifo->nLen--;
    *pData = pFifo->pBuff[pFifo->nHead];
    pFifo->nHead = (pFifo->nHead+1)%pFifo->nMaxLen;
    return eFifo_Success;
}

#define Fifo_GetRear(pFifo)                 ((pFifo)->nRear)
#define Fifo_GetLen(pFifo)                  ((pFifo)->nLen)
#define Fifo_IsFull(pFifo)                  ((pFifo)->nLen >= (pFifo)->nMaxLen)
#define Fifo_IsEmpty(pFifo)                 ((pFifo)->nLen == 0)
#define Fifo_GetRearReverseId(pFifo,nId)    (((pFifo)->nRear + (pFifo)->nMaxLen - nId)%(pFifo)->nMaxLen)
#define Fifo_GetItemByRearId(pFifo,nId)     ((pFifo)->pBuff[Fifo_GetRearReverseId((pFifo), nId)])
#define Fifo_GetHeadReverseId(pFifo,nId)    (((pFifo)->nHead + (pFifo)->nMaxLen - nId)%(pFifo)->nMaxLen)
#define Fifo_GetItemByHeadId(pFifo,nId)     ((pFifo)->pBuff[Fifo_GetHeadReverseId((pFifo), nId)])

typedef int32_t (*fTransferCallback_t)(uint8_t* pBuf, int16_t nLen, eTransferMode_t eMode);

#endif//_ALG_BASE_STRUCTURE_

