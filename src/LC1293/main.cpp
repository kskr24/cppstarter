#include <bits/stdc++.h>
#include <array>
#include <iterator>
#include <queue>
#include <ranges>
#include <vector>

using namespace std;

// LC 1293. Shortest Path in a Grid with Obstacles Elimination
// State: (r, c, k_remaining) where k_remaining = obstacles you may still
// remove. Prune with best_k_seen[r][c]: skip a state if it doesn't beat the
// best remaining-eliminations already recorded at that cell.

int shortestPath(vector<vector<int>>& grid, int k) {
  const auto m = std::ssize(grid);
  const auto n = std::ssize(grid[0]);
  struct State {
    int r;
    int c;
    int k_remaining;
    int steps;
  };

  std::queue<State>            q;
  vector<vector<int>>          dp(m, vector<int>(n, -1));
  constexpr std::array<int, 4> dr{0, 0, -1, 1};
  constexpr std::array<int, 4> dc{-1, 1, 0, 0};

  q.push(State{0, 0, k, 0});
  dp[0][0] = k;

  while (!q.empty()) {
    const auto [r, c, k_remaining, steps] = q.front();
    q.pop();

    if (r == m - 1 && c == n - 1) {
      return steps;
    }

    for (auto i : std::ranges::views::iota(0, 4)) {
      int nr = r + dr[i];
      int nc = c + dc[i];

      if (nr < 0 || nr >= m || nc < 0 || nc >= n) {
        continue;
      }

      int new_k_remaining = k_remaining - 1;

      if (grid[nr][nc] == 1) {
        if (new_k_remaining < 0) {
          continue;
        }
        if (dp[nr][nc] < new_k_remaining) {
          dp[nr][nc] = new_k_remaining;
          q.push(State{nr, nc, new_k_remaining, steps + 1});
        }
      } else {
        if (dp[nr][nc] < k_remaining) {
          dp[nr][nc] = k_remaining;
          q.push(State{nr, nc, k_remaining, steps + 1});
        }
      }
    }
  }
  return -1;
}

int main() { return 0; }
