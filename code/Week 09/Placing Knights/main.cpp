#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef  boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >  graph;
// Interior Property Maps
typedef  boost::graph_traits<graph>::edge_descriptor      edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;
typedef  boost::graph_traits<graph>::vertex_descriptor      vertex_desc;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder {
 graph &G;

public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

bool check_inside(int n, int j, int i){
  if (i < 0 || i > n-1 || j < 0 || j > n-1) return false;
  return true;
}
void solve(){
  int n; cin >> n;
  graph G(n*n);
  edge_adder adder(G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  vector<vector<int>> present(n, vector<int>(n));
  bool p;
  for(int i=0; i<n; ++i){
    for(int j=0; j<n; ++j){
      cin >> p; present[i][j] = p;
    }
  }
  for(int i=0; i<n; ++i){
    for(int j=0; j<n; ++j){
      if(present[i][j]){
        if((i + j) % 2 == 0){
          adder.add_edge(v_source, i*n + j, 1);
          //connect with all existing neighbors reachable by a knights move
          if(check_inside(n, i-2, j+1) && present[i-2][j+1]) adder.add_edge(i*n + j, (i-2)*n + (j+1), 1);
          if(check_inside(n, i-2, j-1) && present[i-2][j-1]) adder.add_edge(i*n + j, (i-2)*n + (j-1), 1);
          if(check_inside(n, i-1, j+2) && present[i-1][j+2]) adder.add_edge(i*n + j, (i-1)*n + (j+2), 1);
          if(check_inside(n, i-1, j-2) && present[i-1][j-2]) adder.add_edge(i*n + j, (i-1)*n + (j-2), 1);
          if(check_inside(n, i+1, j+2) && present[i+1][j+2]) adder.add_edge(i*n + j, (i+1)*n + (j+2), 1);
          if(check_inside(n, i+1, j-2) && present[i+1][j-2]) adder.add_edge(i*n + j, (i+1)*n + (j-2), 1);
          if(check_inside(n, i+2, j-1) && present[i+2][j-1]) adder.add_edge(i*n + j, (i+2)*n + (j-1), 1);
          if(check_inside(n, i+2, j+1) && present[i+2][j+1]) adder.add_edge(i*n + j, (i+2)*n + (j+1), 1);
        }
        else adder.add_edge(i*n + j, v_sink, 1);
      }
    }
  }
  
  
  // Find a min cut via maxflow
  int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  //actually one could already output #fields_present - flow and it would be correct
  //I just wanted to implement MaxIS once and I think it would also be an intuitive example for it
  //std::cout << "maximum flow = minimum cut = " << flow << "\n";
  
  // BFS to find vertex set S
  std::vector<int> vis(n*n+2, false); // visited flags
  std::queue<int> Q; // BFS queue (from std:: not boost::)
  vis[v_source] = true; // Mark the source as visited
  Q.push(v_source);
  while (!Q.empty()) {
    const int u = Q.front();
    Q.pop();
    out_edge_it ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
      const int v = boost::target(*ebeg, G);
      // Only follow edges with spare capacity
      if (rc_map[*ebeg] == 0 || vis[v]) continue;
      vis[v] = true;
      Q.push(v);
    }//for
  }//while
  
  //count size of MaxIS \ {v_source}
  int cnt = 0;
  for(int i=0; i<n; ++i){
    for(int j=0; j<n; ++j){
      if( (i + j) % 2 == 0 && present[i][j] && vis[i*n + j]) ++cnt;
      else if( (i + j) % 2 == 1 && present[i][j] && !vis[i*n + j]) ++cnt;
    }
  }
  
  cout << cnt << endl;
}//solve

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    solve();
  }
}
