#include<iostream>
#include<vector>
#include<algorithm>
#include<limits>

using namespace std;
struct edge{
  int v;
  int l;
};
struct chamber{
  int g;
};
struct rec_res{
  long c;
  long n;
  long t;
};

rec_res rec(int start, vector<chamber> &chambers, vector<vector<edge>> &tunnels){
  //if start node is a leaf, we are done with this subtree
  if(tunnels[start].size() == 0){
    return {chambers[start].g, 1, 0};
  }
  //greedy subtree recursion if start is not a leaf
  vector<rec_res> recursive_results;
  recursive_results.reserve(tunnels[start].size());
  for(auto t : tunnels[start]){
    rec_res result = rec(t.v, chambers, tunnels);
    //for each second of the traversal, we subtract one coin from the total number of coins we can collect
    //for the traversal time, add 2*t.l for traversing both directions
    recursive_results.push_back({result.c - result.n * t.l, result.n, result.t + 2 * t.l});
  }
  sort(recursive_results.begin(), recursive_results.end(), [](const rec_res &r1, const rec_res &r2) -> bool
              { return r1.t * r2.n < r2.t * r1.n; });
  long coins_cnt = chambers[start].g;
  long nodes_cnt = 0;
  long time_ellapsed = 0;
  for(auto r : recursive_results) {
      coins_cnt += r.c - time_ellapsed*r.n;
      nodes_cnt += r.n;
      time_ellapsed += r.t;
  }
  //add the identity node to the total nodes count
  nodes_cnt += 1;
  return {coins_cnt, nodes_cnt, time_ellapsed};
}

void solve(){
  //read in galleons, chambers and tunnel system
  int nn, g, u, v, l;
  cin >> nn;
  vector<chamber> chambers(nn+1);
  vector<vector<edge>> tunnels(nn+1);
  for(int i=0; i<nn; ++i){
    cin >> g;
    chambers[i+1] = chamber({g});
  }
  for(int i=0; i<nn; ++i){
    cin >> u >> v >> l;
    tunnels[u].push_back({v,l});
  }
  
  rec_res result = rec(0, chambers, tunnels);
  cout << result.c << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) solve();
}
