#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

typedef std::vector<std::vector<int>> DP;

int search(
  std::vector<int> const &v,
  DP &dp,
  std::vector<int>::const_iterator start,
  const int m,
  const int k
) {
	int idx = start - v.begin();

	if (m == 0)
		return 0;
	else if (start == v.end())
		return -1;
	else if (dp[idx][m] != -2)
		return dp[idx][m];

	for (auto j = dp[idx].begin() + m - 1; j != dp[idx].begin(); j--)
		if (*j == -1)
			return -1;

	auto next_best = start;
	dp[idx][m] = -1;
	for (auto i = start; i != v.end(); i++) {
		if (*i != -1) {
			int rest = search(v, dp, i + *i, m - 1, k);

			if (rest != - 1 && rest + *i > dp[idx][m]) {
				dp[idx][m] = rest + *i;
				next_best = i;
			}
		}
	}

	for (auto i = next_best; i != start; i--) {
		dp[i - v.begin()][m] = dp[idx][m];
	}

	return dp[idx][m];
}

void testcase() {
	int n, m, k; std::cin >> n >> m >> k;
	std::vector<int> v(n + 1, 0);
	DP dp(n + 1, std::vector<int>(m + 1, -2));

	for (int i = 1; i <= n; i++) {
		int x; std::cin >> x;
		v[i] = v[i - 1] + x;
	}

	for (auto i = v.begin(); i != v.end(); i++) {
		auto tgt = std::lower_bound(i + 1, v.end(), *i + k);

		if (tgt == v.end() || *tgt != *i + k)
			*i = -1;
		else 
			*i = tgt - i;
	}

	int res = search(v, dp, v.begin(), m, k);
	if (res == -1) 
		std::cout << "fail" << std::endl;
	else
		std::cout << res << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
