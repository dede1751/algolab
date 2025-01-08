#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using Graph = boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::undirectedS,
	boost::no_property,
	boost::property<boost::edge_weight_t, int>
>;
using EdgeDesc = boost::graph_traits<Graph>::edge_descriptor;
using Weights = std::vector<std::vector<std::vector<int>>>;

void testcase() {
	int n, e, s, a, b; std::cin >> n >> e >> s >> a >> b;
	std::vector<Graph> graphs(n + 1);
	Weights weights(n, std::vector<std::vector<int>>(n, std::vector<int>(s, -1)));

	for (int i = 0; i < e; i++) {
		int src, tgt; std::cin >> src >> tgt;

		for (int j = 0; j < s; j++) {
			int w; std::cin >> w;

			boost::add_edge(src, tgt, w, graphs[j]);
			weights[src][tgt][j] = weights[tgt][src][j] = w;
		}
	}

	for (int species = 0; species < s; species++) {
		int h; std::cin >> h;
		std::vector<int> pred(n);

		boost::prim_minimum_spanning_tree(
			graphs[species],
			boost::make_iterator_property_map(pred.begin(), boost::get(boost::vertex_index, graphs[species])),
			boost::root_vertex(h)
		);
		
		for (int i = 0; i < n; i++)
			if (i != pred[i])
				boost::add_edge(i, pred[i], weights[i][pred[i]][species], graphs[s]);
	}

	std::vector<int> dist_map(n);
	boost::dijkstra_shortest_paths(
		graphs[s],
		a,
		boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, graphs[s])))
	);

	std::cout << dist_map[b] << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
