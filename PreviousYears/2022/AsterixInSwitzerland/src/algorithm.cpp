#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >	graph;
typedef	boost::graph_traits<graph>::edge_descriptor			edge_desc;
typedef	boost::graph_traits<graph>::out_edge_iterator			out_edge_it;

class edge_adder {
	graph &G;

	public:
		explicit edge_adder(graph &G) : G(G) {}

		void add_edge(int from, int to, long capacity) {
			auto c_map = boost::get(boost::edge_capacity, G);
			auto r_map = boost::get(boost::edge_reverse, G);
			const edge_desc e = boost::add_edge(from, to, G).first;
			const edge_desc rev_e = boost::add_edge(to, from, G).first;
			c_map[e] = capacity;
			c_map[rev_e] = 0;
			r_map[e] = rev_e;
			r_map[rev_e] = e;
		}
};

void testcase() {
	int n, m; std::cin >> n >> m;
	int tot = 0, src = n, dst = src + 1;
	graph g(dst + 1);
	edge_adder adder(g);

	for (int i = 0; i < n; i++) {
		int b; std::cin >> b;

		if (b > 0) {
			tot += b;
			adder.add_edge(src, i, b);
		} else
			adder.add_edge(i, dst, -b);
	}

	for (int i = 0; i < m; i++) {
		int x, y, d; std::cin >> x >> y >> d;
		adder.add_edge(x, y, d);
	}

	long flow = boost::push_relabel_max_flow(g, src, dst);
	std::cout << ((tot > flow) ? "yes" : "no")  << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
