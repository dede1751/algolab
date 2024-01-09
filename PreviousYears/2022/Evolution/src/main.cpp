///2
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <set>

// idea
// binary search on graph
// we save for each node a list of nodes, that is 1, 2, 4, 8, ... far away from the node
// the distance is the logical distance

typedef std::pair<int, int> IP; // used for storing queries and age
typedef std::vector<int> IV;

struct Smaller{
  inline bool operator() (const IP &a, const IP &b){
    if (a.second == b.second) return a.first < b.first;
    else return a.second < b.second;
  }
};
typedef std::set<IP, Smaller> IS;


void solve(){
  int n, q; std::cin >> n >> q;
  
  std::map<std::string, int> stridx;
  std::vector<std::string> idxstr;
  
  IV a;
  
  for (int i=0; i<n; ++i){
    std::string name; std::cin >> name;
    int a_i; std::cin >> a_i;
    stridx[name]=i;
    idxstr.push_back(name);
    a.push_back(a_i);
    
    // std::cout << name << ' ' << a_i << std::endl;
  }
  
  IV n_child(n, 0), parent(n, -1);
  std::vector<IV> clist(n, IV ());
  
  for (int i=0; i<n-1; ++i){
    std::string s, p; std::cin >> s >> p;
    int u=stridx[p], v=stridx[s];
    ++n_child[u];
    parent[v]=u;
    clist[u].push_back(v);
  }
  
  int root=0;
  for (int i=0; i<n; ++i) if (parent[i]==-1){
    root=i;
    break;
  }
  
  std::vector<IV> nlist(n, IV ());
  std::queue<int> queue; queue.push(root);
  
  while (!queue.empty()){
    int node=queue.front(); queue.pop();
    
    // given a node calculate its list first
    // oh, we don't need to explicitly calculate logical distance!
    
    std::size_t i=0;
    int prev_node=parent[node]; // we are considering adding the first element
    
    while(prev_node != -1){
      // std::cout << prev_node << std::endl;
      nlist[node].push_back(prev_node);
      if (i<nlist[prev_node].size()) {
        prev_node=nlist[prev_node][i];
        ++i;
      }
      else break;
    }
    
    for (auto it=clist[node].begin(); it!=clist[node].end(); ++it) queue.push(*it);
  }
  
  // for test case
  // for (int i=0; i<n; ++i){
  //   for (auto it=nlist[i].begin(); it!=nlist[i].end(); ++it){
  //     std::cout << *it << ' ';
  //   }
  //   std::cout << std::endl;
  // }
  
  for (int i=0; i<q; ++i){
    std::string s; int b; std::cin >> s >> b;
    int node=stridx[s];
    
    // we search over the biggest node that have age smaller or equal to b
    // for termination, either the nlist is empty or the first element is bigger than b
    std::size_t idx=0, ref_node=node;
    // idx is the next possible ancestor's position in the current nlist
    // ref_node it the node we are observing at
    
    while(idx<nlist[ref_node].size() && a[nlist[ref_node][idx]]<=b){
      // terminate when the first element's age is bigger than b
      // or there is no element in the current list
      
      // if the next element exists and it is smaller than or equal to b
      if (idx+1<nlist[ref_node].size() && a[nlist[ref_node][idx+1]]<=b) ++idx;
      else {
        ref_node=nlist[ref_node][idx];
        idx=0;
      }
        
    }
    
    std::cout << idxstr[ref_node] << ' ';
  }
  std::cout << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for(int i=0; i<t; ++i) solve();
}