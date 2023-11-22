#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct boat {
  int l;
  int p;
};

bool boat_sorter(boat const& boat1, boat const& boat2){
  return boat1.p <= boat2.p;
}

void solve(){
  int n; cin >> n;
  int l, p;
  vector<boat> boats(n);
  for(int i=0; i<n; ++i){
    cin >> l >> p;
    boats[i] = boat({l, p});
  }
  
  sort(boats.begin(), boats.begin()+n, &boat_sorter);
  
  int end_last_boat = boats[0].p; // always pick leftmost position for first boat in the ring
  int max_num_boats = 1;
  //we can always pick the second boat
  if(n > 1){
    end_last_boat = boats[1].p;
    end_last_boat = max(end_last_boat + boats[1].l, boats[1].p);
    ++max_num_boats;
  }
  int end_last_last_boat = boats[0].p;
  
  //general greedy approach for all other boats
  for(int i=2; i<n; ++i){
    //if possible put boats[i] as left as possible
    if(end_last_boat <= boats[i].p){
      end_last_last_boat = end_last_boat;
      end_last_boat = max(end_last_boat + boats[i].l, boats[i].p);
      ++max_num_boats;
    }
    //boats[i].p is covered by current last boat but using boat[i] instead of the current last boat
    //will decrease end_last_boat --> take boats[i] instead of current last boat
    else if((boats[i].p < end_last_boat) && (end_last_last_boat + boats[i].l < end_last_boat)){
      end_last_boat = max(end_last_last_boat + boats[i].l, boats[i].p);
    }
    //otherwise do not consider boats[i] in the arrangement
  }
  cout << max_num_boats << endl;
}
int main(){
  int t; cin >> t;
  while(t--){
    solve();
  }
}
