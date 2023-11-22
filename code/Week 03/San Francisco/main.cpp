#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;


struct canal {
  int v;
  int p;
};

void solve() {
  int n; cin >> n;
  int m; cin >> m;
  long x; cin >> x;
  int k; cin >> k;
  vector<vector<canal>> canalsys(n, vector<canal>());
  
  //read in all canals in 1D canal list as neighbor list
  int u, v, p;
  for (int i = 0; i < m; ++i) {
    cin >> u; cin >> v; cin >> p;
    canalsys[u].push_back(canal({v, p}));
  }
  //Duplicate the neighborhood of vertex 0 for each Weayaya vertex
  for (int u = 0; u < n; ++u) {
    if (canalsys[u].size() == 0) {
      for (int j = 0; j < canalsys[0].size(); ++j) {
        canalsys[u].push_back(canalsys[0][j]);
      }
    }
  }
  
  vector<long> DP(n, -42);
  DP[0] = 0;
  int k_best = -42;
  //simulate max game score at each vertex for k moves
  // for canal starting from u, going to v with score p we get the recurrence:
  // DP[v] = max(DP[v], DP[u] + p)
  for (int i = 0; i < k; ++i) {
    vector<long> DPNext(n, -42);
    
    for (int u = 0; u < n; ++u) {
      if (DP[u] != -42) {
        for (int j = 0; j < canalsys[u].size(); ++j) {
          DPNext[canalsys[u][j].v] = max(DPNext[canalsys[u][j].v], DP[u] + canalsys[u][j].p);
          if (k_best == -42 && DPNext[canalsys[u][j].v] >= x) {
            k_best = i; break;
          }
        }
      }
    }
    //update DP table
    DP = DPNext;
  }
  if (k_best == -42) cout << "Impossible" << endl;
  else cout << k_best + 1 << endl;
}

int main() {
  int t; cin >> t;
  while(t--){
    solve();
  }
}
