#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>

typedef struct node {
	int distance;
	std::set<int> edges;

	node(): distance(-1), edges({}) {}
} node_t;

void bfs(std::vector<node_t> &g, int root) {
	std::queue<int> q = {};

	g[root].distance = 0;
	q.push(root);
	
	while (!q.empty()) {
		int curr = q.front();
		q.pop();

		for (const auto child: g[curr].edges)
			if (g[child].distance == -1) {
				g[child].distance = g[curr].distance + 1;
				q.push(child);
			}
	}
}

void testcase() {
	int n, m, v; std::cin >> n >> m >> v;
	std::vector<node_t> g(n, node());

	while (m--) {
		int a, b; std::cin >> a >> b;
		g[a].edges.insert(b);
		g[b].edges.insert(a);
	}

	bfs(g, v);

  	for (const auto n: g)
		std::cout << n.distance << " ";
	std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
