#ifndef _ALG_CUSTOMER_H_
#define _ALG_CUSTOMER_H_

#if (defined GD405_ENV || defined GD405_CUSTOMER)
#include "..\..\..\ProjectGD405\ProjectApp\src\Customer\Customer.h"
#elif (defined GD415_ENV || defined GD415_CUSTOMER)
#include "..\..\..\ProjectGD415\ProjectApp\src\Customer\Customer.h"
#elif (defined N32G457_ENV || defined N32G457_CUSTOMER)
#include "..\..\..\ProjectN32G457\ProjectApp\src\Customer\Customer.h"
#else
#include "Customer.h"
#endif

#endif //_ALG_CUSTOMER_H_
