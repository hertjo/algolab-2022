#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;

using namespace std;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

void solve(){
  
  int n, m, k, x, y;
  cin >> n >> m >>k >> x >> y;
  int num_vertices = (k + 1) * n;
  
  int a, b, c, d;
  weighted_graph G = weighted_graph(num_vertices);
  for(int i = 0; i < m; ++i){
    cin >> a >> b >> c >> d;
    //is river edge
    if(d){
      for(int j=0; j<k; ++j){
        boost::add_edge(a + j*n, b + j*n, c, G);
        boost::add_edge(a + j*n, b + (j+1)*n, c, G);
        boost::add_edge(a + (j+1)*n, b + j*n, c, G);
      }
      //for last level treat river edge as normal edge
      boost::add_edge(a + k*n, b + k*n, c, G);
    }
    //is not a river edge
    else{
      for(int j=0; j<k+1; ++j){
        boost:add_edge(a + j*n, b + j*n, c, G);
      }
    }
  }
  
  cout << dijkstra_dist(G, x, y + k*n) << endl;
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    solve();
  }
}
