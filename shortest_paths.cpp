#include <iostream>
#include <limits>
#include <vector>
#include <queue>

using std::vector;
using std::queue;
using std::pair;
using std::priority_queue;

typedef vector<int>::iterator int_iter;
typedef vector<vector<int> >::iterator vec_iter;

void shortest_paths(vector<vector<int> > &adj, vector<vector<int> > &cost,
        int s, vector<long long> &dist, vector<int> &reachable,
        vector<int> &shortest) {

    int n = adj.size();
    int cyclic_node = -1;
    vector<int> prev(n, -1);
    dist[s] = 0;
    reachable[s] = 1;

    //repeat the below for loop for |V| times which <==> n
    int i;
    bool some_node_relaxed = false;
    for (i = 1; i <= n; i++) {
        if (i > 1 && !some_node_relaxed) {
            break;
        }
        some_node_relaxed = false;
        vec_iter adj_it;
        vec_iter cost_it;
        for (adj_it = adj.begin(), cost_it = cost.begin(); adj_it != adj.end() &&
                cost_it != cost.end(); adj_it++, cost_it++) {
            int u = adj_it - adj.begin();
            vector<int> &neighs = *adj_it;
            vector<int> &wts = *cost_it;

            int_iter ngh_it;
            int_iter wt_it;
            for (ngh_it = neighs.begin(), wt_it = wts.begin();
                    ngh_it != neighs.end() && wt_it != wts.end();
                    ngh_it++, wt_it++) {
                int v = *ngh_it;
                int w = *wt_it;
                if ((dist[u] != std::numeric_limits<long long>::max()) &&
                        dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    prev[v] = u;
                    some_node_relaxed = true;
                    if (!reachable[v])
                        reachable[v] = 1;
                    if (i == n) {
                        /* for all nodes relaxed on nth iteration, setting
                         * shortest to zero so that distance to that node
                         * is treated as -INFINITY  
                         */
                        shortest[v] = 0;
                        cyclic_node = v;
                    }
                }
            }
        }
    }

    /* for all nodes which fall in negative cycle, mark their shortest dist 
     * as -INFINITY  
     */
    if (cyclic_node != -1) {
        int x = cyclic_node;
        do {
            int prev_node = prev[x];
            if (prev_node == cyclic_node)
                break;
            shortest[prev_node] = 0;
            x = prev_node;
        } while (1);
    }
}

int main() {
  int n, m, s;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  vector<vector<int> > cost(n, vector<int>());
  for (int i = 0; i < m; i++) {
    int x, y, w;
    std::cin >> x >> y >> w;
    adj[x - 1].push_back(y - 1);
    cost[x - 1].push_back(w);
  }
  std::cin >> s;
  s--;
  vector<long long> distance(n, std::numeric_limits<long long>::max());
  vector<int> reachable(n, 0);
  vector<int> shortest(n, 1);
  shortest_paths(adj, cost, s, distance, reachable, shortest);
  for (int i = 0; i < n; i++) {
    if (!reachable[i]) {
      std::cout << "*\n";
    } else if (!shortest[i]) {
      std::cout << "-\n";
    } else {
      std::cout << distance[i] << "\n";
    }
  }
}
