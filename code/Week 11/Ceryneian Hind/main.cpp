#include <bits/stdc++.h>

// BGL includes
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
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;

// Custom edge adder class
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

int acc_positive(vector<int> &vec){
  int res = 0;
  for(int v : vec) res += max(v,0);
  return res;
}

void solve(){
  //This one's essentially solving a closure problem
  int n, m; cin >> n >> m;
  int MAXN = 500; int MAXS = 1024;
  int INF = MAXN*MAXS;
  vector<int> score(n);
  for(int i=0; i<n; ++i){
    cin >> score[i];
  }
  
  graph G(n+2);
  edge_adder adder(G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  const vertex_desc v_s = n;
  const vertex_desc v_t = n+1;
  for(int i=0; i<m; ++i){
    int u, v; cin >> u >> v;
    adder.add_edge(u, v, INF);
  }
  for(int i=0; i<n; ++i){
    score[i] >= 0 ? adder.add_edge(v_s, i, score[i]) : adder.add_edge(i, v_t, abs(score[i]));
  }
  
  long flow = boost::push_relabel_max_flow(G, v_s, v_t);

  // BFS to find vertex set S of MinCut
  std::vector<int> vis(n+2, false); // visited flags
  std::queue<int> Q; // BFS queue (from std:: not boost::)
  vis[v_s] = true; // Mark the source as visited
  Q.push(v_s);
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
    }
  }
  int conv_score = 0;
  for(int i=0; i<n; ++i){
    if(vis[i]) conv_score += score[i];
  }
  conv_score == 0 ? cout << "impossible" << endl : cout << conv_score << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
