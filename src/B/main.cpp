#include <bits/stdc++.h>

using namespace std;

void solve() {
  int n, k;
  cin >> n >> k;

  std::string s;
  cin >> s;
  int zero = 0, one = 0;

  for (int i = 0; i < n / 2; ++i) {
    if (s[i] == s[n - i - 1]) {
      zero += (s[i] == '0');
      one += (s[i] == '1');
    }
  }
  int good = zero + one;

  int bad_pairs         = n / 2 - good;
  int good_pairs_needed = k - good;

  if (good_pairs_needed >= 0) {
    if (good_pairs_needed % 2 == 0 && good_pairs_needed <= bad_pairs) {
      std::cout << "YES\n";
    } else {
      std::cout << "NO\n";
    }
  } else {
    int rem = (good - k);
    if (rem % 2 == 0 && (zero >= rem/2 && one >= rem/2)) {
      std::cout << "YES\n";
    } else {
      std::cout << "NO\n";
    }
  }
}
int main() {
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}