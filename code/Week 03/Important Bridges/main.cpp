#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

struct edge_component_t
{
  enum
  { num = 555 };
  using kind = boost::edge_property_tag;
} edge_component;

using namespace std;
using graph = boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS,boost::no_property,boost::property<edge_component_t,std::size_t>>;
using edge_component_map = boost::property_map<graph, edge_component_t>::type;
using edge_iter = boost::graph_traits<graph>::edge_iterator;

void solve(){
  int n, m; cin >> n >> m;
  //create undirected graph
  graph G(n);
  int e1, e2;
  for(int i=0; i<m; ++i){
    cin >> e1 >> e2;
    boost:add_edge(e1, e2, G);
  }
  
  //get biconnected components of G
  edge_component_map components = boost::get(edge_component, G);
  biconnected_components(G, components);
  
  //count number of edges in each component
  unordered_map<int, int> biconcomp_sizes;
  edge_iter iter, iter_end;
  for(boost::tie(iter, iter_end) = edges(G); iter != iter_end; ++iter) {
    ++biconcomp_sizes[components[*iter]];
  }
  
  //important bridges are edges in biconnected components of size 1
  vector<pair<int,int>> critical_bridges;
  int k = 0;
  for(boost::tie(iter, iter_end) = edges(G); iter != iter_end; ++iter) {
    if (biconcomp_sizes[components[*iter]] == 1) {
      const int i1 = boost::source(*iter, G);
      const int i2 = boost::target(*iter, G);
      critical_bridges.emplace_back(min(i1, i2), max(i1, i2));
      ++k;
    }
  }
  
  //sort critical edges before printing
  sort(begin(critical_bridges), end(critical_bridges));
  cout << k << endl;
  for (const auto &bridge: critical_bridges){
    cout << bridge.first << " " << bridge.second << endl;
  }
}

int main(){
  int t; cin >> t;
  while(t--){
    solve();
  }
}
