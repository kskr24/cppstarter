#include <bits/stdc++.h>

using namespace std;

template <typename T>
T& smax(T& a, T& b) {
  a = max(a, b);
  return a;
}

template <typename T>
T& smin(T& a, T& b) {
  a = max(a, b);
  return a;
}

void solve() {
  int n;
  std::cin >> n;
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
  int ans = 0;

  std::function<int(int, int)> dfs = [&](int u, int par) {
    vector<int> nodes;
    int         max_child = 0;
    for (auto v : adj[u]) {
      if (v == par) continue;
      smax(max_child, deg[v]);
      int val = dfs(v, u);
      ans     = max(ans, val + deg[u] - 1);
      nodes.push_back(max(val, deg[v]));
    }
    sort(nodes.begin(), nodes.end(), greater<int>());
    if (max_child != 0) {
      ans = max(ans, max_child + deg[u] - 2);
    }
    if (nodes.size() > 1) {
      ans = max(ans, nodes[0] + nodes[1] - 1);
    }
    int rv;

    if (nodes.empty()) {
      rv = max_child;
    } else {
      rv = max(max_child, nodes[0]);
    }

    // std::cout << "Node: " << u << " "
    //           << "return_value: " << rv << "\n";
    return rv;
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