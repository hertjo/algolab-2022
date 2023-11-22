#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

void solve(){
  int n; cin >> n;
  int cnt = 1;

  for(int i=0; i<n; ++i){
    int h; cin >> h;
    if(cnt <= i) {
      while(i < n-1){
        cin >> h;
        ++i;
      }
      break;
    }
    cnt = max(cnt, i + h);
  }
  
  cout << min(cnt, n) << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    solve();
  }
}
