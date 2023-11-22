#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<size_t,K> VB;
typedef CGAL::Triangulation_face_base_2<K> FB;
typedef CGAL::Triangulation_data_structure_2<VB,FB> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef pair<K::Point_2, int> participant;
typedef K::Point_2 light;

void solve(){
  //reading in and storing p + l
  int m, n; cin >> m >> n;
  long x, y, r, h;
  vector<participant> p(m);
  vector<light> lights(n);
  
  for(int i=0; i<m; ++i){
    cin >> x >> y >> r;
    p[i] = participant(K::Point_2(x,y), r);
  }
  cin >> h;
  for(int i=0; i<n; ++i){
    cin >> x >> y;
    lights[i] = light(K::Point_2(x,y));
  }
  //binary search
  vector<size_t> out(m, numeric_limits<size_t>::max());
  size_t left = 0; 
  size_t right = n-1;
  
  while(left<=right){
    int participants_still_in = m;
    size_t mid = (left+right)/2;
    Triangulation t;
    t.insert(lights.begin() + left, lights.begin() + mid + 1); //left is inclusive, right is exclusive
    for(int i=0; i<m; ++i){
      if(out[i] < mid){
        --participants_still_in;
        continue;
      }
      auto v = t.nearest_vertex(p[i].first);
      K::Point_2 q = v->point();
      K::FT sd = CGAL::squared_distance(p[i].first, q);
      K::FT hr = h + p[i].second;
      if(sd < hr*hr){
        out[i] = min(mid, out[i]);
        --participants_still_in;
      }
    }
    
    if(participants_still_in == 0){
      right = mid;
      if(left == right) break;
    } else{
      left = mid + 1;
    }
    
  }//end binary search
  
  size_t last = *max_element(out.begin(), out.end());
  for(int i = 0; i < m; ++i){
    if(out[i] == last) {
      cout << i << " ";
    }
  }
  cout << endl;
} //end solve

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    solve();
  }
}
