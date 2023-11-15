# Copyright (c) 2021 HPMicro
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.13)

set(CONFIG_CHERRYUSB 1)
set(CONFIG_USB_DEVICE 1)
set(CONFIG_USB_DEVICE_HID 1)
set(CONFIG_DMA_MGR 1)

# 关于find_package的介绍可以参考网址：https://www.jianshu.com/p/a0915895dbbc
# 寻找在"HPM_SDK_BASE"目录下的hmp-sdk-config.cmake文件，并执行
find_package(hpm-sdk REQUIRED HINTS $ENV{HPM_SDK_BASE})

project(hpm5300evk)

set(run_in_ram 0)
if(run_in_ram)
    sdk_compile_definitions("-DRUN_IN_RAM=1")
endif()

sdk_app_inc(src)
sdk_app_src(src/main.c)
sdk_app_src(src/app_systick.c)
sdk_app_src(src/app_flash.c)
sdk_app_src(src/app_usb_application.c)
sdk_app_src(src/app_usb_boot.c)
sdk_app_src(src/app_otp.c)
sdk_app_src(src/app_watch_dog.c)
sdk_app_src(src/app_dac.c)
sdk_app_src(src/app_pwm.c)
sdk_app_src(src/app_adc.c)
sdk_app_src(src/app_tim.c)
sdk_app_src(src/app_tsns.c)
sdk_app_src(src/app_dma_mgr.c)

sdk_ses_opt_debug_connection(J-Link)
generate_ses_project()
