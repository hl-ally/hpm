# Copyright (c) 2021 HPMicro
# SPDX-License-Identifier: BSD-3-Clause

if(NOT DEFINED ENV{HPM_SDK_BASE})
    message(FATAL_ERROR "HPM_SDK_BASE is not set yet") 
endif()

set(HPM_SDK_BASE $ENV{HPM_SDK_BASE})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message("")
message("****************************************")
message(STATUS "hpm-sdk-config.cmake message")
message("HPM_SDK_BASE=${HPM_SDK_BASE}")
message("CMAKE_EXPORT_COMPILE_COMMANDS=${CMAKE_EXPORT_COMPILE_COMMANDS}")
message("****************************************")
message("")

# include指令用来载入并运行来自文件或模块得cmake代码
include(${HPM_SDK_BASE}/cmake/application.cmake)
