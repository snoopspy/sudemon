@echo off

:: 1. sudemon 설치 확인
for /f "delims=" %%i in ('adb shell ls /system/bin/sudemon 2^>^&1') do set "output=%%i"
echo %output% | findstr /i "no such file" >nul
if %errorlevel% equ 0 (
    echo sudemon not installed
    exit /b
) else (
    echo sudemon installed
)

:: 2. sudemon 실행 중인지 확인 (root 권한 확인)
for /f "delims=" %%i in ('adb shell su -c whoami 2^>^&1') do set "output=%%i"
echo %output% | findstr /i "root" >nul
if %errorlevel% equ 0 (
    echo sudemon running
    exit /b
) else (
    echo sudemon not running
)
