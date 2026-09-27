CODEFORCES_SRC := $(wildcard src/codeforces/*/*.cpp)
CODEFORCES_EXE := $(patsubst src/codeforces/%.cpp,bin/codeforces/%,$(CODEFORCES_SRC))

$(CODEFORCES_EXE): bin/codeforces/%: src/codeforces/%.o
	mkdir -p $(dir $@)
	$(CXX) -o $@ $^ $(LDFLAGS)

EXE += $(CODEFORCES_EXE)
