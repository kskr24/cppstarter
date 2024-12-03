#include <bits/stdc++.h>

using namespace std;
class SegmentTree {
 public:
  // Instead of giving a fixed size to the vector, we will calculate the size
  // needed to store the store the tree. This size is twice of the nearest
  // larger power of 2
  SegmentTree(const int &n, const vector<int> &v) : v_(v) {
    while (sz_ < n) {
      sz_ *= 2;
    }
    st.assign(2 * sz_, 0);
    build_(0, 0, v_.size() - 1);
  }

  int ans(int l, int r) { return range_query_(l, r, 0, 0, v_.size() - 1); }
  int set(int l, int r, int val) {
    range_update_(l, r, val, 0, 0, v_.size() - 1);
  }

 private:
  int         sz_{1};
  vector<int> v_;
  vector<int> st;

  // gives the left child of the current node
  int left_(int p) { return (p << 1) + 1; }
  // returns the right child of the currrent node
  int right_(int p) { return (p << 1) + 2; }

  void build_(int x, int lx, int rx) {
    if (lx == rx) {
      st[x] = v_[lx];
      return;
    }
    int mid = lx + (rx - lx) / 2;
    build_(left_(x), lx, mid);
    build_(right_(x), mid + 1, rx);

    st[x] = max(st[left_(x)], st[right_(x)]);
  }
  /**
   * l and r refers to the range of the array we have to update.
   * lx and rx which the range of the tree will check over
   */
  void range_update_(int l, int r, int val, int p, int lx, int rx) {
    // if the current range of the tree is beyond what we have to update
    if (r < lx || l > rx) return;
    if (lx == rx) {
      st[p] = st[p] + 1;
    }
    int mid = lx + (rx - lx) / 2;
    // now that we know tree range has some intersection with the range we have
    // to update
    range_update_(l, min(mid, r), val, left_(p), lx, mid);
    range_update_(max(mid + 1, l), r, val, right_(p), mid + 1, rx);
    st[p] = max(st[left_(p)], st[right_(p)]);
  }

  int range_query_(int l, int r, int p, int lx, int rx) {
    if (r < lx || l > rx) return 0;
    if (lx == l && rx == r) return st[p];
    int mid = lx + (rx - lx) / 2;
    return max(range_query_(l, min(mid, r), left_(p), lx, mid),
               range_query_(max(mid + 1, l), r, right_(p), mid + 1, rx));
  }
};
