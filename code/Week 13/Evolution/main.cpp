#include <bits/stdc++.h>

using namespace std;

struct species{
  string name;
  int age;
};

struct query {
  int b;
  int idx;
};

struct Path {
  int index;
  int age;
};

vector<vector<int>> graph;
vector<vector<query>> queries;
vector<int> solutions;
vector<species> specs;
vector<Path> path; 

void dfs(int idx){
  path.push_back({idx, specs[idx].age});
  
  for(auto q: queries[idx]){
    solutions[q.idx] = lower_bound(path.begin(), path.end(), Path{0,q.b}, [](Path a, Path b) {
      return a.age > b.age; 
      })->index;
  }
  for(auto c: graph[idx]) dfs(c);
  path.pop_back();
}

void solve(){
  int n, q; cin >> n >> q;
  
  map<string, int> spec_idx_map;
  specs = vector<species>(n);
  for(int i=0; i<n; ++i){
    string s; int a; cin >> s >> a;
    specs[i] = {s, a};
    spec_idx_map[s] = i;
  }
  
  graph = vector<vector<int>>(n, vector<int>(0));
  vector<bool> isChild(n, false);
  for(int i=0; i<n-1; ++i){
    string s, p; cin >> s >> p;
    graph[spec_idx_map[p]].push_back(spec_idx_map[s]);
    isChild[spec_idx_map[s]] = true;
  }
  int root = 0;
  for(int i=0; i<n; ++i){
    if(!isChild[i]){
      root = i;
      break;
    }
  }
  
  queries = vector<vector<query>>(n, vector<query>(0));
  solutions = vector<int>(q);
  for(int i=0; i<q; ++i){
    string s; int b; cin >> s >> b;
    queries[spec_idx_map[s]].push_back({b, i});
  }
  
  path = vector<Path>(0);
  
  dfs(root);
  
  for(auto ans: solutions) {
    cout << specs[ans].name << " ";
  }
    
  cout << endl; 
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
