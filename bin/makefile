.PHONY : clean install uninstall start stop

all:
	@echo "Select:"
	@echo "make install"
	@echo "make uninstall"
	@echo "make start"
	@echo "make stop"

install:
	adb root
	adb exec-out "mount -o rw,remount /"
	adb push su /system/bin/su2
	adb exec-out "mv /system/bin/su2 /system/bin/su"
	adb push sudemon /system/bin/sudemon
	adb push sudemon.sh /system/bin/sudemon.sh
	adb exec-out "chmod 755 /system/bin/su'"
	adb exec-out "chmod 755 /system/bin/sudemon"
	adb exec-out "chmod 755 /system/bin/sudemon.sh"
	#adb exec-out "mount -o ro,remount /"
	make start

uninstall:
	make stop
	adb root
	adb exec-out "pkill sudemon"
	adb exec-out "mount -o rw,remount /"
	adb exec-out "rm -f /system/bin/su /system/bin/sudemon /system/bin/sudemon.sh"
	#adb exec-out "mount -o ro,remount /"

start:
	adb root
	adb shell sudemon.sh

stop:
	adb root
	adb exec-out "pkill sudemon"
