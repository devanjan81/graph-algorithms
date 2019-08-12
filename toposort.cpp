#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;
using std::pair;

void dfs(vector<vector<int> > &adj, vector<int> &used, vector<int> &order,
        int u) {
    used[u] = 1;
    vector<int> neighs = adj[u];
    for (vector<int>::iterator it = neighs.begin(); it != neighs.end(); it++) {
        int v = *it;
        if (!used[v]) {
            dfs(adj, used, order, v);
        }
    }
    //no neighbors for me, let us add the elem at the beginning of vector
    order.insert(order.begin(), u);
}

vector<int> toposort(vector<vector<int> > &adj) {
  vector<int> used(adj.size(), 0);
  vector<int> order;
  for (int v = 0; v < adj.size(); v++) {
      if (!used[v]) {
          dfs(adj, used, order, v); //start at first vertex
      }
  }
  return order;
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  for (size_t i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
  }
  vector<int> order = toposort(adj);
  for (size_t i = 0; i < order.size(); i++) {
    std::cout << order[i] + 1 << " ";
  }
  std::cout << '\n';
}
