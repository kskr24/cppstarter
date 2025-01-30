GCC_DIR := /usr/local
CXX := $(GCC_DIR)/bin/g++
CPP := $(GCC_DIR)/bin/cpp

.SUFFIXES:

SRC := $(shell /usr/bin/find -L . -path ./.git -prune -o -name '*.cpp' -print)
HDR := $(shell /usr/bin/find -L . -path ./.git -prune -o -name '*.hpp' -print)

# Initialize common variables
CPPFLAGS :=
CXXFLAGS := -std=c++23
#LDFLAGS := -Wl,-rpath
LDLIBS :=
CONFIG += c++23
CPPFLAGS = -Wall -Wextra -g 

CODE_DIR := ./
CPPFLAGS += -I $(CODE_DIR)
CPPFLAGS += -I ../include
CPPFLAGS += -I ../../include

LDLIBS += -lboost
LDLIBS += -lgtest

CXXFLAGS += -Wno-attributes -O2 -fPIC


MY_COMPILE.cpp = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH)
MY_PREPROCESS.cpp = $(CPP) $(CPPFLAGS)
MY_OUTPUT_OPTION = -o $@

%.o: %.cpp
	$(MY_COMPILE.cpp) -c $(MY_OUTPUT_OPTION) $<

EXE := 
SRC_SRC := $(shell /usr/bin/find -L src -maxdepth 1 -name '*.cpp' ! -name "main.cpp" ! -name "test.cpp" -print)
SRC_SRC += $(shell /usr/bin/find -L src -maxdepth 2 -type d -name test -prune -o -name 'main.cpp' -print)

EXE += $(patsubst %/main, %, $(patsubst %.cpp, %, $(patsubst src/%, bin/%, $(SRC_SRC))))
# Also include any files with a .mk extension
SUB_MAKES += $(shell /usr/bin/find -L . -name '*.mk' -print)

# build_type was already included
SUB_MAKES := $(filter-out %build_type.mk , $(SUB_MAKES))


ifneq ($(SUB_MAKES),)
    include $(SUB_MAKES)
endif

all : exe

clean: temp_clean
	rm -f bin/*

temp_clean:
	rm -f src/*/main.o

exe: $(EXE) temp_clean
