#include<bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef int                                                    Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef CGAL::Point_2<K> Point;

using namespace std;

struct bone{
  Point p;
  int idx;
  K::FT dist;
};

void solve(){
  int n, m, k; long s; cin >> n >> m >> s >> k;
  vector<pair<Point, int>> trees; trees.reserve(n);
  for(int i=0; i<n; ++i){
    long x, y; cin >> x >> y;
    trees.emplace_back(make_pair(Point(x,y), i));
  }
  vector<bone> bones(m);
  for(int i=0; i<m; ++i){
    long x, y; cin >> x >> y;
    bones[i] = {Point(x,y), 0, 0};
  }
  
  Triangulation t;
  t.insert(trees.begin(), trees.end());
  
  boost::disjoint_sets_with_storage<> connComp(n);
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    if(t.segment(e).squared_length() <= s){
      connComp.link(i1, i2);
    }
  }
  
  vector<int> compSizes(n, 0); //We know there are at most n connected components
  for(int i=0; i<m; ++i){
    Point p = t.nearest_vertex(bones[i].p)->point();
    bones[i].dist = CGAL::squared_distance(bones[i].p, p);
    Index idx = t.nearest_vertex(bones[i].p)->info();
    bones[i].idx = idx;
    if(CGAL::squared_distance(bones[i].p, p) <= s/4){
      compSizes[connComp.find_set(idx)]++;
    }
  }

  int a = *max_element(compSizes.begin(), compSizes.end());
  
  //For the second query we essentially do the same procedure with a binary search over q=4b^2
  long start = 0; long end = pow(2,51); long q = 0;
  while(start != end){
    q = (start + end) / 2;
    
    boost::disjoint_sets_with_storage<> connComp(n);
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
      Index i1 = e->first->vertex((e->second+1)%3)->info();
      Index i2 = e->first->vertex((e->second+2)%3)->info();
      if(t.segment(e).squared_length() <= q){
        connComp.link(i1, i2);
      }
    }
    
    vector<int> compSizes(n, 0); //We know there are at most n connected components
    for(auto b : bones){
      if(b.dist <= q/4){
        compSizes[connComp.find_set(b.idx)]++;
      }
    }
    if(*max_element(compSizes.begin(), compSizes.end()) < k) start=q+1;
    else end=q;
  }
  cout << a << " " << max(start, q) << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
