#include <functional>
#include <queue>
#include <set>
#include <utility>
#include <vector>
class Graph {
public:
  int                                           V;
  std::vector<std::vector<std::pair<int, int>>> adj;
  void                                          dijkstra1(int s);
  void                                          dijkstra2(int s);
  static constexpr int                          INF = 1e9;
};

void Graph::dijkstra1(int s) {
  std::vector<int> dist(V, INF);
  dist[s] = 0;

  std::set<std::pair<int, int>> pq;

  for (int i = 0; i < V; ++i) {
    pq.emplace(dist[i], i);
  }

  while (!pq.empty()) {
    auto [d, u] = *pq.begin();
    pq.erase(pq.begin());
    for (auto& [w, v] : adj[u]) {
      if (dist[v] <= dist[u] + w) {
        continue;
      }
      pq.erase(pq.find({dist[v], v}));
      dist[v] = dist[u] + w;
      pq.emplace(dist[v], v);
    }
  }
}

void Graph::dijkstra2(int s) {
  using ii = std::pair<int, int>;
  std::vector<int>                                           dist(V, INF);
  std::priority_queue<ii, std::vector<ii>, std::greater<ii>> pq;
  pq.push({0, s});

  while (!pq.empty()) {
    auto& [d, u] = pq.top();
    pq.pop();
    if (dist[u] < d) {
      continue;
    }

    for (auto& [w, v] : adj[u]) {
      if (dist[v] < dist[u] + w) {
        continue;
      }
      dist[v] = dist[u] + w;
      pq.emplace(dist[v], v);
    }
  }
}