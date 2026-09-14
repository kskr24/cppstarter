// LC 864. Shortest Path to Get All Keys
// State: (r, c, mask_of_keys_collected). Locks ('A'-'F') only block movement
// if the matching key ('a'-'f') isn't yet in mask.
#include <array>
#include <iterator>
#include <optional>
#include <queue>
#include <ranges>
#include <string>
#include <vector>

int shortestPathAllKeys(const std::vector<std::string>& grid) {
  using namespace std::ranges::views;

  const auto         m = std::ssize(grid);
  const auto         n = std::ssize(grid[0]);
  std::optional<int> sr, sc;
  int                cnt = 0;

  auto is_smallercase = [](const char ch) {
    return (static_cast<int>(ch) >= 97 && static_cast<int>(ch) <= 102);
  };
  auto is_uppercase = [](const char ch) {
    return (static_cast<int>(ch) >= 65 && static_cast<int>(ch) <= 70);
  };

  for (const auto& [i, row] : enumerate(grid)) {
    for (const auto& [j, ch] : enumerate(row)) {
      if (ch == '@') {
        sr = static_cast<int>(i);
        sc = static_cast<int>(j);
      } else if (is_smallercase(ch)) {
        cnt++;
      }
    }
  }

  struct State {
    int r;
    int c;
    int mask;
    int steps;
  };

  std::queue<State> q;

  constexpr std::array<int, 4> dr{-1, 1, 0, 0};
  constexpr std::array<int, 4> dc{0, 0, -1, 1};
  int                          totalMask = (1 << cnt) - 1;

  std::vector<std::vector<std::vector<int>>> dp(
      m, std::vector<std::vector<int>>(n, std::vector<int>(1 << cnt, 0)));

  q.push(State{sr.value(), sc.value(), 0, 0});
  dp[*sr][*sc][0] = 0;

  while (!q.empty()) {
    const auto [r, c, mask, steps] = q.front();
    q.pop();

    for (auto k : iota(0, 4)) {
      int nr = r + dr[k];
      int nc = c + dc[k];

      // out of the bounds of the grid
      if (nr < 0 || nr >= m || nc < 0 || nc >= n) {
        continue;
      }

      auto ch = grid[nr][nc];

      // there is a wall
      if (ch == '#') {
        continue;
      }
      // don't have key for this lock
      if (is_uppercase(ch) && !(mask & (1 << (ch - 'A')))) {
        continue;
      }

      int new_mask = mask;

      if (is_smallercase(ch)) {
        new_mask |= (1 << (ch - 'a'));
      }

      if (new_mask == totalMask) {
        return steps + 1;
      }

      if (dp[nr][nc][new_mask] == 1) {
        continue;
      }

      // update the states
      dp[nr][nc][new_mask] = 1;
      q.push(State{nr, nc, new_mask, steps+1});
    }
  }
  return -1;
}

int main() { return 0; }
