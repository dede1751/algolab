#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using WeightedGraph = boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::undirectedS,
	boost::no_property,
	boost::property<boost::edge_weight_t, int>
>;
using EdgeDesc = boost::graph_traits<WeightedGraph>::edge_descriptor;

void testcase() {
	int n, m; std::cin >> n >> m;
	WeightedGraph g(n);
	std::vector<EdgeDesc> mst;
	std::vector<int> dist_map(n);

	while (m--) {
		int src, tgt, weight; std::cin >> src >> tgt >> weight;
		boost::add_edge(src, tgt, weight, g);
	}
	boost::kruskal_minimum_spanning_tree(g, std::back_inserter(mst));

	int total = 0;
	for (const auto &i: mst)
		total += boost::get(boost::edge_weight_t(), g, i);
	
	boost::dijkstra_shortest_paths(g, 0,boost::distance_map(
		boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, g)))
	);

	int max = -1;
	for (const auto &i: dist_map)
		max = std::max(i, max);
	
	std::cout << total << " " << max << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
