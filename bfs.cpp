#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using std::vector;
using std::queue;

int distance(vector<vector<int> > &adj, int s, int t) {
    vector<int> dist(adj.size(), INT_MAX);
    dist[s] = 0;

    queue<int> q;
    q.push(s);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        vector<int> neighs = adj[u];
        for (vector<int>::iterator it = neighs.begin(); it != neighs.end(); it++) {
            int v = *it;
            if (dist[v] == INT_MAX) {
                q.push(v);
                dist[v] = dist[u] + 1;
            }
        }
    }

    if (dist[t] != INT_MAX)
        return dist[t];
    return -1;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
    adj[y - 1].push_back(x - 1);
  }
  int s, t;
  std::cin >> s >> t;
  s--, t--;
  std::cout << distance(adj, s, t) << '\n';
}
