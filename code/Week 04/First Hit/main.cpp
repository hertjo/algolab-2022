#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <limits>

typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef EK::Point_2 P;
typedef EK::Segment_2 S;
typedef EK::Line_2 L;
typedef EK::Ray_2 R;

using namespace std;

int64_t round_down(const EK::FT& x)
{
 double a = floor(CGAL::to_double(x));
 while (a > x) a -= 1;
 while (a+1 <= x) a += 1;
 return static_cast<int64_t>(a);
}

S get_ray_segment(P intersection, P ray_source) {
  S seg = S(ray_source, intersection);
  return seg;
}

void solve(int n){
  
  //read in ray
  long x, y, a, b; cin >> x >> y >> a >>b;
  P r1 = P(x, y);
  P r2 = P(a, b);
  R r = R(r1, r2);
  //read in segments
  vector<S> segments;
  for(int i=0; i<n; ++i){
    cin  >> x >> y >> a >> b;
    P s1 = P(x, y);
    P s2 = P(a, b);
    segments.push_back(S(s1, s2));
  }
  
  //shuffle segment vector to min worst case scenario (last 15 points)
  //worst case: all segments lie on ray and are ordered from the farest to the closest..
  random_shuffle(begin(segments), begin(segments)+n);
  //find any hit and cut ray
  bool hit_flag = false;
  S cutted_ray;
  int iter_first_hit = 0;
  for(int i = 0; i<n; ++i){
    if (CGAL::do_intersect(r, segments[i])){
      auto o = CGAL::intersection(r, segments[i]);
      P intersection_point;
      if (const S* os = boost::get<S>(&*o)){
        if(CGAL::squared_distance(r1, os->source()) < CGAL::squared_distance(r1, os->target()))
          intersection_point = os->source();
        else
          intersection_point = os->target();
      }
      else if (const P* op = boost::get<P>(&*o)){
        intersection_point = *op;
      }
      cutted_ray = get_ray_segment(intersection_point, r1);
      hit_flag = true;
      iter_first_hit = i;
      break;
    }
  }
  
  //no hit at all?
  if (!hit_flag){
    cout << "no" << endl;
    return;
  }
  
  //use cutted ray to eventually find first hit
  //iteratively cut ray segment, target of final ray segment then is first hit
  for(int j = iter_first_hit; j < n; ++j){
    if (CGAL::do_intersect(cutted_ray, segments[j])){
      auto o = CGAL::intersection(r, segments[j]);
      P intersection_point;
      if (const S* os = boost::get<S>(&*o)){
        if(CGAL::squared_distance(r1, os->source()) < CGAL::squared_distance(r1, os->target()))
          intersection_point = os->source();
        else
          intersection_point = os->target();
      }
      else if (const P* op = boost::get<P>(&*o)){
        intersection_point = *op;
      }
      cutted_ray = get_ray_segment(intersection_point, r1);
    }
  }
  
  cout << round_down(cutted_ray.target().x()) << " " << round_down(cutted_ray.target().y()) << endl;
}

int main(){
  int n; cin >> n;
  while(n > 0){
    solve(n);
    cin >> n;
  }
}
