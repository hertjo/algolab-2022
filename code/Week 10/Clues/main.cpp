#include<bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef size_t                                                 Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef CGAL::Point_2<K>                                       Point;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;


struct clue{
  Point a;
  Point b;
};

void solve(){
  Index n, m; cin >> n >> m;
  long r; cin >> r;
  
  vector<pair<Point, Index>> radios(0);
  for(Index i=0; i<n; ++i){
    int x, y; cin >> x >> y;
    radios.push_back(make_pair(Point(x, y), i));
  }
  sort(radios.begin(),radios.end(), [](auto r1, auto r2) {
    return r1.first.x() < r2.first.x();
  });
  
  vector<clue> clues(0);
  for(int i=0; i<(int)m; ++i){
    int ax,ay,bx,by; cin >> ax >> ay >> bx >> by;
    clues.push_back({Point(ax, ay), Point(bx, by)});
  }
  //graph G_b is only to check bipartiteness
  graph G_b(n);
  for(int i=0; i<(int)n; ++i){
    for(int j=i+1; j<(int)n; ++j){
      if(radios[j].first.x() - radios[i].first.x() > r) break;
      if(CGAL::squared_distance(radios[i].first, radios[j].first) <= r*r){
        boost::add_edge(i,j,G_b);
      }
    }
  }
  //check whether radio network is without any interferences (i.e. is the graph two-colorable)
  bool is_wo_interf = boost::is_bipartite(G_b);
  if(not is_wo_interf){
    //string nn =  string((int)m, 'n');
    for(int i=0; i<(int)m; ++i) cout << "n";
    cout << endl;
    return;
  }
  
  Delaunay t;
  t.insert(radios.begin(), radios.end());
  //graph G is now constructed from the Delaunay edges
  graph G(n);
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    if(t.segment(e).squared_length() <= r*r) boost::add_edge((int)i1, (int)i2, G);
  }
  //get connected components of this graph
  vector<int> component(num_vertices(G));
  boost::connected_components(G, &component[0]);
  
  //for each clue check, whether it can be routed within the radio network
  for(int i=0; i<(int)m; ++i){
    auto holmes = clues[i].a;
    auto watson = clues[i].b;
    //first check, whether the two radio senders can already reach each other without any stations
    if(CGAL::squared_distance(holmes, watson) <= r*r){
      cout << "y";
      continue;
    }
    auto vHolmes = t.nearest_vertex(holmes);
    auto vWatson = t.nearest_vertex(watson);
    //now check whether both are in reaching distance to the network
    if(CGAL::squared_distance(holmes, vHolmes->point()) > r*r ||
       CGAL::squared_distance(watson, vWatson->point()) > r*r){
         cout << "n";
         continue;
    }
    //eventually check whether the two are in the same connected component
    if(component[vHolmes->info()] == component[vWatson->info()]) cout << "y";
    else cout << "n";
  }
  cout << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
