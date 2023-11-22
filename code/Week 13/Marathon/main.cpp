#include<bits/stdc++.h>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

//Undirected graph for dijkstra computations
typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS,boost::no_property,boost::property<boost::edge_weight_t,int>> weighted_graph;
  
// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph_flow;

//Modified to return the distance map to all nodes from s
vector<int> dijkstra_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map;
}

// Custom edge adder class for graph of type "graph_flow"
class edge_adder {
  graph_flow &G;

 public:
  explicit edge_adder(graph_flow &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

struct street{
  int a; //intersection pa
  int b; //intersection pb
  int c; //width
  int d; //length
};

void solve(){
  
  int n, m, s, f; cin >> n >> m >>s >> f;
  vector<street> streets(m);
  for(int i=0; i<m; ++i){
    int a, b, c, d; cin >> a >> b >> c >> d;
    streets[i] = {a, b, c, d};
  }
  
  weighted_graph G(n);
  for(int i=0; i<m; ++i){
    boost::add_edge(streets[i].a, streets[i].b, streets[i].d, G);
  }
  //compute shortest path from s to all other nodes
  vector<int> dist = dijkstra_dist(G, s);
  
  graph_flow G_f(n);
  edge_adder adder(G_f);
  for(auto st : streets){
    //check whether street (a,b) / (b,a) is on some shortest path from s to b / s to a
    //add it to the flow graph if that is the case and weight it by the width of the street
    //Why it is that easy: 
    //If 'a' is on some shortest path 's'->'f', then 's'->'a' is on that shortest path 
    if(dist[st.a] + st.d == dist[st.b]) adder.add_edge(st.a, st.b, st.c);
    if(dist[st.b] + st.d == dist[st.a]) adder.add_edge(st.b, st.a, st.c);
  }
  
  long max_runners = boost::push_relabel_max_flow(G_f, s, f);
  cout << max_runners << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
