# Copyright (c) 2021 HPMicro
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.13)

set(CONFIG_CHERRYUSB 1)
set(CONFIG_USB_DEVICE 1)
set(CONFIG_USB_DEVICE_HID 1)

set(CUSTOM_GCC_LINKER_FILE ./application.icf)

find_package(hpm-sdk REQUIRED HINTS $ENV{HPM_SDK_BASE})

project(app)

sdk_app_inc(user/inc)
sdk_app_src(user/src/main.c)
sdk_app_src(user/src/app_systick.c)
sdk_app_src(user/src/app_watch_dog.c)
sdk_app_src(user/src/CommService.c)
sdk_app_src(user/src/Communication.c)
sdk_app_src(user/src/FlashPara.c)
sdk_app_src(user/src/HardWareInfo.c)
sdk_app_src(user/src/GlobalVariables.c)
sdk_app_src(user/src/TouchSignal.c)
sdk_app_src(user/src/Evaluation.c)
sdk_app_src(user/src/Calibration.c)


sdk_app_inc(API/inc)
sdk_app_src(API/src/Api_Flash.c)
sdk_app_src(API/src/Api_Math.c)
sdk_app_src(API/src/Api_Crc.c)
sdk_app_src(API/src/Api_CommandQueue.c)
sdk_app_src(API/src/Api_UsbDesc.c)
sdk_app_src(API/src/Api_UsbDevice.c)

sdk_app_inc(alg/inc)

sdk_app_inc(Customer)


sdk_ses_opt_debug_connection(J-Link)
generate_ses_project()
