#include <iostream>
#include <algorithm>
#include <vector>

#include <CGAL/Gmpq.h>

using namespace std;

struct biker {
  int64_t y_0;
  int64_t x_1;
  int64_t y_1;
  int index;
};

CGAL::Gmpq slope(const biker& b){
  return CGAL::Gmpq((b.y_1 - b.y_0), b.x_1); //slope = delta y / delta x
}

bool biker_sorter( const biker& b1, const biker& b2){ return b1.y_0 < b2.y_0; }

void solve(){
  //read in all biker information
  int n; cin >> n;
  int64_t y_0, x_1, y_1;
  vector<biker> bikers(n);
  for(int i=0; i<n; ++i){
    cin >> y_0 >> x_1 >> y_1;
    bikers[i] = biker({y_0, x_1, y_1, i});
  }
  
  //sort bikers in ascending order w.r.t. y_0
  sort(begin(bikers), end(bikers), biker_sorter);
  
  //get slope vector
  vector<CGAL::Gmpq> slopes(n);
  for(int i=0; i<n; ++i){
    slopes[i] = slope(bikers[i]);
  }
  
  vector<bool> will_reach_the_sun(n, true);
  CGAL::Gmpq ultimate_g_slope = slopes[0];
  //first run to find out "most parallel" biker with regards to the x-axis
  for(int i=0; i<n; ++i){
    //if bikers have same slope, take the slope of the biker "coming from right"
    if(CGAL::abs(slopes[i]) < CGAL::abs(ultimate_g_slope) ||
       (CGAL::abs(slopes[i]) == CGAL::abs(ultimate_g_slope) && slopes[i] > 0)){
         ultimate_g_slope = slopes[i];
    }
    //if biker comes from top withsmaller slope --> it'll hit line of biker with
    // the current ultimate_g_slope
    else if(slopes[i] < ultimate_g_slope){
      will_reach_the_sun[i] = false;
    }
  }
  
  //second run now from the top to discard all remaining degenrate cases
  ultimate_g_slope = slopes[n-1];
  for(int i=n-2; i>=0; --i){
    if(CGAL::abs(slopes[i]) <= CGAL::abs(ultimate_g_slope)){
      ultimate_g_slope = slopes[i];
    }
    else if(slopes[i] > ultimate_g_slope){
      will_reach_the_sun[i] = false;
    }
  }
  
  vector<int> indices;
  for(int i=0; i<n; ++i){
    if(will_reach_the_sun[i]) indices.push_back(bikers[i].index);
  }
  sort(begin(indices), end(indices));
  
  for(int i=0; i<indices.size(); ++i){
    cout << indices[i] << " ";
  }
  cout << endl;
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    solve();
  }
}
