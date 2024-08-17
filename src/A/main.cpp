#include <bits/stdc++.h>

using namespace std;

#define pb push_back
#define eb emplace_back
#define X first
#define Y second

typedef std::vector<int> vi;
typedef long long int lli;

typedef std::pair<lli, lli> ii;
typedef std::vector<lli> vlli;

using a3 = std::array<lli, 3>;

int n, m;
lli t0, t1, t2;

void solve() {
  cin >> n >> m;
  cin >> t0 >> t1 >> t2;
  vector<vector<a3>> adj(n);
  int a, b, x, y;
  for (int i = 0; i < m; i++) {
    cin >> a >> b >> x >> y;
    a--, b--;
    adj[a].eb(a3{b, x, y});
    adj[b].eb(a3{a, x, y});
  }
  vlli dist(n);
  auto check = [&](const lli &mid) -> bool {
    dist.assign(n, LONG_LONG_MAX);
    auto can_take_bus = [&](const lli &l, const lli &r) -> bool {
      if (l >= t2 || r <= t1)
        return true;
      return false;
    };

    set<ii> pq;
    dist[0] = mid;

    pq.insert(ii{dist[0], 0});

    while (!pq.empty()) {
      // take the first element
      auto [d, u] = *pq.begin();
      pq.erase(pq.begin());
      if (d == LONG_LONG_MAX)
        continue;
      // relaxation
      for (const auto &node : adj[u]) {
        const lli &v = node[0], &l1 = node[1], &l2 = node[2];
        lli eta = d + l2;

        if (can_take_bus(d, d + l1)) {
          // reach via bus at d+l1 time
          eta = min(eta, d + l1);
        } else {
          // you can straight away walk or wait at u till the call is on and
          // then pick a bus; if you walk (d+l2) if you wait and then take a bus
          // max(t1, d) + l1
          eta = min(eta, max(d, t2) + l1);
        }
        // relax the edges
        if (dist[v] <= eta)
          continue;
        pq.erase(ii{dist[v], v});
        dist[v] = eta;
        pq.insert(ii{dist[v], v});
      }
    }

    return dist[n - 1] <= t0;
  };

  lli lo = -1, hi = t0 + 1;

  while (lo + 1 < hi) {
    lli mid = lo + (hi - lo) / 2;
    if (check(mid)) {
      lo = mid;
    } else {
      hi = mid;
    }
  }

  std::cout << lo << "\n";
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  // freopen("txt.in", "r", stdin);
  // freopen("txt.out", "w", stdout);
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
}
