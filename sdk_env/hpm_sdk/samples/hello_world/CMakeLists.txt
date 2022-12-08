# Copyright (c) 2021 HPMicro
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.13)

set(CONFIG_CHERRYUSB 1)
set(CONFIG_USB_DEVICE 1)
set(CONFIG_CHERRYUSB_DEVICE 1)
set(CONFIG_CHERRYUSB_DEVICE_HID 1)
set(CONFIG_CHERRYUSB_DEVICE_CDC 1)

find_package(hpm-sdk REQUIRED HINTS $ENV{HPM_SDK_BASE})

project(hello_world)

sdk_compile_definitions(-DBOARD_SHOW_CLOCK=0)

sdk_inc(../cherryusb/config)
sdk_app_src(src/hello_world.c)
sdk_app_src(src/pwm_beep.c)
sdk_app_src(src/pwm_rgb_led.c)
sdk_app_src(src/usb_dev_boot.c)
sdk_app_src(src/usb_dev_app.c)
generate_ses_project()
