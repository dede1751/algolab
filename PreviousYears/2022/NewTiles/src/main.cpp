///2
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <bitset>

typedef std::vector<int> IV;
typedef std::map<int, int> IIM;
typedef unsigned int ui;
typedef std::vector<ui> UIV;
typedef std::map<ui, ui> UUM;
// idea
// we store a map, keys are the possible outcome sates states given previous input
// an outcome is a state with some possible tiling plan
// This map maps a state to its maximum tile
// then we compare the current states possible tiling plan, and see with previous outcome is compatible with current tiling plan
// then we calculate the state value

void generate_outcome(UIV & o_l, UIV & t_l, ui s, ui w, ui cur_w, ui cur_t) {
  // o_l: outcome list, t_l: tile list, s: state that we are referencing to, w: total width
  // cur_t: current tile, 1 for tiling there, 0 for no tiling operation there
  // cur_w: current considering location, initially cur_w = 0, ends at w-1, if cur_w = w all bits are considered and it push operation into list
  // initially cur_w=0, cur_o = 0, cur_t = 0. recursively each time, ++cur_w, if cur_w == w, output
  if (cur_w == w){
    t_l.push_back(cur_t);
    o_l.push_back(s^cur_t); // the out come is just the xor of s and t
  } else { // cur_w < w
    // test the current location is possible or not
    if (((s>>cur_w)&3U)==3U){
      // both current and next position is 1, then we tile here
      ui new_t = (cur_t | (3U<<cur_w)); // add tile to the corresponding position, 3in binary is 0011, 
      generate_outcome(o_l, t_l, s, w, cur_w+2U, new_t);
    }
    // not tile-able, or we choose not to tile, we
    ui new_t = cur_t; // no tile operation
    generate_outcome(o_l, t_l, s, w, cur_w+1U, new_t);
  }
}

inline bool compatible(ui s, ui t) {
  return (((s^t)&t) == 0);
}

inline ui num_tile(ui t) {
  return __builtin_popcount(t)/2;
}

void solve(){
  int h, w; std::cin >> h >> w;
  
  // first input state
  ui init_s=0;
  for (int i=0; i<w; ++i){
    ui possible; std::cin >> possible;
    init_s = (init_s|(possible<<i));
  }
  
  UIV prev_o_l, cur_o_l, cur_t_l;
  UUM prev_m, cur_m;
  
  // for init state
  cur_o_l.push_back(init_s);
  cur_m[init_s] = 0U;
  
  for (int j=1; j<h; ++j){
    // get state
    ui state=0U;
    for (int i=0; i<w; ++i){
      int possible; std::cin >> possible;
      state = (state|(possible<<i));
    }
    
    // handle with privous vectors
    prev_m.swap(cur_m);
    cur_m.clear();
    prev_o_l.swap(cur_o_l);
    cur_o_l.clear();
    cur_t_l.clear();
    
    // calculate state
    generate_outcome(cur_o_l, cur_t_l, state, w, 0U, 0U);
    
    // do the maximization
    for (std::size_t i=0; i<cur_o_l.size(); ++i){
      ui o=cur_o_l[i], t=cur_t_l[i];
      for (auto iter=prev_m.begin(); iter!=prev_m.end(); ++iter){
        int p_o=iter->first, p_max_tile=iter->second;
        if (compatible(p_o, t)){
          if (cur_m.find(o)==cur_m.end()) cur_m[o]=p_max_tile+num_tile(t);
          else cur_m[o]=std::max(cur_m[o], p_max_tile+num_tile(t));
        }
      }
    }
    
    // std::cout << prev_m.size() << ' ' << cur_o_l.size() << std::endl;
  }
  
  ui max_tile = 0;
  for (auto iter=prev_m.begin(); iter!=prev_m.end(); ++iter){
    max_tile = std::max(max_tile, iter->second);
  }
  
  std::cout << max_tile << std::endl;
}

int main(){
  
  // UIV cur_o_l, cur_t_l;
  // ui state = (1U<<3|1U<<10|3U<<4);
  // generate_outcome(cur_o_l, cur_t_l, state, 15, 0U, 0U);
  // for (std::size_t i=0; i<cur_o_l.size(); ++i) std::cout << std::bitset<15>(cur_o_l[i]) << ' ' << std::bitset<15>(cur_t_l[i]) << std::endl;
  
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}