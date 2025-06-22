#include <bits/stdc++.h>

using namespace std;

void solve() {
  string s;
  cin >> s;
 
  int n = 1000 * (s[0] - '0') + 100 * (s[1] - '0') + 10 * (s[2] - '0') +
          (s[3] - '0');
  
  for (int i = 0; i * i <= 9999; ++i) {
    if (i * i == n) {
      std::cout << 0 << " " << i;
      std::cout << "\n";
      return;
    }
  }
  std::cout << "-1\n";
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}