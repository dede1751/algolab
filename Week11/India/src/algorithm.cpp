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
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

class edge_adder {
	graph &G;

	public:
	explicit edge_adder(graph &G) : G(G) {}
	edge_desc add_edge(int from, int to, long capacity, long cost) {
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

		return e;
	}
};

void testcase() {
	int c, g, b, k, a; std::cin >> c >> g >> b >> k >> a;
	int src = c, dst = src + 1;
	graph G(dst + 1);
	edge_adder adder(G);

	auto c_map = boost::get(boost::edge_capacity, G);
	auto rc_map = boost::get(boost::edge_residual_capacity, G);
	auto in = adder.add_edge(src, k, 0, 0);

	for (int i = 0; i < g; i++) {
		int x, y, d, e; std::cin >> x >> y >> d >> e;
		adder.add_edge(x, y, e, d);
	}

	int start = 0, end = b + 1;
	while (start < end) {
		int mid = start + (end - start) / 2;

		c_map[in] = mid;
		boost::successive_shortest_path_nonnegative_weights(G, src, a);
		long flow = c_map[in] - rc_map[in], cost = boost::find_flow_cost(G);

		if (flow == mid && cost <= b)
			start = mid + 1;
		else
			end = mid;
	}

	std::cout << start - 1 << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
