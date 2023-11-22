#include<bits/stdc++.h>
using namespace std;
//Approaches: Precomputation of interval sums & 2D DP
void solve(){
  int n, m, k;
  cin >> n >> m >> k;
  vector<int> vpsums(n+1, 0);
  for(int i=1; i<=n; ++i){
    cin >> vpsums[i];
  }
  partial_sum(vpsums.begin(), vpsums.end(), vpsums.begin());
  //DP[i][j] = max possible effect if we have i Hocruxes and j army members
  vector<vector<int>> DP(m+1, vector<int>(n+1, 0));
  
  for(int i=1; i<=n; ++i){
    int target = vpsums[i] - k;
    auto it = lower_bound(vpsums.begin(), vpsums.end(), target);
    int j = it - vpsums.begin();
    bool possible = *it == target ? true : false;
    
    int toAdd = i-j;
    //First member can destroy feasible interval of Hocruxes?
    if(possible) DP[1][i] = toAdd;
    DP[1][i] = max(DP[1][i], DP[1][i-1]);
    
    for(int p=2; p<=m; ++p){
      //We do not necessarily have to destroy Hocrux i
      DP[p][i] = DP[p][i-1];
      //We have a feasible interval for person p, and with first p-1 members
      //we can already destroy j Hocruxes
      if(possible && DP[p-1][j] != 0) DP[p][i] = max(DP[p][i],DP[p-1][j] + toAdd);
    }
  }
  if(DP.back().back() == 0) cout << "fail" << endl;
  else cout << DP.back().back() << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
