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

void testcase() {
	int n; std::cin >> n;
	int src = n * n, dst = src + 1;
	graph g(dst + 1);
	edge_adder adder(g);
 	auto rc_map = boost::get(boost::edge_residual_capacity, g);

	int board[n][n];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			std::cin >> board[i][j];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (board[i][j]) {
				if ((i + j) % 2 != 0)
					adder.add_edge(i * n + j, dst, 1);
				else {
					int x_offsets[8] = {2, 2, 1, 1, -1, -1, -2, -2};
					int y_offsets[8] = {1, -1, 2, -2, 2, -2, 1, -1};

					for (int offset = 0; offset < 8; offset++) {
						int x = j + x_offsets[offset], y = i + y_offsets[offset];

						if (x >= 0 && x < n && y >= 0 && y < n && board[y][x])
							adder.add_edge(i * n + j, y * n + x, 1);
					}

					adder.add_edge(src, i * n + j, 1);
				}
			}

	boost::push_relabel_max_flow(g, src, dst);
	std::vector<int> vis(n * n + 2, false);
	std::queue<int> Q;
	vis[src] = true;
	Q.push(src);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		
		out_edge_it ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, g); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, g);
			if (rc_map[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}

	int count = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			if (board[i][j]) {
				if ((i + j) % 2 != 0)
					count += 1 - vis[i * n + j];
				else
					count += vis[i * n + j];
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
