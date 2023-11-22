#include<bits/stdc++.h>
using namespace std;
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
using traits = boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>;
using graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long>>>>>; // new! weightmap corresponds to costs

using edge_desc = boost::graph_traits<graph>::edge_descriptor;
using out_edge_it = boost::graph_traits<graph>::out_edge_iterator; // Iterator
using vertex_desc = boost::graph_traits<graph>::vertex_descriptor;

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

struct request{
  int s;
  int t;
  int d;
  int a;
  int p;
};

void solve(){
  
  int n, s; cin >> n >> s;
  //int MAXL = 100;//max number of cars per station
  int MAXA = 100000;//max arrival time
  int MAXP = 100;//max profit per booking
  vector<int> l(s);
  vector<request> R(n);
  vector<set<int>> booking_times(s); //store relevant booking times per station
  vector<map<int,int>> m(s); //hashmap for (local) ccordinate compression
  vector<int> m_psums(s+1, 0); //prefix sums of coordinate lengths
  
  //we always need a node for starting and end time for every station
  for(int i=0; i<s; ++i){
    booking_times[i].insert(0); 
    booking_times[i].insert(MAXA);
  }
  for(int i=0; i<s; ++i) cin >> l[i];
  const int pseudoINF = accumulate(l.begin(), l.end(), 0);
  
  int si, ti, di, ai, pi;
  for(int i=0; i<n; ++i){
    cin >> si >> ti >> di >> ai >> pi;
    booking_times[si-1].insert(di); booking_times[ti-1].insert(ai);
    R[i] = {si-1, ti-1, di, ai, pi};
  }
  
  for(int i=0; i<s; ++i){
    int coord = 0;
    for(auto &t : booking_times[i]){
      m[i][t] = coord; ++coord;
    }
    m_psums[i+1] = m_psums[i] + m[i].size();
  }
  
  graph G(m_psums.back());
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_target = boost::add_vertex(G);
  
  for(int i=0; i<s; ++i){
    adder.add_edge(v_source, m_psums[i], l[i], 0);
    adder.add_edge(m_psums[i+1]-1, v_target, pseudoINF, 0);
    int curr_it = -1;
    int t_prev = 0;
    for(auto &t : booking_times[i]){
      if(curr_it >=0) adder.add_edge(m_psums[i]+curr_it, m_psums[i]+curr_it+1, pseudoINF, MAXP*(t - t_prev));
      ++curr_it; t_prev = t;
    }
  }
  for(int i=0; i<n; ++i){
    adder.add_edge(m_psums[R[i].s] + m[R[i].s][R[i].d], 
                   m_psums[R[i].t] + m[R[i].t][R[i].a], 
                   1, 
                   (R[i].a - R[i].d)*MAXP - R[i].p);
  }
  // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  int s_flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
      s_flow += c_map[*e] - rc_map[*e];     
  }
  int cost = MAXP*MAXA*s_flow - boost::find_flow_cost(G);
  std::cout << cost << "\n";
  
}//solve

int main(){
  int t; cin >> t;
  while(t--) solve();
}
