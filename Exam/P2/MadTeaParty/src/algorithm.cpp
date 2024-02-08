#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >	graph;
typedef	boost::graph_traits<graph>::edge_descriptor			edge_desc;
typedef	boost::graph_traits<graph>::out_edge_iterator			out_edge_it;

class edge_adder {
 	graph &G;

 	public:
	explicit edge_adder(graph &G) : G(G) {}

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

const int di[6] = {-1, -1, 0, 0, 1, 1};
const int dj[6] = {-1, 1, -1, 1, -1, 1};

void testcase() {
	int n, m; std::cin >> n >> m;
	std::vector<std::vector<int>> occs(n, std::vector<int>(m));

	int src = n * m, dst = src + 1;
	graph g(dst + 1);
	edge_adder adder(g);
 	auto rc_map = boost::get(boost::edge_residual_capacity, g);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			std::cin >> occs[i][j];

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (occs[i][j]) {
				if (j % 2 == 0) {
					adder.add_edge(i*m + j, dst, 1);
					continue;
				}

				adder.add_edge(src, i*m + j, 1);
				for (int k = 0; k < 6; k++) {
					int tgt_i = i + di[k], tgt_j = j + dj[k];

					if (tgt_i >= 0 && tgt_i < n && tgt_j >= 0 && tgt_j < m && occs[tgt_i][tgt_j])
						adder.add_edge(i*m + j, tgt_i*m + tgt_j, 1);
				}
			}
		}
	}

	boost::push_relabel_max_flow(g, src, dst);

	std::vector<int> vis(dst + 1, false);
	std::queue<int> Q;

	vis[src] = true;
	Q.push(src);
	while (!Q.empty()) {
		const int u = Q.front(); Q.pop();
		
		out_edge_it ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, g); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, g);
			if (rc_map[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}

	int count = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (occs[i][j]) {
				if ((vis[i*m + j] && j % 2 != 0) || (!vis[i*m + j] && j % 2 == 0))
					count++;
			}
		}
	}

	std::cout << count << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
