#include <iostream>
#include <numeric>
#include <cmath>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int round_down(const CGAL::Quotient<ET> &input_val)
{
  double num = std::floor(CGAL::to_double(input_val));
  while (num > input_val) num -= 1;
  while (num+1 <= input_val) num += 1;
  return static_cast<int>(num);
}

template<typename Iter_T>
int vectorNorm(Iter_T first, Iter_T last) {
  return (int) sqrt(inner_product(first, last, first, 0.0L));
}

void solve(int n, int d){
  
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  int a_i, b;
  vector<int> a(d);
  for(int i=0; i<n; ++i){
    for(int j=0; j<d; ++j){
      cin >> a_i;
      a[j] = a_i;
      lp.set_a(j, i, a_i);
    }
    lp.set_a(d, i, vectorNorm(begin(a), begin(a)+d));
    cin >> b;
    lp.set_b(i, b);
  }
  
  lp.set_l(d, true, 0);
  lp.set_c(d, -1);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  if(s.is_infeasible()) cout << "none" << endl;
  else if(s.is_unbounded()) cout << "inf" << endl;
  else cout << round_down(-s.objective_value()) << endl;
}

int main(){
  int n, d; cin >> n;
  while(n){
    cin >> d;
    solve(n, d);
    cin >> n;
  }
}
