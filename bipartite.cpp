#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using std::vector;
using std::queue;

static inline int flipColor(int &color) {
    return (color == 0) ? 1 : 0;
}

int bipartite(vector<vector<int> > &adj) {
    int s = 0; //choosing any node, choosing first node as starting pt
    int color = 0;
    int ret = 1; //graph is bipartite
    vector<int> dist(adj.size(), INT_MAX);
    dist[s] = color;

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
                dist[v] = flipColor(dist[u]);
            } else {
                //this is the case of red edges
                //check if the already processed edge is of the same color as u
                if (dist[u] == dist[v]) {
                    ret = 0;
                    break;
                }
            }
        }
    }
    return ret;
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
    std::cout << bipartite(adj) << '\n';
}
