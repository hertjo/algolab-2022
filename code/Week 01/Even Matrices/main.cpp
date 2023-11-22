#include <iostream>
#include <vector>

using namespace std;

void solve(){
  int n; cin >> n;
  vector<vector<int>> pM(n+1, vector<int>(n+1, 0));
  
  for(int i=1; i<=n; i++) {
    for(int j=1; j<=n; j++){
      int xij; cin >> xij;
      pM[i][j] = pM[i-1][j] + pM[i][j-1] - pM[i-1][j-1] + xij;
    }
  }

  int cnt= 0;

  for(int i=1; i<=n; i++) {
    for(int i2=i;  i2<=n; i2++) {
      int odd_cnt = 0;
      
      for(int x=1; x<=n; x++) {
        int sum = pM[i2][x] - pM[i-1][x];
        if (sum & 1) ++odd_cnt;
      }
      
      int even_cnt = n - odd_cnt;
      cnt += (even_cnt*(even_cnt-1))/2 + even_cnt + (odd_cnt*(odd_cnt-1))/2;
    }
  }
  cout << cnt << endl;
}

int main() {
  int t; cin >> t;
  while(t--) {
    solve();
  }
}
