# .SUFFIXES: .C .o
# CXX := g++
ifeq ($(ARCH), arm)
	# this is for cross compile
	CXX := arm-linux-gnueabihf-g++
	STRIP := arm-linux-gnueabihf-strip
else ifeq ($(ARCH), arm64)
	# this is for cross compile
	CXX := aarch64-linux-gnu-g++
	STRIP := aarch64-linux-gnu-strip
else
	CXX := clang++
	STRIP := strip
endif

CXXFLAGS := -std=c++11  -Wno-deprecated-declarations 
INCLUDES := -I include
# CC       = gcc
# CFLAGS   = -Wall
# LDFLAGS  = 
TARGET   = ccglue

ifndef MODE
	# $(warning MODE not defined)
	MODE = release
endif

$(info Compile Mode: $(MODE))

ifeq ($(MODE), release)
	CXXFLAGS += -O3 -Wl,--gc-sections
else
	CXXFLAGS += -g
endif
# compile tool as staticly
CXXFLAGS += -static


# OJBFILEES = OBJECTS
SOURCEDIR = ./src
SRCFILE = $(wildcard $(SOURCEDIR)/*.cpp)

all: $(TARGET)

$(TARGET): $(SRCFILE)
	$(CXX) $(INCLUDES) $(CXXFLAGS)  -o $(TARGET) $(SRCFILE)
ifeq ($(MODE), release)
	$(STRIP) --strip-unneeded $(TARGET)
endif

clean:
	rm -f $(TARGET)
