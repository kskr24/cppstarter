#include <bits/stdc++.h>

using namespace std;
class SegmentTree {
public:
  // Instead of giving a fixed size to the vector, we will calculate the size
  // needed to store the store the tree. This size is twice of the nearest
  // larger power of 2
  SegmentTree(int n, const vector<int>& v) : v_(v) {
    while (sz_ < n) {
      sz_ *= 2;
    }

    st_.assign(2 * sz_, 0);
    lazy_.assign(2 * sz_, -1);

    build_(0, 0, v_.size() - 1);
  }

  int RMQ(int p, int L, int R, int i, int j) {
    propagate_(p, L, R);
    if (i > j) {
      return -1;
    }
    // query range [i, j] covers [l,R]
    if ((L >= i) && (R <= j)) {
      return st_[p];
    }

    int m = L + (R - L) / 2;
    return conquer_(RMQ(l_(p), L, m, i, min(m, j)),
                    RMQ(r_(p), m + 1, R, max(i, m + 1), j));
  }

  void update(int p, int L, int R, int i, int j, int val) {
    propagate_(p, L, R);

    if (i > j) {
      return;
    }
    if (L >= i && R <= j) {
      lazy_[p] = val;
      propagate_(p, L, R);
      return;
    }
    int m = L + (R - L) / 2;
    update(l_(p), L, m, i, min(m, j), val);
    update(r_(p), m + 1, R, max(i, m + 1), j, val);

    int lsubtree = (lazy_[l_(p)] != -1) ? lazy_[l_(p)] : st_[l_(p)];
    int rsubtree = (lazy_[r_(p)] != -1) ? lazy_[r_(p)] : st_[r_(p)];

    st_[p] = (lsubtree < rsubtree) ? lsubtree : rsubtree;
  }

private:
  int         sz_{1};
  vector<int> lazy_;
  vector<int> v_;
  vector<int> st_;

  // gives the left child of the current node
  int l_(int p) { return (p << 1) + 1; }
  // returns the right child of the currrent node
  int r_(int p) { return (p << 1) + 2; }

  int conquer_(int a, int b) {
    if (a == -1) {
      return b;
    }
    if (b == -1) {
      return a;
    }

    return min(a, b);
  }
  void build_(int p, int L, int R) {
    if (L == R) {
      st_[p] = v_[L];
      return;
    }

    int mid = L + (R - L) / 2;
    build_(l_(p), L, mid);
    build_(r_(p), mid + 1, R);

    st_[p] = conquer_(l_(p), r_(p));
  }

  void propagate_(int p, int L, int R) {
    if (lazy_[p] == -1) {
      return;
    }
    st_[p] = lazy_[p];
    if (L != R) {
      lazy_[l_(p)] = lazy_[r_(p)] = lazy_[p];
    } else {
      v_[L] = lazy_[p];
    }
    lazy_[p] = -1;
  }
};
