///3
#include <iostream>
#include <vector>
// BGL include
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  
  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }

  void add_street(int v1, int v2){
    this->add_edge(v1, v2, 1);
    this->add_edge(v2, v1, 1);
  }
};

void solve(){
  int n, m, s; std::cin >> n >> m >> s;
  std::vector<int> num_store (n, 0);
  
  for (int i=0; i<s; ++i){
    int store_loc; std::cin >> store_loc;
    ++num_store[store_loc];
  }
  
  graph G(n); edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  // add source to home to target, equivalent to add s store at home
  adder.add_edge(v_source, 0, s);

  for (int i=0; i<n; ++i) if (num_store[i]>0) adder.add_edge(i, v_sink, num_store[i]);

  for (int i=0; i<m; ++i) { // add edges
    int v1, v2; std::cin >> v1 >> v2; adder.add_street(v1, v2);
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << ((flow==(long long)s) ? "yes" : "no") << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}