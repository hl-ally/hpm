# Copyright (c) 2021 HPMicro
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.13)

set(CONFIG_CHERRYUSB 1)
set(CONFIG_USB_DEVICE 1)
set(CONFIG_USB_DEVICE_HID 1)

# 关于find_package的介绍可以参考网址：https://www.jianshu.com/p/a0915895dbbc
# 寻找在"HPM_SDK_BASE"目录下的hmp-sdk-config.cmake文件，并执行
find_package(hpm-sdk REQUIRED HINTS $ENV{HPM_SDK_BASE})

project(boot)

sdk_app_inc(user/inc)
sdk_app_src(user/src/main.c)
sdk_app_src(user/src/app_systick.c)
sdk_app_src(user/src/app_usb_boot.c)
sdk_app_src(user/src/app_watch_dog.c)
sdk_app_src(user/src/Upgrade.c)

sdk_app_inc(API/inc)
sdk_app_src(API/src/Api_Flash.c)
sdk_app_src(API/src/Api_Math.c)
sdk_app_src(API/src/Api_Crc.c)

sdk_ses_opt_debug_connection(J-Link)
generate_ses_project()
