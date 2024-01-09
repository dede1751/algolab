///4
#include <iostream>
#include <map>
#include <vector>
#include <queue>
// bgl
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator          out_edge_it;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> f_graph; // flow graph

typedef traits::vertex_descriptor f_vertex_desc;
typedef traits::edge_descriptor f_edge_desc;

class edge_adder {
  f_graph &G;

 public:
  explicit edge_adder(f_graph &G) : G(G) {}

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
};

typedef std::vector<int> IV;

void solve(){
  int n, m, s, f; std::cin >> n >> m >> s >> f;
  
  weighted_graph wG (n); edge_desc e;
  weight_map length = boost::get(boost::edge_weight, wG);
  std::map<edge_desc, int> width;
  
  for (int i=0; i<m; ++i){
    int a, b, c, d; std::cin >> a >> b >> c >> d;
    e = boost::add_edge(a, b, wG).first; 
    length[e]=d; 
    width[e]=c;
  }
  
  IV dist_map(n);
  boost::dijkstra_shortest_paths(wG, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, wG))));
  
  // we use a map to map the index of original graph to the new graph
  std::map<int, int> idxmap; int idx_counter=0;
  // we use a queue to iterate over all possible vertex
  std::queue<int> q; q.push(f);
  
  // we can construct the new graph at the same time
  f_graph fG (0); edge_adder adder(fG);
  
  // we register the node on new graph when we add the node to the queue
  idxmap[f]=idx_counter; ++idx_counter; boost::add_vertex(fG);
  
  while (!q.empty()){
    int node=q.front(); q.pop();
    
    // iterate over all edges
    out_edge_it oe_beg, oe_end;
    for (boost::tie(oe_beg, oe_end) = boost::out_edges(node, wG); oe_beg != oe_end; ++oe_beg){
      int target_node=boost::target(*oe_beg, wG);
      if (dist_map[target_node] + length[*oe_beg] == dist_map[node]){ 
        // meaning this node is on the shortest path network, 
        // and this edge is about to add into the new graph
        
        // add this target node into new graph
        if (idxmap.find(target_node)==idxmap.end()){
          idxmap[target_node]=idx_counter; ++idx_counter; boost::add_vertex(fG);
          
          // also this node is first time visited, add this to queue
          q.push(target_node);
        }
        
        // add this new edge to new graph
        adder.add_edge(idxmap[target_node], idxmap[node], width[*oe_beg]);
      }
    }
  }
  
  long flow = boost::push_relabel_max_flow(fG, idxmap[s], idxmap[f]);
  std::cout << flow << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i=0; i<t; ++i) solve();
}