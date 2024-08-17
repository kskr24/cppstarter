#include <bits/stdc++.h>

using namespace std;

vector<int> dp(300001);

void solve() {
  int a, b;
  cin >> a >> b;
  //
  int needed = b ^ dp[a - 1];

  if (needed == 0) {
    std::cout << a << "\n";
  } else if (needed == a) {
    std::cout << a + 2 << "\n";
  } else {
    std::cout << a + 1 << "\n";
  }
}

int main() {
  int t;
  cin >> t;
  dp[0] = 0;
  for (int i = 1; i < 300001; i++) {
    dp[i] = dp[i - 1] ^ i;
  }

  while (t--) {
    solve();
  }
  return 0;
}
