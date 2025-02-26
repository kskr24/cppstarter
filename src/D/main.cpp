#include <bits/stdc++.h>

using namespace std;

void solve() {
  int n, s1, s2;
  cin >> n >> s1 >> s2;
  vector<vector<int>> G1(n);
  vector<vector<int>> G2(n);

  int m1;
  cin >> m1;
  int a, b;
  for (int i = 0; i < m1; ++i) {
    cin >> a >> b;
    a--, b--;
    G1[a].push_back(b);
    G2[b].push_back(a);
  }

  int m2;
  cin >> m2;
  for (int i = 0; i < m2; ++i) {
    cin >> a >> b;
    a--, b--;
    G1[a].push_back(b);
    G2[b].push_back(a);
  }

  int ans = 0;
  // Dijkstra
  vector<vector<int>> d(n, vector<int>(n, 1e9));
  d[s1][s2] = 0;

  std::set<std::pair<int, std::pair<int, int>>> st;
  st.insert(std::make_pair(0, {s1, s2}));

  while (!st.empty()) {
    auto [u1, u2] = st.begin()->second;
    st.erase(st.begin());

    for (auto v1 : G1[u1]) {
      for (auto v2 : G2[u2]) {
        int wt = abs(u1 - u2);
        if (d[v1][v2] < d[u1][u2] + wt) {
          st.erase({d[v1][v2], {v1, v2}});
          d[v1][v2] = d[u1][u2] + wt;
          st.insert({d[v1][v2], {v1, v2}});
        }
      }
    }

    // relax all the edges
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