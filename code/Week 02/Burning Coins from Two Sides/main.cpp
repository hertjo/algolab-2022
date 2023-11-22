#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

void solve(){
  int n; cin >> n;
  vector<int> v(n);
  
  //read in coin values in v
  for(int i=0; i<n; ++i){
    int temp; cin >> temp;
    v[i] = temp;
  }
  
  //init and base case
  vector<vector<int>> DP(n,vector<int> (n,0));
  for(int i=0; i<n; ++i){
    //it's my turn
    if (n & 1) DP[i][i] = v[i];
  }
  
  //DP: fill super diagonals of 2D table from bottom right
  //to top right in a zig zag manner
  for(int i = n-1; i >= 0; --i) {
    for(int j = i+1; j < n; ++j) {
      //it's my turn
      if ((n-j+i) & 1) DP[i][j] = max(DP[i+1][j] + v[i], DP[i][j-1] + v[j]);
      ///it's my friends turn
      else DP[i][j] = min(DP[i+1][j], DP[i][j-1]);
    }
  }
  
  //optimal value is in DP[0][n-1]
  cout << DP[0][n-1] << endl;
}


int main(){
  int t; cin >> t;
  while(t--){
    solve();
  }
}
