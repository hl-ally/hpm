# Copyright (c) 2021 HPMicro
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.13)
set(CUSTOM_GCC_LINKER_FILE ./application.icf)
find_package(hpm-sdk REQUIRED HINTS $ENV{HPM_SDK_BASE})

project(app)

sdk_app_src(src/main.c)
#sdk_app_src(src/app_systick.c)

sdk_ses_opt_debug_connection(J-Link)
generate_ses_project()
