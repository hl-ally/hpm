#ifndef _ALG_API_STDINT_
#define _ALG_API_STDINT_

#include "stdint.h"

#define UNUSED(x)   ((x) = (x))
#define SIZEARR(x)  ((int32_t)(sizeof((x))/sizeof((x)[0])))
typedef float fix13_t;   //Float<<13
typedef float fix_t;     //Float<<5
typedef float fix_64t;
#endif //_ALG_API_STDINT_

