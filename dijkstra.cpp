#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <climits>

using std::vector;
using std::queue;
using std::pair;
using std::priority_queue;
using std::map;
typedef vector<int>::iterator intIter;

struct Node {
    Node(int v, int dval) {
        vertex = v;
        dist_val = dval;
    }

    int vertex;
    int dist_val;
};

struct NodeCmp {
   bool operator() (const Node& lhs, const Node& rhs) const {
        return lhs.dist_val > rhs.dist_val;
    }
};

#define MAX_VAL 100000001
int distance(vector<vector<int> > &adj, vector<vector<int> > &cost,
        int s, int t) {
    int n = adj.size();
    vector<int> dist(n, MAX_VAL);
    vector<int> prev(n, 0);
    dist[s] = 0;

    priority_queue<Node, vector<Node>, NodeCmp> pq;
    map<int, int> upd_entries;
    for (intIter it = dist.begin(); it != dist.end(); it++) {
        Node n(it - dist.begin(), *it);
        pq.push(n);
    }

    while (!pq.empty()) {
        Node n = pq.top();
        auto it = upd_entries.find(n.vertex);
        if (it != upd_entries.end()) {
            if (it->second == n.dist_val) {
                pq.pop();
                continue;
            }
        }
        int u = n.vertex;
        pq.pop();

        vector<int> &neighs = adj[u];
        vector<int> &wts = cost[u];
        intIter it1, it2;
        for (it1 = neighs.begin(), it2 = wts.begin();
                it1 != neighs.end() && it2 != wts.end(); it1++, it2++) {
            int v = *it1;
            int w = *it2;
            if (dist[v] > dist[u] + w) {
                int old = dist[v];
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push(Node(v, dist[v])); //updated entry is push as a new entry
                //put the old entry in a map
                //this is a hack since c++ pq is non mutable, i.e we cannot change
                //priority value
                upd_entries[v] = old;
            }
        }
    }

    int ret = dist[t];
    return (ret == MAX_VAL) ? -1 : ret;
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
    int s, t;
    std::cin >> s >> t;
    s--, t--;
    std::cout << distance(adj, cost, s, t) << '\n';
}
