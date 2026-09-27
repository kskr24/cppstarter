#include <bits/stdc++.h>

class UnionFind {
private:
  int                      n;
  mutable std::vector<int> p;
  std::vector<int>         rank;
  std::vector<int>         set_size;
  int                      num_sets;

public:
  UnionFind(int nodes) : n(nodes), num_sets(nodes) {
    p.assign(n, 0);
    for (int i{0}; i < n; ++i) {
      p[i] = i;
    }
    rank.assign(n, 0);
    set_size.assign(n, 1);
  }

  [[nodiscard]] int find_set(int i) const {
    return (p[i] == i) ? i : (p[i] = find_set(p[i]));
  }
  [[nodiscard]] bool is_same_set(int i, int j) const {
    return find_set(i) == find_set(j);
  }
  [[nodiscard]] int num_disjoint_sets() const { return num_sets; }
  [[nodiscard]] int size_of_set(int i) const { return set_size[find_set(i)]; }

  void union_set(int i, int j) {
    if (is_same_set(i, j)) {
      return;
    }

    int x = find_set(i);
    int y = find_set(j);

    if (rank[x] > rank[y]) {
      std::swap(x, y);
    }
    p[x] = y;
    if (rank[x] == rank[y]) {
      ++rank[y];
    }
    set_size[y] += set_size[x];
    num_sets--;
  }
};

int main() {
  using namespace std;
  int n, m;
  cin >> n >> m;

  UnionFind uf(n);
  for ([[maybe_unused]] auto i : std::views::iota(0, m)) {
    int x, y;
    cin >> x >> y;
    --x;
    --y;
    uf.union_set(x, y);
  }

  std::cout << uf.num_disjoint_sets() - 1 << "\n";
  std::set<int> s;
  for (auto i : std::views::iota(0, n)) {
    s.insert(uf.find_set(i));
  }

  int base = *s.begin();
  for (auto city : s) {
    if (base == city) {
      continue;
    }
    std::cout << base + 1 << " " << city + 1 << "\n";
  }
}