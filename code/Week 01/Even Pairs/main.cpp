#include <iostream>
#include <vector>

using namespace std;

void solve(){
  int n; cin >> n;
  int E=0;
  int O=0;
  vector<int> S(n);
  //calculate partial sums
  for (int i=0; i<n; ++i){
    int x; cin >> x;
    S[i] = S[i-1] + x;
    if (S[i] % 2 == 0) ++E;
  }
  O = n - E;
  //closed form solution
  cout << E*(E-1)/2 + O*(O-1)/2 + E << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    solve();
  }
}
