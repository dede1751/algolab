#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
using VertexDesc = boost::graph_traits<Graph>::vertex_descriptor;

void testcase() {
	int n, c, f; std::cin >> n >> c >> f;
	std::vector<std::unordered_set<std::string>> s(n, std::unordered_set<std::string>{});
	std::vector<VertexDesc> mate_map(n);
	Graph g(n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < c; j++) {
			std::string x; std::cin >> x;
			s[i].insert(x);
		}

		for (int j = 0; j < i; j++) {
			int count = 0;
			for (const auto &p: s[i]) {
				if (s[j].find(p) != s[j].end())
					count++;
				
				if (count > f) {
					boost::add_edge(i, j, g);
					break;
				}
			}
		}
	}

	boost::edmonds_maximum_cardinality_matching(
		g,
		boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, g))
	);

	std::string res = "not optimal";
	for (const auto &i: mate_map)
		if (mate_map[i] >= (unsigned long int)n) { // null vertex is broken for some reason???
			res = "optimal";
			break;
		}
	
	std::cout << res << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
