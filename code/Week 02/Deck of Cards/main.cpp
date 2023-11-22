#include <iostream>
#include <vector>
#include <limits>

using namespace std;

void solve() {
  int n; cin >> n;
  int k; cin >> k;
  
  //read in card values
  int tmp;
  vector<int> v(n);
  for (int k = 0; k < n; ++k) {
    cin >> tmp; v[k] = tmp;
  }
  
  //init left, right pointer and current best value
  int max = numeric_limits<int>::max();
  int i = 0, j = 0;
  int bestLeft = 0, bestRight = 0;
  int bestVal = max; 
  int sum = 0; 
  int val; 
  
  //Sliding window
  //For lex. order only update when new bestVal is strictly better than old bestVal
  while (j < n) {
    sum += v[j];
    val = abs(k - sum);
    while (i <= j) {
      if (abs(k - (sum - v[i])) < val) {
        sum -= v[i];
        val = abs(k - sum);
        ++i;
      }
      else break;
    }
    if (val < bestVal) {
      bestLeft = i; 
      bestRight = j;
      bestVal = val;
    }
    ++j;
  }
  cout << bestLeft << " " << bestRight << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    solve();
  }
}
