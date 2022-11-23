#include <stdio.h>
#include "pwm_beep.h"
#include "hpm_soc.h"


/*
 * PWM控制BEEP,IO口初始化
*/
void init_beep_pwm_pin(void)
{
    HPM_IOC->PAD[IOC_PAD_PC20].FUNC_CTL = IOC_PC20_FUNC_CTL_TRGM3_P_06;
}

void pwm_beep_init(void)
{
    printf("pwm beep init\n");
}

void pwm_beep_process(void)
{
    
}
