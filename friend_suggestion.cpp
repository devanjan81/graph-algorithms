#include <cstdio>
#include <cassert>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
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


const Len INFINITE = std::numeric_limits<Len>::max() / 4;
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
    vector<set<int>> workset_;
    // Stores a flag for each node which is True iff the node was visited
    // either by forward or backward search.
    //vector<bool> visited_;

    //list of proc nodes for fwd and back search
    vector<set<int>> proc_nodes_;

    enum {
        NODE_PROCESSED_SINGLE_DIR = 1,
        NODE_PROCESSED_BOTH_DIR = 2
    };

public:
    Bidijkstra(int n, Adj adj, Adj cost)
        : n_(n), adj_(adj), cost_(cost), distance_(2, vector<Len>(n, INFINITE)),
        workset_(2), proc_nodes_(2)
    {
        //workset_.reserve(2);
        //proc_nodes_.reserve(2);
    }

    // Initialize the data structures before new query,
    // clear the changes made by the previous query.
    void clear() {
        for (auto v : workset_[0])
            distance_[0][v] = INFINITE;
        for (auto v : workset_[1])
            distance_[1][v] = INFINITE;
        workset_[0].clear();
        workset_[1].clear();
        proc_nodes_[0].clear();
        proc_nodes_[1].clear();
    }

    Len shortest_distance() {
        vector<int> common;
        Len distance = INFINITE;
        std::set_union(proc_nodes_[0].begin(), proc_nodes_[0].end(),
                proc_nodes_[1].begin(), proc_nodes_[1].end(),
                std::back_inserter(common));

        for (const auto &u : common) {
            Len dist_s = distance_[0][u];
            Len dist_r = distance_[1][u];
            if (dist_s != INFINITE && dist_r != INFINITE
                    && (dist_s + dist_r < distance)) {
                distance = dist_s + dist_r;
            }
        }
        return (distance == INFINITE) ? -1 : distance;
    }

    //dir can have 2 values: 0 for fwd search, 1 for back search
    void process(Queue &q, BidirSet &stale_entries, int dir, int u) {
        //for all (u, v) in E(G) or E(Gr)
        //     Relax(u, v)
        //add u to processed nodes list 

        vector<int> &edges = adj_[dir][u]; //edge list in fwd/reverse graph
        vector<int> &wts = cost_[dir][u];
        int num_edges = edges.size();
        for (int i = 0; i < num_edges; i++) {
            int v = edges[i];
            int cost = wts[i];
            Len dist_v =  distance_[dir][v];
            Len dist_u = distance_[dir][u];
            if ((dist_u != INFINITE) && (dist_v > dist_u + cost)) {
                //update the old entries in stale_entries set
                stale_entries[dir].insert(make_pair(dist_v, v));
                distance_[dir][v] = dist_u + cost;
                //instead of updating the old dist value in pq, put a new one
                q[dir].push(make_pair(distance_[dir][v], v));

                // Add the node to the current working set so that we can clean
                // it up for the subsequent queries
                workset_[dir].insert(v);
            }
        }
        proc_nodes_[dir].insert(u);
    }

    // Returns the distance from s to t in the graph.
    Len query(int s, int t) {
        clear();

        //first init dist array for forward and backward search to zero for s and t
        distance_[0][s] = 0;
        distance_[1][t] = 0;
        workset_[0].insert(s);
        workset_[1].insert(t);

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
        DistPair pq_node;
        vector<int> common;
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

            process(q, stale_entries, 0, v);

            // Node v processed in fwd dir has also been processed in backward dir
            // time to stop BidirDijkstra algo
            if ((v == t) || (proc_nodes_[1].find(v) != proc_nodes_[1].end())) {
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
            
            process(q, stale_entries, 1, v);
            // Node v processed in backward dir is also seen in fwd dir
            // Time to stop BidirDijkstra algo
            if ((v == s) || (proc_nodes_[0].find(v) != proc_nodes_[0].end())) {
                break;
            }

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
