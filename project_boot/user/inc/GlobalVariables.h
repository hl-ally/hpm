#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H
//#include "GlobalDefaultDefine.h"
//#include "GlobalDataStructure.h"
//#include "HardWareInfo.h"
#include "Api_Math.h"
//#include "Api_CommandQueue.h"
//#include "AlgCfgPara.h"

#define MAX_LED_ON_CNT                 (3000)
#define ADC_BUFFER_LEN                 (MAX_LED_ON_CNT * ADC_CH_TOTAL)

#ifndef Firmware_Version
#define Firmware_Version                0x0100
#endif
#define PRODUCT_BOARD_TYPE              0x02
#define CRC32_DATA                      0x022B3C5F

#define EP_IN_ID                        0xFD
#define EP_OUT_ID                       0xFE



#endif  // GLOBAL_VARIABLES_H

