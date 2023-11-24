/*
 * Copyright (c) 2021 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <stddef.h>
#include "board.h"
#include "hpm_debug_console.h"
#include "hpm_gpio_drv.h"



int main(void)
{
    int u;
    board_init();

    board_init_led_pins();

    board_timer_create(1000, board_led_toggle);

    printf("hello world\n");
    while(1)
    {
        ;
    }
    return 0;
}
