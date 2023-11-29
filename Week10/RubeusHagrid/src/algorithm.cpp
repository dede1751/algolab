#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property,
  boost::property<boost::edge_weight_t, int>
> graph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

std::vector<long> galleons, weights, counts;

void fill_stats(graph& g, const int src) {
	weight_map weight_map = boost::get(boost::edge_weight, g);

	out_edge_it oe_beg, oe_end;
	for (boost::tie(oe_beg, oe_end) = boost::out_edges(src, g); oe_beg != oe_end; ++oe_beg) {
		int tgt = boost::target(*oe_beg, g);
		fill_stats(g, tgt);

		weights[src] += weight_map[*oe_beg] + weights[tgt];
		counts[src] += counts[tgt];
	}
} 

long solve(graph& g, const int src, long time) {
	weight_map weight_map = boost::get(boost::edge_weight, g);
	std::vector<std::pair<int, int>> children;

	out_edge_it oe_beg, oe_end;
	for (boost::tie(oe_beg, oe_end) = boost::out_edges(src, g); oe_beg != oe_end; ++oe_beg)
		children.push_back({boost::target(*oe_beg, g), weight_map[*oe_beg]});

	std::sort(children.begin(), children.end(),
		[](const std::pair<int, int>& e1, const std::pair<int, int>& e2) -> bool {
			auto [tgt1, w1] = e1;
			auto [tgt2, w2] = e2;

			return (w1 + weights[tgt1]) * counts[tgt2] < (w2 + weights[tgt2]) * counts[tgt1];
		}
	);

	long sum = galleons[src] - time;
	for (const auto [tgt, weight]: children) {
		sum += solve(g, tgt, time + weight);
		time += 2 * (weight + weights[tgt]);
	}

	return sum;
}

void testcase() {
	int n; std::cin >> n;
	graph g(n + 1);
	galleons = std::vector<long>(n + 1, 0);
	weights = std::vector<long>(n + 1, 0);
	counts = std::vector<long>(n + 1, 1);

	for (int i = 1; i <= n; i++)
		std::cin >> galleons[i];
	
	for (int i = 0; i < n; i++) {
		int u, v, l; std::cin >> u >> v >> l;
		boost::add_edge(u, v, l, g);
	}

	fill_stats(g, 0);
	std::cout << solve(g, 0, 0) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
