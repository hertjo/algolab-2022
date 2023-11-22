#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

struct food{
  int p;
  vector<int> C;
};

struct nutrient{
  int min;
  int max;
};

int round_down(const CGAL::Quotient<ET> &input_val)
{
  double num = std::floor(CGAL::to_double(input_val));
  while (num > input_val) num -= 1;
  while (num+1 <= input_val) num += 1;
  return static_cast<int>(num);
}

void solve(int n, int m){
  //read in all nutrients
  int min, max;
  vector<nutrient> nutrients(n);
  for(int i=0; i<n; ++i){
    cin >> min >> max;
    nutrients[i] = {min, max};
  }
  //read in all foods
  int p, c;
  vector<food> foods(m);
  vector<int> C(n, 0);
  for(int i=0; i<m; ++i){
    cin >> p;
    foods[i] = {p, C};
    for(int j=0; j<n; ++j){
      cin >> c;
      foods[i].C[j] = c;
    }
  }
  
  //construct linear minimization problem
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  //set the coefficients of A and b
  for(int i=0; i<n; ++i){
    for(int j=0; j<m; ++j){
      lp.set_a(j, i, foods[j].C[i]);
      lp.set_a(j, n+i, -foods[j].C[i]);
    }
    lp.set_b(i, nutrients[i].max);
    lp.set_b(n+i, -nutrients[i].min);
  }
  //objective function
  for(int j=0; j<m; ++j){
    lp.set_c(j, foods[j].p);
  }
  
  //solve; the only two result types are infeasible and optimal
  //unbounded is not possible because we are minimizing and set a lower bound of 0 for all variables
  Solution s = CGAL::solve_linear_program(lp, ET());
  if(s.is_infeasible()) cout << "No such diet." << endl;
  else cout << round_down(s.objective_value()) << endl;
  
}

int main(){
  int n, m; cin >> n >> m;
  
  while(not (n == 0 && m == 0)){
    solve(n, m);
    cin >> n >> m;
  }
}
