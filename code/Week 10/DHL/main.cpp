#include<bits/stdc++.h>

using namespace std;

void solve(){
  int n; cin >> n;
  vector<int> a_psum(n+1, 0);
  vector<int> b_psum(n+1, 0);
  for(int i=1; i<=n; ++i) cin >> a_psum[i];
  for(int i=1; i<=n; ++i) cin >> b_psum[i];
  partial_sum(a_psum.begin(), a_psum.end(), a_psum.begin());
  partial_sum(b_psum.begin(), b_psum.end(), b_psum.begin());
  
  vector<vector<int>> DP(n+1, vector<int>(n+1, INT_MAX));
  DP[0][0] = 0;
  for(int i=1; i<=n; ++i){
    for(int j=1; j<=n; ++j){
      for(int k=i-1; k>=0; --k){
        for(int l=j-1;l>=0; --l){
          if(DP[k][l] != INT_MAX){
            DP[i][j] = min(DP[i][j], DP[k][l] + (a_psum[i]-a_psum[k] - i+k)*(b_psum[j]-b_psum[l] - j+l));
          }
          if((a_psum[i]-a_psum[k] - i+k)*(b_psum[j]-b_psum[l] - j+l) >= DP[i][j]) break;
        }
      }
    }
  }
  cout << DP.back().back() << endl;
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
