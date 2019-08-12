#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;
using std::pair;

struct Node {
    Node(): visited(0), previsit(0), postvisit(0) {}
    void setPreVisitTime() {previsit = clock++;}
    void setPostVisitTime() {postvisit = clock++;}
    int index;
    int visited;
    int previsit;
    int postvisit;
    static int clock;
};
int Node::clock = 1;

bool nodeCmp(Node &n1, Node &n2) {
    return (n1.postvisit > n2.postvisit);
}

typedef vector<int> NeighLst;
typedef vector<int>::iterator NeighIter;
typedef vector<NeighLst> Graph;
typedef vector<Node> Vertices;
typedef vector<Node>::iterator VIter;

static void explore(Graph &g, Vertices &vList, int v) {
    //storing the index will help later when we do some sorting operation
    //on the vertex list
    vList[v].index = v;
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

void explore2(Graph &adj, vector<int> &used, vector<int> &scc, int u) {
    used[u] = 1;
    scc.push_back(u);
    vector<int> neighs = adj[u];
    for (vector<int>::iterator it = neighs.begin(); it != neighs.end(); it++) {
        int v = *it;
        if (!used[v]) {
            explore2(adj, used, scc, v);
        }
    }
}

static void depthFirstSearch(Graph &g, Vertices &v) {
    for (int i = 0; i < v.size(); i++) {
        if (!v[i].visited) {
            explore(g, v, i);
        }
    }
    return;
}

static void reverseGraph(Graph &in, Graph &out) {
    for (int u = 0; u < in.size(); u++) {
        NeighLst &neighs = in[u];
        for (NeighIter it = neighs.begin(); it != neighs.end(); it++) {
            int v = *it;
            /* in graph has a link u --> v, reverse in out graph */
            out[v].push_back(u); 
        }
    }
}

int number_of_strongly_connected_components(Graph &adj) {
  int result = 0;
  //Create the reverse Graph. Nodes with largest post order in G^R will help
  //identify the sink comps of G
  Graph gR(adj.size(), NeighLst());
  reverseGraph(adj, gR);
  //Create the vertices list which will have the vertices visited state,
  //pre order and post order visit times
  Vertices v(adj.size(), Node());
  depthFirstSearch(gR, v);

  //sort the vertices such that vertex with greatest post order val is at the 
  //beginning of list
  std::sort(v.begin(), v.end(), nodeCmp);

  //Now run explore to find out the SCCs
  //run a different version of explore inline with toposort so that the vertices
  //in the same explore recursion call can be returned in a list
  vector<int> used(adj.size(), 0);
  vector<int> sccs;
  for (VIter it = v.begin(); it != v.end(); it++) {
      if (!used[it->index]) {
          //calling explore on reverse graph will explore all the vertices in 
          //one call which we don't want, so call the org graph
          explore2(adj, used, sccs, it->index);
          result++;
          sccs.clear();
      }
  }

  return result;
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
  std::cout << number_of_strongly_connected_components(adj) << '\n';
}
