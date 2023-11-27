#ifndef _ALG_API_MATH_
#define _ALG_API_MATH_

#include <math.h>
#include <limits.h>
#include "float.h"
#include "AlgDefaultDefined.h"
#include "AlgBaseStructure.h"

//极限定义
#define INT32_MAX_SFX                           (0x7FFFFFFF)
#define MAX_FLT_VALUE_SFX                       (FLT_MAX)
#define MIN_FLT_VALUE_SFX                       (-(FLT_MAX))
#define MAX_INT_VALUE_SFX                       ((int32_t)INT_MAX)
#define MIN_INT_VALUE_SFX                       ((int32_t)INT_MIN)
#define MAX_UINT_VALUE_SFX                      ((uint32_t)UINT_MAX)
#define MIN_UINT_VALUE_SFX                      (0)
#define MAX_INT16_VALUE_SFX                     ((int16_t)32767)
#define MIN_INT16_VALUE_SFX                     ((int16_t)-32767)
#define KBSPC_MAX_UNT_RATE                      (0.00009f)
#define PI_FIX13                                (3.14159f)
#define INFINITE_MIN_SFX                        (0.000001f)
#define RAD2DEG(x)                              (((x)*180.0f)/PI_FIX13)
#define DEG2RAD(x)                              (((x)*PI_FIX13)/180.0f)

#define MAX_FIT_ORDER                       (5)
#define LAG_LR_EXT_MUL                      (1)

//另一种访问相同数据结构方式
#ifndef ACC_COORDF
#define ACC_COORDF(a, xy)                   (((stAxCoordF_t *)(&(a)))->arrCoord[(xy)])       //浮点转定点后使用
#endif
#ifndef EG2AX
#define EG2AX(edge)                         ((eAxis_t)((edge)/2))
#endif
#ifndef REVER_AX
#define REVER_AX(eAxis)                     ((eAxis_t)(1-(eAxis)))
#endif

//高度转化使用
#define FN_SWAP_INT32(a,b)                  {int32_t c = (a); (a) = (b); (b) = (c);}
#define div_s64(a,b)                        ((a)/(b))
#define DIV_S64(a,b)                        ((b==0)?(div_s64((a), (1))):(div_s64((a), (b))))

#define INT_TO_FIX13(x)                     ((float)(x))
#define INT_TO_FIX5(x)                      ((float)(x))
#define FIX13_TO_INT(x)                     ((int32_t)(x))
#define FIX5_TO_INT(x)                      ((int32_t)(x))
#define FIX13_TO_FIX5(x)                    ((float)(x))
#define FIX5_TO_FIX13(x)                    ((float)(x))
#define FLO_TO_FIX13(x)                     ((float)(x))
#define FLO_TO_FIX5(x)                      ((float)(x))
#define FIX5_PARA(a,b)                      (((float)(a))/((float)(b)))                         //将参数转换为定位格式
#define FIX13_PARA(a,b)                     (((float)(a))/((float)(b)))                        //将参数转换为定位格式
#define FIX5_PARA_POW2(a,b)                 (FIX_5MUL5_TO5(FIX5_PARA((a),(b)),FIX5_PARA((a),(b))))  //将参数转换为定位格式
#define FIX13_PARA_POW2(a,b)                (FIX_13MUL13_TO13_64(FIX13_PARA((a),(b)),FIX13_PARA((a),(b))))  //将参数转换为定位格式

#define FIX13_TO_FLO(x)          ((float)(x))
#define FIX5_TO_FLO(x)           ((float)(x))

//定点乘法集合
#define FIX_5MUL5_TO5(x, y)      (((x)*(y)))
#define FIX_5MUL5_TO5_64(x, y)   (((x)*(y)))
#define FIX_5MUL5_TO13(x,y)      (((x)*(y)))
#define FIX_5MUL5_TO13_64(x,y)   (((x)*(y)))
#define FIX_5MUL5_TOINT(x,y)     ((int32_t)((x)*(y)))
#define FIX_5MUL5_TOINT_64(x,y)  ((int64_t)((x)*(y)))
#define FIX_5MUL100_INT(x)       (FIX_5MUL5_TOINT(x, FIX5_PARA(100, 1)))
//-----------------------
#define FIX_5MUL13_TO5(x,y)      (((x)*(y)))
#define FIX_5MUL13_TO5_64(x,y)   (((x)*(y)))
#define FIX_5MUL13_TO13(x,y)     (((x)*(y)))
#define FIX_5MUL13_TO13_64(x,y)  (((x)*(y)))
#define FIX_13MUL5_TO5(x,y)      (((x)*(y)))
#define FIX_13MUL5_TO5_64(x,y)   (((x)*(y)))
#define FIX_13MUL5_TO13(x,y)     (((x)*(y)))
#define FIX_13MUL5_TO13_64(x,y)  (((x)*(y)))
#define FIX_13MULINT_TOINT(x,y)  ((int32_t)((x)*(y)))
#define FIX_13MUL1000_TOINT(x)   ((int32_t)FIX_13MULINT_TOINT(x,1000))
#define FIX_5MULINT_TOINT(x,y)   ((int32_t)((x)*(y)))
#define FIX_5MUL1000_TOINT(x)    ((int32_t)FIX_5MULINT_TOINT(x,1000))

