#include <bits/stdc++.h>

#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include<CGAL/Delaunay_triangulation_2.h>
#include<CGAL/QP_models.h>
#include<CGAL/QP_functions.h>
#include<CGAL/Gmpz.h>

#define int long //as I do not trust myself anymore when debugging...

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef CGAL::Point_2<K> Point;
typedef int IT;
typedef CGAL::Gmpz ET; //Working with Gmpz is faster, as we are dealing with decimal numbers, just multiply relevant parts by 100
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int floor_to_int(const CGAL::Quotient<CGAL::Gmpz>& x)
{
 double a = std::floor(CGAL::to_double(x));
 while (a > x) a -= 1;
 while (a+1 <= x) a += 1;
 return a;
}

struct warehouse{
  Point p;
  int s;
  int a;
};

struct stadium{
  Point p;
  int d;
  int u;
};

struct contour{
  Point center;
  int r;
};

void solve(){
  
  int n, m, c; cin >> n >> m >> c;
  vector<Point> union_w_s(n+m);
  vector<warehouse> w(n);
  vector<stadium> s(m);
  vector<vector<int>> r(n, vector<int>(m));
  vector<contour> l(c);
  for(int i=0; i<n; ++i){
    int x, y, s, a; cin >> x >> y >> s >> a;
    union_w_s[i] = Point(x,y);
    w[i] = {Point(x,y), s, a};
  }
  for(int i=0; i<m; ++i){
    int x, y, d, u; cin >> x >> y >> d >> u;
    union_w_s[n+i] = Point(x,y);
    s[i] = {Point(x,y), d, u};
  }
  for(int i=0; i<n; ++i){
    for(int j=0; j<m; ++j){
      cin >> r[i][j];
    }
  }
  for(int i=0; i<c; ++i){
    int x,y,r; cin >> x >> y >> r;
    l[i] = {Point(x,y), r};
  }
  
  Triangulation t; 
  t.insert(union_w_s.begin(), union_w_s.end());
  
  //store all contour lines that contain at least one warehouse or stadium (at most 100 by assumption)
  vector<contour> contours(0);
  for(auto ll : l){
    Point c = ll.center;
    if(CGAL::squared_distance(c, t.nearest_vertex(c)->point()) < ll.r*ll.r)
      contours.push_back(ll);
  }
  
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  int eq_idx = 0;
  //supply constraints
  for(int i=0; i<n; ++i){
    for(int j=0; j<m; ++j) lp.set_a(i*m + j, eq_idx, 1);
    lp.set_b(eq_idx++, w[i].s);
  }
  //demand (=) and upper limit alcohol constraints
  for(int j=0; j<m; ++j){
    for(int i=0; i<n; ++i) lp.set_a(i*m + j, eq_idx, 1);
    lp.set_b(eq_idx++, s[j].d);
    for(int i=0; i<n; ++i) lp.set_a(i*m + j, eq_idx, -1);
    lp.set_b(eq_idx++, -s[j].d);
    for(int i=0; i<n; ++i) lp.set_a(i*m + j, eq_idx, w[i].a);
    lp.set_b(eq_idx++, s[j].u*100); //make sure to be able to work with Gmpz!
  }
  //objective function (-1h lifetime bruv)
  for(int i=0; i<n; ++i){
    for(int j=0; j<m; ++j){
      int profit_ij = 100*r[i][j];
      //now check all relevant contour lines, there are only <=100 of them
      for(auto c : contours){
        int d_w = CGAL::squared_distance(w[i].p, c.center);
        int d_s = CGAL::squared_distance(s[j].p, c.center);
        if(d_w < c.r*c.r && d_s > c.r*c.r) profit_ij--;
        if(d_s < c.r*c.r && d_w > c.r*c.r) profit_ij--;
      }
      lp.set_c(i*m + j, -profit_ij); //we want to maximize!
    }
  }
  
  Solution sol = CGAL::solve_linear_program(lp, ET());
  if(sol.is_infeasible()) cout << "RIOT!" << endl;
  else cout << floor_to_int(-sol.objective_value() / 100) << endl; // Do not forget that we were working with Gmpz -> divide by 100
  
}

signed main(){
  int t; cin >> t;
  while(t--) solve();
  return 0;
}
