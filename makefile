.PHONY : su sudemon clean install uninstall

all: su sudemon

su:
	cd src/su; make; cd ../..
ifdef ANDROID_NDK_ROOT
	llvm-strip bin/su
else
	strip bin/su
endif	

sudemon:
	cd src/sudemon; make; cd ../..
ifdef ANDROID_NDK_ROOT
	llvm-strip bin/sudemon
else
	strip bin/sudemon
endif	

clean:
	cd src/su; make clean; cd ../..
	cd src/sudemon; make clean; cd ../..

install:
	adb root
	adb exec-out "mount -o rw,remount /system"
	adb push bin/su /system/bin/su2
	adb exec-out "mv /system/bin/su2 /system/bin/su"
	adb push bin/sudemon /system/bin/sudemon
	adb push bin/sudemon.sh /system/bin/sudemon.sh
	adb exec-out "chmod 755 /system/bin/su'"
	adb exec-out "chmod 755 /system/bin/sudemon"
	adb exec-out "chmod 755 /system/bin/sudemon.sh"
	#adb exec-out "mount -o ro,remount /system"

uninstall:
	adb root
	adb exec-out "pkill sudemon"
	adb exec-out "mount -o rw,remount /system"
	adb exec-out "rm -f /system/bin/su /system/bin/sudemon /system/bin/sudemon.sh"
	#adb exec-out "mount -o ro,remount /system"
