#include <iostream>
#include <vector>
#include <limits>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

struct poster{
  int x;
  int y;
};

bool hit_vertically(const poster &p1, const poster &p2, int w, int h){
  return ET(2*abs(p1.x - p2.x), w) >= ET(2*abs(p1.y - p2.y), h);
}

double round_up(const CGAL::Quotient<ET> &x)
{
 double a = std::ceil(CGAL::to_double(x));
 while (a < x) a += 1;
 while (a-1 >= x) a -= 1;
 return a;
}

void solve(){
  int n, m, h, w, x, y;
  cin >> n >> m >> h >> w;
  vector<poster> nP(n);
  vector<poster> oP(m);
  for(int i=0; i<n; ++i){
    cin >> x >> y;
    nP[i] = {x, y};
  }
  for(int i=0; i<m; ++i){
    cin >> x >> y;
    oP[i] = {x, y};
  }
  
  Program lp (CGAL::SMALLER, true, 1, false, 0);
  int constr = -1;
  for(int i=0; i<n; ++i){
    
    for(int j=i+1; j<n; ++j){
      ++constr;
      if(hit_vertically(nP[i], nP[j], w, h)){
        lp.set_a(i, constr, ET(w, 2)); lp.set_a(j, constr, ET(w, 2)); 
        lp.set_b(constr, abs(nP[i].x - nP[j].x));
      }
      else{
        lp.set_a(i, constr, ET(h, 2)); lp.set_a(j, constr, ET(h, 2)); 
        lp.set_b(constr, abs(nP[i].y - nP[j].y));
      }
    }
    
    ET tightest_rhs = ET(numeric_limits<double>::max());
    ET rhs;
    for(int j=0; j<m; ++j){
      rhs = max(ET(2 * abs(nP[i].x - oP[j].x) - w, w), ET(2 * abs(nP[i].y - oP[j].y) - h, h));
      if(rhs < tightest_rhs){
        tightest_rhs = rhs;
      }
    }
    lp.set_u(i, true, tightest_rhs);
    
    lp.set_c(i, -2*(w+h));
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  cout << long(round_up(-s.objective_value())) << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    solve();
  }
}
