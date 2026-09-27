#include <bits/stdc++.h>

class UnionFind {
private:
  int              n;
  std::vector<int> par;
  std::vector<int> rank;
  std::vector<int> set_size;
  int              num_sets;

public:
  UnionFind(int nodes) : n(nodes), num_sets(nodes) {
    for (int i{0}; i < n; ++i) {
      par[i] = i;
    }
    rank.assign(n, 0);
    set_size.assign(n, 1);
  }

  int  find_set(int i) { return (par[i] == i) ? i : find_set(par[i]); }
  bool is_same_set(int i, int j) { return find_set(i) == find_set(j); }
  int  num_disjoint_sets() { return num_sets; }
  int  size_of_set(int i) { return set_size[find_set(i)]; }

  void union_set(int i, int j) {
    if (is_same_set(i, j)) {
      return;
    }

    int x = find_set(i);
    int y = find_set(j);

    if (rank[x] > rank[y]) {
      std::swap(x, y);
    }
    par[x] = y;
    if (rank[x] == rank[y]) {
      ++rank[y];
    }
    set_size[y] += set_size[x];
    num_sets--;
  }
};