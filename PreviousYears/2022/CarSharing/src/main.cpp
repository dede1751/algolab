///4
#include <iostream>
#include <set>
#include <algorithm>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <cmath>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

// the idea is that we add a spacial temperal graph
// for each side, we make a list of the minutes it envolve (both start and end)
// we encode this node by (time)*s+s_i
// we use a map to map the actual index on graph
// for the none profiting edges, we let ist capacity to be infinity, but with highest cost f
// the highest total cost is no transaction happend...

// std::numeric_limits<int>::max()

typedef std::vector<int> IV;
typedef std::pair<int, int> IP;
typedef std::set<int> IS;

void solve(){
  int n, s; std::cin >> n >> s;
  
  IV l(s, 0); for (int i=0; i<s; ++i) std::cin >> l[i];
  
  IV s_l, t_l, d_l, a_l, p_l;
  std::vector<IS> s_time (s, IS {}); // station's timeline
  
  int t_min=100000, t_max=0;
  for (int i=0; i<n; ++i){
    int s_i, t_i, d_i, a_i, p_i; std::cin >> s_i >> t_i >> d_i >> a_i >> p_i;
    s_l.push_back(s_i-1); // be careful of coding
    t_l.push_back(t_i-1);
    d_l.push_back(d_i);
    a_l.push_back(a_i);
    p_l.push_back(p_i);
    s_time[s_i-1].insert(d_i);
    s_time[t_i-1].insert(a_i);
    t_min=std::min(d_i, t_min);
    t_max=std::max(a_i, t_max);
  }
  
  // now get a mapping about the index
  int idx_counter=0; std::map<int, int> e_idx;
  for (int i=0; i<s; ++i){
    s_time[i].insert(t_min);
    s_time[i].insert(t_max);
    for (auto iter=s_time[i].begin(); iter!=s_time[i].end(); ++iter){
      int id=(*iter)*s+i;
      e_idx[id]=idx_counter; ++idx_counter;
    }
  }
  
  // now build the graph
  graph G(idx_counter); edge_adder adder(G);  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  int f_max=100, inf_cap=100000;
  long max_cost=0;
  
  for (int i=0; i<s; ++i) if (!s_time[i].empty()) {
    // first add the iteredges
    for (auto iter=std::next(s_time[i].begin(), 1); iter!=s_time[i].end(); ++iter){
      int ids=(*std::next(iter,-1))*s+i;
      int ide=(*iter)*s+i;
      // int dt=order[e_idx[ide]]-order[e_idx[ids]];
      int dt=(*iter)-(*std::next(iter,-1));
      adder.add_edge(e_idx[ids], e_idx[ide], inf_cap, dt*f_max);
    }
    
    // start
    int ids=(*s_time[i].begin())*s+i;
    adder.add_edge(v_source, e_idx[ids], l[i], 0);
    // end
    int ide=(*s_time[i].rbegin())*s+i;
    adder.add_edge(e_idx[ide], v_sink, inf_cap, 0);
    
    // max_cost += (order[e_idx[ide]]-order[e_idx[ids]])*f_max*l[i]; // this is the highest cost if no request is accepted
    max_cost += ((*s_time[i].rbegin())-(*s_time[i].begin()))*f_max*l[i];
  }

  for (int i=0; i<n; ++i){
    int ids=(d_l[i])*s+s_l[i];
    int ide=(a_l[i])*s+t_l[i];
    // int dt=order[e_idx[ide]]-order[e_idx[ids]];
    int dt=a_l[i]-d_l[i];
    adder.add_edge(e_idx[ids], e_idx[ide], 1, dt*f_max - p_l[i]);
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  long cost = boost::find_flow_cost(G);
  std::cout << max_cost-cost << std::endl;

}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}