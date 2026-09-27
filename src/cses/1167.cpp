#include <bits/stdc++.h>

int main() {
  using namespace std;
  int n, m;

  cin >> n >> m;
  vector<vector<int>> adj(n);
  for ([[maybe_unused]] auto i : std::views::iota(0, m)) {
    int a, b;
    cin >> a >> b;
    a--, b--;

    adj[a].emplace_back(b);
    adj[b].emplace_back(a);
  }

  vector<int> dist(n, 1e9);
  vector<int> p(n, 0);
  dist[0] = 0;
  p[0]    = -1;
  queue<int> q;
  q.push(0);

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    for (auto v : adj[u]) {
      if (dist[v] != 1e9) {
        continue;
      }
      p[v]    = u;
      dist[v] = dist[u] + 1;
      q.push(v);
    }
  }

  if (dist[n - 1] == 1e9) {
    cout << "IMPOSSIBLE\n";
    return 0;
  }

  std::cout << dist[n - 1] + 1 << "\n";
  vector<int> path;
  for (int cur = n - 1; cur != -1;) {
    path.push_back(cur);
    cur = p[cur];
  }

  for (auto v : views::reverse(path)) {
    std::cout << v + 1 << " ";
  }
  std::cout << "\n";
}