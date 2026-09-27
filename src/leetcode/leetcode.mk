LEETCODE_SRC := $(wildcard src/leetcode/*.cpp)
LEETCODE_EXE := $(patsubst src/leetcode/%.cpp,bin/leetcode/%,$(LEETCODE_SRC))

$(LEETCODE_EXE): bin/leetcode/%: src/leetcode/%.o | bin/leetcode
	$(CXX) -o $@ $^ $(LDFLAGS)

bin/leetcode:
	mkdir -p $@

EXE += $(LEETCODE_EXE)
