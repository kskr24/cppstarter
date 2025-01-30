bin/L: src/L/main.o
	$(CXX) -o $@ $^ $(LDFLAGS)
