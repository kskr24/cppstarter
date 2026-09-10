
#include "threadsafe_stack.hpp"

#include <iostream>

int main() {
  ThreadsafeStack<int> stack;
  stack.push(10);
  stack.push(30);
  int n = 0;
  stack.pop(n);

  std::cout << n << "\n";
  return 0;
}
