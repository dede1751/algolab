#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
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
	int m, n, k, c; std::cin >> m >> n >> k >> c;
	const int src = 2 * n * m, dst = src + 1;
	graph g(dst + 1);
	edge_adder adder(g);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			int in = i * m + j, out = in + n * m;

			adder.add_edge(in, out, c);

			if (i != 0)
				adder.add_edge(out, in - m, 1);
			else
				adder.add_edge(out, dst, 1);
			
			if (i != n - 1)
				adder.add_edge(out, in + m, 1);
			else
				adder.add_edge(out, dst, 1);
			
			if (j != 0)
				adder.add_edge(out, in - 1, 1);
			else
				adder.add_edge(out, dst, 1);
			
			if (j != m - 1)
				adder.add_edge(out, in + 1, 1);
			else
				adder.add_edge(out, dst, 1);
		}

	for (int i = 0; i < k; i++) {
		int x, y; std::cin >> x >> y;
		int in = y * m + x;
		adder.add_edge(src, in, 1);
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
