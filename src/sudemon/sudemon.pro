TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
DESTDIR = $${PWD}/../../bin
INCLUDEPATH += $${PWD}/..

SOURCES += \
	*.cpp \
	$${PWD}/../gaux.cpp \
	$${PWD}/../gsignal.cpp \
	$${PWD}/../gtrace.cpp

HEADERS += \
	*.h \
	$${PWD}/../gaux.h \
	$${PWD}/../gsignal.h \
	$${PWD}/../gtrace.h
