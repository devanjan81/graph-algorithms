#include <iostream>
#include <vector>

using std::vector;
using std::cout;
struct NodeState {
	int label;
	bool visited;
	NodeState()
	{
		label = visited = 0;
	}
};

typedef vector<vector<int> > AdjTbl;
typedef vector<NodeState> VerticesSt;
typedef vector<NodeState>::iterator VIterator;
typedef vector<int> NeighLst;
typedef vector<int>::iterator NeighIter;

static void explore(AdjTbl &adj, VerticesSt &vertSt, int vertex, int label)
{
    vertSt[vertex].visited = true;
    vertSt[vertex].label = label;

    NeighLst neighs = adj[vertex];
    for (NeighIter it = neighs.begin(); it != neighs.end(); it++) {
        if (!vertSt[*it].visited) {
            explore(adj, vertSt, *it, label);
        }
    }
    return;
}

static void DFS(AdjTbl &adj, VerticesSt &vertSt)
{
    static int label = 1;
    int i = 0;
    for (VIterator it = vertSt.begin(); it != vertSt.end(); i++, it++) {
        NodeState st = *it;
        if (!st.visited) {
            explore(adj, vertSt, i, label);
        }
        label++;
    }
}

static int reach(vector<vector<int> > &adj, VerticesSt &vertSt, int x, int y)
{
    DFS(adj, vertSt);
    size_t sz = adj.size();

    if (x < 0 || y < 0 || x >= sz || y >= sz) {
        return 0;
    }
    if (vertSt[x].label == vertSt[y].label)
        return 1;
    return 0;
}

int main() {
    size_t n, m;
    std::cin >> n >> m;
    NodeState defNodeSt;
    vector<vector<int> > adj(n, vector<int>());
    vector<NodeState> vertices(n, defNodeSt);
    for (size_t i = 0; i < m; i++) {
        int x, y;
        std::cin >> x >> y;
        adj[x - 1].push_back((y - 1));
        adj[y - 1].push_back((x - 1));
    }
    int x, y;
    std::cin >> x >> y;
    std::cout << reach(adj, vertices, x - 1, y - 1) << '\n';
    return 0;
}
