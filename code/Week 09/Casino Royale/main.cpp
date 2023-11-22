#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};


struct mission{
  int x;
  int y;
  int q;
};

void solve(){
  int n, m, l; cin >> n >> m >> l;
  vector<mission> missions(m);
  
  int x, y, q;
  for(int i=0; i<m; ++i){
    cin >> x >> y >> q;
    missions[i] = mission({x, y, q});
  }
  
  graph G(n);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  int aux = 128;
  adder.add_edge(v_source, 0, l, 0);
  for(int i=0; i<n-1; ++i){
    adder.add_edge(i, i+1, l, 0 + aux);
  }
  for(int i=0; i<m; ++i){
    int diff = missions[i].y - missions[i].x;
    adder.add_edge(missions[i].x, missions[i].y, 1, -missions[i].q + diff*aux);
  }
  adder.add_edge(n-1, v_sink, l, 0);
  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int cost2 = boost::find_flow_cost(G);
  cout << -cost2 + (n-1)*aux*l<< endl;
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
