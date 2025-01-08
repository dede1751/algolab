#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;

class edge_adder {
	graph &G;

	public:
		explicit edge_adder(graph &G) : G(G) {}
		void add_edge(int from, int to, long capacity, long cost) {
			auto c_map = boost::get(boost::edge_capacity, G);
			auto r_map = boost::get(boost::edge_reverse, G);
			auto w_map = boost::get(boost::edge_weight, G);
			const edge_desc e = boost::add_edge(from, to, G).first;
			const edge_desc rev_e = boost::add_edge(to, from, G).first;
			c_map[e] = capacity;
			c_map[rev_e] = 0;
			r_map[e] = rev_e;
			r_map[rev_e] = e;
			w_map[e] = cost;
			w_map[rev_e] = -cost;
		}
};

void testcase() {
	int e, w, m, d, p, l; std::cin >> e >> w >> m >> d >> p >> l;
	int src = e + w, extra = src + 1, danger = extra + 1;
	int dst = danger + 1, extra_dst = dst + 1;
	graph g(extra_dst + 1);
	edge_adder adder(g);

	if (p < l * std::max(e, w)) {
		std::cout << "No schedule!" << std::endl;

		for (int i = 0; i < m + d; i++) {
			int u, v, r; std::cin >> u >> v >> r;
		}
		return;
	}

	adder.add_edge(src, extra, p - l*e, 0);
	adder.add_edge(extra, danger, p - l*e, 0);
	adder.add_edge(extra_dst, dst, p - l*w, 0);

	for (int i = 0; i < e; i++) {
		adder.add_edge(src, i, l, 0);
		adder.add_edge(extra, i, p, 0);
	}

	for (int i = e; i < e + w; i++) {
		adder.add_edge(i, dst, l, 0);
		adder.add_edge(i, extra_dst, p, 0);
	}

	for (int i = 0; i < m; i++) {
		int u, v, r; std::cin >> u >> v >> r;
		adder.add_edge(u, e + v, 1, r);
	}

	for (int i = 0; i < d; i++) {
		int u, v, r; std::cin >> u >> v >> r;
		adder.add_edge(danger, extra_dst, 1, r);
	}

	boost::successive_shortest_path_nonnegative_weights(g, src, dst);
    int cost = boost::find_flow_cost(g);

	int flow = 0;
	auto c_map = boost::get(boost::edge_capacity, g);
    auto rc_map = boost::get(boost::edge_residual_capacity, g);
    out_edge_it eg, eend;
    for(boost::tie(eg, eend) = boost::out_edges(boost::vertex(src, g), g); eg != eend; ++eg) {
        flow += c_map[*eg] - rc_map[*eg];    
    }

	if (flow == p)
		std::cout << cost << std::endl;
	else
		std::cout << "No schedule!" << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
