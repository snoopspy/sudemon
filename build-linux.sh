make

#
# release
#
mkdir -p release
cd release

#
# bin files 
#
cp ../bin/su .
cp ../bin/sudemon .
cp ../bin/sudemon.sh .
cp ../bin/android-install .
cp ../bin/android-start .
cp ../bin/android-stop .
cp ../bin/android-uninstall .

#
# compress
#
tar czf sudemon-$(sed 's/"//g' ../version.txt).tar.gz *
cd ..
