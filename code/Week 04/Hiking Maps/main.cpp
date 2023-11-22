#include <iostream>
#include <vector>
#include <limits>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel EK;
typedef EK::Point_2 P;
typedef EK::Triangle_2 T;
typedef EK::Line_2 L;

using namespace std;

bool point_is_inside_triangle(const P &p, const L &l1, const L &l2, const L &l3){
  return not (l1.has_on_positive_side(p) 
          || l2.has_on_positive_side(p) 
          || l3.has_on_positive_side(p));
}
void solve(){
  int m, n; cin >> m >> n;
  //read in hiking path in form of the leg points
  vector<P> leg_points;
  int x, y;
  for(int i=0; i<m; ++i){
    cin >> x >> y;
    leg_points.push_back(P(x,y));
  }
  //read in triangle edge points
  //build triangle by intersecting the three lines from the edge points (--> exact construction)
  //map[t_i][l_j] = true iff triangle t_i fully contains leg l_j
  int q0x, q0y, q1x, q1y, q2x, q2y, q3x, q3y, q4x, q4y, q5x, q5y;
  vector<vector<bool>> triangle_contains_leg(n, vector<bool> (m, true));
  for(int i=0; i<n; ++i){
    cin >> q0x >> q0y >> q1x >> q1y >> q2x >> q2y >> q3x >> q3y >> q4x >> q4y >> q5x >> q5y;
    P q0 = P(q0x, q0y); P q1 = P(q1x, q1y);
    P q2 = P(q2x, q2y); P q3 = P(q3x, q3y);
    P q4 = P(q4x, q4y); P q5 = P(q5x, q5y);
    
    //make sure that inside the triangle always means that the querried point
    // lies on the positive side of each line --> make sure that the lines are in right turns
    //--> no need for explicit construction of vertex points --> can use EPIC kernel
    if(CGAL::left_turn(q0, q1, q2)){
      swap(q0, q1);
    }
    if(CGAL::left_turn(q2, q3, q4)){
      swap(q2, q3);
    }
    if(CGAL::left_turn(q4, q5, q1)){
      swap(q4, q5);
    }
    
    L l1 = L(q0, q1);
    L l2 = L(q2, q3);
    L l3 = L(q4, q5);
    
    for(int j=0; j<m-1; ++j){
      triangle_contains_leg[i][j] = point_is_inside_triangle(leg_points[j], l1, l2, l3) 
                                    && point_is_inside_triangle(leg_points[j+1], l1, l2, l3);
    }
    
    //get triangle vertices by intersecting the three lines --> Need EPEC kernel!
    //--> gives
    //const auto o_1 = CGAL::intersection(l1, l2);
    //const auto o_2 = CGAL::intersection(l2, l3);
    //const auto o_3 = CGAL::intersection(l3, l1);
    //const P vertex_1 = *boost::get<P>(&*o_1);
    //const P vertex_2 = *boost::get<P>(&*o_2);
    //const P vertex_3 = *boost::get<P>(&*o_3);
    
    //T t = T(vertex_1, vertex_2, vertex_3);
    
    //for (int j=0; j<m-1; ++j){
    //  triangle_contains_leg[i][j] = !t.has_on_unbounded_side(leg_points[j]) 
    //                                && !t.has_on_unbounded_side(leg_points[j+1]);
    //}
  }
  
  //sliding window to find smallest intervall of triangles that fully contains the hiking map
  int shortest_interval = numeric_limits<int>::max();
  int legs_in_window_cnt = 0;
  int left = 0; int right = 0;
  vector<int> leg_in_window(m-1, 0);
  while(true){
    if (legs_in_window_cnt == m-1){
      for(int j=0; j<m-1; ++j){
        if(triangle_contains_leg[left][j] && --leg_in_window[j] == 0){
          --legs_in_window_cnt;
        }
      }
      ++left;
    }
    else{
      if (right == n){
        break;
      }
      for(int j=0; j<m-1; ++j){
        if(triangle_contains_leg[right][j] && ++leg_in_window[j] == 1){
          ++legs_in_window_cnt;
        }
      }
      ++right;
    }//else
    
    if (legs_in_window_cnt == m-1){
      shortest_interval = min(shortest_interval, right - left);
    }
  }//while
  
  cout << shortest_interval << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    solve();
  }
}
