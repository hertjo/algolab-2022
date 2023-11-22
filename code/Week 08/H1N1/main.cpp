#include<bits/stdc++.h>
using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<size_t,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Delaunay;
typedef CGAL::Point_2<K>                                       Point;

void solve(int n){
  vector<Point> infIndividuals(0);
  for(int i=0; i<n; ++i){
    int x, y; cin >> x >> y;
    infIndividuals.push_back(Point(x,y));
  }
  Delaunay t;
  t.insert(infIndividuals.begin(), infIndividuals.end());
  //number finite Delaunay faces (Voronoi vertices) and store information in the faces directly 
  //-> O(1) access
  //start at index '1', as we later label infinite faces with '0'
  int face_cnt = 1;
  for(auto f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f){
    f->info() = face_cnt++;
  }
  //adjacency list, implicitly storing all finite edges
  //Idea for the motion planning: Move along the edges of the Voronoi diagram 
  //as going to neighboring triangles in the Delaunay triangulation
  vector<vector<pair<int, double>>> edges(face_cnt);
  for(auto f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f){
    int u = f->info();
    for(int i=0; i<3; ++i){//iterate over the three neighboring faces
      auto f2 = f->neighbor(i);
      int v = t.is_infinite(f2) ? 0 : f2->info(); //label infinite faces with '0'
      auto p1 = f->vertex((i+1)%3)->point();
      auto p2 = f->vertex((i+2)%3)->point();
      
      double dist = CGAL::squared_distance(p1, p2);
      edges[u].push_back({v, dist});
      if (v==0) edges[v].push_back({u, dist}); //also store finite edges of infinite faces
    }
  }
  
  //preprocessing, s.t. for each query later we can do a simple lookup
  vector<double> max_dist(face_cnt, -1);//for each face store max distance possible to maintain while moving out of the convex hull
  priority_queue<pair<double, int>> Q;
  Q.push({numeric_limits<double>::max(), 0}); //store (dist, index) pairs --> Q ordered with max dist being on top
  //starting from infinite faces --> convex hull edges upper-bound the escape through them
  //we somewhat bubble from the convex hull edges (all labeled by 0) to the inner edges..
  //Note to myself, make a visualization/animation of this traversal idea
  while(!Q.empty()){
    auto p = Q.top(); Q.pop();
    double val = p.first;
    int u = p.second;
    
    if(max_dist[u] != -1) continue; //we can safely continue here, since we started with the max (priority queue)
    max_dist[u] = val;
    
    for(auto next : edges[u]){
      if(max_dist[next.first] == -1) Q.push({min(next.second, val), next.first});
    }
  }
  
  int m; cin >> m;
  for(int i = 0; i < m;i++) {
    int x,y; double d; cin >> x >> y >> d;
    Point p = Point(x,y);
    // Already infected at starting position?
    double dist = CGAL::squared_distance(p, t.nearest_vertex(p)->point());
    if(d > dist) {
      cout << "n";
      continue;
    }
    auto f = t.locate(p); //locates to which triangle the point p belongs
    auto idx = t.is_infinite(f) ? 0 : f->info();
    //length of an edge (which is distance between two infected people) has to be >= sqrt(d)
    //-> squared distance has to be <= (2sqrt(d))^2 = 4d
    if(4*d > max_dist[idx]) cout << "n";
    else cout << "y";
    
  }
  cout << endl;
}

int main(){
  int n; cin >> n;
  while(true){
    if(n==0) break;
    solve(n);
    cin >> n;
  }
}
