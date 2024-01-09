///4
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <map>
#include <iostream>
#include <queue>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef Delaunay::Finite_faces_iterator                        Face_iterator;
typedef Delaunay::Finite_vertices_iterator                     Vertices_iterator;
// typedef std::map<Delaunay::Face_handle, int>                   Indexmap;
typedef std::map<Tds::Face_handle, double>                Dmap;
enum Color {Free=0, Block=1, Border=2};
typedef std::map<Tds::Face_handle, Color>                 Colormap;
typedef std::pair<Tds::Face_handle, double>               FDpair;
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;
typedef std::tuple<int, int, double> PPD; // point_idx_1, point_idx_2, distance_square, (id1 < id2)
// idea: increase from isolation, find the funciton of num_connect(s)
// for distinguishing color use vector<int> to map to smaller color -> more efficient
// maintain an priority queue

double distance2(const Delaunay::Vertex_handle & v1, const Delaunay::Vertex_handle & v2){
  double dx=v1->point().x()-v2->point().x(), dy=v1->point().y()-v2->point().y();
  return dx*dx+dy*dy;
}

int comp_id(const int &a, const std::vector<int> &comp_map){
  int b=a;
  while(comp_map[b]!=-1) b = comp_map[b];
  return b;
}

int max_family(const int &k, const std::vector<int> &k_stats){
  if (k==2){
    return k_stats[0] + k_stats[1]/2;
  } 
  else if (k==3){
   if (k_stats[1] == k_stats[2]){
     return k_stats[0] + k_stats[1];
   } else if (k_stats[1] > k_stats[2]){
     return k_stats[0] + k_stats[2] + (k_stats[1] - k_stats[2])/3;
   } else {
     return k_stats[0] + k_stats[1] + (k_stats[2] - k_stats[1])/2;
   }
  }
  else if (k==4){
    if (k_stats[1] == k_stats[3]){
      return  k_stats[0] + k_stats[1] + k_stats[2]/2;
    } else if (k_stats[1] > k_stats[3]){ // 2 and 1 remain now
      return k_stats[0] + k_stats[3] + (k_stats[1]-k_stats[3] + 2*k_stats[2])/4;
    } else { // 3 and 2 remain now
      return k_stats[0] + k_stats[1] + std::min(k_stats[3]-k_stats[1], k_stats[2]) + std::abs(k_stats[3]-k_stats[1]-k_stats[2])/2;
    }
  } 
  else 
  {
    return std::accumulate(k_stats.begin(), k_stats.end(), 0);
  }
}

struct Greater {
  inline bool operator() (const PPD& e1, const PPD& e2) {
    if (! (std::get<2>(e1) == std::get<2>(e2))){
      return std::get<2>(e1) > std::get<2>(e2);
    } else if (! (std::get<0>(e1) == std::get<0>(e2))){
      return std::get<0>(e1) > std::get<0>(e2);
    } else return std::get<1>(e1) > std::get<1>(e2);
  }
};

void solve(){
  int n, k, f0; double s0; std::cin >> n >> k >> f0 >> s0;
  
  std::vector<IPoint> points;
  points.reserve(n);
  
  for (Index i = 0; i < (unsigned int)n; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);
  }
  
  Delaunay t;
  t.insert(points.begin(), points.end());
  
  std::map<Delaunay::Vertex_handle, int> id_map; int id=0;
  for (Vertices_iterator v = t.finite_vertices_begin(); v!= t.finite_vertices_end(); ++v){
    id_map[v] = id; ++id;
  }
  
  // now put all edges in priority queue
  std::priority_queue<PPD, std::vector<PPD>, Greater> edge_queue;
  
  for (Vertices_iterator v = t.finite_vertices_begin(); v!= t.finite_vertices_end(); ++v) if (t.incident_vertices(v)!=0) {
    Delaunay::Vertex_circulator vn = t.incident_vertices(v);
    do{
      if ((!t.is_infinite(vn)) && id_map[v] < id_map[vn]){
        // add to priority queue
        // first calculate distance
        edge_queue.push(PPD(id_map[v], id_map[vn], distance2(v, vn)));
      }
    } while( ++vn!=t.incident_vertices(v));
  }
  
  // component map
  std::vector<int> comp_map (n, -1);
  // how many number of tents in this component, -1 means it is assigned to another id
  std::vector<int> num_tents (n, 1);
  
  std::vector<int> k_stats (k, 0); // give a statistics about how many of 1, 2, k-1 and >=k, 0 for >= k
  k_stats[ (k==1) ? 0: 1] = n; // initially all are isolated
  
  // now begin
  std::map<double, int> sf_map;
  std::map<int, double> fs_map;
  // int num_comp = n;
  while ( !edge_queue.empty() ){
    double s=std::get<2>(edge_queue.top());
    
    int f = max_family(k, k_stats);
    fs_map[f] = s; // if there is no decrease, this can update the maximum
    sf_map[s] = f;
    
    while (!edge_queue.empty() && std::get<2>(edge_queue.top()) == s ){
      // connect all component that have edges == s
      PPD top = edge_queue.top();
      int id1=std::get<0>(top), id2=std::get<1>(top);
      if (comp_id(id1, comp_map) != comp_id(id2, comp_map)){
        // two vertices are not in same component, 
        // then color the larger id one to the smaller id one
        int comp_id_1 =  std::min(comp_id(id1, comp_map),comp_id(id2, comp_map));
        int comp_id_2 =  std::max(comp_id(id1, comp_map),comp_id(id2, comp_map));
        comp_map[comp_id_2] = comp_id_1;
        
        int k_1 = (num_tents[comp_id_1] >= k) ? 0: num_tents[comp_id_1];
        int k_2 = (num_tents[comp_id_2] >= k) ? 0: num_tents[comp_id_2];
        
        // reduce  the number of current k stats by 1
        --k_stats[k_1];
        --k_stats[k_2];
        
        num_tents[comp_id_1] += num_tents[comp_id_2];
        num_tents[comp_id_2] = 0;
        
        int k_3 = (num_tents[comp_id_1] >= k) ? 0: num_tents[comp_id_1];
        
        // increase the merged component's stats by 1
        ++k_stats[k_3];
      }
      edge_queue.pop();
    }
  }
  sf_map[double(1LL<<52)] = 1; // this is in case when end() is searched
  
  std::cout << (long long)fs_map.lower_bound(f0)->second << ' ' <<( sf_map.lower_bound(s0)->second) << std::endl;
  // std::cout << k << std::endl;
  
}

int main() {
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) {
    // std::cout << i+1 << ' ';
    solve();
  }
}