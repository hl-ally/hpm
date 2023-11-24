#ifndef _API_MATH_
#define _API_MATH_
#include <stdint.h>
#include <limits.h>
#include <math.h>

extern float MagicSqrt(float x);

#define PI                                  (3.141592653f)
#define INFINITE_MIN                        (0.000001f)

#define BIT0                                0x0001
#define BIT1                                0x0002
#define BIT2                                0x0004
#define BIT3                                0x0008
#define BIT4                                0x0010
#define BIT5                                0x0020
#define BIT6                                0x0040
#define BIT7                                0x0080
#define BIT8                                0x0100
#define BIT9                                0x0200
#define BIT10                               0x0400
#define BIT11                               0x0800
#define BIT12                               0x1000
#define BIT13                               0x2000
#define BIT14                               0x4000
#define BIT15                               0x8000

//宏函数定义
#define FN_GetDistance(X1, Y1, X2, Y2)      (MagicSqrt((((X1) - (X2)) * ((X1) - (X2)) + ((Y1) - (Y2)) * ((Y1) - (Y2)))))
#define FN_GetDistancePow2(X1, Y1, X2, Y2)  (((X1) - (X2)) * ((X1) - (X2)) + ((Y1) - (Y2)) * ((Y1) - (Y2)))
#define FN_MIN(a, b)                        (((a) > (b)) ? (b) : (a))
#define FN_MAX(a, b)                        (((a) < (b)) ? (b) : (a))
#define FN_ABS(a)                           (((a) >= 0 )? (a) : (-(a)))
#define FN_BYTE(d, n)                       ((uint8_t)((d) >> ((n) * 8)))
#define FN_UINT8(p)                         (*((uint8_t*)(p)))
#define FN_UINT16(p)                        ((*((uint8_t*)(p))) + ((*((uint8_t*)(p) + 1)) << 8))
#define FN_UINT32(p)                        ((*((uint8_t*)(p))) + ((*((uint8_t*)(p) + 1)) << 8) + ((*((uint8_t*)(p) + 2)) << 16) + ((*((uint8_t*)(p) + 3)) << 24))
#define FN_ANOTHER_POS_TO_INT32(a, x, b)    ((int32_t)((a) * (x) + (b)))
#define FN_ANOTHER_POS(a, x, b)             (((a) * (x) + (b)))
#define FN_LOBYTE(x)                        ((uint8_t)((x) & 0x00FF))
#define FN_HIBYTE(x)                        ((uint8_t)(((x) & 0xFF00) >>8))
#define FN_GetAbsSub(a, b)                  (((a) >= (b)) ? ((a) - (b)) : ((b) - (a)))
#define FN_ArrSize(arrName)                 (sizeof(arrName)/(sizeof(arrName[0])))


extern void *MemSet(void *pDst, int nChar, uint32_t nCount);
extern const void *MemCpy(void *pDst, const void *pSrc, uint32_t nCount);


#endif

