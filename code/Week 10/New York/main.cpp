#include <bits/stdc++.h>

using namespace std;

vector<vector<int>> adjacency_list;
vector<int> temp;//temp at each c.p.
set<int> s;//sorted array to store c.p. with feasible paths fcoming from it
multiset<int> values; //sorted array to store the temps along a path
deque<int> path;//queue that stores cps along a path
vector<int> p;//store incoming node for every node in p:
unsigned int n, m, k;//make them global and thank past-me later..

void dfs(int idx){
  //check if current path is feasible
  if(path.size() == m){
    int minTemp = *values.begin();
    int maxTemp = *values.rbegin();
    
    if(maxTemp - minTemp <= k){
      s.insert(path[0]);
    }
  }
  //if not feasible, search via dfs over all reachable c.p.s from idx
  for(int next : adjacency_list[idx]){
    path.push_back(next);
    values.insert(temp[next]);
    
    if(path.size() > m){
      //delete first element along path and remove its temperature in values
      values.erase(values.find(temp[path[0]]));
      path.pop_front();
    }
    
    dfs(next);
    
    //if first element along path is not New York:
    //make its unique incoming node the new beginning of the path
    //in the callbacks of all dfs(next) calls, this will eventually restore the path before the call
    if(path[0] != 0){
      int p_idx = p[path[0]];
      path.push_front(p_idx);
      values.insert(temp[p_idx]);
    }
    //before considering other neighbors of node idx, remove node next from the path
    values.erase(values.find(temp[next]));
    path.pop_back();
  }
}

void solve(){
  cin >> n >> m >> k;
  temp = vector<int>(n);
  for(int i=0; i<n; ++i){
    cin >> temp[i];
  }
  adjacency_list = vector<vector<int>>(n, vector<int>(0));
  p = vector<int>(n,0);
  int u, v;
  for(int i=0; i<n-1; ++i){
    cin >> u >> v;
    adjacency_list[u].push_back(v);
    //as we have an oriented rooted tree, the incoming node will be unique (for the root we fix it to 0)
    p[v] = u;
  }
  s = set<int>();
  values = multiset<int>();
  path = deque<int>(0);
  path.push_back(0);
  values.insert(temp[0]);
  
  dfs(0);
  
  //retrieve feasible paths from s
  if(s.empty()) cout << "Abort mission";
  else{
    for(int cp : s) cout << cp << " ";
  }
  cout << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
