#include <bits/stdc++.h>

using namespace std;

class Solution {
 public:
  void dfs(const vector<vector<pair<int, int>>>& G,
           int                                   u,
           int                                   par,
           vector<vector<long long>>&            dp,
           int                                   k) {
    vector<long long> deltas;
    for (auto [v, x] : G[u]) {
      if (v == par) continue;
      dfs(G, v, u, dp, k);

      dp[u][0] += dp[v][1];
      dp[u][1] += dp[v][1];

      deltas.push_back(max(0ll, dp[v][0] + x - dp[v][1]));
    }

    std::sort(deltas.begin(), deltas.end(), greater<long long>());

    for (int i = 0; i < k && i < (int)deltas.size(); ++i) {
      dp[u][1] += deltas[i];
      dp[u][0] += deltas[i];

      if (i == k - 1) {
        dp[u][0] -= deltas[i];
      }
    }
    return;
  }

  long long maximizeSumOfWeights(vector<vector<int>>& edges, int k) {
    int                            n = edges.size() + 1;
    vector<vector<pair<int, int>>> G(n);
    vector<vector<long long>>      dp(n, vector<long long>(2, 0));
    for (auto& x : edges) {
      G[x[0]].push_back({x[1], x[2]});
      G[x[1]].push_back({x[0], x[2]});
    }
    dfs(G, 0, -1, dp, k);
    return dp[0][1];
  }
};

int main() {
  vector<vector<int>> edges{{0, 1, 29}, {1, 2, 13}, {0, 3, 41}};
  int                 k = 1;
  Solution            s;
  std::cout << s.maximizeSumOfWeights(edges, k) << "\n";
}