#define FIX_13MUL13_TO5(x,y)     (((x)*(y)))
#define FIX_13MUL13_TO5_64(x,y)  (((x)*(y)))
#define FIX_13MUL13_TO13(x,y)    (((x)*(y)))
#define FIX_13MUL13_TO13_64(x,y) (((x)*(y)))
#define FIX_13MUL13_TOINT(x,y)   ((int32_t)((x)*(y)))
#define FIX_13MUL13_TOINT_64(x,y) ((int32_t)((x)*(y)))
#define FIX_13MUL100_INT(x)       ((int32_t)FIX_13MULINT_TOINT(x,100))

#define FIX_5DIV5_TO5(x,y)       (DIV_S64((x),(y)))
#define FIX_5DIV5_TO5_64(x,y)    (DIV_S64((x),(y)))
#define FIX_5DIV5_TOINT(x,y)     (FIX5_TO_INT(FIX_5DIV5_TO5((x),(y))))
#define FIX_5DIV5_TO13(x,y)      (DIV_S64((x),(y)))
#define FIX_5DIV5_TO13_64(x,y)   (DIV_S64((x),(y)))
#define FIX_5DIV5_TOFLO(x,y)     (DIV_S64((x),(y)))
#define FIX_5DIV13_TO5_64(x,y)   (DIV_S64((x),(y)))
#define FIX_5DIV13_TO13_64(x,y)  (DIV_S64((x),(y)))
//-----------------------
#define FIX_13DIV13_TO5(x,y)     (DIV_S64((x),(y)))
#define FIX_13DIV13_TO5_64(x,y)  (DIV_S64((x),(y)))
#define FIX_13DIV13_TO13(x,y)    (DIV_S64((x),(y)))
#define FIX_13DIV13_TOINT(x,y)   ((int32_t)DIV_S64((x),(y)))
#define FIX_13DIV13_TO13_64(x,y) (DIV_S64((x),(y)))
#define FIX_13DIV5_TO5(x,y)      (DIV_S64((x),(y)))
#define FIX_13DIV5_TOINT(x,y)    ((int32_t)DIV_S64((x),(y)))
#define FIX_13DIV5_TOINT_64(x,y) ((int32_t)DIV_S64((x),(y)))

//宏功能函数
#define FN_GET_DIST_POW2_SFX(X1, Y1, X2, Y2)    ((int32_t)(((X1)-(X2))*((X1)-(X2)))+(int32_t)(((Y1)-(Y2))*((Y1)-(Y2))))
#define FN_GET_DISTF_POW2_SFX(X1, Y1, X2, Y2)   ((((X1)-(X2))*((X1)-(X2)))+(((Y1)-(Y2))*((Y1)-(Y2))))
#define FN_GET_DIST_SFX(X1, Y1, X2, Y2)         (FnSqrtF((((X1)-(X2)))*((X1)-(X2))+((int32_t)((Y1)-(Y2)))*((Y1)-(Y2))))
#define FN_GET_COORD_DIST_POW2_SFX(p1,p2)       (FN_GET_DIST_POW2_SFX((p1).nX, (p1).nY, (p2).nX, (p2).nY))
#define FN_GET_COORDF_DIST_POW2_SFX(p1,p2)      (FN_GET_DISTF_POW2_SFX((p1).fX, (p1).fY, (p2).fX, (p2).fY))
#define FN_GET_COORD_DIST_SFX(p1,p2)            (FnSqrtF(FN_GET_DIST_POW2_SFX((p1).nX, (p1).nY, (p2).nX, (p2).nY)))
#define FN_GET_COORDF_DIST_SFX(p1,p2)           (FnSqrtF(FN_GET_DISTF_POW2_SFX((p1).fX, (p1).fY, (p2).fX, (p2).fY)))
#define FN_GET_POW2_SFX(x)                      ((x)*(x))
#define FN_MIN_SFX(a, b)                        ((a) > (b) ? (b) : (a))
#define FN_MAX_SFX(a, b)                        ((a) < (b) ? (b) : (a))
#define FN_ABS_SFX(a)                           ((a) >= 0 ? (a) : (-(a)))
#define FN_ABS_SUB_SFX(a, b)                    (((a) >= (b)) ? ((a) - (b)) : ((b) - (a)))
#define FN_BYTE_SFX(d, n)                       ((uint8_t)((d) >> ((n) * 8)))
#define FN_UINT8_SFX(p)                         (*((uint8_t*)(p)))
#define FN_UINT16_SFX(p)                        ((*((uint8_t*)(p))) + ((*((uint8_t*)(p) + 1)) << 8))
#define FN_UINT32_SFX(p)                        ((*((uint8_t*)(p))) + ((*((uint8_t*)(p) + 1)) << 8) + ((*((uint8_t*)(p) + 2)) << 16) + ((*((uint8_t*)(p) + 3)) << 24))
#define FN_SUB_MIN_ZERO_SFX(a, b)               ((a) > (b) ? ((a) - (b)) : 0)
#define FN_ALIGN_4BYTES_SFX(x)                  ((((x)+3)/4)*4)           //对齐4字节的倍数
#define FN_ALIGN_1K_SFX(x)                      ((((x)+1023)/1024)*1024)  //对齐1K的倍数
#define FN_ALIGN_2K_SFX(x)                      ((((x)+2047)/2048)*2048)  //对齐2K的倍数
#define FN_ALIGN_X_SFX(x, alignX)               ((((x)+((alignX)-1))/(alignX))*(alignX))  //对齐alignX的倍数
#define FN_CONV_1K_SFX(x)                       (((x)+1023)/1024)

