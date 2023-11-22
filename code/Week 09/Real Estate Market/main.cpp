// Includes
// ========
#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator
typedef boost::graph_traits<graph>::vertex_descriptor             vertex_desc;

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

void solve(){
  int n, m, s; cin >> n >> m >> s;
  vector<int> limits(s);
  vector<int> states(m);
  int ll, ss;
  for(int i=0; i<s; ++i){
    cin >> ll; limits[i] = ll;
  }
  for(int i=0; i<m; ++i){
    cin >> ss; states[i] = ss-1; //states start at 1
  }
  
  graph G(n+m+s);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  edge_adder adder(G);
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  int b;
  for(int i=0; i<n; ++i){
    adder.add_edge(v_source, i, 1, 0);
    for(int j=0; j<m; ++j){
      cin >> b;
      adder.add_edge(i, n+j, 1, -b + 101); //add 101 to make cost nonnegative
    }
  }
  for(int i=0; i<m; ++i){
    adder.add_edge(n+i, n+m+states[i], 1, 0);
  }
  for(int i=0; i<s; ++i){
    adder.add_edge(n+m+i, v_sink, limits[i], 0);
  }
  
  //solution 1: (80/100) with negative weights
  //int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  //boost::cycle_canceling(G);
  //int cost = boost::find_flow_cost(G);
  //cout << flow << " " << -cost << endl;
  
  //solution 2: (100/100) with nonegative weights only
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int cost = boost::find_flow_cost(G);
  // Iterate over all edges leaving the source to sum up the flow values.
  int flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
      flow += c_map[*e] - rc_map[*e];     
  }
  cout << flow << " " << -cost + flow*101 << endl; //make sure to subtract flow*101 to get actual total cost
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
