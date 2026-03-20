@echo off

for /f "delims=" %%i in ('adb shell "[ -f /system/bin/sudemon ] && echo exists || echo not found" 2^>^&1') do set output=%%i

echo !output! | findstr /i "no devices" >nul
if !errorlevel! == 0 (
    echo !output!
    exit /b
)

if "!output!"=="exists" (
    echo sudemon installed
) else (
    echo sudemon not installed
    exit /b
)

for /f "delims=" %%i in ('adb shell su -c whoami 2^>^&1') do set output=%%i

echo !output! | findstr /i "root" >nul
if !errorlevel! == 0 (
    echo sudemon running
    exit /b
) else (
    echo sudemon not running
)
