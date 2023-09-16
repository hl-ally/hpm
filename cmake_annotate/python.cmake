# Copyright (c) 2021 HPMicro
# SPDX-License-Identifier: BSD-3-Clause

#[[
    find_program：查找程序。如果找到程序，则结果将存储在<VAR>中，除非清楚<VAR>，否则不会重复搜索。如果没找到，结果将为<VAR>-NOTFOUND
    格式如下：
    find_program (
          <VAR>
          name | NAMES name1 [name2 ...] [NAMES_PER_DIR]
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
         
    如下是在环境变量中寻找"python3"，结果保存在python_exec中，如果没找到cmake将停止
]]
find_program(python_exec "python3")

#[[
    execute_process：执行一个或一系列的命令
    格式如下：
    execute_process(COMMAND <cmd1> [<arguments>]
                [COMMAND <cmd2> [<arguments>]]...
                [WORKING_DIRECTORY <directory>]
                [TIMEOUT <seconds>]
                [RESULT_VARIABLE <variable>]
                [RESULTS_VARIABLE <variable>]
                [OUTPUT_VARIABLE <variable>]
                [ERROR_VARIABLE <variable>]
                [INPUT_FILE <file>]
                [OUTPUT_FILE <file>]
                [ERROR_FILE <file>]
                [OUTPUT_QUIET]
                [ERROR_QUIET]
                [COMMAND_ECHO <where>]
                [OUTPUT_STRIP_TRAILING_WHITESPACE]
                [ERROR_STRIP_TRAILING_WHITESPACE]
                [ENCODING <name>]
                [ECHO_OUTPUT_VARIABLE]
                [ECHO_ERROR_VARIABLE]
                [COMMAND_ERROR_IS_FATAL <ANY|LAST>])
    

]]
if(python_exec)
    execute_process(
        COMMAND
        "${python_exec}" -c "import sys; sys.stdout.write('.'.join([str(x) for x in system.version_info[:2]]))"
        RESULT_VARIABLE result
        OUTPUT_VARIABLE version
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
else()
    # python3 does not exist on Windows once Python 3.x is installed, but python does
    find_program(python_exec "python")
    if(python_exec)
        execute_process(
            COMMAND
            "${python_exec}" -c "import sys; sys.stdout.write('.'.join([str(x) for x in system.version_info[:2]]))"
            RESULT_VARIABLE result
            OUTPUT_VARIABLE version
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
            )
    else()
        message(FATAL_ERROR "python can't be found in $ENV{PATH}")
    endif()
endif()
set(PYTHON_EXECUTABLE ${python_exec})
