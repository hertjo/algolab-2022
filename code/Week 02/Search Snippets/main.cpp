#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

int word_cnt = 0;
void solve(){
  int n; cin >> n;
  vector <int> m(n);
  for(int i=0; i<n; ++i){
    int m_i; cin >> m_i;
    word_cnt += m_i;
    m[i] = m_i;
  }
  
  vector<pair<int,int>> p;
  for(int i=0; i<(int)m.size(); ++i){
    for(int j=0; j< m[i]; ++j){
      int p_ij; cin >> p_ij;
      p.emplace_back(p_ij, i);
    }
  }
  
  //sort words accoring to their positions in the document
  sort(begin(p), end(p), [&](const auto &q, const auto &r){
    return q.first < r.first;
  });
  
  vector<int> counts(n, 0);
  int words_missing_in_interval = n;
  int opt_interval_size = p.back().first;
  int a = 0;
  
  for(int b=0; b<(int)p.size(); ++b){
    //move back pointer until all required words are in the interval [a,b]
    if(!counts[p[b].second]){
      --words_missing_in_interval;
    }
    ++counts[p[b].second];
    //now move forward pointer until one required word is missing again in the interval [a,b]
    while(!words_missing_in_interval){
      int curr_interval_size = p[b].first - p[a].first + 1;
      opt_interval_size = min(opt_interval_size, curr_interval_size);
      --counts[p[a].second];
      if(!counts[p[a].second]){
        ++words_missing_in_interval;
      }
      ++a;
    }
    
  }
  
  cout << opt_interval_size << endl;
}


int main(){
  int t; cin >> t;
  while(t--){
    solve();
  }
}
