adb root
adb remount
adb push su /system/bin/su2
adb exec-out "mv /system/bin/su2 /system/bin/su"
adb push sudemon /system/bin/sudemon
adb push sudemon.sh /system/bin/sudemon.sh
adb exec-out "chmod 755 /system/bin/su'"
adb exec-out "chmod 755 /system/bin/sudemon"
adb exec-out "chmod 755 /system/bin/sudemon.sh"
call .\android-start.bat
