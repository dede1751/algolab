#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_set>

typedef std::vector<std::vector<std::pair<int,long>>> adjacency;
typedef std::vector<std::vector<long>> dp;

void testcase() {
	int n, m; std::cin >> n >> m;
	adjacency fwd_adj(n, std::vector<std::pair<int, long>>());
	adjacency bwd_adj(n, std::vector<std::pair<int, long>>());
	dp dp(n, std::vector<long>(n, -1));

	for (int i = 0; i < m; i++) {
		int u, v, f; std::cin >> u >> v >> f;

		if (u < v)
			fwd_adj[u].push_back({v, f});
		else
			bwd_adj[v].push_back({u, f});
	}

	dp[0][0] = 0;
	for (int sum = 0; sum <= 2*(n - 1); sum++) {
		for (int i = 0; i <= sum; i++) {
			int j = sum - i;

			if (i >= n
				|| j >= n
				|| (i == j && i != 0)
				|| dp[i][j] == -1)
				continue;

			for (const auto [next_i, p_i]: fwd_adj[i])
				if (next_i >= j)
					dp[next_i][j] = std::max(dp[next_i][j], dp[i][j] + p_i);
			
			for (const auto [next_j, p_j]: bwd_adj[j])
				if (next_j >= i)
					dp[i][next_j] = std::max(dp[i][next_j], dp[i][j] + p_j);
		}
	}

	std::cout << dp[n - 1][n - 1] << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
