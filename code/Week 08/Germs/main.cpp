#include<bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef CGAL::Point_2<K> Point;

using namespace std;

struct dish{
  int l;
  int b;
  int r;
  int t;
};

double p_inv(double dist){
  return sqrt(dist - 0.5);
}

void solve(int n){
  int ld, bd, rd, td;
  cin >> ld >> bd >> rd >> td;
  dish d = {ld, bd, rd, td};
  vector<Point> bacteria; bacteria.reserve(n);
  for(int i=0; i<n; ++i){
    int x, y; cin >> x >> y;
    bacteria.push_back(Point(x,y));
  }
  
  Triangulation t;
  t.insert(bacteria.begin(), bacteria.end());
  
  vector<double> first_collide_time; first_collide_time.reserve(n);
  for(auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
    //distance of vertex to the boundaries
    double x = v->point().x();
    double y = v->point().y();
    double min_time_collide = sqrt(min({abs(d.l - x),abs(d.r - x),abs(d.b - y),abs(d.t - y)}) - 0.5);
    auto c = t.incident_edges(v);
    //circulate all incident edges and compute time to first collision
    if(c != 0){
      do{
        if(t.is_infinite(c)) continue;
        //since both bacteria are growing at the same time, we are interested when the radius is of size edge/2-->collision
        min_time_collide = min(min_time_collide, p_inv(sqrt(t.segment(c).squared_length()) / 2));
      } while(++c != t.incident_edges(v));
    }
    first_collide_time.push_back(min_time_collide);
  }
  sort(first_collide_time.begin(), first_collide_time.end());
  cout << ceil(first_collide_time[0]) << " " << ceil(first_collide_time[n/2]) << " " << ceil(first_collide_time[n-1]) << endl;
}

int main(){
  int n; cin >> n;
  while(n != 0){
    solve(n);
    cin >> n;
  }
}
