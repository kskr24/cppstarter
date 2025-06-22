#include <bits/stdc++.h>
#include <array>
#include <climits>
#include <iterator>

using namespace std;

void solve() {
  long long n;
  cin >> n;
  std::map<long long, long long> row, col;
  for (int i = 0; i < n; ++i) {
    long long x, y;
    cin >> x >> y;
    row[x]++;
    col[y]++;
  }

  auto top    = row.begin();
  auto bottom = row.rbegin();

  auto left  = col.begin();
  auto right = col.rbegin();

  // reduce from the top
  long long ans = LONG_LONG_MAX;
  if (row.size() > 1) {
    auto      nxt        = std::next(top);
    long long new_height = bottom->first - nxt->first + 1;
    long long new_width  = right->first - left->first + 1;
    if (new_height * new_width >= n) {
      ans = min(ans, new_height * new_width);
    }

    auto prv   = std::prev(bottom);
    new_height = prv->first - top->first + 1;
    new_width  = right->first - left->first;

    if (new_height * new_width >= n) {
      ans = min(ans, new_height * new_width);
    }
  }

  if (col.size() > 1) {
    auto      nxt        = std::next(left);
    long long new_width  = right->first - nxt->first + 1;
    long long new_height = bottom->first - top->first + 1;
    if (new_height * new_width >= n) {
      ans = min(ans, new_height * new_width);
    }

    auto prv   = std::prev(right);
    new_width  = prv->first - left->first + 1;
    new_height = bottom->first - top->first + 1;
    if (new_height * new_width >= n) {
      ans = min(ans, new_height * new_width);
    }
  }

  std::cout << ans << "\n";
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}