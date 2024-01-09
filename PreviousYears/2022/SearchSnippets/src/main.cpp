///1
#include <iostream>
#include <vector>
using namespace std;

void solve() {
  int n;
  cin >> n;
  int m[n];
  for (int i=0; i<n; ++i) {
    cin >> m[i];
  }
  // idea is to construct l(position), denoting the shortest distance to all kind of words, -1 means failure, and will be deleted from the array.
  vector<pair<int, int>> old={}, neww={};
  vector<int> p;
  int temp;
  for (int i=0; i<n; ++i) {
    p.clear();
    for (int j=0; j<m[i]; ++j) {
      cin >> temp;
      p.push_back(temp);
    }
    if (i==0) {
      // begining
      old.clear();
      neww.clear();
      for (auto iter=p.begin(); iter!=p.end(); ++iter) {
        neww.push_back(pair<int, int>(*iter, 0));
      }
    } else {
      // for this iteration, if p < old.iter, add new p; else update old.iter
      // we only keep valid position, if a position already fails, delete it.
      neww.clear();
      auto it_o = old.begin(); auto it_p = p.begin();
      while (it_o != old.end() && it_p != p.end()) {
        // if p ends before old, this is ok since all position behind is useless
        // if old ends before p, this is also ok, since for the last point of old, there is also a valid word.
        if (it_o->first < *it_p) {
          // we have to do pruning, if the ending position is always the same, we choose a smaller one.
          while (next(it_o)!= old.end() && next(it_o)->first < *it_p && next(it_o)->first + next(it_o)->second == it_o->first + it_o->second) {
            ++it_o;
          }
          // update it_o to neww
          neww.push_back(pair<int, int>(it_o->first, max(it_o->second, *it_p - it_o->first)));
          ++it_o;
        } else {
          // it_o->first > *it_p, insert new p
          neww.push_back(pair<int, int>(*it_p, it_o->second + it_o->first - *it_p));
          ++it_p;
        }
      }
    }
    neww.swap(old); // every time, the old contains all data
  }
  int l=old.begin()->second+1;
  for (auto i=old.begin(); i!=old.end(); ++i) {
    if (i->second+1 < l) {
      l = i->second+1;
    }
  }
  cout << l << endl;
}


int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  for (int i=0; i<t; ++i) {
    solve();
  }
}