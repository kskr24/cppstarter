// CSES1687 Companies Queries I

#include <bits/stdc++.h>
using namespace std;

int main() {
  constexpr int LOGMAX = 20;
  int           n, Q;
  std::cin >> n >> Q;

  std::vector<std::vector<int>> far(LOGMAX + 1, std::vector<int>(n + 1, -1));

  for (int i = 2; i <= n; ++i) {
    cin >> far[0][i];
  }

  for (int h = 1; h <= LOGMAX; ++h) {
    for (int i = 1; i <= n; ++i) {
      far[h][i] = far[h - 1][i] == -1 ? -1 : far[h - 1][far[h - 1][i]];
    }
  }

  int x, k;
  for (int q = 0; q < Q; ++q) {
    cin >> x >> k;

    for (int l = LOGMAX; l >= 0; --l) {
      if (k & (1 << l)) {
        x = far[l][x];
        if (x == -1) {
          break;
        }
      }
    }
    std::cout << x << "\n";
  }
}
