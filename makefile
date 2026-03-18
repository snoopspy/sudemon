.PHONY : su sudemon clean

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
