#include <cstdio>
#include <cassert>
#include <vector>
#include <queue>
#include <set>
#include <limits>
#include <utility>

using namespace std;
using std::vector;
using std::pair;
using std::set;

// Distances can grow out of int type
typedef long long Len;
// External vector of size 2 - for forward and backward search.
// Internal 2-dimensional vector is vector of adjacency lists for each node.
typedef vector<vector<vector<int>>> Adj;
typedef pair<Len, int> DistPair;

// Vector of two priority queues - for forward and backward searches.
// Each priority queue stores the closest unprocessed node in its head.
typedef priority_queue<DistPair, vector<DistPair>,
        std::greater<DistPair> > PriorityQueue;
typedef vector<PriorityQueue> Queue;
typedef vector<int>::iterator int_iter;
typedef set<DistPair, std::less<DistPair>> Set;
typedef vector<Set> BidirSet;

const Len INFINITY = numeric_limits<Len>::max() / 4;

class Bidijkstra {
    // Number of nodes
    int n_;
    // Graph adj_[0] and cost_[0] correspond to the initial graph,
    // adj_[1] and cost_[1] correspond to the reversed graph.
    // Graphs are stored as vectors of adjacency lists corresponding
    // to nodes.
    // Adjacency list itself is stored in adj_, and the corresponding
    // edge costs are stored in cost_.
    Adj adj_;
    Adj cost_;
    // distance_[0] stores distances for the forward search,
    // and distance_[1] stores distances for the backward search.
    vector<vector<Len>> distance_;
    // Stores all the nodes visited either by forward or backward search.
    vector<int> workset_;
    // Stores a flag for each node which is True iff the node was visited
    // either by forward or backward search.
    vector<bool> visited_;

    enum {
        NODE_PROCESSED_SINGLE_DIR = 1,
        NODE_PROCESSED_BOTH_DIR = 2
    };

public:
    Bidijkstra(int n, Adj adj, Adj cost)
        : n_(n), adj_(adj), cost_(cost), distance_(2, vector<Len>(n, INFINITY)), visited_(n)
    { workset_.reserve(n); }

    // Initialize the data structures before new query,
    // clear the changes made by the previous query.
    void clear() {
#if 1
        for (int i = 0; i < workset_.size(); ++i) {
            int v = workset_[i];
            distance_[0][v] = distance_[1][v] = INFINITY;
            visited_[v] = false;
        }
#endif
#if 0
        for (int i = 0; i < n_; i++) {
            distance_[0][i] = distance_[1][i] = INFINITY;
            visited_[i] = false;
        }
#endif
        workset_.clear();
    }

    Len shortest_distance() {
        Len distance = INFINITY;
        for (vector<int>::iterator it = workset_.begin(); it != workset_.end(); it++) {
            int u = *it;
            Len dist_s = distance_[0][u];
            Len dist_r = distance_[1][u];
            if (dist_s != INFINITY && dist_r != INFINITY
                    && (dist_s + dist_r < distance)) {
                distance = dist_s + dist_r;
            }
        }
        return (distance == INFINITY) ? -1 : distance;
    }

    //dir can have 2 values: 0 for fwd search, 1 for back search
    int process(Queue &q, BidirSet &stale_entries, int dir, int u) {
        //for all (u, v) in E(G) or E(Gr)
        //     Relax(u, v)
        //add u to workset

        vector<int> &edges = adj_[dir][u]; //edge list in fwd/reverse graph
        vector<int> &wts = cost_[dir][u];
        int num_edges = edges.size();
        for (int i = 0; i < num_edges; i++) {
            int v = edges[i];
            int cost = wts[i];
            Len dist_v =  distance_[dir][v];
            Len dist_u = distance_[dir][u];
            if ((dist_u != INFINITY) && (dist_v > dist_u + cost)) {
                //update the old entries in stale_entries set
                stale_entries[dir].insert(make_pair(dist_v, v));
                distance_[dir][v] = dist_u + cost;
                //instead of updating the old dist value in pq, put a new one
                q[dir].push(make_pair(distance_[dir][v], v));

                if (!visited_[v]) {
                    visited_[v] = true;
                    workset_.push_back(v);
                }
            }
        }
        if (!visited_[u]) {
            workset_.push_back(u);
            visited_[u] = true;
            return NODE_PROCESSED_SINGLE_DIR;
        }
        return NODE_PROCESSED_BOTH_DIR;
             
    }

    // Returns the distance from s to t in the graph.
    Len query(int s, int t) {
        clear();

        //first init dist array for forward and backward search to zero for s and t
        distance_[0][s] = 0;
        distance_[1][t] = 0;

        //now populate the pq for both fwd and backward search using the dist arrays
        Queue q(2);
        BidirSet stale_entries(2);
        for (int i = 0; i < n_; i++) {
            // distance_[0][i] fwd dist node i dist value
            // distance_[1][i] backward dist node i dist value
            q[0].push(make_pair(distance_[0][i], i));
            q[1].push(make_pair(distance_[1][i], i));
        }

        int v;
        int ret;
        DistPair pq_node;
        do {
            pq_node = q[0].top();
            auto it = stale_entries[0].find(pq_node);
            if (it != stale_entries[0].end()) {
                if (*it == pq_node) {
                    q[0].pop();
                    continue;
                }
            }
            v = pq_node.second;
            q[0].pop();

            ret = process(q, stale_entries, 0, v);
            if (ret == NODE_PROCESSED_BOTH_DIR) {
                break;
            }

            do {
                pq_node = q[1].top();
                it = stale_entries[1].find(pq_node);
                if (it != stale_entries[1].end()) {
                    if (*it == pq_node) {
                        q[1].pop();
                        continue;
                    }
                }
                break;
            } while (true);
            v = pq_node.second;
            q[1].pop();
            
            ret = process(q, stale_entries, 1, v);
            if (ret == NODE_PROCESSED_BOTH_DIR)
                break;

        } while (true);

        return shortest_distance();
        //visit(q, 0, s, 0);
        //visit(q, 1, t, 0);
    }
};

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    Adj adj(2, vector<vector<int>>(n));
    Adj cost(2, vector<vector<int>>(n));
    for (int i=0; i<m; ++i) {
        int u, v, c;
        scanf("%d%d%d", &u, &v, &c);
        adj[0][u-1].push_back(v-1);
        cost[0][u-1].push_back(c);
        adj[1][v-1].push_back(u-1);
        cost[1][v-1].push_back(c);
    }

    Bidijkstra bidij(n, adj, cost);

    int t;
    scanf("%d", &t);
    for (int i=0; i<t; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        printf("%lld\n", bidij.query(u-1, v-1));
    }
}
