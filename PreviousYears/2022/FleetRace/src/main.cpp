///4
#include <iostream>

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


void solve(){
  int b, s, p; std::cin >> b >> s >> p;
  int b_offset=5, s_offset = b_offset+b; long max_c=51;
  
  graph G(s_offset+s); edge_adder adder(G);
  int v_source=3, v_sink=2;
  
  for (int i=0; i<p; ++i){
    int b_i, s_i, c_i; std::cin >> b_i >> s_i >> c_i;
    adder.add_edge(b_offset+b_i, s_offset+s_i, 1, max_c - c_i);
  }
  // adding virtual edges to make the optimal solution also have solution
  // but this is not efficient
  // must find a better way to add edge
  // for (int i=0; i<b; ++i) for (int j=0; j<s; ++j) adder.add_edge(b_offset+i, s_offset+j, 1, max_c);
  
  for(int i=0; i<b; ++i) {
    adder.add_edge(v_source, b_offset+i, 1, 0);
    adder.add_edge(b_offset+i, v_sink, 1, max_c);
  }
  for(int i=0; i<s; ++i) {
    adder.add_edge(s_offset+i, v_sink, 1, 0);
  }
  
  // long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  long cost = boost::find_flow_cost(G);
  // std::cout << flow << ' ' << flow*max_c << ' ' << cost << std::endl;
  std::cout << b * max_c - cost << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}