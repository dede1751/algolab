#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;

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

void testcase() {
	int n, pg, ph, eg, eh, fg, fh, sg, sh; std::cin >> n >> pg >> ph >> eg >> eh >> fg >> fh >> sg >> sh;
	int src = pg + ph + 2*n, dst = src + 1;
	graph g(dst + 1);
	edge_adder adder(g);

	int max_pref = -1;
	std::vector<int> preference(n);
	for (int i = 0; i < n; i++) {
		std::cin >> preference[i];
		max_pref = std::max(max_pref, preference[i]);
	}
	
	adder.add_edge(src, 0, sg, 0);
	for (int i = 0; i < eg; i++) {
		int u, v, c; std::cin >> u >> v >> c;
		adder.add_edge(u, v, c, 0);
	}

	adder.add_edge(pg, dst, sh, 0);
	for (int i = 0; i < eh; i++) {
		int u, v, c; std::cin >> u >> v >> c;
		adder.add_edge(pg + v, pg + u, c, 0);
	}

	for (int i = 0; i < fg; i++) {
		int u, v, c; std::cin >> u >> v >> c;
		adder.add_edge(u, pg + ph + v, 1, 0);
	}

	for (int i = 0; i < fh; i++) {
		int u, v, c; std::cin >> u >> v >> c;
		adder.add_edge(pg + ph + n + v, pg + u, 1, 0);
	}

	for (int i = 0; i < n; i++) 
		adder.add_edge(pg + ph + i, pg + ph + n + i, 1, max_pref - preference[i]);

	boost::successive_shortest_path_nonnegative_weights(g, src, dst);
    long cost = boost::find_flow_cost(g);

	auto c_map = boost::get(boost::edge_capacity, g);
    auto rc_map = boost::get(boost::edge_residual_capacity, g);
    out_edge_it e = boost::out_edges(boost::vertex(dst, g), g).first;

	long yarns = rc_map[*e] - c_map[*e];
	long total_preference = yarns * max_pref - cost;
	std::cout << yarns << " " << total_preference << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
