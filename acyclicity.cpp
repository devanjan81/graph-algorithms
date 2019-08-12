#include <iostream>
#include <vector>

using std::vector;
using std::pair;

struct Node {
    Node(): visited(0), previsit(0), postvisit(0) {}
    void setPreVisitTime() {previsit = clock++;}
    void setPostVisitTime() {postvisit = clock++;}
    int visited;
    int previsit;
    int postvisit;
    static int clock;
};
int Node::clock = 1;

typedef vector<int> NeighLst;
typedef vector<int>::iterator NeighIter;
typedef vector<NeighLst> Graph;
typedef vector<Node> Vertices;
typedef vector<Node>::iterator VIter;

static void explore(Graph &g, Vertices &vList, int v) {
    vList[v].visited = true;
    vList[v].setPreVisitTime();

    NeighLst neighs = g[v];
    for (NeighIter it = neighs.begin(); it != neighs.end(); it++) {
        int w = *it;
        if (!vList[w].visited) {
            explore(g, vList, w);
        }
    }
    vList[v].setPostVisitTime();
}

static void depthFirstSearch(Graph &g, Vertices &v) {
    for (int i = 0; i < v.size(); i++) {
        if (!v[i].visited) {
            explore(g, v, i);
        }
    }
    return;
}

int acyclic(Graph &g, Vertices &v) {
    int preu, postu, prev, postv;
    depthFirstSearch(g, v);
    for (int i = 0; i < g.size(); i++) {
        preu = v[i].previsit;
        postu = v[i].postvisit;
        NeighLst neighs = g[i];
        for (NeighIter it = neighs.begin(); it != neighs.end(); it++) {
            int w = *it;
            prev = v[w].previsit;
            postv = v[w].postvisit;
            //Found a back edge
            if ((preu > prev) && (postu < postv)) {
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    size_t n, m;
    std::cin >> n >> m;
    Graph adj(n, NeighLst());
    Vertices v(n, Node()); //check if it gets init to 0
    for (size_t i = 0; i < m; i++) {
        int x, y;
        std::cin >> x >> y;
        adj[x - 1].push_back(y - 1);
    }
    std::cout << acyclic(adj, v) << '\n';
    return 0;
}
