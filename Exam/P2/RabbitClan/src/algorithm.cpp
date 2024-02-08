#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

typedef std::vector<std::vector<long>> vec2d;

void testcase() {
	int n, m; std::cin >> n >> m;
	vec2d dp(n + 1, std::vector<long>(m + 1, 0)), recsum(n + 1, std::vector<long>(m + 1, 0));

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			long r; std::cin >> r;
			recsum[i][j] = r + recsum[i - 1][j] + recsum[i][j - 1] - recsum[i - 1][j - 1];
		}
	}

	for (int i = n - 1; i >= 1; i--) {
		for (int j = m - 1; j >= 1; j--) {
			long east = dp[i][j + 1] + recsum[n][j] - recsum[i][j];
			long south = dp[i + 1][j] + recsum[i][m] - recsum[i][j];
			dp[i][j] = std::min(east, south);
		}
	}

	std::cout << dp[1][1] << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
