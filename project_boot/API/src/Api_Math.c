#include "Api_Math.h"
#include <string.h>



//填充内存
void* MemSet(void *pDst, int nChar, uint32_t nCount)
{
    return memset(pDst, nChar, nCount);
}

const void *MemCpy(void *pDst, const void *pSrc, uint32_t nCount)
{
    return memcpy(pDst, pSrc, nCount);
}



