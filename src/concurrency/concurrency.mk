bin/concurrency: src/concurrency/main.o
	$(CXX) -o $@ $^ $(LDFLAGS)
