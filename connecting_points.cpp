#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using std::vector;
using std::pow;
using std::sqrt;
using std::cout;
using std::sort;
using std::pair;

class DisjointSet {
    public:
        DisjointSet(int n) {
            parent.resize(n);
            rank.resize(n);
            for (int i = 0; i < n; i++) {
                parent[i] = i;
                rank[i] = 0;
            }
        }

        int find(int i) {
            if (i != parent[i])
                parent[i] = find(parent[i]);
            return parent[i];
        }

        bool is_same_set(int i, int j) {
            if (find(i) == find(j))
                return true;
            else
                return false;
        }

        void union_set(int i, int j) {
            int id_i = find(i);
            int id_j = find(j);

            if (id_i == id_j)
                return;

            if (rank[id_i] > rank[id_j]) {
                parent[id_j] = id_i;
            } else {
                //This includes the case rank[id_i] is <= rank[id_j]
                parent[id_i] = id_j;
                //if they are equal, increase the rank
                //since we are hanging the i-th tree under j-th tree, increase the
                //rank of j-th tree
                rank[id_j] += 1;
            }
        }

    private:
        vector<int> parent;
        vector<int> rank;
};

struct Edge {
    Edge() : u(0), v(0), wt(0) {}
    Edge(int v1, int v2, double w) {
        u = v1; v = v2; wt = w;
    }
    int u;
    int v;
    double wt;
};

typedef vector<Edge>::iterator edge_iter;

struct EdgeCmp {
    bool operator()(const Edge& lhs, const Edge& rhs) {
        return lhs.wt < rhs.wt;
    }
} edge_comparator;

double minimum_distance(vector<pair<int, int> > &points) {
  double result = 0.;
  int n = points.size();
  //vector<Edge> edges((n * (n - 1)) / 2);
  vector<Edge> edges;

  for (int i = 0; i < n - 1; i++) {
      for (int j = i + 1; j < n; j++) {
          pair<int, int> p1 = points[i];
          pair<int, int> p2 = points[j];
          double w = pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2);
          w = sqrt(w);
          Edge e(i, j, w);
          edges.push_back(e);
      }
  }
  sort(edges.begin(), edges.end(), edge_comparator);

#if 0
  for (edge_iter it = edges.begin(); it != edges.end(); it++) {
      cout << "wt for pt (" << it->u << ", " << it->v << ") " << std::setprecision(10) << it->wt << '\n';
  }
#endif

  DisjointSet set(n);

  for (edge_iter it = edges.begin(); it != edges.end(); it++) {
      if (set.find(it->u) != set.find(it->v)) {
          set.union_set(it->u, it->v);
         result += it->wt;
      }
  }

  return result;
} 

int main() {
  size_t n;
  int x, y;
  std::cin >> n;
  vector<pair<int, int> > points(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> x >> y;
    pair<int, int> p = std::make_pair(x, y);
    points[i] = p;
  }
  std::cout << std::setprecision(10) << minimum_distance(points) << std::endl;
}
