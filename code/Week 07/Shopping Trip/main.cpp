#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
        boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;
  public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, long capacity) {
      auto c_map = boost::get(boost::edge_capacity, G);
      auto r_map = boost::get(boost::edge_reverse, G);
      const auto e = boost::add_edge(from, to, G).first;
      const auto rev_e = boost::add_edge(to, from, G).first;
      c_map[e] = capacity;
      c_map[rev_e] = 0; // reverse edge has no capacity!
      r_map[e] = rev_e;
      r_map[rev_e] = e;
    }
};

void solve(){
 int n, m, s;
 cin >> n >> m >> s;
 graph G(n-1);
 edge_adder adder(G);
 const vertex_desc v_sink = boost::add_vertex(G);
 int shop;
 for(int i=0; i<s; ++i){
   cin >> shop;
   adder.add_edge(shop, v_sink, 1);
 }
 int a, b;
 for(int i=0; i<2*m; i+=2){
   cin >> a >> b;
   adder.add_edge(a, b, 1);
   adder.add_edge(b, a, 1);
 }
 
 long flow = boost::push_relabel_max_flow(G, 0, v_sink);
 if (flow >= s) cout << "yes" << endl;
 else cout << "no" << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    solve();
  }
}
