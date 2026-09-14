#include <bits/stdc++.h>

using namespace std;

void solve() {
  int n;
  string       s;
  cin >> n >> s;
  vector<int> f(26, 0);
  vector<int> l(n, 0);
  vector<int> r(n, 0);

  l[0]          = 1;
  f[s[0] - 'a'] = 1;
  for (auto i{1}; i < n; ++i) {
    l[i] = l[i - 1] + (f[s[i] - 'a'] == 0);
    f[s[i] - 'a']++;
  }
  f.assign(26, 0);
  r[n - 1]          = 1;
  f[s[n - 1] - 'a'] = 1;
  for (auto i{n - 2}; i >= 0; --i) {
    r[i] = r[i + 1] + (f[s[i] - 'a'] == 0);
    f[s[i] - 'a']++;
  }

  int sum = 0;
  for (auto i{0}; i < n - 1; ++i) {
    sum = max(sum, l[i] + r[i + 1]);
  }

  std::cout << sum << "\n";
}

int main() {
  int t;
  cin >> t;

  while (t--) {
    solve();
  }
}