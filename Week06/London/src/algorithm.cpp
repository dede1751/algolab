#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
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

const int ALPHABET = 26;

void testcase() {
	int h, w; std::cin >> h >> w;
	std::vector<std::vector<char>> front(h, std::vector<char>(w)), back(h, std::vector<char>(w));
	int in_pairs[ALPHABET][ALPHABET] = {0};
	int out_occs[ALPHABET] = {0};

	std::string line; std::cin >> line;
	for (auto c: line)
		out_occs[c - 'A']++;

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++) {
			char x; std::cin >> x;
			front[i][j] = x - 'A';
		}
	
	for (int i = 0; i < h; i++)
		for (int j = w - 1; j >= 0; j--) {
			char x; std::cin >> x;
			back[i][j] = x - 'A';
		}
	
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++) {
			int f = front[i][j], b = back[i][j];
			in_pairs[std::min(f, b)][std::max(f, b)]++;
		}
	
	graph g((ALPHABET + 1) * ALPHABET + 2);
	edge_adder adder(g);
	const int src = (ALPHABET + 1) * ALPHABET, dst = src + 1;
	for (int f = 0; f < ALPHABET; f++)
		for (int b = f; b < ALPHABET; b++) {
			adder.add_edge(src, f * ALPHABET + b, in_pairs[f][b]);
			adder.add_edge(f * ALPHABET + b, ALPHABET * ALPHABET + f, in_pairs[f][b]);
			adder.add_edge(f * ALPHABET + b, ALPHABET * ALPHABET + b, in_pairs[f][b]);
		}
	
	for (int i = 0; i < ALPHABET; i++)
		adder.add_edge(ALPHABET * ALPHABET + i, dst, out_occs[i]);

	long flow = boost::push_relabel_max_flow(g, src, dst);
	std::cout << ((flow == (long)line.length()) ? "Yes" : "No") << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
