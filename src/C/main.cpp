#include <bits/stdc++.h>

using namespace std;

void solve() {
  int n;
  cin >> n;
  vector<vector<int>> adj(n);
  vector<int>         deg(n, 0);
  for (int i = 1; i < n; ++i) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    adj[a].push_back(b);
    adj[b].push_back(a);
    // update the degrees vector
    deg[a]++;
    deg[b]++;
  }
  int ans           = 0;
  int max_non_child = 0;

  std::function<void(int, int)> dfs = [&](int u, int par) {
    ans           = max(ans, deg[u] + max_non_child - 1);
    int max_child = 0;
    for (auto v : adj[u]) {
      if (v == par) continue;
      if (max_child != 0) {
        ans = max(ans, max_child + deg[v] - 1);
      }
      max_child = max(max_child, deg[v]);
      dfs(v, u);
      ans = max(ans, deg[u] + max_non_child - 1);
    }
    ans = max(ans, deg[u] + max_non_child - 1);
    if (max_child != 0) {
      ans = max(ans, max_child + deg[u] - 2);
    }
    max_non_child = max(max_non_child, max_child);
  };

  dfs(0, -1);
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