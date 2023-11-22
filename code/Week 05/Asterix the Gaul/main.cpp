#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct movement {
  long d;
  long t;
};

bool subsetsum_sorter(movement m1, movement m2){ return (m1.t < m2.t); }
bool subsetsum_rec(vector<movement>& mvmts, long D, long T, int i){ // O(2^n)
  if(D <= 0 && T>0) return true;
  if((i == 0 && D >= 0)  || T <= 0) return false;
  return subsetsum_rec(mvmts, D - mvmts[i-1].d, T - mvmts[i-1].t, i-1) || subsetsum_rec(mvmts, D, T, i-1);
}
bool subsetsum(const vector<movement>& mvmts, long D, long T, int n){ // O(n*2^n)
  for(int s = 0; s < 1<<n; ++s){ //Iterate through all 2^n subsets 
    long distance = 0;
    long ttime = 0;
    for(int i = 0; i < n; ++i){
      if (s & 1<<i){ // if i-th element in subset
        distance += mvmts[i].d;
        ttime += mvmts[i].t;
      }
      if(ttime >= T) break;
    }
    if(ttime < T && distance >= D) return true;
  }
  return false;
}

bool split_n_list(const vector<movement>& mvmnts, long D, long T, int n){ // O(n*2^(n/2))
  int n1 = n / 2;
  int n2 = n - n1;
  
  vector<movement> list_1;
  vector<movement> list_2;
  
  for(int s=0; s < 1<<n1; ++s){
    long d1, t1; d1 = 0; t1 = 0;
    for(int i=0; i<n1; ++i){
      if (s & 1 << i){
        d1 += mvmnts[i].d;
        t1 += mvmnts[i].t;
      }
    }
    //Already found valid combination of moves in first subset?
    if (d1 >= D && t1 < T) return true;
    //else only consider subset if cum. traveltime is less than T
    else if(t1 < T) list_1.push_back(movement({d1, t1}));
  }
    
  for(int s=0; s < 1<<n2; ++s){
    long d2, t2; d2 = 0; t2 = 0;
    for(int i=0; i<n2; ++i){
      if (s & 1 << i){
        d2 += mvmnts[n1 + i].d;
        t2 += mvmnts[n1 + i].t;
      }
    }
    //Already found valid combination of moves in second subset?
    if (d2 >= D && t2 < T) return true;
    //else only consider subset if cum. traveltime is less than T
    else if(t2 < T) list_2.push_back(movement({d2, t2}));
  }
  
  //sort list_2 in ascending order w.r.t the cumulative travel time of the subsets
  sort(begin(list_2), end(list_2), subsetsum_sorter);
  //since we only want the distance sums to be greater than D, we can replace the distances of all subset sums
  //with the same cum. travel time by the max. travel distance among them and smaller times
  //--> This step helps us to do standard binary search later
  for(int i=1; i<list_2.size(); ++i){
      list_2[i].d = max(list_2[i].d, list_2[i-1].d);
  }
  
  //in each iteration fix element (t1, d1) in list_1 and do binary search to check if there is
  // (t2, d2) s.t. t1 + t2 < T and d1 + d2 >= D.
  for(auto m1 : list_1){
    long left = 0;
    long right = list_2.size()-1;
    long middle = (left + right) / 2;
    while(left <= right){
      if(list_2[middle].t + m1.t < T){
        if(list_2[middle].d + m1.d >= D) return true;
        else left = middle + 1;
      }
      else{
        right = middle - 1;
      }
      middle = (left + right) / 2;
    }
  }
  
  return false;
}
//gives 20/100 for iterative and 60/100 for recursive solution
void solve_subsetsum(){
  
  int n, m;
  long D, T;
  cin >> n >> m >> D >> T;
  
  vector<movement> mvmnts(n);
  long d, t;
  for(int i=0; i<n; ++i){
    cin >> d >> t;
    mvmnts[i] = movement({d, t});
  }
  vector<long> gulps(m);
  //gulps[0] = 0;
  long s;
  for(int i=0; i<m; ++i){
    cin >> s;
    gulps[i] = s;
  }
  
  long left = 0;
  long right = m-1;
  long middle = right / 2;

  if(subsetsum_rec(mvmnts, D, T, n)){
    cout << 0 << endl;
    return;
  }
  
  //binary search over m
  while(left <= right){
    
    for(int i=0; i<n; ++i){
      mvmnts[i].d = mvmnts[i].d + gulps[middle];
    }
    
    if(subsetsum_rec(mvmnts, D, T, n)){
      right = middle;
      if (left == right) break;
    }
    else left = middle + 1;
    
    for(int i=0; i<n; ++i){
      mvmnts[i].d = mvmnts[i].d - gulps[middle];
    }
    
    middle = (left + right) / 2;
  }
  
  if(left < m) cout << left + 1 << endl;
  else cout << "Panoramix captured" << endl;
  
}

//gives 100/100 using split and list technique
void solve_splitlist(){
  
  int n, m;
  long D, T;
  cin >> n >> m >> D >> T;
  
  vector<movement> mvmnts(n);
  long d, t;
  for(int i=0; i<n; ++i){
    cin >> d >> t;
    mvmnts[i] = movement({d, t});
  }
  vector<long> gulps(m);
  //gulps[0] = 0;
  long s;
  for(int i=0; i<m; ++i){
    cin >> s;
    gulps[i] = s;
  }
  
  long left = 0;
  long right = m-1;
  long middle = right / 2;

  if(split_n_list(mvmnts, D, T, n)){
    cout << 0 << endl;
    return;
  }
  
  //binary search over m
  while(left <= right){
    
    for(int i=0; i<n; ++i){
      mvmnts[i].d = mvmnts[i].d + gulps[middle];
    }
    
    if(split_n_list(mvmnts, D, T, n)){
      right = middle;
      if (left == right) break;
    }
    else left = middle + 1;
    
    for(int i=0; i<n; ++i){
      mvmnts[i].d = mvmnts[i].d - gulps[middle];
    }
    
    middle = (left + right) / 2;
  }
  
  if(left < m) cout << left + 1 << endl;
  else cout << "Panoramix captured" << endl;
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    solve_splitlist();
  }
}