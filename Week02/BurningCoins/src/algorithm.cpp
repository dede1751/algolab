#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

typedef std::vector<std::vector<std::pair<int, int>>> MEMO;

std::pair<int, int> search(MEMO& memo, std::vector<int> &v, const int i, const int j, const int side) {
	if (i == j)
		return std::make_pair(0, 0);
	
	if (memo[i][j].first != -1)
		return memo[i][j];
	
	auto pick_start = search(memo, v, i + 1, j, 1 - side);
	int start_val = v[i] + (side ? pick_start.second : pick_start.first);

	auto pick_end = search(memo, v, i, j - 1, 1 - side);
	int end_val = v[j - 1] + (side ? pick_end.second : pick_end.first);

	if (start_val > end_val)
		memo[i][j] = std::make_pair(side ? pick_start.first : start_val, side ? start_val : pick_start.second);
	else
		memo[i][j] = std::make_pair(side ? pick_end.first : end_val, side ? end_val : pick_end.second);

	return memo[i][j];
}

void testcase() {
	int n; std::cin >> n;
	std::vector<int> v(n);

	for (auto& i: v)
		std::cin >> i;

	MEMO memo(n + 1, std::vector<std::pair<int, int>>(n + 1, std::make_pair(-1, -1)));
	std::cout << search(memo, v, 0, n, 0).first << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
