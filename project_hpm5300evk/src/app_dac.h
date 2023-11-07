#ifndef _APP_DAC_H
#define _APP_DAC_H
#include <math.h>
#include "board.h"
#include "hpm_dac_drv.h"

#define DAC_STEP_CFG_IDX DAC_STEP_CFG_STEP0

#define DAC_BUFF0_COUNT (DAC_SOC_MAX_BUFF_COUNT / 32)  /* 2048 */
#define DAC_BUFF1_COUNT (DAC_SOC_MAX_BUFF_COUNT / 32)  /* 2048 */

#define PI (3.1415926f)

void dac_test_init(void);
void dac_test_process(void);

#endif
