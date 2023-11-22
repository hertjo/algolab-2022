#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
        boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

using namespace std;

struct game{
  int a;
  int b;
};

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
  int n, m;
  cin >> n >> m;
  vector<int> score_player_doc(n, 0);
  vector<int> scoreboard(n);
  vector<game> games_not_recorded;
  graph G;
  edge_adder adder(G);
  
  int a, b, c;
  for(int i=0; i<m; ++i){
    cin >> a >> b >> c;
    if(c == 0) games_not_recorded.push_back({a, b});
    else if(c == 1) ++score_player_doc[a];
    else ++score_player_doc[b];
  }
  int s;
  for(int i=0; i<n; ++i){
    cin >> s;
    scoreboard[i] = s;
  }
  int num_games = (int) games_not_recorded.size();
  for(int i=0; i<num_games; ++i){
    adder.add_edge(i, num_games + games_not_recorded[i].a, 1);
    adder.add_edge(i, num_games + games_not_recorded[i].b, 1);
  }
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  for(int i=0; i<num_games; ++i){
    adder.add_edge(v_source, i, 1);
  }
  int sum_needed = 0;
  for(int i=0; i<n; ++i){
    sum_needed += (scoreboard[i] - score_player_doc[i]);
    adder.add_edge(num_games + i, v_sink, scoreboard[i] - score_player_doc[i]);
  }
  
  // Calculate flow from source to sink
  // The flow algorithm uses the interior properties (managed in the edge adder)
  // - edge_capacity, edge_reverse (read access),
  // - edge_residual_capacity (read and write access).
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  
  if((flow == sum_needed) && (flow == num_games)) cout << "yes" << endl;
  else cout << "no" << endl;
  
}

int main(){
  int t; cin >> t;
  while(t--){
    solve();
  }
}
