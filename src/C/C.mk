bin/C: src/C/main.o
	$(CXX) -o $@ $^ $(LDFLAGS)
