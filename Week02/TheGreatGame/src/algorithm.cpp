#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <array>

typedef std::vector<std::vector<int>> ADJ;
typedef std::vector<std::pair<int, int>> DP;

int search(ADJ const &adj, DP &dp, const unsigned int pos, const bool minimize) {
	if (pos == dp.size() - 1)
		return 0;
	else {
		int memo = minimize ? dp[pos].second : dp[pos].first;
		
		if (memo != -1)
			return memo;
	}

	int res = minimize ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min();
	for (const auto tgt: adj[pos])
		if (minimize)
			res = std::min(res, 1 + search(adj, dp, tgt, false));
		else
			res = std::max(res, 1 + search(adj, dp, tgt, true));
	
	if (minimize) {
		dp[pos].second = res;
	} else {
		dp[pos].first = res;
	}

	return res;
}

void testcase() {
	int n, m, r, b; std::cin >> n >> m >> r >> b;
	ADJ adj(n + 1, std::vector<int>());
	DP dp(n + 1, std::pair<int,int>(-1, -1));

	for (int i = 0; i < m; i++) {
		int u, v; std::cin >> u >> v; 
		adj[u].push_back(v);
	}

	int red = search(adj, dp, r, true);
	int black = search(adj, dp, b, true);

	std::cout << (((red < black) || (red == black && red % 2 != 0)) ? '0' : '1') << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
