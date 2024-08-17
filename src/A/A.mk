bin/A: src/A/main.o
	$(CXX) -o $@ $^ $(LDFLAGS)
