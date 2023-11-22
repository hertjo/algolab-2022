#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

//we need sqrt kernel since we need to compute an exact sqrt of the radius
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt SK;
typedef CGAL::Min_circle_2_traits_2<SK> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

using namespace std;

double ceil_to_double(const SK::FT& x)
{
 double a = std::ceil((CGAL::to_double(x)));
 while (a-1 >= x) a -= 1;
 while (a < x) a += 1;
 return a;
}

void solve(int n){
  vector<SK::Point_2> citizens;
  long x, y;
  while(n--){
    cin >> x >> y;
    citizens.push_back(SK::Point_2(x, y));
  }
  //radius of minimum enclosing circle over all citizens is the solution
  //we ceil the result to the next integral solution
  Min_circle mc(citizens.begin(), citizens.end(), true);
  Traits::Circle c = mc.circle();
  cout << long(ceil_to_double(CGAL::sqrt(c.squared_radius()))) << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while(n > 0){
    solve(n);
    cin >> n;
  }
}
