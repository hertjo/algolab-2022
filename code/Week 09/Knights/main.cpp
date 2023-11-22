#include<bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

using traits = boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>;
using graph = boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::directedS,
  boost::no_property,
  boost::property<boost::edge_capacity_t, int,
    boost::property<boost::edge_residual_capacity_t, int,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor>
    >
  >
>;
using vertex_desc = boost::graph_traits<graph>::vertex_descriptor;

class edge_adder {
  graph &g;

 public:
  explicit edge_adder(graph &g) : g(g) {}

  void add_edge(int from, int to, int capacity) {
    auto c_map = boost::get(boost::edge_capacity, g);
    auto r_map = boost::get(boost::edge_reverse, g);
    const auto e = boost::add_edge(from, to, g).first;
    const auto rev_e = boost::add_edge(to, from, g).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

struct Point{
  int x;
  int y;
};

void solve(){
  int n, m, k, c; cin >> n >> m >> k >> c;
  if(k==0){ cout << 0 << endl; return; }
  vector<Point> knights(k);
  for(int i=0; i<k; ++i){
    int x, y; cin >> x >> y;
    knights[i] = {x, y};
  }
  
  graph G(2*n*m);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_target = boost::add_vertex(G);
  
  //connect source to all knights
  for(auto ki : knights){
    adder.add_edge(v_source, n*ki.y + ki.x, 1);
  }
  
  for(int i=0; i<n; ++i){
    for(int j=0; j<m; ++j){
      //constraint on vertices (as edge constraint on v_in->v_out)
      adder.add_edge(n*j + i, n*m + n*j + i, c);
      //connect boarders with target
      int num_boarders = (i==0) + (i==n-1) + (j==0) + (j==m-1);
      if(num_boarders>0) adder.add_edge(n*m + n*j + i, v_target, num_boarders);
      //connect intersections (v_out(i-1, j)->v_in(i, j) and v_out(i, j-1)->v_in(i, j)) in both directions
      if(i>0){
        adder.add_edge(n*m + n*j + (i-1), n*j + i, 1);
        adder.add_edge(n*m + n*j + i, n*j + (i-1), 1);
      }
      if(j>0){
        adder.add_edge(n*m + n*(j-1) + i, n*j + i, 1);
        adder.add_edge(n*m + n*j + i, n*(j-1) + i, 1);
      }
    }
  }
  
  int flow = boost::push_relabel_max_flow(G, v_source, v_target);
  cout << flow << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
