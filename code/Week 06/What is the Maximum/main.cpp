#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int round_down(const CGAL::Quotient<ET> &input_val)
{
  double num = std::floor(CGAL::to_double(input_val));
  while (num > input_val) num -= 1;
  while (num+1 <= input_val) num += 1;
  return static_cast<int>(num);
}

int round_up(const CGAL::Quotient<ET> &input_val)
{
  double num = std::ceil(CGAL::to_double(input_val));
  while (num < input_val) num += 1;
  while (num-1 >= input_val) num -= 1;
  return static_cast<int>(num);
}

void solve_program_1(int a, int b){
  
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  
  const int X = 0;
  const int Y = 1;
  
  lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
  lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);
  lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);
  
  lp.set_c(X, a); lp.set_c(Y, -b);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  if (s.is_unbounded()) cout << "unbounded" << endl; 
  else if (s.is_infeasible()) cout << "no" << endl;
  else cout << round_down(-s.objective_value()) << endl;

}

void solve_program_2(int a, int b){
  
  Program lp (CGAL::LARGER, false, 0, true, 0);
  
  const int X = 0;
  const int Y = 1;
  const int Z = 2;
  
  lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, -4);
  lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a*b);
  lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, -1);
  
  lp.set_c(X, a); lp.set_c(Y, b); lp.set_c(Z, 1);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  if (s.is_unbounded()) cout << "unbounded" << endl; 
  else if (s.is_infeasible()) cout << "no" << endl;
  else cout << round_up(s.objective_value()) << endl;
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int p, a, b;
  cin >> p;
  while(p){
    cin >> a >> b;
    if(p == 1) solve_program_1(a, b);
    else solve_program_2(a, b);
    cin >> p;
  }
}
