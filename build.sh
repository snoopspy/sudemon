make

#
# release
#
mkdir -p release
cd release

#
# linux
#
mkdir -p linux
cd linux

#
# linux bin files 
#
cp ../../bin/su .
cp ../../bin/sudemon .
cp ../../bin/sudemon.sh .
cp ../../bin/android-install .
cp ../../bin/android-start .
cp ../../bin/android-stop .
cp ../../bin/android-uninstall .

#
# linux compress
#
tar czf sudemon-linux-$(sed 's/"//g' ../../version.txt).tar.gz *
cd ..

#
# win
#
mkdir -p win
cd win

#
# win bin files 
#
cp ../../bin/su .
cp ../../bin/sudemon .
cp ../../bin/sudemon.sh .
cp ../../bin/android-install.bat .
cp ../../bin/android-start.bat .
cp ../../bin/android-stop.bat .
cp ../../bin/android-uninstall.bat .

#
# win compress
#
zip sudemon-win-$(sed 's/"//g' ../../version.txt).zip *
cd ..