#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
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
};

void testcase() {
	int l, p; std::cin >> l >> p;
	int src = l, dst = l + 1;
	graph g(l + 2);
	edge_adder adder(g);
	std::vector<int> supply(l, 0);

	for (int i = 0; i < l; i++) {
		int g, d; std::cin >> g >> d;
		supply[i] = g - d;
	}

	for (int i = 0; i < p; i++) {
		int f, t, min, max; std::cin >> f >> t >> min >> max;
		supply[f] -= min;
		supply[t] += min;
		adder.add_edge(f, t, max - min);
	}

	int tot = 0;
	for (int i = 0; i < l; i++) {
		if (supply[i] > 0)
			adder.add_edge(src, i, supply[i]);
		else {
			tot -= supply[i];
			adder.add_edge(i, dst, -supply[i]);
		}
	}

	long flow = boost::push_relabel_max_flow(g, src, dst);
	std::cout << ((flow == tot) ? "yes" : "no") << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
