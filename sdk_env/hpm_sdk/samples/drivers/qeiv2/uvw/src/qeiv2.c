/*
 * Copyright (c) 2021 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "board.h"
#include "hpm_soc_feature.h"
#include "hpm_trgm_drv.h"
#include "hpm_qeiv2_drv.h"

#ifndef BOARD_APP_QEI_BASE
#define BOARD_APP_QEI_BASE BOARD_BLDC_QEI_BASE
#endif

static volatile uint32_t pos;

/* API for extern */

/* Static function declaration */
static void qeiv2_init(void);

/* Function definition */
int main(void)
{
    board_init();
    board_init_qeiv2_abz_uvw_pins(BOARD_APP_QEI_BASE);
    qeiv2_init();

    printf("qeiv2 uvw encoder example\n");

    for (uint32_t i = 0; i < 100; i++) {
        pos = qeiv2_get_postion(BOARD_APP_QEI_BASE);
        printf("pos:%#x\n", pos);
    }

    while (1) {
        ;
    }
    return 0;
}


static void qeiv2_init(void)
{
    qeiv2_reset_counter(BOARD_APP_QEI_BASE);

    qeiv2_set_work_mode(BOARD_APP_QEI_BASE, qeiv2_work_mode_uvw);
    qeiv2_config_abz_uvw_signal_edge(BOARD_APP_QEI_BASE, true, true, true, true, true);
    qeiv2_set_uvw_position_opt(BOARD_APP_QEI_BASE, qeiv2_uvw_opt_1_qeo);
    qeiv2_config_uvw_position(BOARD_APP_QEI_BASE, QEIV2_UVW_POS_CFG_UVW_POS0_CFG, QEIV2_UVW_OPT_1_POS_SEL_HIGH, QEIV2_UVW_OPT_1_POS_SEL_LOW, QEIV2_UVW_OPT_1_POS_SEL_HIGH, true);
    qeiv2_config_uvw_position(BOARD_APP_QEI_BASE, QEIV2_UVW_POS_CFG_UVW_POS1_CFG, QEIV2_UVW_OPT_1_POS_SEL_HIGH, QEIV2_UVW_OPT_1_POS_SEL_LOW, QEIV2_UVW_OPT_1_POS_SEL_HIGH, true);
    qeiv2_config_uvw_position(BOARD_APP_QEI_BASE, QEIV2_UVW_POS_CFG_UVW_POS2_CFG, QEIV2_UVW_OPT_1_POS_SEL_HIGH, QEIV2_UVW_OPT_1_POS_SEL_HIGH, QEIV2_UVW_OPT_1_POS_SEL_LOW, true);
    qeiv2_config_uvw_position(BOARD_APP_QEI_BASE, QEIV2_UVW_POS_CFG_UVW_POS3_CFG, QEIV2_UVW_OPT_1_POS_SEL_LOW, QEIV2_UVW_OPT_1_POS_SEL_HIGH, QEIV2_UVW_OPT_1_POS_SEL_LOW, true);
    qeiv2_config_uvw_position(BOARD_APP_QEI_BASE, QEIV2_UVW_POS_CFG_UVW_POS4_CFG, QEIV2_UVW_OPT_1_POS_SEL_LOW, QEIV2_UVW_OPT_1_POS_SEL_HIGH, QEIV2_UVW_OPT_1_POS_SEL_LOW, true);
    qeiv2_config_uvw_position(BOARD_APP_QEI_BASE, QEIV2_UVW_POS_CFG_UVW_POS5_CFG, QEIV2_UVW_OPT_1_POS_SEL_LOW, QEIV2_UVW_OPT_1_POS_SEL_LOW, QEIV2_UVW_OPT_1_POS_SEL_HIGH, true);
    qeiv2_set_uvw_position(BOARD_APP_QEI_BASE, QEIV2_UVW_POS_UVW_POS0, 0x15555555u);  /* 30 deg */
    qeiv2_set_uvw_position(BOARD_APP_QEI_BASE, QEIV2_UVW_POS_UVW_POS1, 0x3FFFFFFFu);  /* 90 deg */
    qeiv2_set_uvw_position(BOARD_APP_QEI_BASE, QEIV2_UVW_POS_UVW_POS2, 0x6AAAAAAAu);  /* 150 deg */
    qeiv2_set_uvw_position(BOARD_APP_QEI_BASE, QEIV2_UVW_POS_UVW_POS3, 0x95555555u);  /* 210 deg */
    qeiv2_set_uvw_position(BOARD_APP_QEI_BASE, QEIV2_UVW_POS_UVW_POS4, 0xBFFFFFFFu);  /* 270 deg */
    qeiv2_set_uvw_position(BOARD_APP_QEI_BASE, QEIV2_UVW_POS_UVW_POS5, 0xEAAAAAAAu);  /* 330 deg */

    qeiv2_release_counter(BOARD_APP_QEI_BASE);
}
