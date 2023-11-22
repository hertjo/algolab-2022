#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;
void solve(){
  int n; cin >> n;
  int m; cin >> m;
  int r; cin >> r;
  int b; cin >> b;
  
  //read in transition edges in adjacency list
  vector<vector<int>> adjacent(n, vector<int>());
  int temp_u, temp_v;
  for(int i=0; i<m; ++i){
    cin >> temp_u; cin >> temp_v;
    adjacent[temp_u - 1].push_back(temp_v - 1); 
  }
  //maintain two 1D tables:
  //minMoves: #moves when S plays red/ M plays black
  //maxMoves: #moves when S plays black/ M plays red
  vector<int> minMoves(n, numeric_limits<int>::max());
  vector<int> maxMoves(n, numeric_limits<int>::min());
  minMoves[n-1] = 0;
  maxMoves[n-1] = 0;
  for(int i=n-1; i>=0; --i){
    for(int j:adjacent[i]){
      minMoves[i] = min(minMoves[i], maxMoves[j] + 1);
      maxMoves[i] = max(maxMoves[i], minMoves[j] + 1);
    }
  }
  
  if (minMoves[r-1] < minMoves[b-1]) cout << 0 << endl;
  else if (minMoves[r-1] > minMoves[b-1]) cout << 1 << endl;
  else if (minMoves[r-1] & 1) cout << 0 << endl;
  else cout << 1 << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    solve();
  }
}
