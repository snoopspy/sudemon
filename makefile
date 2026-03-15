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
	adb push bin/su /system/bin/su2
	adb exec-out "mv /system/bin/su2 /system/bin/su"
	adb push bin/sudemon /system/bin/sudemon
	adb exec-out "chmod 755 /system/bin/su'"
	adb exec-out "chmod 755 /system/bin/sudemon"
	#adb exec-out "mount -o ro,remount /"

uninstall:
	adb root
	adb exec-out "mount -o rw,remount /"
	adb exec-out "rm -f /system/bin/su /system/bin/sudemon"
	#adb exec-out "mount -o ro,remount /"
