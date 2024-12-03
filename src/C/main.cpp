#include <bits/stdc++.h>

using namespace std;

void solve() {
  int n;
  cin >> n;
  vector<vector<char>> v(2, vector<char>(n + 1));

  vector<vector<int>> dp(n + 1, vector<int>(3, -1e9));
  char c;
  for (int i = 0; i < 2; ++i) {
    for (int j = 1; j <= n; ++j) {
      cin >> c;
      v[i][j] = (c == 'A');
    }
  }

  auto win = [&](int x) -> int { return x >= 2; };
  //
  auto straight1 = [&](int x) -> int {
    return win(v[0][x] + v[0][x - 1] + v[0][x - 2]);
  };
  auto straight2 = [&](int x) -> int {
    return win(v[1][x] + v[1][x - 1] + v[1][x - 2]);
  };
  auto curved1 = [&](int x) -> int {
    return win(v[0][x] + v[0][x - 1] + v[1][x - 1]);
  };
  auto curved2 = [&](int x) -> int {
    return win(v[1][x] + v[0][x - 1] + v[1][x - 1]);
  };

  // base case
  dp[0][0] = 0;
  dp[2][1] = curved1(2);
  dp[2][2] = curved2(2);

  for (int i = 3; i <= n; ++i) {
    // ith row ends in a bar
    dp[i][0] = max(dp[i][0], dp[i - 3][0] + straight1(i) + straight2(i));
    //   X
    // X X
    dp[i][0] =
        max(dp[i][0], win(v[0][i] + v[1][i] + v[1][i - 1]) + dp[i - 1][1]);
    // X X
    //   X
    dp[i][0] =
        max(dp[i][0], win(v[0][i] + v[1][i] + v[0][i - 1]) + dp[i - 1][2]);

    // either pick
    // X X
    // X
    dp[i][1] = max(dp[i][1], dp[i - 2][0] + curved1(i));

    // or
    //   X X X
    // X X X
    
      dp[i][1] = max(dp[i][1], dp[i - 3][1] + straight1(i) + straight2(i - 1));

    // either pick
    // X
    // X X
    dp[i][2] = max(dp[i][2], dp[i - 2][0] + curved2(i));

    // or
    // X X X
    //   X X X
    
      dp[i][2] = max(dp[i][2], dp[i - 3][2] + straight2(i) + straight1(i - 1));
  }

  std::cout << dp[n][0] << "\n";
}

int main() {
  // ios_base::sync_with_stdio(false);
  // cin.tie(NULL);
  // freopen("txt.in", "r", stdin);
  // freopen("txt.out", "w", stdout);
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
}
