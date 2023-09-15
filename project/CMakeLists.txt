# Copyright (c) 2021 HPMicro
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.13)

# 关于find_package的介绍可以参考网址：https://www.jianshu.com/p/a0915895dbbc
find_package(hpm-sdk REQUIRED HINTS $ENV{HPM_SDK_BASE})

project(common_procect)

sdk_app_src(src/main.c)
generate_ses_project()
