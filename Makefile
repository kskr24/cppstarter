GCC_DIR := /usr/local
CXX := $(GCC_DIR)/bin/g++
CPP := $(GCC_DIR)/bin/cpp

.SUFFIXES:

# Included sub-.mk files define concrete targets like `bin/A`; without this,
# GNU make would pick the first such target as the default goal instead of `all`.
.DEFAULT_GOAL := all

SRC := $(shell /usr/bin/find -L . -path ./.git -prune -o -name '*.cpp' -print)
HDR := $(shell /usr/bin/find -L . -path ./.git -prune -o -name '*.hpp' -print)

# Initialize common variables
CPPFLAGS :=
CXXFLAGS := -std=c++23
#LDFLAGS := -Wl,-rpath
LDLIBS :=
CONFIG += c++23
# -Wthread-safety is a Clang-only diagnostic; GCC rejects it.
WARNINGS := -Wall -Wextra -Wpedantic -Wshadow -Wconversion
CPPFLAGS = $(WARNINGS) -g

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

# --- Tests ----------------------------------------------------------------
# Any src/<name>/test.cpp becomes bin/<name>_test, built + run via `make test`.
GTEST_LDLIBS := -lgtest -lgtest_main -pthread
TEST_SRC     := $(shell /usr/bin/find -L src -maxdepth 2 -name 'test.cpp' -print)
TEST_EXE     := $(patsubst src/%/test.cpp,bin/%_test,$(TEST_SRC))
TEST_NAMES   := $(patsubst bin/%_test,%,$(TEST_EXE))

bin/%_test: src/%/test.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(GTEST_LDLIBS)
# --------------------------------------------------------------------------

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
	rm -f src/*/main.o src/*/test.o

exe: $(EXE) temp_clean

.PHONY: tests test check $(addprefix test-,$(TEST_NAMES))

tests: $(TEST_EXE)

# Build + run every discovered test binary. Non-zero exit if any suite fails.
test check: $(TEST_EXE)
	@fail=0; total=0; \
	for t in $(TEST_EXE); do \
	  total=$$((total+1)); \
	  printf '\n=== %s ===\n' $$t; \
	  if ! $$t; then fail=$$((fail+1)); fi; \
	done; \
	echo; \
	echo "Test binaries: $$total, failed: $$fail"; \
	[ $$fail -eq 0 ]

# Convenience: `make test-concurrency` builds + runs just that suite.
$(addprefix test-,$(TEST_NAMES)): test-%: bin/%_test
	./$<
