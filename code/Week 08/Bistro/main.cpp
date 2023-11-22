#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

using namespace std;

void solve(int n){
  vector<K::Point_2> pts;
  pts.reserve(n);
  for(int i=0; i<n; ++i){
    long x, y; cin >> x >> y;
    pts.push_back(K::Point_2(x,y));
  }
  //construct triangulation
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  //find nearest vertex for every single bistro position
  //and compute its the squared euclidean distance to it
  int m; cin >> m;
  for(int i=0; i<m; ++i){
    long x, y; cin >> x >> y;
    K::Point_2 p = K::Point_2(x,y);
    auto v = t.nearest_vertex(p);
    K::Point_2 q = v->point();
    long sd = (long)CGAL::squared_distance(p, q);
    cout << (long)sd << endl;
  }
}

int main(){
  int n; cin >> n;
  while(n != 0){
    solve(n);
    cin >> n;
  }
}
