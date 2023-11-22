#include<bits/stdc++.h>

// BGL include
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

using namespace std;

struct debt_rel{
  int i;
  int j;
  int d;
};

void solve(){
  long n, m; cin >> n >> m;
  vector<int> balance(n);
  for(int i=0; i<n; ++i) cin >> balance[i];
  
  vector<debt_rel> debts(m);
  for(int k=0; k<m; ++k){
    int ii, jj, d; cin >> ii >> jj >> d;
    debts[k] = {ii, jj, d};
  }
  
  graph G(n);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_target = boost::add_vertex(G);
  
  long total_pos_balance = 0;
  for(int i=0; i<n; ++i){
    if(balance[i] > 0){
      adder.add_edge(v_source, i, balance[i]);
      total_pos_balance += balance[i];
    }
    else if(balance[i] < 0) adder.add_edge(i, v_target, -balance[i]);
  }
  for(int k=0; k<m; ++k){
    adder.add_edge(debts[k].i, debts[k].j, debts[k].d);
  }
  
  // if there is some free-standing province, then the incoming balance will
  // be smaller than the outgoing money to other provinces
  // --> the total flow will be reduced and therefore will be < total_pos_balance
  long flow = boost::push_relabel_max_flow(G, v_source, v_target);
  cout << (flow < total_pos_balance ? "yes" : "no") << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
