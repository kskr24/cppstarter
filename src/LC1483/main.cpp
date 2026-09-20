// LC1483
#include <array>
#include <queue>
#include <vector>

using namespace std;

class TreeAncestor {
private:
  std::vector<std::vector<int>> far;

public:
  TreeAncestor(int n, vector<int>& parent) {
    far.resize(21, vector<int>(50001, -1));
    for (int i = 0; i < n; ++i) {
      far[0][i] = parent[i];
    }
    for (int h = 1; h <= 20; ++h) {
      for (int i = 1; i < n; ++i) {
        if (far[h - 1][i] == -1) {
          continue;
        }
        far[h][i] = far[h - 1][far[h - 1][i]];
      }
    }
  }

  int getKthAncestor(int node, int k) {
    return -1;
    // for (int h = LOGMAX; h >= 0; --h) {
    //   if (k & (1 << h)) {
    //     node = far[h][node];
    //     if (node == -1) {
    //       return -1;
    //     }
    //   }
    // }
    // return node;
  }
};

int main() { return 0; }
