#include<bits/stdc++.h>

using namespace std;

//return tuple {selected,
//              covered by parent (might still be taken)
//              not covered by parent (might be taken)}

tuple<int, int, int> dfs(vector<int> &costs, vector<vector<int>> &tree, int curr){
  //if curr is a final destination (leaf)
  if(tree[curr].empty()){
    return {costs[curr], 0, costs[curr]};
  }
  int best_if_not_repaired = INT_MAX;
  int sum_if_not_repaired = 0;
  int sum_if_repaired = 0;
  for(int next : tree[curr]){
    auto result = dfs(costs, tree, next);
    sum_if_not_repaired += get<2>(result);
    sum_if_repaired += get<1>(result);
    best_if_not_repaired = min(best_if_not_repaired, get<0>(result) - get<2>(result));
  }
  
  return {costs[curr] + sum_if_repaired, 
          min(costs[curr] + sum_if_repaired, sum_if_not_repaired),
          min(sum_if_not_repaired + best_if_not_repaired, costs[curr] + sum_if_repaired)};
}

void solve(){
  int n; cin >> n;
  vector<vector<int>> tree(n, vector<int>(0));
  vector<int> costs(n);
  for(int i=0; i<n-1; ++i){
    int j, k; cin >> j >> k;
    tree[j].push_back(k);
  }
  for(int i=0; i<n; ++i) cin >> costs[i];
  
  auto result = dfs(costs, tree, 0);
  cout << get<2>(result) << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
