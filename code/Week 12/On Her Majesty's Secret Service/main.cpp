#include<bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;


vector<int> dijkstra_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map;
}

int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);  // exterior property map

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  return matching_size;
}

using namespace std;

void solve(){
  int n, m, a, s, c, d; cin >> n >> m >> a >> s >> c >> d;
  weighted_graph G(n);
  for(int i=0; i<m; ++i){
    char w; int x, y, z; cin >> w >> x >> y >> z;
    boost::add_edge(x, y, z, G);
    if(w == 'L') boost::add_edge(y, x, z, G);
  }
  vector<int> agents(a);
  for(int i=0; i<a; ++i) cin >> agents[i];
  vector<int> shelters(s);
  for(int i=0; i<s; ++i) cin >> shelters[i];
  
  //get shortest paths between all pairs of relevant positions
  vector<vector<int>> dist_maps(n, vector<int>(n));
  for(int i=0; i<n; ++i){
    dist_maps[i] = dijkstra_dist(G, i);
  }
  
  //binary search on number of seconds for all agents to enter a shelter
  int start = 0;
  int end = INT_MAX / 2;
  while(start < end){
    int mid = (start + end) / 2;
    graph G_m(a + c*s);
    for(int i=0; i<a; ++i){
      for(int j=0; j<s; ++j){
        if(dist_maps[agents[i]][shelters[j]] <= mid-d){
          boost::add_edge(i, a+j, G_m);
        }
        if(c==2 && dist_maps[agents[i]][shelters[j]] <= mid-2*d){
          boost::add_edge(i,a+s+j, G_m);
        }
      }
    }
    int matching_size = maximum_matching(G_m);
    if(matching_size == a) end = mid;
    else start = mid+1;
  }
  
  cout << start << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
