call .\android-stop.bat
adb root
adb exec-out "pkill sudemon"
adb exec-out "mount -o rw,remount /"
adb exec-out "rm -f /system/bin/su /system/bin/sudemon /system/bin/sudemon.sh"
rem adb exec-out "mount -o ro,remount /"
