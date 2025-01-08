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

#define MAX_T 100000L
#define MAX_P 100L

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
			auto w_map = boost::get(boost::edge_weight, G); // new!
			const edge_desc e = boost::add_edge(from, to, G).first;
			const edge_desc rev_e = boost::add_edge(to, from, G).first;
			c_map[e] = capacity;
			c_map[rev_e] = 0; // reverse edge has no capacity!
			r_map[e] = rev_e;
			r_map[rev_e] = e;
			w_map[e] = cost;   // new assign cost
			w_map[rev_e] = -cost;   // new negative cost
		}
};

typedef struct Trip {
	int idx, depart, arrive, dst, p;
} Trip;

int n, s;
std::vector<int> l;
std::vector<std::vector<Trip>> trips;

void testcase() {
	std::cin >> n >> s;
	l = std::vector<int>(s);
	trips = std::vector<std::vector<Trip>>(s, std::vector<Trip>());

	int src = n + s, snk = src + 1;
	graph g(snk + 1);
	edge_adder adder(g);

	long tot_l = 0;
	for (int i = 0; i < s; i++) {
		std::cin >> l[i];
		tot_l += l[i];
	}

	for (int i = 0; i < n; i++) {
		int si, ti, di, ai, pi; std::cin >> si >> ti >> di >> ai >> pi;
		trips[si - 1].push_back({i, di, ai, ti - 1, pi});
	}

	for (int i = 0; i < s; i++) {
		auto& t = trips[i];

		t.push_back({n + i, MAX_T, MAX_T + 1, -1, 0});
		std::sort(t.begin(), t.end(),
			[](const Trip& t1, const Trip& t2) -> bool {
	      		return t1.depart < t2.depart;
        });

		adder.add_edge(src, t[0].idx, l[i], MAX_P*t[0].depart);
		adder.add_edge(n + i, snk, tot_l, 0);

		for (int j = 1; j < t.size(); j++)
			adder.add_edge(t[j - 1].idx, t[j].idx, tot_l, MAX_P*(t[j].depart - t[j - 1].depart));
	}

	for (int i = 0; i < s; i++) {
		for (const Trip& ti: trips[i]) {
			if (ti.dst == -1)
				continue;

			auto tj = std::lower_bound(trips[ti.dst].begin(), trips[ti.dst].end(),
				Trip({0, ti.arrive, 0, 0, 0}),
				[](const Trip& t1, const Trip& t2) -> bool {
					return t1.depart < t2.depart;
			});

			if (tj != trips[ti.dst].end())
				adder.add_edge(ti.idx, tj->idx, 1, MAX_P*(tj->depart - ti.depart) - ti.p);
		}
	}

	boost::successive_shortest_path_nonnegative_weights(g, src, snk);
	long cost = boost::find_flow_cost(g);    
    
	std::cout << (tot_l*MAX_P*MAX_T - cost) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
