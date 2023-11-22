// Includes
#include <bits/stdc++.h>
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
                boost::property <boost::edge_weight_t, long> > > > > graph;

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

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
    w_map[e] = cost;
    w_map[rev_e] = -cost;
  }
};

struct match{
  int u;
  int v;
  int r;
};

void solve(){
  int e, w, m, d; cin >> e >> w >> m >> d;
  int INF = 4*10*10*10 + 4*10*10 + 1;
  int p, l; cin >> p >> l;
  vector<match> matches(m+d);
  for(int i=0; i<m+d; ++i){
    int u, v, r; cin >> u >> v >> r;
    matches[i] = {u, e+v, r};
  }
  
  graph G(2*(e+w));
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  const vertex_desc v_s = boost::add_vertex(G);
  const vertex_desc v_s2 = boost::add_vertex(G);
  const vertex_desc v_t2 = boost::add_vertex(G);
  const vertex_desc v_t = boost::add_vertex(G);
  
  adder.add_edge(v_s, v_s2, p-e*l, 0);
  for(int i=0; i<e; ++i){
    adder.add_edge(v_s, i, l, 0);
    adder.add_edge(v_s2, i, INF, 0);
    adder.add_edge(v_s2, e+w + i, INF, 0);
  }
  for(int i=0; i<m; ++i) adder.add_edge(matches[i].u, matches[i].v, 1, matches[i].r);
  for(int i=m; i<m+d; ++i) adder.add_edge(e+w + matches[i].u, e+w + matches[i].v, 1, matches[i].r);
  for(int i=e; i<e+w; ++i){
    adder.add_edge(i, e+w + i, INF, 0);
    adder.add_edge(i, v_t, l, 0);
    adder.add_edge(e+w + i, v_t2, INF, 0);
  }
  adder.add_edge(v_t2, v_t, p-w*l, 0);
  
  boost::successive_shortest_path_nonnegative_weights(G, v_s, v_t);
  int cost = boost::find_flow_cost(G);
  int flow = 0;
  out_edge_it ee, eend;
  for(boost::tie(ee, eend) = boost::out_edges(boost::vertex(v_s,G), G); ee != eend; ++ee) {
      flow += c_map[*ee] - rc_map[*ee];     
  }
  if(p < l*max(e,w) || flow < p) cout << "No schedule!" << endl;
  else cout << cost << endl;
}

int main(){
  int t; cin >> t;
  while(t--) solve();
}
