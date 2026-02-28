TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
DESTDIR = $${PWD}/../../bin
INCLUDEPATH += $${PWD}/..

SOURCES += \
	*.cpp \
	$${PWD}/../gaux.cpp \
	$${PWD}/../gsignal.cpp

HEADERS += \
	*.h \
	$${PWD}/../gaux.h \
	$${PWD}/../gsignal.h
