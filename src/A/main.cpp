#include <bits/stdc++.h>

#include <algorithm>

using namespace std;
void solve() {}
struct X {
  void operator<(const X&) {}
};

namespace foo {
template <class T>
void bar() {
  T{} < T{};
}

}  // namespace foo

int main() {
  int t;
  foo::bar<X>();
  cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
