TARGET=../../bin/sudemon
SRCS=$(wildcard *.cpp) ../gaux.cpp ../gsignal.cpp ../gtrace.cpp
OBJS=$(SRCS:%.cpp=%.o)

CPPFLAGS+=-g -I..
CPPFLAGS+=-O2 # for release
CPPFLAGS+=-Wall -Wextra
ifdef ANDROID_NDK_ROOT
LDFLAGS+=-static-libstdc++
endif
LDLIBS+=-lpcap -pthread

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@
ifdef ANDROID_NDK_ROOT
	termux-elf-cleaner --api-level 23 $(TARGET)
endif	

clean:
	rm -f $(TARGET) $(OBJS)

test:
	@echo $(SRCS)
	@echo $(OBJS)

