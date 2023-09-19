/*
 * Copyright (c) 2023 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "board.h"
#include <stdio.h>
#include "hpm_opamp_drv.h"

void init_opamp(void)
{
    hpm_opamp_ctrl0_cfg_t opamp_cfg;

    opamp_cfg.vip_sel = vip_inp_sel_vip0;
    opamp_cfg.is_vssa_disconnect = true;
    opamp_cfg.cap_sel1 = cap_sel_X7;
    opamp_cfg.cap_sel2 = 1;
    opamp_cfg.function_mode = 0;
    opamp_cfg.gap_sel = gpa_sel_127r;
    opamp_cfg.vim_sel = vim_inm_sel_floating;
    opamp_cfg.iref_sel = isel_iref_x_1;
    opamp_cfg.vswitch_sel = vswitch_sel_vsupply_x_0_5;
    opamp_cfg.opaout_sel = opaout_sel_vout_pad;
    opamp_cfg.en_lv = true;
    hpm_opamp_ctrl0_set(BOARD_APP_OPAMP, &opamp_cfg);
}

int main(void)
{
    board_init();
    init_opamp_pins();
    init_opamp();
    while (1) {
        ;
    }
}
