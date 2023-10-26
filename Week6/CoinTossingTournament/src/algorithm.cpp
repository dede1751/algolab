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
	int n, m; std::cin >> n >> m;
	std::vector<int> sb(n, 0);
	std::vector<std::pair<int, int>> games;

	for (int i = 0; i < m; i++) {
		int a, b, c; std::cin >> a >> b >> c;

		if (c == 0)
			games.push_back(std::make_pair(a, b));
		else if (c == 1)
			sb[a]++;
		else
			sb[b]++;
	}

	int sum = 0;
	bool impossible = false;
	for (int i = 0; i < n; i++) {
		int s; std::cin >> s;
		sb[i] = s - sb[i];
		sum += sb[i];

		if (sb[i] < 0)
			impossible = true;
	}

	if (impossible || (int)games.size() != sum) {
		std::cout << "no" << std::endl;
		return;
	}

	int k = (int)games.size(), src = k + n, dst = k + n + 1;
	graph g(k + n + 2);
	edge_adder adder(g);

	for (int i = 0; i < k; i++) {
		adder.add_edge(src, i, 1);
		adder.add_edge(i, k + games[i].first, 1);
		adder.add_edge(i, k + games[i].second, 1);
	}

	for (int i = 0; i < n; i++)
		if (sb[i] != 0)
			adder.add_edge(k + i, dst, sb[i]);
	
	long flow = boost::push_relabel_max_flow(g, src, dst);
	std::cout << ((flow == k) ? "yes" : "no") << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
