bin/hierarchical_mutex: src/hierarchical_mutex/main.o
	$(CXX) -o $@ $^ $(LDFLAGS)
