#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

//BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::edge_iterator edge_it;

using namespace std;
using vertex_desc = boost::graph_traits<graph>::vertex_descriptor;

int maximum_matching_size(const graph &G) {
  int n = boost::num_vertices(G);
  vector<vertex_desc> mate_map(n); // exterior property map
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
  int n, c, f; cin >> n >> c >> f;

  //read in characteristic in unordered map
  unordered_map<string, vector<int>> charac_map;
  string charact;
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < c; ++j) {
      cin >> charact;
      charac_map[charact].push_back(i);
    }
  }
  //for each pair of students count number of characteristics they have in common
  vector<vector<int>> pair_count(n, vector<int> (n, 0));
  for(auto x : charac_map){
    for(int i=0; i<(int)x.second.size(); ++i){
      for(int j=i+1; j<(int)x.second.size(); ++j){
        pair_count[x.second[i]][x.second[j]]++;
      }
    }
  }
  
  //construct graph: add edge if student pair has more than f characteristics in common
  graph G(n);
  for(int i=0; i<n-1; ++i){
    for(int j=i+1; j<n; ++j){
      if(pair_count[i][j] > f){
        boost::add_edge(i, j, G);
      }
    }
  }
  
  //optimal if G does not has a matching that matches all students
  int matching_size = maximum_matching_size(G);
  if(matching_size == n/2) cout << "not optimal" << endl;
  else cout << "optimal" << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    solve();
  }
}
