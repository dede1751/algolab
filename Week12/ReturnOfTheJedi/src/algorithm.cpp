#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property,
  boost::property<boost::edge_weight_t, int>
> graph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor            edge_desc;

int n, tatooine;
std::vector<std::vector<int>> max_dists, weights, mst_adj;
std::vector<std::vector<bool>> in_mst;
std::vector<int> pred;
std::vector<bool> visited;

void fill_dists(const int root, const int curr, int max) {
	max_dists[root][curr] = max_dists[curr][root] = max;
	visited[curr] = true;

	for (const int next: mst_adj[curr]) {
		if (!visited[next])
			fill_dists(root, next, std::max(max, weights[curr][next]));
	}
}

void testcase() {
	std::cin >> n >> tatooine;
	graph g(n);

	max_dists = std::vector<std::vector<int>>(n, std::vector<int>(n));
	weights = std::vector<std::vector<int>>(n, std::vector<int>(n));
	mst_adj = std::vector<std::vector<int>>(n, std::vector<int>());
	in_mst = std::vector<std::vector<bool>>(n, std::vector<bool>(n, false));
	pred = std::vector<int>(n);

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			int c; std::cin >> c;
			boost::add_edge(i, j, c, g);
			weights[i][j] = weights[j][i] = c;
		}
	}

	boost::prim_minimum_spanning_tree(
		g,
		boost::make_iterator_property_map(pred.begin(), boost::get(boost::vertex_index, g)),
		boost::root_vertex(tatooine - 1)
	);

	long cost = 0;
	for (int i = 0; i < n; i++) {
		if (i != pred[i]) {
			mst_adj[i].push_back(pred[i]);
			mst_adj[pred[i]].push_back(i);
			in_mst[i][pred[i]] = in_mst[pred[i]][i] = true;
			cost += weights[i][pred[i]];
		}
	}

	for (int i = 0; i < n; i++) {
		visited = std::vector<bool>(n, false);
		fill_dists(i, i, -1);
	}

	int best = std::numeric_limits<int>::max();
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (!in_mst[i][j])
				best = std::min(best, weights[i][j] - max_dists[i][j]);
		}
	}

	std::cout << cost + best << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
