///4
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <limits>
// CGAL
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef K::Point_2 P;
typedef Delaunay::Vertex_handle VH;

typedef std::tuple<int,int,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

typedef std::pair<K::Point_2,int> IPoint;


typedef std::pair<int, K::FT> IdxFT;

struct Greater{
  inline bool operator () (const IdxFT &a, const IdxFT &b){
    return a.second > b.second; // according to cppref, we use greater to put the smallest on top
  }
};

void solve(){
  int n; double s; int h; std::cin >> n >> s >> h;
  
  // the dual operation may result in extremely large distance...
  K::FT inf = std::numeric_limits<double>::max(); 

  std::vector<IPoint> points;
  points.reserve(n);
  for (int i=0; i<n; ++i) {
    int x, y; std::cin >> x >> y;
    points.emplace_back(P(x, y), i);
  }
  Delaunay t;
  t.insert(points.begin(), points.end());
  
  // we encode each edge as i*n+j, where i<j
  // for each edge we need is minimal and maximal, two map
  // for d3, we just need a map storing the number of faces with the value
  
  // we can use a single for loop to iterate over all faces
  // and use finite faces for d3, and the maximum value of each edge
  // then iterate over all fintite edges for its minimum
  
  // very important!!!
  // the mid point of an edge may not be the spot, as there might be another tree closer to it...
  // we have to check this
  
  std::map<K::FT, int> nd2, nd3; std::map<int, K::FT> emax, emin;
  
  for (auto e=t.finite_edges_begin(); e!=t.finite_edges_end(); ++e){
    
    VH v1=e->first->vertex((e->second+1)%3);
    VH v2=e->first->vertex((e->second+2)%3);
    
    int id1 = v1->info();
    int id2 = v2->info();
  
    if (id1>id2) {
      std::swap(id1, id2);
      std::swap(v1, v2);
    }
    
    int edge_id=id1*n+id2;
    
    K::FT sqd=CGAL::squared_distance(v1->point(), v2->point())/4;
  
    // we have to check the mid point
    P mid=CGAL::midpoint(v1->point(), v2->point());
    VH vp=t.nearest_vertex(mid);
    K::FT sqdp=CGAL::squared_distance(vp->point(), mid);
    
    if (sqdp==sqd){
      // this mid point is OK
      emin[edge_id]=sqd;
    } else {
      emin[edge_id]=inf;
    }
    
    if (nd2.find(sqd)==nd2.end()) nd2[sqd]=1; else ++nd2[sqd]; // a2 is not affected by this midponint issue...because it is the smallest
  }
  
  int a2=nd2.begin()->second;
  
  for (auto f=t.all_faces_begin(); f!=t.all_faces_end(); ++f){
    if (t.is_infinite(f)){
      // infinite face, we only set the finite edge's maximum to infinity
      for (int i=0; i<3; ++i) if (t.is_infinite(f->vertex(i%3))){
        int id1=f->vertex((i+1)%3)->info();
        int id2=f->vertex((i+2)%3)->info();
        if (id1>id2) std::swap(id1, id2);
        
        int edge_id=id1*n+id2;
        emax[edge_id]=inf;
      }
    } else {
      P center=t.dual(f);
      K::FT sqd=CGAL::squared_distance(center, f->vertex(1)->point());
      
      // for a3 calculation
      if (nd3.find(sqd)==nd3.end()) nd3[sqd]=1; else ++nd3[sqd];
      
      // for edge max calculation
      for (int i=0; i<3; ++i){
        int id1=f->vertex((i+1)%3)->info();
        int id2=f->vertex((i+2)%3)->info();
        if (id1>id2) std::swap(id1, id2);
        int edge_id=id1*n+id2;
        if (emax.find(edge_id)==emax.end() || emax[edge_id]<sqd) emax[edge_id]=sqd;
        if (emin[edge_id]>sqd) emin[edge_id]=sqd;
      }
    }
  }
  
  int a3=nd3.begin()->second;
  
  // we can avoid exact kernel if we now the r^2 of the circle of three point...
  // no it turns out this r2 is not an integer....
  
  int as=0; 
  for (auto iter=emax.begin(); iter!=emax.end(); ++iter){
    int edge_id=iter->first;
    if (s >= emin[edge_id] && s <= emax[edge_id] ) ++as;
  }
  
  // for the dmax, we need two priority queue, q1 stores does in consideration
  // q2 stores those unused,
  // q1 sorted by maximum, lowest maximum will be throw away
  // q2 stored by minimum, lowest minimum will be added in q1
  
  std::priority_queue<IdxFT, std::vector<IdxFT>, Greater> q1, q2;
  for (auto iter=emin.begin(); iter!=emin.end(); ++iter) q2.push(IdxFT(iter->first, iter->second));
  
  // the optimal number happens only after adding some points, therefore, we only need to consider 
  // s of adding in
  K::FT current = q2.top().second; std::size_t a_max=0;
  
  while (!q2.empty()){
    // first add all q2's qualified to q1
    while(!q2.empty() && current==q2.top().second){
      int edge_id=q2.top().first; q2.pop();
      q1.push(IdxFT(edge_id, emax[edge_id]));
    }
    // then remove all edge of maximum smaller than current
    while (!q1.empty() && current>q1.top().second){
      q1.pop();
    }
  
    a_max=std::max(a_max, q1.size());
    // finally update current
    if (!q2.empty()){
      current=q2.top().second;
    }
  }
  
  
  std::cout << a2 << ' ' << a3 << ' ' << as << ' ' << a_max << std::endl;
  
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}