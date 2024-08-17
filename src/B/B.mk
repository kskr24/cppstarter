bin/B: src/B/main.o
	$(CXX) -o $@ $^ $(LDFLAGS)
