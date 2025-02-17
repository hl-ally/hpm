@echo off
rem Copyright 2022 hpmicro
rem SPDX-License-Identifier: BSD-3-Clause
if not "%~1"=="true" (
    setlocal enabledelayedexpansion
)

cd ..

@rem Please modify the following variable
set TOOLCHAIN_NAME=rv32imac-ilp32-multilib-win
set HPM_SDK_BASE=%cd%\sdk_env\hpm_sdk

echo %HPM_SDK_BASE%

@rem Normally the following variables don't need to be updated
set GNURISCV_TOOLCHAIN_PATH=%cd%\sdk_env\toolchains\%TOOLCHAIN_NAME%
set PATH=%cd%\sdk_env\tools\cmake\bin;%GNURISCV_TOOLCHAIN_PATH%\bin;%cd%\sdk_env\tools\python3;%cd%\sdk_env\tools\Python3\Scripts;%cd%\sdk_env\tools\ninja;%cd%\sdk_env\tools\openocd;%cd%\sdk_env\tools\scripts;C:\Windows;C:\Windows\System32
set PYTHONPATH=%cd%\sdk_env\tools\Python3\Lib\site-packages
set HPM_SDK_TOOLCHAIN_VARIANT=
set LONG_PATH_ENABLED=false
pushd %cd%\sdk_env\hpm_sdk
set NEED_EXIT_FLAG=%~1
cls 
rem check if long path support has been enabled or not
set value=0
FOR /F "usebackq skip=2 tokens=1-2*" %%A IN (`REG QUERY HKLM\SYSTEM\CurrentControlSet\Control\FileSystem /v LongPathsEnabled 2^>nul`) DO (
    set value=%%C
)

if '%value%' NEQ '0x1' (
    rem "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
    >nul 2>&1 rd /q /s "%WinDir%\System32\hpmicro_test_permissions"
    >nul 2>&1 md "%WinDir%\System32\hpmicro_test_permissions"
    if '!errorlevel!' NEQ '0' (
        call :UACPrompt
        echo +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        echo ^|     HPMicro SDK Env Tool    ^|
        echo +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        echo.
        echo - WARNING: long path support has not been enabled yet
        echo.
    ) else (
        >nul 2>&1 rd /q /s "%WinDir%\System32\hpmicro_test_permissions"
        if exist "%temp%\getadmin.vbs" (
            echo +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            echo ^|     HPMicro SDK Env Tool: Enabling long path support    ^|
            echo +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            echo.
            goto gotAdmin
        ) else (
            echo +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            echo ^|     HPMicro SDK Env Tool    ^|
            echo +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            echo.
            echo - WARNING: long path support has not been enabled yet
            echo.
            >nul 2>&1 REG ADD HKLM\SYSTEM\CurrentControlSet\Control\FileSystem /v LongPathsEnabled /t REG_DWORD /d 1 /f
            echo - INFO: Long path support has been enabled
            set LONG_PATH_ENABLED=true
        )
    )
) else (
    echo +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    echo ^|     HPMicro SDK Env Tool    ^|
    echo +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    echo.
    echo - INFO: Long path support has been enabled
    set LONG_PATH_ENABLED=true
)

if exist "%HPM_SDK_BASE%/env.cmd" (
    echo - INFO: HPM_SDK is found at %HPM_SDK_BASE%
) else (
    echo - ERROR: HPM_SDK can not be found at %HPM_SDK_BASE%, please update environment variable HPM_SDK_BASE
)

if "%HPM_SDK_TOOLCHAIN_VARIANT%"=="" (
    set SDK_ENV_EXPECTED_GCC_BIN="%GNURISCV_TOOLCHAIN_PATH%\bin\riscv32-unknown-elf-gcc.exe"
)

if "%HPM_SDK_TOOLCHAIN_VARIANT%"=="nds-gcc" (
    set SDK_ENV_EXPECTED_GCC_BIN="%GNURISCV_TOOLCHAIN_PATH%\bin\riscv32-elf-gcc.exe"
)
if "%HPM_SDK_TOOLCHAIN_VARIANT%"=="nds-llvm" (
    set SDK_ENV_EXPECTED_GCC_BIN="%GNURISCV_TOOLCHAIN_PATH%\bin\riscv32-elf-clang.exe"
)

