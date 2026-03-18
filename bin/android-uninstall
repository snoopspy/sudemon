#!/bin/sh
set -x

./android-stop
adb root
adb exec-out "pkill sudemon"
adb exec-out "mount -o rw,remount /"
adb exec-out "rm -f /system/bin/su /system/bin/sudemon /system/bin/sudemon.sh"
#adb exec-out "mount -o ro,remount /"
