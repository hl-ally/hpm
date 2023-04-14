@echo off
rem Copyright 2022 hpmicro
rem SPDX-License-Identifier: BSD-3-Clause
rem This script can produce segger project under samples_ses_project dir
setlocal enabledelayedexpansion

@rem Please modify the following variable
set VBSCRIPT=Msgbox("Do you want to remove segger project?",1,"Warning")
for /f "Delims=" %%a in ('MsHta VBScript:Execute("CreateObject(""Scripting.Filesystemobject"").GetStandardStream(1).Write(%VBSCRIPT:"=""%)"^)(Close^)') do Set "MSHTA_RETURN_VALUE=%%a"
if not %MSHTA_RETURN_VALUE% == 1 (
    goto EXIT
)
cd %~dp0
if exist "start_cmd.cmd" (
    call start_cmd.cmd true 
) else (
    echo "start_cmd.cmd can not be found"
    pause
    goto EXIT
)
if "%HPM_SDK_BASE%"=="" (
    echo HPM_SDK_BASE needs to be defined first, pointing to the root directory of HPM_SDK
    pause
    goto EXIT
)
set LONG_PATH_VALUE=0
:CHECK_LONG_PATH
FOR /F "usebackq skip=2 tokens=1-2*" %%A IN (`REG QUERY HKLM\SYSTEM\CurrentControlSet\Control\FileSystem /v LongPathsEnabled 2^>nul`) DO (
    set LONG_PATH_VALUE=%%C
)
if not '%LONG_PATH_VALUE%' EQU '0x1' (
    goto CHECK_LONG_PATH
)
set HPM_PROJECT_BASE=%~dp0
set SES_PROJ=%HPM_PROJECT_BASE%samples_ses_project
if exist "%SES_PROJ%" (
    rd /q /s %SES_PROJ%
)

for /D %%B in ("%HPM_SDK_BASE%\samples\*") DO (
    set SAMPLE=%%~nB
    echo !SAMPLE!
::    if exist "%%B\CMakeLists.txt" (
::        echo %SES_PROJ%\!BOARD!
::        if exist "%SES_PROJ%\!BOARD!" (
::            rd /q /s %SES_PROJ%\!BOARD!
::        )
::    )
)

goto EXIT
:EXIT
pause
endlocal