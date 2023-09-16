# Copyright (c) 2021-2022 HPMicro
# SPDX-License-Identifier: BSD-3-Clause

if(NOT DEFINED ENV{GNURISCV_TOOLCHAIN_PATH})
    message(FATAL_ERROR "GNURISCV_TOOLCHAIN_PATH is not set yet")
endif()

# 设置缓存条目
# [[
    set(<variable> <value> ... CACHE <type> <docstring> [FORCE])
    将缓存条目variable设置为值<value>...，除非用户进行设置或使用了选项FORCE，默认情况下缓存条目的值不会被覆盖。
    缓存条目可以通过CMAKE的GUI界面的add entry按钮来增加。缓存条目的实质为可以跨层级进行传递的变量，类似于全局变量
    缓存条目的<type>主要有以下几类：
    BOOL：布尔值ON/OFF，CMAKE的GUI界面对此类缓存条目会提供一个复选框
    FILEPATH：文件路径，CMAKE的GUI界面对此类缓存条目会提供一个文本选择框
    PATH：目录路径，CMAKE的GUI界面对此类缓存条目会提供一个目录选择框
    STRING/STRINGS：文本行，CMAKE的GUI界面对此类缓存条目会提供一个文本框（STRING）或下拉选择框（STRINGS）
    INTERNAL：文本行，但是只用于内部，不对外呈现。主要用于运行过程中存储变量，因此使用该type意味着使用FORCE
    
    缓存条目的几个注意事项：
    1）如果变量先前未定义或者使用了FORCE选项，则缓存条目会直接被赋值
    2）可以在使用cmake构建的使用-D选项来给缓存条目赋值，这样CMakeLists.txt内的set命令只会为缓存条目添加类型
    3）如果变量类型是目录或者文件路径，通过-D选择传入的若只是相对路径，那么set命令会给这个相对路径前添加当前的工作目录
        以变成绝对路径（如果已经是绝对路径则不会处理）
]]
set(APP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "application source directory")
set(APP_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE PATH "application binary directory")
set(__build_dir ${APP_BIN_DIR}/build_tmp)

message("*************************")
message(STATUS "application.cmake message")
message(STATUS "APP_SRC_DIR=${APP_SRC_DIR}")
message(STATUS "APP_BIN_DIR=${APP_BIN_DIR}")
message(STATUS "__build_dir=${__build_dir}")


set(APP_NAME demo)
set(APP_ELF_NAME ${APP_NAME}.elf)
set(APP_BIN_NAME ${APP_NAME}.bin)
set(APP_MAP_NAME ${APP_NAME}.map)
set(APP_ASM_NAME ${APP_NAME}.asm)

set(HPM_SDK_LIB hpm_sdk_lib)
set(HPM_SDK_LIB_ITF hpm_sdk_lib_itf)

# store all options
# 创建一个空的静态库hpm_sdk_lib
add_library(${HPM_SDK_LIB} STATIC "")
# 动态链接
target_link_libraries(${HPM_SDK_LIB} PUBLIC ${HPM_SDK_LIB_ITF})
# 创建一个接口库
add_library(${HPM_SDK_LIB_ITF} INTERFACE)

add_library(app STATIC "")

message(STATUS "Application: ${APP_SRC_DIR}")

# TO_CMAKE_PATH是file中的一个路径转换，类似将${HPM_SDK_BASE}赋值给PROJECT_SOURCE_DIR
#[[
    file({TO_CMAKE_PATH | TO_NATIVE_PATH} <path> <out-var>)
    TO_CMAKE_PATH：用途是将文件路径从其他表示法转换为 CMake 的路径表示法。
                    CMake 的路径表示法使用 / 作为路径分隔符，可以在不同操作系统上使用，而不受操作系统特定的路径分隔符的限制。
    TO_NATIVE_PATH：将文件路径从CMAKE的路径表示法转换为特定操作系统的本地路径标识法
]]
file(TO_CMAKE_PATH "${HPM_SDK_BASE}" PROJECT_SOURCE_DIR)

set(PROJECT_SOURCE_DIR ${HPM_SDK_BASE})
set(PROJECT_BINARY_DIR ${__build_dir})

