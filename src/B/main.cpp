#include <bits/stdc++.h>

using namespace std;

void solve() {
  int n;
  cin >> n;
  vector<int> v(n);
  for (int i = 0; i < n; ++i) {
    cin >> v[i];
  }

  std::sort(v.begin(), v.end());
  int last_in_the_bag = 0;
  for (int i = 0; i < n; i += 2) {
    if (v[i] == v[i + 1] || last_in_the_bag + 1 == v[i + 1]) {
      last_in_the_bag = v[i];
    } else {
      std::cout << "No\n";
      return;
    }
  }
  std::cout << "Yes\n";
}
int main() {
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}