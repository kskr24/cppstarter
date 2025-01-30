#include <bits/stdc++.h>

#include <algorithm>

using namespace std;
void solve() {
  int n;
  cin >> n;
  long long x, y, sum = 0, ans = 0;
  cin >> x >> y;
  vector<long long> v(n);

  for (int i = 0; i < n; i++) {
    cin >> v[i];
    sum += v[i];
  }
  std::sort(v.begin(), v.end());

  for (int i = 0; i < n; ++i) {
    long long  low  = sum - v[i] - x;
    long long high = sum - v[i] - y;

    auto left  = std::upper_bound(v.begin() + i + 1, v.end(), low);
    auto right = std::lower_bound(v.begin() + i + 1, v.end(), high);

    if (right <= left) {
      long long diff = left - right;
      //std::cout << v[i] << " " << diff << "\n";
      ans += max(0ll, diff);
    }
  }
  std::cout << ans << "\n";
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
