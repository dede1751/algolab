#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/biconnected_components.hpp>

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
using EdgeDesc = boost::graph_traits<Graph>::edge_descriptor;

void testcase() {
	int n, m; std::cin >> n >> m;
	Graph g(n);
	std::map<EdgeDesc, int> map;
	
	for (int i = 0; i < m; i++) {
		int a, b; std::cin >> a >> b;
		boost::add_edge(a, b, g);
	}

	auto bc = boost::biconnected_components(g, boost::make_assoc_property_map(map));

	std::vector<int> counts(bc, 0);
	for (const auto i: map)
		counts[i.second]++;
	
	int k = 0;
	std::set<std::pair<int, int>> critical;
	for (const auto i: map)
		if (counts[i.second] == 1) {
			k++;

			int src = boost::source(i.first, g), tgt = boost::target(i.first, g);
			critical.insert(std::make_pair(std::min(src, tgt), std::max(src, tgt)));
		}
	
	std::cout << k << std::endl;
	for (const auto i: critical)
		std::cout << i.first << " " << i.second << "\n";
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
