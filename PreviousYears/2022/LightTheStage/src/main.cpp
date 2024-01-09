///0
#include <iostream>
#include <vector>
#include <algorithm>
// cgal
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;

void solve(){
  int m, n; std::cin >> m >> n;
  std::vector<double> dx(m, 0), dy(m, 0), dr(m, 0);
  
  // remaining list shows how many disk are in consideration
  std::vector<int> remain_list;
  for (int i=0; i<m; ++i){
    std::cin >> dx[i] >> dy[i] >> dr[i];
    remain_list.push_back(i);
  }
  
  int h; std::cin >> h;
  double x, y;
  std::vector<IPoint> lights;
  for (int j=0; j<n; ++j){
    std::cin >> x >> y;
    lights.emplace_back(K::Point_2(x, y), j);
  }
  
  Delaunay t;
  
  int left=0, right=n, mid=n; bool do_continue=true;
  
  while(left<right && do_continue){
    if (left == right-1) do_continue=false;
    // left is always accesable, meaning there is always some left
    // right is always in accessable,
    // mid = (left + right)/2,  and is always smaller than right
    // setting mid=n makes it possible to integregate the first trial
    // by starting layer, we can shink the search region!!! This is essential for speeding up!!!
    t.clear(); t.insert(lights.begin()+left, lights.begin()+mid); 

    std::vector<int> new_remain_list {};
    
    for (auto dp=remain_list.begin(); dp!=remain_list.end(); ++dp){
      int j=*dp;
      Tds::Vertex_handle v = t.nearest_vertex(K::Point_2(dx[j], dy[j]));
      long long x = dx[j] - v->point().x(), y = dy[j] - v->point().y(), r_ref = dr[j]+h;
      unsigned long long dist_sq = x*x + y*y, dist_ref=r_ref*r_ref; // have to use unsinged, otherwise overflow
      
      if (dist_sq >= dist_ref) new_remain_list.push_back(j);
    }
    
    if (!new_remain_list.empty()){
      // there are some remain
      if (mid==n){
        // first trial sucess, or trial end (this can also be excluded by left < right - 1)
        do_continue=false;
      } 
      left=mid;
      // there are new remained, so swap, remain_list is always non-empty, and ordered
      remain_list.swap(new_remain_list);
    } else {
      // no remain in this trial
      right=mid;
    }
    mid=(left+right) - (left+right)/2; // we have to make sure the last one is accessed
  }
  // std::sort(remain_list.begin(), remain_list.end());
  for (auto dp=remain_list.begin(); dp!=remain_list.end(); ++dp) std::cout << *dp << ' ';
  std::cout << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}