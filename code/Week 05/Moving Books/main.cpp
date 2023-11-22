#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void solve(){
  int n, m; cin >> n >> m;
  vector<int> friend_strength(n);
  vector<int> box_weight(m);
  int s;
  for(int i=0; i<n; ++i){
    cin >> s;
    friend_strength[i] = s;
  }
  int w;
  for(int i=0; i<m; ++i){
    cin >> w;
    box_weight[i] = w;
  }
  //use reverse iterator to sort in descending order
  sort(friend_strength.rbegin(), friend_strength.rend());
  sort(box_weight.rbegin(), box_weight.rend());
  
  //problem is impossible to solve if heaviest box is heavier than strongest friend
  if(box_weight[0] > friend_strength[0]){
    cout << "impossible" << endl;
    return;
  }
  
  
  //binary search
  //Is there a better way to find optimal value ?? gets full points though..
  int l = (m-1)/n + 1;
  int r = m;
  
  while(l < r) {
    int middle = (l+r)/2;
    int friends_used = (m-1)/middle + 1;

    bool possible = true;
    for(int i = 0; i < friends_used; i++) {
      if (friend_strength[i] < box_weight[i*middle]) {
        possible = false;
        break;
      }
    }
    if(possible) r = middle;
    else l = middle+1;
  }
  
  cout << 3*l - 1 << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while(t--){
    solve();
  }
}
