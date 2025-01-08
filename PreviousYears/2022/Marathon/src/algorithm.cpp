#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      graph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;

#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> flow_graph;
typedef	boost::graph_traits<flow_graph>::edge_descriptor			edge_desc;
typedef	boost::graph_traits<flow_graph>::out_edge_iterator			out_edge_it;

class edge_adder {
	flow_graph &G;

	public:
		explicit edge_adder(flow_graph &G) : G(G) {}

		void add_edge(int from, int to, long capacity) {
			auto c_map = boost::get(boost::edge_capacity, G);
			auto r_map = boost::get(boost::edge_reverse, G);
			const edge_desc e = boost::add_edge(from, to, G).first;
			const edge_desc rev_e = boost::add_edge(to, from, G).first;
			c_map[e] = capacity;
			c_map[rev_e] = 0;
			r_map[e] = rev_e;
			r_map[rev_e] = e;
		}
};

void testcase() {
	int n, m, s, f; std::cin >> n >> m >> s >> f;
	std::vector<std::vector<std::tuple<int, int, int>>> adj(n, std::vector<std::tuple<int, int, int>>());
	graph g(n);

	flow_graph fg(n);
	edge_adder adder(fg);

	for (int i = 0; i < m; i++) {
		int a, b, c, d; std::cin >> a >> b >> c >> d;
		boost::add_edge(a, b, d, g);
		adj[a].push_back({b, c, d});
		adj[b].push_back({a, c, d});
	}

	std::vector<int> dist_map(n);
	boost::dijkstra_shortest_paths(g, s,
		boost::distance_map(boost::make_iterator_property_map(
		dist_map.begin(), boost::get(boost::vertex_index, g))));
	
	std::queue<int> q;
	std::vector<bool> visited(n, false);
	q.push(f);
	while (!q.empty()) {
		int curr = q.front(); q.pop();
		if (curr == s || visited[curr])
			continue;
		
		visited[curr] = true;
		int min_dist = dist_map[curr];
		for (const auto &[pred, c, d]: adj[curr]) {
			if (dist_map[pred] != std::numeric_limits<int>::max()
				&& dist_map[pred] + d == min_dist
			) {
				q.push(pred);
				adder.add_edge(pred, curr, c);
			}
		}
	}
	
	std::cout << boost::push_relabel_max_flow(fg, s, f) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
