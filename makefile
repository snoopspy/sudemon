.PHONY : su sudemon clean install uninstall

all: su sudemon

su:
	cd src/su; make; cd ../..
	llvm-strip bin/su

sudemon:
	cd src/sudemon; make; cd ../..
	llvm-strip bin/sudemon

clean:
	cd src/su; make clean; cd ../..
	cd src/sudemon; make clean; cd ../..

install:
	adb root
	adb exec-out "mount -o rw,remount /"
	adb push bin/su /system/xbin/su2
	adb exec-out "mv /system/xbin/su2 /system/xbin/su"
	adb push bin/sudemon /system/xbin/sudemon
	adb exec-out "chmod 755 /system/xbin/su'"
	adb exec-out "chmod 755 /system/xbin/sudemon"
	adb exec-out "mount -o ro,remount /"

uninstall:
	adb root
	adb exec-out "mount -o rw,remount /"
	adb exec-out "rm -f /system/xbin/su /system/xbin/sudemon"
	adb exec-out "mount -o ro,remount /"
