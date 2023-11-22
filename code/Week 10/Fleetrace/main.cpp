#include<vector>
#include<iostream>
#include<algorithm>
#include<cmath>

// BGL includes
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
                boost::property <boost::edge_weight_t, long> > > > > graph; //weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator over edges of G/G_res
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

using namespace std;

void solve(){
  int b, s, p;
  cin >> b >> s >> p;
  graph G(s+b);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_target = boost::add_vertex(G);
  
  int bi, si, ci;
  vector<int> bb(p);
  vector<int> ss(p);
  vector<int> cc(p);
  for(int i=0; i<p; ++i){
    cin >> bi >> si >> ci;
    bb[i] = bi; ss[i] = si; cc[i] = ci;
  }
  int max_cost = *max_element(cc.begin(), cc.end());
  for(int i=0; i<b; ++i){
    adder.add_edge(v_source, i, 1, 0);
    adder.add_edge(i, v_target, 1, max_cost);
  }
  for(int i=0; i<s; ++i){
    adder.add_edge(b+i, v_target, 1, 0);
  }
  for(int i=0; i<p; ++i){
    adder.add_edge(bb[i], b + ss[i], 1, -cc[i] + max_cost);
  }
  
  // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  int cost2 = boost::find_flow_cost(G);
  std::cout << -(cost2 - b*max_cost) << endl;

}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
