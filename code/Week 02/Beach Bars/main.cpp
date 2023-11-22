#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

void solve(){
  
  //read in locations and sort them along the real line
  int n; cin >> n;
  vector<int> coords(n);
  int temp;
  for(int i=0; i<n; ++i){
    cin >> temp;
    coords[i] = temp;
  }
  sort(begin(coords), end(coords));

  int l=0;
  vector<int> bars;
  int max_dist_opt = numeric_limits<int>::max();
  int bar_count_opt = 0;
  
  //Sliding window
  for(int r=0; r<n; ++r){
    while(abs(coords[r] - coords[l]) > 200) ++l;
    int range = coords[r] - coords[l];
    int max_dist = (range + 1) / 2; //integer divide
    
    if(r-l+1 > bar_count_opt){
      bars.clear();
      bar_count_opt = r-l+1;
      max_dist_opt = max_dist;
      
      //make sure to add ALL possible locations
      //when range is an odd number, there exist two possibilities, so we add both of them
      if(range & 1) bars.push_back(coords[l] + max_dist - 1);
      bars.push_back(coords[l] + max_dist);
    }
    else if(r-l+1 == bar_count_opt && max_dist <= max_dist_opt){
      if(max_dist < max_dist_opt){
        bars.clear();
        max_dist_opt = max_dist;
      }
      if (range & 1) bars.push_back(coords[l] + max_dist - 1);
      bars.push_back(coords[l] + max_dist);
    }
  }
  
  cout << bar_count_opt << " " << max_dist_opt << endl;
  for (int bar : bars){
    cout << bar << " ";
  }
  cout << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    solve();
  }
}
