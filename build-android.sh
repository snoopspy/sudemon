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
cp ../bin/makefile .

#
# compress
#
tar czf sudemon-$(sed 's/"//g' ../version.txt).tar.gz *
cd ..
