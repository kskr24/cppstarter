#include <bits/stdc++.h>

using namespace std;

template <typename T> T& smax(T& a, T& b) {
  a = max(a, b);
  return a;
}

template <typename T> T& smin(T& a, T& b) {
  a = max(a, b);
  return a;
}

void solve() {
  int n;
  cin >> n;
  vector<int> v(n);

  for (int i = 0; i < n; ++i) {
    cin >> v[i];
  }

  vector<int> sizes;

  int cnt = 1;
  for (int i = 1; i < n; ++i) {
    if (v[i] <= v[i - 1] + 1) {
      if (v[i] == v[i - 1])
        continue;
      cnt++;
    } else {
      sizes.push_back(cnt);
      cnt = 1;
    }
  }
  sizes.push_back(cnt);

  long long ans = 0;

  for (auto& x : sizes) {
    ans += (x + 1) / 2;
  }
  std::cout << ans << "\n";
}

int main() {
  // ios_base::sync_with_stdio(false);
  // cin.tie(NULL);
  // freopen("txt.in", "r", stdin);
  // freopen("txt.out", "w", stdout);
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
}