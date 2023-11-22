#include <iostream>
#include <vector>
#include <climits>
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
struct char_pair{
  int f;
  int b;
};

void solve(){
  int h, w;
  cin >> h >> w;
  string note; int n;
  cin >> note; n = note.length();
  
  graph G(26*26 + 26);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  //demands
  vector<int> char_in_notes_cnt(26, 0);
  for(int i=0; i<n; ++i){
    int idx = (int)(note[i] - 'A');
    ++char_in_notes_cnt[idx];
  }
  for(int i=0; i<26; ++i){
    adder.add_edge(i + 26*26, v_sink, char_in_notes_cnt[i]);
  }
  
  //supplies
  vector<int> pair_counts(26*26, 0);
  vector<char_pair> pairs(h*w, {0,0});
  string line;
  for(int i=0; i<h; ++i){
    cin >> line;
    for(int j=0; j<w; ++j){
      int idx = (int)(line[j] - 'A');
      pairs[i*w + j].f = idx;
    }
  }
  for(int i=0; i<h; ++i){
    cin >> line;
    for(int j=0; j<w; ++j){
      int idx = (int)(line[w-j-1] - 'A');
      pairs[i*w + j].b = idx;
    }
  }
  for(auto fb_pair : pairs){
    ++pair_counts[fb_pair.f * 26 + fb_pair.b];
  }
  for(int i=0; i<26*26; ++i){
    adder.add_edge(v_source, i, pair_counts[i]);
  }
  for(int i=0; i<26; ++i){
    for(int j=0; j<26; ++j){
      adder.add_edge(26*i + j, 26*26 + i, INT_MAX);
      adder.add_edge(26*i + j, 26*26 + j, INT_MAX);
    }
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  if(flow == n) cout << "Yes" << endl;
  else cout << "No" << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    solve();
  }
}
