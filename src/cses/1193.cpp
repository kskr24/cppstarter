#include <bits/stdc++.h>

constexpr std::array<int, 4>  kr{-1, 1, 0, 0};
constexpr std::array<int, 4>  kc{0, 0, -1, 1};
constexpr std::array<char, 4> dir{'U', 'D', 'L', 'R'};

using namespace std;
int main() {
  int n, m;
  int sr, sc, dr, dc;
  cin >> n >> m;
  vector<string> grid(n);
  for (auto& row : grid) {
    cin >> row;
  }

  for (auto i{0}; i < n; ++i) {
    for (auto j{0}; j < m; ++j) {
      if (grid[i][j] == 'A') {
        sr = i;
        sc = j;
        break;
      }
    }
  }

  std::queue<pair<int, int>> q;
  bool                       found = false;
  vector<vector<int>>        visited(n, vector<int>(m, 0));
  vector<vector<int>>        par(n, vector<int>(m));

  q.emplace(sr, sc);
  visited[sr][sc] = 1;

  while (!q.empty()) {
    auto [r, c] = q.front();
    q.pop();

    if (grid[r][c] == 'B') {
      found = true;
      dr    = r;
      dc    = c;
      break;
    }

    for (auto k{0}; k < 4; ++k) {
      int nr = r + kr[k];
      int nc = c + kc[k];

      if (nr < 0 || nr >= n || nc < 0 || nc >= m) {
        continue;
      }
      if (grid[nr][nc] == '#' || visited[nr][nc]) {
        continue;
      }

      visited[nr][nc] = 1;
      par[nr][nc]     = k;
      q.emplace(nr, nc);
    }
  }

  if (!found) {
    std::cout << "NO\n";
    return 0;
  }

  string path;
  for (int r = dr, c = dc; r != sr || c != sc;) {
    int k = par[r][c];
    path.push_back(dir[k]);
    r -= kr[k];
    c -= kc[k];
  }
  reverse(path.begin(), path.end());

  std::cout << "YES\n" << path.size() << '\n' << path << '\n';
}