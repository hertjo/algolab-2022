#include<bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// input type
typedef CGAL::Gmpq IT;
// exact type for solver
typedef CGAL::Gmpq ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

//Delaunay fun
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef CGAL::Point_2<K> Point;

using namespace std;

struct asteroid{
  Point p;
  int d;
};
struct shootingPoint{
  Point p;
  double maxSR;
};

void solve(){
  
  int a, s, b; cin >> a >> s >> b;
  long e; cin >> e;
  
  vector<asteroid> particles(a);
  for(int i=0; i<a; ++i){
    int x, y, d; cin >> x >> y >> d;
    particles[i] = {Point(x,y), d};
  }
  vector<shootingPoint> shootingPoints(s);
  for(int i=0; i<s; ++i){
    int x, y; cin >> x >> y;
    shootingPoints[i] = {Point(x,y), DBL_MAX};
  }
  if(b){
    //nothing would prevent us from simply choosing infinitely large radii for each shot
    //However, since we must not be noticed by the hunters, we set a max possible square radius for
    //each shot. Hereby we use the Delaunay triangulation over hunters to detect the nearest shootingPoint
    //for each hunter. The squared distnace between this hunter and the found shootingPoint will constrain the respective radius
    vector<Point> bountyHunters(b);
    for(int i=0; i<b; ++i){
      int x, y; cin >> x >> y;
      bountyHunters[i] = Point(x,y);
    }
    
    Triangulation t;
    t.insert(bountyHunters.begin(), bountyHunters.end());
    
    for(int i=0; i<s; ++i){
      shootingPoints[i].maxSR = CGAL::squared_distance(t.nearest_vertex(shootingPoints[i].p)->point(), shootingPoints[i].p);
    }
  }
  
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  //constraint on total energy
  int cIdx = 0;
  for(int i=0; i<s; ++i){
    lp.set_a(i, cIdx, 1);
  }
  lp.set_b(cIdx++, e);
  
  //constraints to ensure each particle gets completely destroyed
  for(int i=0; i<a; ++i){
    for(int j=0; j<s; ++j){
      auto d_ij = CGAL::squared_distance(particles[i].p, shootingPoints[j].p);
      if(d_ij <= shootingPoints[j].maxSR){
        lp.set_a(j, cIdx, -ET(1, max(K::FT(1), d_ij)));
      }
    }
    lp.set_b(cIdx++, -particles[i].d);
  }
  
  Solution sol = CGAL::solve_linear_program(lp, ET());
  if(sol.is_infeasible()) cout << "n" << endl;
  else cout << "y" << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