//计算点到直线的垂直距离
#define CalVerticalDistX(x,y,a,b)               (FN_ABS_SFX((x)-(a)*(y)-(b))/(FnSqrtF(1+(a)*(a))))
#define CalVerticalDistY(x,y,a,b)               (FN_ABS_SFX((a)*(x)-(y)+(b))/(FnSqrtF(1+(a)*(a))))
#define CalVerticalDistXPow2(x,y,a,b)           ((((x)-(a)*(y)-(b))*((x)-(a)*(y)-(b)))/(1+(a)*(a)))
#define CalVerticalDistYPow2(x,y,a,b)           ((((a)*(x)-(y)+(b))*((a)*(x)-(y)+(b)))/(1+(a)*(a)))
#define CalVerticalDist(x,y,a,b)                (CalVerticalDistY((x),(y),(a),(b)))
#define CalVerticalDistPow2(x,y,a,b)            (CalVerticalDistYPow2((x),(y),(a),(b)))
#define CalABCVerticalDist(x,y,a,b,c)           (FN_ABS_SFX((a)*(x) + (b)*(y) + (c))/(FnSqrtF((a)*(a) + (b)*(b))))

#define GetRectFWidthHeight(arrCoordF,fWH)      ((fWH)->fX = (FN_ABS_SUB_SFX((arrCoordF)[2].fX, arrCoordF[0].fX)),\
                                                    (fWH)->fY = (FN_ABS_SUB_SFX((arrCoordF)[2].fY, arrCoordF[0].fY)))
#define GetRectCenter(arrCoordF,pCoordF)        ((pCoordF)->fX = (((arrCoordF)[2].fX + arrCoordF[0].fX)/2),\
                                                    (pCoordF)->fY = (((arrCoordF)[2].fY + arrCoordF[0].fY)/2))

#define GetRectBottom(arrCoordF)                ((arrCoordF)[0].fY)
#define GetRectTop(arrCoordF)                   ((arrCoordF)[2].fY)
#define GetRectLeft(arrCoordF)                  ((arrCoordF)[0].fX)
#define GetRectRight(arrCoordF)                 ((arrCoordF)[2].fX)
#define GetRectCoordAxis(arrCoordF, eAxis, bBig)     ((eAxis) == X? (arrCoordF)[bBig<<1].fX: (arrCoordF)[bBig<<1].fY)
#define IsPointFInRectF(pCoordF, pRectF)        ((((((pCoordF)->fX-(pRectF)[0].fX)*((pCoordF)->fX-(pRectF)[2].fX)) <= 0)\
                                                    && ((((pCoordF)->fY-(pRectF)[0].fY)*((pCoordF)->fY - (pRectF)[2].fY)) <= 0))?1:0)
#define IsPointFInRect2PtF(pCoordF, pRect2PtF)  ((((((pCoordF)->fX-(pRect2PtF)[0].fX)*((pCoordF)->fX-(pRect2PtF)[1].fX)) <= 0)\
                                                    && ((((pCoordF)->fY-(pRect2PtF)[0].fY)*((pCoordF)->fY - (pRect2PtF)[1].fY)) <= 0))?1:0)
                                                    
#define Get2PointManhattanDist(p1,p2)           (FN_ABS_SUB_SFX((p1).fX, (p2).fX) + FN_ABS_SUB_SFX((p1).fY, (p2).fY))

//高度转化使用
#define FN_SWAP_INT32(a,b)                      {int32_t c = (a); (a) = (b); (b) = (c);}
#define div_s64(a,b)                            ((a)/(b))
#define DIV_S64(a,b)                            ((b==0)?(div_s64((a), (1))):(div_s64((a), (b))))

