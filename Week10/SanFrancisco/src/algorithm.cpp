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
> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator out_edge_it;

typedef std::vector<std::vector<long>> dp;

void fill_dp(weighted_graph& g, dp& dp, const int n, const int k) {
	weight_map weight_map = boost::get(boost::edge_weight, g);

	for (int d = 1; d <= k; d++) {
		for (int src = 0; src < n; src++){
			long best = -1;
			out_edge_it oe_beg, oe_end;
			for (boost::tie(oe_beg, oe_end) = boost::out_edges(src, g); oe_beg != oe_end; ++oe_beg) {
				int tgt = boost::target(*oe_beg, g);
				long weight = weight_map[*oe_beg];

				best = std::max(best, weight + dp[tgt][d - 1]);
			}

			dp[src][d] = (best == -1) ? dp[0][d] : best;
		}
	}
}

void testcase() {
	long n, m, x, k; std::cin >> n >> m >> x >> k;
	weighted_graph g(n);
	dp dp(n, std::vector<long>(k + 2, 0));

	for (int i = 0; i < m; i++) {
		int u, v, p; std::cin >> u >> v >> p;
		boost::add_edge(u, v, p, g);
	}

	fill_dp(g, dp, n, k);

	if (dp[0][k] < x) {
		std::cout << "Impossible" << std::endl;
		return;
	}

	long start = 0, end = k;
	while (start < end) {
		long mid = start + (end - start) / 2;

		if (dp[0][mid] < x)
			start = mid + 1;
		else
			end = mid;
	}

	std::cout << start << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
