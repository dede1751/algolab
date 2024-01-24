#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_set>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      graph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor          vertex_desc;

typedef std::vector<std::vector<std::pair<int, int>>> adj;

void testcase() {
	int n, m, k, x, y; std::cin >> n >> m >> k >> x >> y;
	adj river_adj = adj(n, std::vector<std::pair<int, int>>());
	graph g(n + k);

	for (int i = 0; i < m; i++) {
		int a, b, c, d; std::cin >> a >> b >> c >> d;

		boost::add_edge(a, b, c, g);
		if (d) {
			river_adj[a].push_back({b, c});
			river_adj[b].push_back({a, c});
		}
	}
	
	std::vector<int> dist_map(n + k);

	boost::dijkstra_shortest_paths(g, x,
		boost::distance_map(boost::make_iterator_property_map(
		dist_map.begin(), boost::get(boost::vertex_index, g))));
	
	for (int j = 0; j < k; j++) {
		for (int i = 0; i < n; i++) {
			if (dist_map[i] == std::numeric_limits<int>::max())
				continue;
			
			for (const auto [tgt, c]: river_adj[i])
				boost::add_edge(n + j, tgt, dist_map[i] + c, g);
		}

		boost::dijkstra_shortest_paths(g, n + j,
			boost::distance_map(boost::make_iterator_property_map(
			dist_map.begin(), boost::get(boost::vertex_index, g))));
	}

	std::cout << dist_map[y] << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