# 设置lib输出路径和可执行文件输出路径
set(LIBRARY_OUTPUT_PATH ${APP_BIN_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${APP_BIN_DIR}/output)

# search board in extra_board_path if given
# BOARD_SEARCH_PATH为空，如下if语句不执行
if(BOARD_SEARCH_PATH AND EXISTS ${BOARD_SEARCH_PATH})
    find_path(BOARD_SEARCH_DIR NAMES ${BOARD}.yaml PATHS ${BOARD_SEARCH_PATH}/* NO_DEFAULT_PATH)
    if(BOARD_SEARCH_DIR)
        set(BOARD_MESSAGE "Board (custom board): ${BOARD} from ${BOARD_SEARCH_PATH}")
        set(HPM_BOARD_DIR ${BOARD_SEARCH_PATH}/${BOARD})
    endif()
endif()

# search board in sdk
# BOARD_SEARCH_DIR为空，如下if语句执行
if(NOT BOARD_SEARCH_DIR)
    #[[
        find_path:用于查找指定的文件的目录，如果没找到，返回的结果为<VAR>-NOTFOUND。
        格式如下：
        find_path (
          <VAR>
          name | NAMES name1 [name2 ...]
          [HINTS [path | ENV var]... ]
          [PATHS [path | ENV var]... ]
          [REGISTRY_VIEW (64|32|64_32|32_64|HOST|TARGET|BOTH)]
          [PATH_SUFFIXES suffix1 [suffix2 ...]]
          [VALIDATOR function]
          [DOC "cache documentation string"]
          [NO_CACHE]
          [REQUIRED]
          [NO_DEFAULT_PATH]
          [NO_PACKAGE_ROOT_PATH]
          [NO_CMAKE_PATH]
          [NO_CMAKE_ENVIRONMENT_PATH]
          [NO_SYSTEM_ENVIRONMENT_PATH]
          [NO_CMAKE_SYSTEM_PATH]
          [NO_CMAKE_INSTALL_PREFIX]
          [CMAKE_FIND_ROOT_PATH_BOTH |
           ONLY_CMAKE_FIND_ROOT_PATH |
           NO_CMAKE_FIND_ROOT_PATH]
         )
        
        NAMES:为要查找的目录中的文件指定一个或多个可能的名字
        PATHS：指定除默认位置外要搜索的目录。HINTS与PATHS区别：HINTS是在搜索系统路径之前先搜索HINTS指定的路径。PATHS是先搜索系统路径，然后再搜索PATHS指定的路径。
        NO_DEFAULT_PATH:如果指定了次选项，则不会向搜索中添加其他路径。默认搜索路径将失效，只会搜索PATHS和HINTS指定的路径
        
        如下搜索中的${BOARD}中的BOARD是在执行cmake指令时通过-D参数指定的。具体在generate_project.cmd文件中
        cmake --fresh -GNinja -DBOARD=%b% -DCMAKE_BUILD_TYPE=%t% ..
    ]]
    find_path(SDK_BOARD_DIR NAMES ${BOARD}.yaml PATHS ${HPM_SDK_BASE}/boards/* NO_DEFAULT_PATH)
    if(SDK_BOARD_DIR)
        set(BOARD_MESSAGE "Board: ${BOARD}")
        set(HPM_BOARD_DIR ${HPM_SDK_BASE}/boards/${BOARD})
    endif()
endif()

if(NOT HPM_BOARD_DIR)
    message(FATAL_ERROR "No board named '${BOARD}' found")
endif()
message(STATUS "${BOARD_MESSAGE}")
set(BOARD_YAML ${HPM_BOARD_DIR}/${BOARD}.yaml)

# 在工程文件中寻找包含app.yaml的目录
find_path(APP_YAML_PATH NAMES app.yaml PATHS ${APP_SRC_DIR} NO_DEFAULT_PATH)

if(NOT RV_ABI)
    set(RV_ABI "ilp32")
endif()

if(NOT RV_ARCH)
    set(RV_ARCH "rv32imac")
endif()

# add extention
include(${HPM_SDK_BASE}/cmake/python.cmake)
include(${HPM_SDK_BASE}/cmake/cmake-ext.cmake)
include(${HPM_SDK_BASE}/cmake/toolchain.cmake)
include(${HPM_SDK_BASE}/cmake/ide/segger.cmake)
include(${HPM_SDK_BASE}/cmake/extra_flags.cmake)
include(${HPM_SDK_BASE}/cmake/ccache.cmake)

# distclean target
add_custom_target(
    distclean
    COMMAND ${CMAKE_COMMAND} -DBIN_DIR=${APP_BIN_DIR}
    -DSRC_DIR=${APP_SRC_DIR} -P ${HPM_SDK_BASE}/cmake/cleanup.cmake
)

if(APP_YAML_PATH)
    check_board_capability(${BOARD_YAML} "${APP_YAML_PATH}/app.yaml" result)
    if(${result} STREQUAL "1")
        message(FATAL_ERROR "${BOARD} can not support this sample")
    endif()
    check_excluded_targets("${APP_YAML_PATH}/app.yaml" excluded_targets)
endif()

# default build type is set to debug
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE debug)
endif()

if((excluded_targets) AND (NOT ${CMAKE_BUILD_TYPE} STREQUAL ""))
    foreach(t IN ITEMS  ${excluded_targets})
        string(COMPARE EQUAL ${CMAKE_BUILD_TYPE} ${t} result)
        if (${result} EQUAL 1)
            message(FATAL_ERROR "\n!!! target ${CMAKE_BUILD_TYPE} has been excluded for this example\n")
        endif()
    endforeach()
endif()

get_soc_name_of_board(${BOARD_YAML} soc_name)
set(HPM_SOC ${soc_name})

# device name
get_device_name_of_board(${BOARD_YAML} device_name)
set(HPM_DEVICE_NAME ${device_name})

get_flash_size_of_board(${BOARD_YAML} flash_size)
get_extram_size_of_board(${BOARD_YAML} extram_size)

string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)
if(NOT extram_size)
    string(FIND ${build_type} "sdram" found)
    if(${found} GREATER_EQUAL 0)
        message(FATAL_ERROR "\n!!! target ${build_type} is not supported for ${BOARD}\n")
    endif()
endif()


if(NOT HEAP_SIZE)
    SET(HEAP_SIZE 0x4000)
endif()

if(NOT STACK_SIZE)
    SET(STACK_SIZE 0x4000)
endif()

# skip compiler check
set(CMAKE_C_COMPILER_FORCED 1)
set(CMAKE_CXX_COMPILER_FORCED 1)

enable_language(C CXX ASM)

add_subdirectory(${HPM_SDK_BASE} ${__build_dir})

# link final executable
target_link_libraries(app PUBLIC ${HPM_SDK_LIB_ITF})

if(${APP_SRC_DIR} STREQUAL ${APP_BIN_DIR})
    message(FATAL_ERROR "source directory is the same with binary directory.\
    please specify a build directory, e.g. cmake -Bbuild -H.")
endif()

