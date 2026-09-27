#include <bits/stdc++.h>

using namespace std;

void solve() {
  int n;
  cin >> n;
  vector<int> boys(n);
  for (auto i{0}; i < n; ++i) {
    cin >> boys[i];
  }
  int m;
  cin >> m;
  vector<int> girls(m);
  for (auto i{0}; i < m; ++i) {
    cin >> girls[i];
  }
  std::ranges::sort(boys);
  std::ranges::sort(girls);

  int i = 0, j = 0;
  int ans = 0;
  while (i < n && j < m){
    if(abs(boys[i] - girls[j]) <= 1){
      ans++;
      i++;
      j++;
    }else{
      
    }
  }
}

int main() {
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
}