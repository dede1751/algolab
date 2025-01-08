#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include <stack>

typedef struct node {
	int discovery, completion;
	std::set<int> edges;

	node(): discovery(-1), completion(-1), edges({}) {}
} node_t;

int dfs(std::vector<node_t> &g, int curr, int step) {
	g[curr].discovery = step++;
	for (const auto child: g[curr].edges)
		if (g[child].discovery == -1)
			step = dfs(g, child, step);
	
	g[curr].completion = step++;
	return step;
}

void testcase() {
	int n, m, v; std::cin >> n >> m >> v;
	std::vector<node_t> g(n, node());

	while (m--) {
		int a, b; std::cin >> a >> b;
		g[a].edges.insert(b);
		g[b].edges.insert(a);
	}

	dfs(g, v, 0);

	for (const auto n: g)
		std::cout << n.discovery << " ";
	std::cout << std::endl;

	for (const auto n: g)
		std::cout << n.completion << " ";
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
