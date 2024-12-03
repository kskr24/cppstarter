#include <bits/stdc++.h>
using namespace std;

void solve() {
  vector<int> v{1, 4, 10, 12, 13};

  std::sort(v.begin(), v.end());

  int key;
  cin >> key;

  int ans = -1;
  int l = 0, r = 4;
  while (l <= r) {
    int mid = (l + r) / 2;

    if (key == v[mid]) {
      ans = mid;
      break;
    } else if (v[mid] > key) {
      r = mid - 1;
    } else {
      l = mid + 1;
    }
  }

  std::cout << ans << "\n";
}

int binary_search(int l, int r, vector<int> v, int key) {
  if (l > r) return -1;

  int mid = (l + r) / 2;

  if (v[mid] == key) {
    return mid;
  } else if (v[mid] < key) {
    return binary_search(mid + 1, r, v, key);
  } else {
    return binary_search(l, mid - 1, v, key);
  }
}

int main() {
  solve();
  // int t;
  // cin >> t;
  // while (t--) {
  //   solve();
  // }
  // return 0;
}
