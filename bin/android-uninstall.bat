call .\android-stop.bat
adb root
adb exec-out "pkill sudemon"
adb remount
adb exec-out "rm -f /system/bin/su /system/bin/sudemon /system/bin/sudemon.sh"
