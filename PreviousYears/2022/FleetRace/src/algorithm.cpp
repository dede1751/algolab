#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

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
			auto w_map = boost::get(boost::edge_weight, G);
			const edge_desc e = boost::add_edge(from, to, G).first;
			const edge_desc rev_e = boost::add_edge(to, from, G).first;
			c_map[e] = capacity;
			c_map[rev_e] = 0;
			r_map[e] = rev_e;
			r_map[rev_e] = e;
			w_map[e] = cost;
			w_map[rev_e] = -cost;
		}
};

void testcase() {
	int b, s, p; std::cin >> b >> s >> p;
	int src = b + s, snk = src + 1;
	graph g(snk + 1);
	edge_adder adder(g);

	for (int i = 0; i < b; i++) {
		adder.add_edge(src, i, 1, 0);
		adder.add_edge(i, snk, 1, 50);
	}

	for (int i = b; i < b + s; i++) {
		adder.add_edge(i, snk, 1, 0);
	}

	for (int i = 0; i < p; i++) {
		int bi, si, ci; std::cin >> bi >> si >> ci;
		adder.add_edge(bi, b + si, 1, 50 - ci);
	}

	boost::successive_shortest_path_nonnegative_weights(g, src, snk);
	int cost = boost::find_flow_cost(g);

	std::cout << (50*b - cost) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
