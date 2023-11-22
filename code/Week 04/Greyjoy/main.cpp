#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct waterway {
  int l;
  vector<int> islands;
};
void solve(){
  int n, k, w; cin >> n >> k >> w;
  int c;
  vector<int> men(n);
  for(int i=0; i<n; ++i){
    cin >> c;
    men[i] = c;
  }
  int l, ri;
  vector<waterway> waterways(w);
  for(int i=0; i<w; ++i){
    cin >> l;
    vector<int> r(l);
    for(int j=0; j<l; ++j){
      cin >> ri;
      r[j] = ri;
    }
    waterways[i] = {l, r};
  }
  
  //sliding window over all waterways individually
  //find best solution that is constrained to be on a single waterway only
  int max_cnt = 0;
  for(int i=0; i<w; ++i){
    int left = 0; //inclusive
    int right = 0; //exclusive
    int curr_sum = 0;
    while(true){
      int curr_cnt = right - left;
      if(curr_sum == k){
        max_cnt = max(max_cnt, curr_cnt);
        curr_sum -= men[waterways[i].islands[left++]];
      }
      else if(curr_sum < k){
        if(right == waterways[i].l) break;//since right is exclusive
        curr_sum += men[waterways[i].islands[right++]];
      }
      else{
        curr_sum -= men[waterways[i].islands[left++]];
      }
    }
  }
  
  //check whether better solution exists when combining two waterways over Pyke
  //use hashmap as lookup table with entries (#men_along_path, #islands_along_path)
  unordered_map<int, int> max_cnt_combined;
  for(int i=0; i<w; ++i){
    vector<int> path_sums(1,0);
    int len = waterways[i].l;
    for(int j=1, sum=0; j<len; ++j){
      //precompute mencounts starting after Pyke along the waterway i
      //stop when sum is bigger than ironmen count
      sum += men[waterways[i].islands[j]];
      if(sum >= k) break;
      path_sums.push_back(sum);
    }
    //use hashmap to lookup whether a path along waterway i
    //can be combined over Pyke s.t. the total mencount is exactly k
    //update max_cnt if necessary
    for(int j=1; j<(int)path_sums.size(); ++j){
      int cnt_missing = k - path_sums[j] - men[0];
      auto iter = max_cnt_combined.find(cnt_missing);
      if(iter != end(max_cnt_combined)) max_cnt = max(max_cnt, iter->second + j + 1);
    }
    //only insert path_sum if the respective #islands is atriclty
    //bigger than any other island_count that is matching path_sum
    for(int j=1; j<(int)path_sums.size(); ++j){
      auto iter = max_cnt_combined.find(path_sums[j]);
      if (iter != end(max_cnt_combined) && iter->second < j){
        iter->second = j;
      }
      else{
        max_cnt_combined.emplace(path_sums[j], j);
      }
    }
  }
  
  cout << max_cnt << endl;
  
}

int main(){
  int t; cin >> t;
  while(t--){
    solve();
  }
}
