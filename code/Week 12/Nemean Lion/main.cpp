#include<bits/stdc++.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

typedef CGAL::Point_2<K> Point;
typedef K::FT Float;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Finite_edges_iterator Edge_iterator;
typedef Triangulation::Finite_vertices_iterator Vertex_iterator;

using namespace std;

void solve(){
  // Solution for 60/100 only!
  Float s;
  long n, h;
  cin >> n >> s >> h;
  vector<Point> positions(n);
  for(int i=0; i<n; ++i){
    int x, y; cin >> x >> y;
    positions[i] = Point(x,y);
  }
  
  Triangulation t;
  t.insert(positions.begin(), positions.end());
  
  int a2, a3, as, amax = 0;
  as = h; amax = h;
  Float minDist = LONG_MAX;
  int admSets = 0;
  for(Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    if(s > t.segment(e).squared_length()) ++as;
    if(minDist > t.segment(e).squared_length()){
      minDist = t.segment(e).squared_length();
      admSets = 1;
    }
    else if(minDist == t.segment(e).squared_length()) ++admSets;
  }
  a2 = admSets;
  
  Float minDist2 = LONG_MAX;
  admSets = 0;
  for(Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f){
    Point p = f->vertex(0)->point();
    Point p_mid = t.dual(f);
    Float dist = CGAL::squared_distance(p, p_mid);
    if(minDist2 > dist){
      minDist2 = dist;
      admSets = 1;
    }
    else if(minDist2 == dist) ++admSets;
  }
  a3 = admSets;
  
  cout << a2 << " " << a3 << " " << h << " " << h << endl;
}

int main(){
  int t; cin >> t;
  while(t--) solve();
}
