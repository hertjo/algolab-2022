#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel EK;
typedef EK::Point_2 P;
typedef EK::Segment_2 S;
typedef EK::Ray_2 R;

void solve(int n){
  long x, y, a, b; cin >> x >> y >> a >> b;
  //read in ray
  P r1 = P(x, y);
  P r2 = P(a, b);
  R ray = R(r1, r2);
  
  //read in segments
  vector<S> segments;
  while(n--){
    cin >> x >> y >> a >> b;
    P s1 = P(x, y);
    P s2 = P(a, b);
    segments.push_back(S(s1, s2));
  }
  
  for (auto segment: segments){
    if(CGAL::do_intersect(ray, segment)){
      cout << "yes" << endl;
      return;
    }
  }
  cout << "no" << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while (n>0){
    solve(n);
    cin >> n;
  }
}
