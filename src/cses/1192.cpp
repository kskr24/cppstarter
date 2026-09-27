// CSES1192
#include <bits/stdc++.h>

void dfs(std::vector<std::vector<int>>&  visited,
         const std::vector<std::string>& grid,
         int                             r,
         int                             c,
         int                             n,
         int                             m);
int  main() {
  int n, m;
  std::cin >> n >> m;

  std::vector<std::string> grid(n);
  for (auto& s : grid) {
    std::cin >> s;
  }
  std::vector<std::vector<int>> visited(n, std::vector<int>(m, 0));
  int                           total = 0;
  for (const auto& [i, row] : std::ranges::views::enumerate(grid)) {
    for (const auto& [j, ch] : std::ranges::views::enumerate(row)) {
      if (visited[i][j] || ch == '#') {
        continue;
      }
      dfs(visited, grid, i, j, n, m);
      total++;
    }
  }
  std::cout << total << "\n";
}

constexpr std::array<int, 4> dr{-1, 1, 0, 0};
constexpr std::array<int, 4> dc{0, 0, -1, 1};

void dfs(std::vector<std::vector<int>>&  visited,
         const std::vector<std::string>& grid,
         int                             r,
         int                             c,
         int                             n,
         int                             m) {
  visited[r][c] = 1;
  for (int k = 0; k < 4; ++k) {
    int nr = r + dr[k];
    int nc = c + dc[k];

    if (nr < 0 || nr >= n || nc < 0 || nc >= m || visited[nr][nc] ||
        grid[nr][nc] == '#') {
      continue;
    }
    dfs(visited, grid, nr, nc, n, m);
  }
}