//KB空间里求波动大小
#define GetKBSpcUnt(k, b, s, i)            ((i)-(((k)*(s))+(b)))
#define GetKBSpcUntAbs(k, b, s, i)         (FN_ABS_SFX(GetKBSpcUnt((k),(b),(s),(i))))

//计算两点斜率
#define GetPointsSlope(eAxis,p1,p2)         ((fix13_t)FIX_5DIV5_TO13_64((ACC_COORDF((p1), (eAxis))-ACC_COORDF((p2), (eAxis))),(ACC_COORDF((p1), REVER_AX((eAxis)))-ACC_COORDF((p2), REVER_AX(eAxis)))))
#define GetPointsInter(eAxis,p1,s)          ((fix_t)(ACC_COORDF((p1), (eAxis))-FIX_5MUL13_TO5_64(ACC_COORDF((p1), REVER_AX((eAxis))),(s))))

extern int32_t FnSqrt(int32_t nValue);
extern float FnSqrtF(float fValue);
extern float FnTanF(float fValue);
extern stCoordF_t GetPolygonCenterF(stCoordF_t *arrCoordF, int32_t nCount);
extern int32_t GetCoordRectF(stCoordF_t *arrCoordF, int32_t nCount, stCoordF_t* arrRectF);
extern int32_t GetExtCoordRectF(stCoordF_t* arrCoordF, int32_t nCount, stCoordF_t* arrRectF, float fExtLen);
extern stRectF_t GetCoordInRectF(stCoordF_t *arrCoordF, stCoordF_t *arrRectF);
extern stCoordF_t Get2RectRelativePos(stCoordF_t* arrRectA, stCoordF_t* arrRectB);
extern stCoordF_t GetPosAndRectRelativePos(stCoordF_t* pCoordF, stCoordF_t* arrRectF);
extern stRectF_t GetPolygonRectF(stCoordF_t* arrCoordF, int32_t nCount);
extern float GetAreaSqF(stCoordF_t *arrCoordF, int32_t nCount);
extern stLiAbcF_t LeastLinearFit(stCoordF_t *arrCoordF, int32_t nCoordCount, float *pfLambda, stLiSloIcpF_t *pKbL);
extern float CalVariance(float arrData[], int32_t nCount, float *fAver);
extern int32_t IsOnRectangle(stCoordF_t stCoordF1, stCoordF_t stCoordF2, stCoordF_t stPointF);
extern int32_t IsOnSegment(stCoordF_t stSegCoordF1, stCoordF_t stSegCoordF2, stCoordF_t stPointF);
extern int32_t IsRectFCross(stCoordF_t *arrRectF1, stCoordF_t *arrRectF2);
extern int32_t IsPointInPyg(stCoordF_t *arrCoordF, int32_t nCount, stCoordF_t stPointF);
extern float GetPoint2PygDistPow2(stCoordF_t stCoordF, stCoordF_t *arrCoordF, int32_t nCoordCount);
extern void QuickSortRecursion(int32_t *pData, int32_t nLeft, int32_t nRight);
extern float GetPoint2RectManhattanDistance(stCoordF_t stCoordF, stCoordF_t *arrRectF);
extern float  Get2RectManhattanDistance(stCoordF_t* arrRectA, stCoordF_t* arrRectB);

extern int32_t MathSquareRoot(int32_t nToBeSqrted);
extern int64_t MathSquareRoot_64(int64_t nToBeSqrted);
extern int32_t SortInt16(int16_t *arrData, int32_t nLen);
extern int32_t CalcLineDist2CoordF(stLiSloIcpF_t stLine, stCoordF_t stTestPt, float fDist, stCoordF_t *arrCoordRet);
extern stCoordF_t CalcLineDistCoordF(stLiAbcF_t stLine, stCoordF_t stTestPt, stCoordF_t stRefPt, float fDist);
extern fix13_t Get2PtsVerticalDist(stCoordF_t stPt0, stCoordF_t stPt1, stCoordF_t stTestPt);
extern int32_t PolyFit(eAxis_t eAxis, stCoordF_t *arrCoordF, int32_t nCount, int32_t nOrder, fix_64t *arrCoef);
extern fix_t GetPolyFitCoefValue(fix_64t *arrCoef, fix_t x, int32_t nOrder);
extern fix_t GetPolyFitDist(fix_64t *arrCoef, int32_t nOrder, fix_t fX1, fix_t fX2, fix_t h);
extern eAxis_t GetPolyFitAxis(stCoordF_t *arrCoordF, int32_t nCount);
extern stLiAbcF_t GetABCEquation(stCoordF_t stFirst, stCoordF_t stSecond);

#endif//_ALG_API_MATH_

