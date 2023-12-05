#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

typedef std::vector<std::vector<std::pair<int,long>>> adjacency;
typedef std::vector<int> memo;

//adjacency adj;
adjacency forward_adj;
adjacency backward_adj;
memo mem;
//std::vector<bool> taken;

long solve(const int fwd, const long rats) {
	if (fwd == (int)forward_adj.size() - 1)
		return rats;

	long best = -1;
	for (const auto& [fwd_tgt, fwd_r]: forward_adj[fwd]) {
		
		best = std::max(best, solve(fwd_tgt, rats + fwd_r));
	}

	return best;
}

// int solve(const int src, bool forward, const int rats) {
// 	if (src == 0 && !forward)
// 		return rats;
// 	else if (src == (int)adj.size() - 1)
// 		forward = false;

// 	int best = -1;
// 	for (const auto& [tgt, r]: adj[src]) {
// 		if (forward && src < tgt) {
// 			taken[tgt] = true;
// 			int res = solve(tgt, forward, rats + r);
// 			taken[tgt] = false;

// 			best = std::max(best, res);
// 		} else if (!forward && src > tgt && !taken[tgt]) {
// 			int res = solve(tgt, forward, rats + r);
// 			best = std::max(best, res);
// 		}
// 	}

// 	return best;
// }

void testcase() {
	int n, m; std::cin >> n >> m;
	forward_adj = adjacency(n, std::vector<std::pair<int, long>>());
	backward_adj = adjacency(n, std::vector<std::pair<int, long>>());
	mem = memo(n, -1);


	// adj = adjacency(n, std::vector<std::pair<int, int>>());
	// taken = std::vector<bool>(n, false);

	for (int i = 0; i < m; i++) {
		int u, v, f; std::cin >> u >> v >> f;
		//adj[u].push_back({v, f});

		if (u < v)
			forward_adj[u].push_back({v, f});
		else
			backward_adj[v].push_back({u, f});
	}

	std::cout << 2 * solve(0, 0) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