if exist "%SDK_ENV_EXPECTED_GCC_BIN%" (
    echo - INFO: Toolchain is found at %GNURISCV_TOOLCHAIN_PATH%
) else (
    echo - WARNING: Toolchain can not found at %GNURISCV_TOOLCHAIN_PATH%, please update environment variable GNURISCV_TOOLCHAIN_PATH
)

if "%HPM_SDK_TOOLCHAIN_VARIANT%"=="nds-gcc" goto CYGWIN_PATH_WARNING
if "%HPM_SDK_TOOLCHAIN_VARIANT%"=="nds-llvm" goto CYGWIN_PATH_WARNING

if not exist "%APPDATA%\OpenOCD" (
    md "%APPDATA%\OpenOCD"
)

if exist %HPM_SDK_BASE%\boards\openocd (
    xcopy /S /Q /Y %HPM_SDK_BASE%\boards\openocd "%APPDATA%\OpenOCD\" > nul 2>&1
    if ERRORLEVEL 1 (
        echo - WARNING: openocd config file failed to be copied to %APPDATA% from %HPM_SDK_BASE%\boards\openocd
    ) else (
        echo - INFO: openocd config file has been copied to %APPDATA% from %HPM_SDK_BASE%\boards\openocd
    )
)
if exist tools\openocd\tcl (
    xcopy /S /Q /Y tools\openocd\tcl "%APPDATA%\OpenOCD\" > nul 2>&1
    if ERRORLEVEL 1 (
        echo - WARNING: openocd config file failed to be copied to %APPDATA% from tools\openocd\tcl
    ) else (
        echo - INFO: openocd config file copied to %APPDATA% from tools\openocd\tcl
    )
)
if exist tools\openocd\scripts (
    xcopy /S /Q /Y tools\openocd\scripts "%APPDATA%\OpenOCD\" > nul 2>&1
    if ERRORLEVEL 1 (
        echo - WARNING: openocd config file failed to be copied to %APPDATA% from tools\openocd\scripts
    ) else (
        echo - INFO: openocd config file has been copied to %APPDATA% from tools\openocd\scripts
    )
)
set value=0
FOR /F "usebackq skip=2 tokens=1-2*" %%A IN (`REG QUERY HKLM\SYSTEM\CurrentControlSet\Control\FileSystem /v LongPathsEnabled 2^>nul`) DO (
    set value=%%C
)
if '%value%' EQU '0x1' (
    if "%LONG_PATH_ENABLED%"=="false" (
        echo - INFO: Long path support has been enabled
    )
)

goto DONE

:gotAdmin
    if exist "%temp%\getadmin.vbs" ( del "%temp%\getadmin.vbs" )
    >nul 2>&1 REG ADD HKLM\SYSTEM\CurrentControlSet\Control\FileSystem /v LongPathsEnabled /t REG_DWORD /d 1 /f
    echo - INFO: Long path support has been enabled, press any key to close this window
    pause
    exit

:UACPrompt
echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
echo UAC.ShellExecute "cmd.exe", "/c %~s0 %~1", "", "runas", 1 >> "%temp%\getadmin.vbs"

"%temp%\getadmin.vbs"
rem del "%temp%\getadmin.vbs"
exit /b 0

:CYGWIN_PATH_WARNING
echo.
echo WARNING: Currently, Andes toolchain is specified, please make sure cygwin path is appended to current PATH environment variable of current console

:DONE
echo.
popd

set SUPPORTED_BUILD_TYPES=release debug flash_xip flash_xip_release flash_sdram_xip flash_sdram_xip_release flash_uf2 flash_uf2_release flash_sdram_uf2 flash_sdram_uf2_release sec_core_img sec_core_img_release
set USER_BUILD_TYPE=flash_xip_release
set run_in_ram=1

if "%run_in_ram%"=="1" (
    echo "run in ram"
    set USER_BUILD_TYPE=release
    
) else (
    echo "run in flash"
    set USER_BUILD_TYP=flash_xip_release
    
)

if not "%NEED_EXIT_FLAG%"=="true" (
    cd %~dp0
    echo %USER_BUILD_TYPE%
    call generate_project -f -b hpm5300evk -t %USER_BUILD_TYPE%
    pause
    ::cmd
    ::C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe
    endlocal
)
