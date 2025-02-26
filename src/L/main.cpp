#include <bits/stdc++.h>

using namespace std;
class A {
 public:
  // A(int x, int y) : x_{x}, y_{y} {}
  void print() { std::cout << x_ << " " << y_ << "\n"; }

 private:
  int x_{}, y_{};
};

int main() {
  // A aa{12, 15};
  A aa{};
  aa.print();

  return 0;
}