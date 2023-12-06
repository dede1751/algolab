#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using Graph = boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::directedS,
	boost::no_property,
	boost::property<boost::edge_weight_t, int>
>;

void testcase() {
	int n, m, k , t; std::cin >> n >> m >> k >> t;
	Graph g(2 * n);
	std::vector<int> tp(n);

	for (int i = 0; i < t; i++){
		int x; std::cin >> x;
		tp[i] = x;
	}

	for (int i = 0; i < m; i++) {
		int u, v, c; std::cin >> u >> v >> c;
		boost::add_edge(v, u, c, g);
	}

	std::vector<int> scc(2 * n);	
	int scc_count = boost::strong_components(
		g,
		boost::make_iterator_property_map(scc.begin(), boost::get(boost::vertex_index, g))
	);

	std::vector<std::vector<int>> subnets(scc_count);
	for (int i = 0; i < t; i++)
		subnets[scc[tp[i]]].push_back(tp[i]);

	for (int i = 0; i < scc_count; i++) {
		for (const auto src: subnets[i]) {
			boost::add_edge(n + i, src, 0, g);
			boost::add_edge(src, n + i, subnets[i].size() - 1, g);
		}
	}

	std::vector<int> dists(2 * n);
	boost::dijkstra_shortest_paths(
		g,
		n - 1,
		boost::distance_map(boost::make_iterator_property_map(dists.begin(), boost::get(boost::vertex_index, g)))
	);

	int min = 1000001;
	for (int i = 0; i < k; i++)
		min = std::min(min, dists[i]);

	if (min < 1000001)
		std::cout << min << std::endl;
	else
		std::cout << "no" << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
