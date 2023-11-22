#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <tuple>

using namespace std;

struct ball{
  int t;
  int index;
};

bool ball_sorter(const ball &b1, const ball &b2){
  return b1.t < b2.t;
}

int defuse(int n, int curr_time, vector<int> &times, int j, vector<bool> &is_defused) {
  int t_j = times[j];
  if(j >= (n - 1) / 2) { 
    if(curr_time < t_j) {
      is_defused[j] = true;
      return curr_time + 1;
    } else {
      return -1;
    }
  } else {
    int b1 = 2 * j + 1;
    int b2 = 2 * j + 2;
    if(!is_defused[b1]) {
      curr_time = defuse(n, curr_time, times, b1, is_defused);
    }
    if(curr_time == -1) {
      return -1;
    }
    if(!is_defused[b2]) {
      curr_time = defuse(n, curr_time, times, b2, is_defused);
    }
    if(curr_time == -1 || curr_time >= t_j) {
      return -1;
    } else {
      is_defused[j] = true;
      return curr_time + 1;
    }
  }
}

void solve(){
  int n; cin >> n;
  int t;
  vector<ball> balls(n);
  vector<int> times(n);
  vector<bool> deactivated(n, false);
  for(int i=0; i<n; ++i){
    cin >> t;
    balls[i] = {t, i};
    times[i] = t;
  }
  sort(begin(balls), begin(balls) + n, ball_sorter);
  int curr_time = 0;
  for(int i=0; i<n; ++i){
    int idx = balls[i].index;
    tuple<bool, int> res;
    if(not deactivated[idx]){
      curr_time = defuse(n, curr_time, times, idx, deactivated);
    }
    if(curr_time == -1){
      cout << "no" << endl;
      return;
    }
  }
  cout << "yes" << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    solve();
  }
}
