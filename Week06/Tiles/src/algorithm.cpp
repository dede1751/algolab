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
	int w, h; std::cin >> w >> h;
	int src = 2*w*h, dst = 2*w*h + 1;
	long d = 0;
	std::vector<std::vector<bool>> garden(h, std::vector<bool>(w, false));
	graph g(2*w*h + 2);
	edge_adder adder(g);

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++) {
			char x; std::cin >> x;
			int idx1 = i * w + j, idx2 = w * h + i * w + j;

			if (x == 'x')
				continue;

			garden[i][j] = true;
			d++;
			adder.add_edge(src, idx1, 1);
			adder.add_edge(idx2, dst, 1);

			if (i > 0 && garden[i - 1][j]) {
				adder.add_edge((i - 1) * w + j, idx2, 1);
				adder.add_edge(idx1, h * w + (i - 1) * w + j, 1);
			}
			if (j > 0 && garden[i][j - 1]) {
				adder.add_edge(i * w + j - 1, idx2, 1);
				adder.add_edge(idx1, h * w +i * w + j - 1, 1);
			}
		}
	
	if (d % 2) {
		std::cout << "no" << std::endl;
		return;
	}

	long flow = boost::push_relabel_max_flow(g, src, dst);
	std::cout << ((flow == d) ? "yes" : "no") << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
