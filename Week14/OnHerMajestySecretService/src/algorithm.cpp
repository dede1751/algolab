#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <tuple>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property,                         // no vertex property
  boost::property<boost::edge_weight_t, int>  // interior edge weight property
> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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
	int n, m, a, s, c, d; std::cin >> n >> m >> a >> s >> c >> d;
	std::vector<std::tuple<int, int, int>> dists;
	weighted_graph g(n + a + s);

	for (int i = 0; i < m; i++) {
		char w; std::cin >> w;
		int x, y, z; std::cin >> x >> y >> z;
		boost::add_edge(x, y, z, g);

		if (w == 'L')
			boost::add_edge(y, x, z, g);
	}

	for (int i = 0; i < a; i++) {
		int p; std::cin >> p;
		boost::add_edge(n + i, p, 0, g);
	}

	for (int i = 0; i < s; i++) {
		int p; std::cin >> p;
		boost::add_edge(p, n + a + i, d, g);
	}

	for (int i = 0; i < a; i++) {
		std::vector<int> dist_map(n + a + s);

		boost::dijkstra_shortest_paths(
			g, n + i,
			boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, g)))
		);

		for (int j = 0; j < s; j++) {
			int dist = dist_map[n + a + j];

			if (dist != std::numeric_limits<int>::max()) {
				dists.push_back({dist, i, a + j});

				if (c == 2)
					dists.push_back({dist + d, i, a + s + j});
			}
		}
	}

	std::sort(dists.begin(), dists.end());

	int start = 0, end = dists.size();
	while (start < end) {
		int mid = start + (end - start) / 2;
		int src = a + 2*s, dst = src + 1;
		graph g(dst + 1);
		edge_adder adder(g);

		for (int i = 0; i < a; i++)
			adder.add_edge(src, i, 1);

		for (int i = 0; i <= mid; i++)
			adder.add_edge(std::get<1>(dists[i]), std::get<2>(dists[i]), 1);
		
		for (int i = 0; i < s; i++) {
			adder.add_edge(a + i, dst, 1);
			adder.add_edge(a + s + i, dst, 1);
		}

		if (boost::push_relabel_max_flow(g, src, dst) == a)
			end = mid;
		else
			start = mid + 1;
	}

	std::cout << std::get<0>(dists[start]) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
