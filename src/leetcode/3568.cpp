#include <array>
#include <optional>
#include <queue>
#include <ranges>
#include <string>
#include <vector>

struct State {
  int r;
  int c;
  int energy;
  int mask;
  int steps;
};

int minMoves(const std::vector<std::string>& classroom, int energy) {
  const auto         m = std::ssize(classroom);
  const auto         n = std::ssize(classroom[0]);
  std::queue<State>  q;
  std::optional<int> sr, sc;

  std::vector<std::vector<int>> id(m, std::vector<int>(n, -1));
  int                           counter = 0;

  for (const auto& [i, row] : std::views::enumerate(classroom)) {
    for (const auto& [j, ch] : std::views::enumerate(row)) {
      if (ch == 'S') {
        sr = static_cast<int>(i);
        sc = static_cast<int>(j);
      } else if (ch == 'L') {
        id[i][j] = counter++;
      }
    }
  }
  if (counter == 0) {
    return 0;
  }

  const int                    totalMask = (1 << counter) - 1;
  constexpr std::array<int, 4> dr{-1, 1, 0, 0};
  constexpr std::array<int, 4> dc{0, 0, -1, 1};

  std::vector<std::vector<std::vector<int>>> dp(
      m, std::vector<std::vector<int>>(n, std::vector<int>(1 << counter, -1)));

  q.push(State{*sr, *sc, energy, 0, 0});
  dp[*sr][*sc][0] = energy;

  while (!q.empty()) {
    const auto [r, c, e, mask, steps] = q.front();
    q.pop();

    for (int k = 0; k < 4; ++k) {
      const int nr = r + dr[k];
      const int nc = c + dc[k];

      if (nr < 0 || nr >= m || nc < 0 || nc >= n || classroom[nr][nc] == 'X') {
        continue;
      }

      int ne = e - 1;
      if (ne < 0) {
        continue;
      }

      if (classroom[nr][nc] == 'R') {
        ne = energy;
      }

      int nmask = mask;
      if (classroom[nr][nc] == 'L' && id[nr][nc] != -1) {
        nmask |= (1 << id[nr][nc]);
      }
      if (nmask == totalMask) {
        return steps + 1;
      }

      if (ne <= dp[nr][nc][nmask]) {
        continue;
      }

      dp[nr][nc][nmask] = ne;
      q.push(State{nr, nc, ne, nmask, steps + 1});
    }
  }

  return -1;
}

int main() { return 0; }