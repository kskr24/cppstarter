#include <bits/stdc++.h>

using namespace std;

void solve() {
  int n, m;

  cin >> n >> m;

  vector<long long> a(n), b(m);

  for (int i = 0; i < n; ++i) {
    cin >> a[i];
  }
  for (int j = 0; j < m; ++j) {
    cin >> b[j];
  }

  // prefix sum over a
  vector<long long> pref(n + 1);
  for (int i = 0; i < n; ++i) {
    pref[i + 1] = pref[i] + a[i];
  }

  vector<vector<int>> dp(n + 1, vector<int>(m, 1e9));
  // dp[i][j] = minimum cost to when we have removed i elements
  // and value of k reached is j

  dp[0][0] = 0;
  
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  // freopen("txt.in", "r", stdin);
  // freopen("txt.out", "w", stdout);
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
}
