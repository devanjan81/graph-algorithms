#include <iostream>
#include <vector>
#define MAX_VAL 100000001

using std::vector;
typedef vector<int>::iterator int_iter;
typedef vector<vector<int> >::iterator vec_iter;

int negative_cycle(vector<vector<int> > &adj, vector<vector<int> > &cost) {
    int is_cyclic = 0;
    int n = adj.size();
    vector<int> dist(n, MAX_VAL);
    vector<int> prev(n, 0);
    dist[0] = 0;

    //repeat the below for loop for |V| times which <==> n
    int i;
    for (i = 1; i <= n; i++) {
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
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    prev[v] = u;
                    if (i == n)
                        is_cyclic = 1;
                }
            }
        }
    }
    return is_cyclic;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    vector<vector<int> > adj(n, vector<int>());
    vector<vector<int> > cost(n, vector<int>());
    for (int i = 0; i < m; i++) {
        int x, y, w;
        std::cin >> x >> y >> w;
        adj[x - 1].push_back(y - 1);
        cost[x - 1].push_back(w);
    }
    std::cout << negative_cycle(adj, cost) << '\n';
}
