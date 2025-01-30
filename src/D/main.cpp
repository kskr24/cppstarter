#include <bits/stdc++.h>

using namespace std;

typedef long long              ll;
typedef vector<int>            vi;
typedef pair<int, int>         pii;
typedef vector<pair<int, int>> vii;

#define pb push_back
#define mp make_pair
#define to(a, n) for (int i = a; i < n; i++)
#define too(a, n) for (int i = a; i <= n; i++)
#define fro(a, n) for (int i = n; i > a; i--)
#define froo(a, n) for (int i = n; i >= a; i--)

void solve() {
  int n;
  cin >> n;
  string s;
  cin >> s;

  vector<vector<int>>            adj(n);
  vector<vector<pair<int, int>>> len(n);
  vector<vector<int>>            go(n, vector<int>(n));
  vector<vector<int>>            dp(n, vector<int>(n));

  for (int i = 1; i < n; i++) {
    int a, b;
    cin >> a >> b;
    adj[--a].push_back(--b);
    adj[b].push_back(a);
  }

  std::function<void(int, int, int, int)> dfs =
      [&](int cur, int src, int p, int dist) {
        go[src][cur] = p;
        len[dist].push_back({src, cur});
        for (int edge_node : adj[cur]) {
          if (edge_node == p) continue;
          dfs(edge_node, src, cur, dist + 1);
        }
        return;
      };

  for (int i = 0; i < n; i++) {
    dfs(i, i, i, 0);
  }

  for (int i = 0; i < n; i++) {
    for (auto p : len[i]) {
      int u = p.first;
      int v = p.second;
      if (i == 0)
        dp[u][v] = 1;
      else if (i == 1)
        dp[u][v] = 1 + (s[u] == s[v]);
      else {
        int x = dp[u][go[u][v]];
        int y = dp[v][go[v][u]];
        int z = dp[go[v][u]][go[u][v]] + 2 * (s[u] == s[v]);

        dp[u][v] = max({x, y, z});
      }
    }
  }

  int ans = INT_MIN;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      ans = max(ans, dp[i][j]);
    }
  }
  cout << ans << "\n";

  return;
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}