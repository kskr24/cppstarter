#include <bits/stdc++.h>

int main() {
  using namespace std;
  int n, m;
  cin >> n >> m;

  vector<vector<int>> adj(n);

  for (auto i{0}; i < m; ++i) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    adj[a].push_back(b);
    adj[b].push_back(a);
  }

  vector<int> color(n, 1e9);
  bool        isBipartite = true;

  for (auto start{0}; start < n && isBipartite; ++start) {
    if (color[start] != 1e9) {
      continue;
    }

    queue<int> q;
    q.push(start);
    color[start] = 0;

    while (!q.empty() && isBipartite) {
      auto u = q.front();
      q.pop();

      for (auto& v : adj[u]) {
        if (color[v] == 1e9) {
          color[v] = 1 - color[u];
          q.push(v);
        } else if (color[v] == color[u]) {
          isBipartite = false;
          break;
        }
      }
    }
  }

  if (!isBipartite) {
    std::cout << "IMPOSSIBLE\n";
    return 0;
  }

  for (auto c : color) {
    std::cout << c + 1 << " ";
  }
  std::cout << "\n";
}