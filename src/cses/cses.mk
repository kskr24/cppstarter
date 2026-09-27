CSES_SRC := $(wildcard src/cses/*.cpp)
CSES_EXE := $(patsubst src/cses/%.cpp,bin/cses/%,$(CSES_SRC))

$(CSES_EXE): bin/cses/%: src/cses/%.o | bin/cses
	$(CXX) -o $@ $^ $(LDFLAGS)

bin/cses:
	mkdir -p $@

EXE += $(CSES_EXE)
