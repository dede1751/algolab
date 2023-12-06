#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

using AP = std::vector<std::pair<long, long>>;
using MEMO = std::vector<std::vector<std::vector<long>>>;

void fill_dp(AP& a_pots, MEMO& m, int H, int n) {
	for (int i = 0; i < n; i++)
		for (int j = 1, sum = 0; j <= n; j++) {
			if (i + j - 1 < n)
				sum += a_pots[i + j - 1].first;
			
			m[i][H][j] = sum;
		}

	for (int i = n - 1; i >= 0; i--)
		for (int h = H - 1; h >= 0; h--)
			for (int j = 1; j <= n; j++) {
				int new_h = std::min(H, h + (int)a_pots[i].second);
				long take = m[i + 1][new_h][j - 1], leave = m[i + 1][h][j];

				if (take != 0 || new_h == H)
					take += a_pots[i].first;

				m[i][h][j] = std::max(take, leave);
			}
}

void testcase() {
	long n, m, a, b, P, H, W; std::cin >> n >> m >> a >> b >> P >> H >> W;
	MEMO memo(n + 1, std::vector<std::vector<long>>(H + 1, std::vector<long>(n + 1, 0)));
	std::vector<std::pair<long, long>> a_pots(n);
	std::vector<long> b_pots(m);

	for (int i = 0; i < n; i++) {
		int p, h; std::cin >> p >> h;
		a_pots[i] = std::make_pair(p, h);
	}

	for (int i = 0; i < m; i++)
		std::cin >> b_pots[i];
	
	std::sort(b_pots.begin(), b_pots.end(), std::greater<int>());
	
	int start_idx = b_pots[0] >= W ? 0 : m;
	for (int i = 1; i < m; i++) {
		b_pots[i] += b_pots[i - 1];

		if (start_idx == m && b_pots[i] >= W)
			start_idx = i;
	}

	if (start_idx == m) {
		std::cout << "-1" << std::endl;
		return;
	}

	std::sort(a_pots.begin(), a_pots.end(), std::greater<std::pair<int, int>>());
	fill_dp(a_pots, memo, H, n);

	for (int i = start_idx; i < m; i++) {
		int max_pots = a == 0 ? n : (b_pots[i] - W) / a;

		for (long j = 0; j <= max_pots; j++)
			if (memo[0][0][j] >= P + (i + 1) * b) {
				std::cout << (i + j + 1) << std::endl;
				return;
			}
	}

	std::cout << "-1" << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
