#include <iostream>
#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
typedef boost::adjacency_list<boost::vecS,
        boost::vecS,
        boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;

int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n); // exterior property map
  const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();
  boost::edmonds_maximum_cardinality_matching(G,
          boost::make_iterator_property_map(mate_map.begin(),
          boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(),
    boost::get(boost::vertex_index, G)));
  
  return matching_size;
}

void solve(){
  int w, h;
  cin >> w >> h;
  vector<vector<int>> garden(h, vector<int>(w, 0));
  string s;
  int spaces_to_be_tiled = 0;
  for(int i=0; i<h; ++i){
    cin >> s;
    for(int j=0; j<w; ++j){
      if(string(1, s[j]) == "."){
        ++spaces_to_be_tiled;
        ++garden[i][j];
      }
    }
  }
  
  if(spaces_to_be_tiled % 2 == 1){
    cout << "no" << endl;
    return;
  }
  graph G(h*w);
  for(int i=0; i<h; ++i){
    for(int j=0; j<w; ++j){
      if(garden[i][j]){
        if(i+1<h && garden[i+1][j  ]) boost::add_edge(i*w + j, (i+1)*w + j  , G);
        if(j+1<w && garden[i  ][j+1]) boost::add_edge(i*w + j, i    *w + j+1, G);
      }
    }
  }
  
  int matching_size = maximum_matching(G);
  if(matching_size == spaces_to_be_tiled/2) cout << "yes" << endl;
  else cout << "no" << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    solve();
  }
}
