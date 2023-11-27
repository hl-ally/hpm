#ifndef _CUSTOMER_H_
#define _CUSTOMER_H_

#include "BoardId.h"

#define CUSTOMER_DEFAULT              0
#define CUSTOMER_SEEWO                1     //SR


#define CUSTOMER_ID                   CUSTOMER_SEEWO

#if (CUSTOMER_ID == CUSTOMER_DEFAULT)
#include "Customer_Default.h"
#elif (CUSTOMER_ID == CUSTOMER_SEEWO)
#include "Customer_Seewo.h"
#include "CustomerAlgPara_Seewo.h"

#else
#error "Please define CUSTOMER_ID"
#endif
//默认低高度参数
#include "CustomerAlgPara_Default.h"
#endif //_CUSTOMER_H_


