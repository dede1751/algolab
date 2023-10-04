#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

typedef std::vector<std::vector<std::pair<int, int>>> MEMO;

std::pair<int, int> search(
	MEMO& memo,
	std::vector<int> &v,
	const int i,
	const int j,
	const int p,
	const int m,
	const int k
) {
    if (i == j)
		return std::make_pair(0, 0);
	
	if (memo[i][j].first != -1)
		return memo[i][j];

	int next = (p + 1 == m) ? 0 : p + 1;
	auto pick_start = search(memo, v, i + 1, j, next, m, k);
	int start_val = v[i] + (p != k ? pick_start.second : pick_start.first);

	auto pick_end = search(memo, v, i, j - 1, next, m, k);
	int end_val = v[j - 1] + (p != k ? pick_end.second : pick_end.first);

	if (start_val > end_val)
		memo[i][j] = std::make_pair(p != k ? pick_start.first : start_val, p != k ? start_val : pick_start.second);
	else
		memo[i][j] = std::make_pair(p != k ? pick_end.first : end_val, p != k ? end_val : pick_end.second);

	return memo[i][j];
}


void testcase() {
	int n, m, k; std::cin >> n >> m >> k;
	std::vector<int> v(n);

	for (auto& i: v)
		std::cin >> i;
	
	MEMO memo(n + 1, std::vector<std::pair<int, int>>(n + 1, std::make_pair(-1, -1)));
	std::cout << search(memo, v, 0, n, 0, m, k).first << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
