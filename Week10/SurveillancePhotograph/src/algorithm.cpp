#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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
			c_map[rev_e] = 0;
			r_map[e] = rev_e;
			r_map[rev_e] = e;
		}
};

void testcase() {
	int n, m, k, l; std::cin >> n >> m >> k >> l;
	int src = 2 * n, dst = src + 1;
	graph g(dst + 1);
	edge_adder adder(g);

	for (int i = 0; i < k; i++) {
		int x; std::cin >> x;
		adder.add_edge(src, x, 1);
		adder.add_edge(n + x, dst, 1);
	}

	for (int i = 0; i < l; i++) {
		int x; std::cin >> x;
		adder.add_edge(x, n + x, 1);
	}

	for (int i = 0; i < m; i++) {
		int x, y; std::cin >> x >> y;
		adder.add_edge(x, y, k);
		adder.add_edge(n + x, n + y, 1);
	}

	long flow = boost::push_relabel_max_flow(g, src, dst);
	std::cout << flow << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
