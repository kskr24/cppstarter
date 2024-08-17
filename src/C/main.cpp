#include <bits/stdc++.h>
#include <stdio.h>


bool solve2(const int &n, const std::string &s, const std::vector<int> v) {
  if ((int)s.size() != n)
    return false;

  std::map<char, int> mp;
  std::set<int> st1;
  std::set<int> st2;
  for (int i = 0; i < n; i++) {
    st1.insert(v[i]);
    st2.insert(s[i]);
    if (mp.find(s[i]) == mp.end()) {
      mp[s[i]] = v[i];
    } else {
      if (mp[s[i]] != v[i])
        return false;
    }
  }
  return st1.size() == st2.size();
}

using namespace std;
void solve() {
  int n;
  scanf("%d", &n);
  vector<int> v(n);
  for (int i = 0; i < n; ++i) {
    scanf("%d", &v[i]);
  }
  int m;
  scanf("%d", &m);
  string s;
  for (int i = 0; i < m; i++) {
    cin >> s;
    if (solve2(n, s, v)) {
      printf("Yes\n");
    } else {
      printf("No\n");
    }
  }
}

int main() {
  int t;
  scanf("%d", &t);
  while (t--) {
    solve();
  }
  return 0;
}
