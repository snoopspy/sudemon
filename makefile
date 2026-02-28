.PHONY : su sudemon clean install uninstall

all: su sudemon

su:
	cd src/su; make; cd ../..

sudemon:
	cd src/sudemon; make; cd ../..

clean:
	cd src/su; make clean; cd ../..
	cd src/sudemon; make clean; cd ../..

install:
	adb root
	adb exec-out "mount -o rw,remount /"
	adb push bin/su bin/sudemon /system/xbin
	adb exec-out "chmod 755 /system/xbin/su'"
	adb exec-out "chmod 755 /system/xbin/sudemon"
	adb exec-out "mount -o ro,remount /"

uninstall:
	adb root
	adb exec-out "mount -o rw,remount /"
	adb exec-out "rm /system/xbin/su /system/xbin/sudemon"
	adb exec-out "mount -o ro,remount /system"
	adb exec-out "mount -o ro,remount /"
