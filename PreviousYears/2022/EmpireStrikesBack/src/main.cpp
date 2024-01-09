///3
#include <iostream>
#include <limits>
// LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
// triangulation
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef std::vector<int> IV;
typedef std::vector<long long> LV;
typedef std::vector<double> DV;

typedef K::Point_2 P;

void solve(){
  int a, s, b, e; std::cin >> a >> s >> b >> e;
  
  IV x_a, y_a, d_a;
  for (int i=0; i<a; ++i){
    int x, y, d; std::cin >> x >> y >> d;
    x_a.push_back(x);
    y_a.push_back(y);
    d_a.push_back(d);
  }
  
  IV x_s, y_s;
  for (int i=0; i<s; ++i){
    int x, y; std::cin >> x >> y;
    x_s.push_back(x);
    y_s.push_back(y);
  }
  
  std::vector<P> P_b;
  for (int i=0; i<b; ++i){
    int x, y; std::cin >> x >> y;
    P_b.push_back(P(x, y));
  }
  
  Triangulation t; t.insert(P_b.begin(), P_b.end());
  
  DV maxr2; // maximum radius of each shooter
  if (P_b.size() > 0) for (int i=0; i<s; ++i){
    maxr2.push_back(CGAL::squared_distance(t.nearest_vertex(P(x_s[i], y_s[i]))->point(), P(x_s[i], y_s[i])));
  }
  
  std::vector<IV> s_in_range (a, IV {});
  std::vector<DV> d2_in_range (a, DV {});
  for (int i=0; i<a; ++i){
    for (int j=0; j<s; ++j){
      double d2=CGAL::squared_distance(P(x_a[i], y_a[i]), P(x_s[j], y_s[j]));
      if (P_b.size() ==0 ||  d2< maxr2[j]){
        s_in_range[i].push_back(j); // if there is not b, we don't judge
        d2_in_range[i].push_back(d2);
      }
    }
  }
  
  // now construct our program
  Program lp (CGAL::LARGER, true, 0, false, 0); 
  for (int i=0; i<a; ++i){
    lp.set_b(i, d_a[i]);
    
    for (std::size_t j=0; j<s_in_range[i].size(); ++j){
      lp.set_a(s_in_range[i][j], i, IT(1)/IT(std::max(1LL, (long long) (d2_in_range[i][j]) )));
    }
  }
  
  // set energy constraint
  lp.set_b(a, -e);
  for (int i=0; i<s; ++i){
    lp.set_a(i, a, -1);
  }
  
  Solution solve = CGAL::solve_linear_program(lp, ET());
  
  if (!solve.is_infeasible()) std::cout << 'y' << std::endl;
  else std::cout << 'n' << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for(int i=0; i<t; ++i) solve();
}