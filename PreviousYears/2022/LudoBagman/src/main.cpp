///4
#include <iostream>
#include <algorithm>
#include <limits>
#include <map>
// bgl
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
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

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

typedef std::vector<int> IV;

void solve() {
  int e, w, m, d, p, l; std::cin >> e >> w >> m >> d >> p >> l;
  
  // construct graph
  graph G(e+w); edge_adder modifier(G);
  const vertex_desc s = boost::add_vertex(G);
  const vertex_desc t = boost::add_vertex(G);
  const vertex_desc gs = boost::add_vertex(G); // gate source
  const vertex_desc gt = boost::add_vertex(G); // gate target
  
  // add edges
  for (int i=0; i<m; ++i){
    int u, v, r; std::cin >> u >> v >> r;
    modifier.add_edge(u, e+v, 1, r);
  }
  
  int inf=m+d;
  // add source to east
  for (int i=0; i<e; ++i){
    modifier.add_edge(s, i, l, 0); // maximum cap is l
    modifier.add_edge(gs, i, inf, 0); // residue flow goes this way, m is the max flow of this graph
  }
  // add west to target
  for (int i=0; i<w; ++i){
    modifier.add_edge(e+i, t, l, 0); // maximum cap is l
    modifier.add_edge(e+i, gt, inf, 0); // residue flow goes this way, m is the max flow of this graph
  }
  // add gate
  modifier.add_edge(s, gs, p-e*l, 0);
  modifier.add_edge(gt, t, p-w*l, 0);
  
  // the result for difficult flow is easy to calculate
  for (int i=0; i<d; ++i){
    int u, v, r; std::cin >> u >> v >> r;
    modifier.add_edge(gs, gt, 1, r);
  }
  
  int flow=boost::push_relabel_max_flow(G, s, t);
  if (flow<p) std::cout << "No schedule!" << std::endl;
  else {
    boost::successive_shortest_path_nonnegative_weights(G, s, t);
    std::cout << boost::find_flow_cost(G) << std::endl;
  }

}


int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}