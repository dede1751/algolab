#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

#define MAX 1000001
#define R 100

void testcase() {
	int n; std::cin >> n;
	std::vector<bool> beach(2 * MAX + 2 * R, false);

	for (int i = 0; i < n; i++) {
		int x; std::cin >> x;
		beach[x + MAX + R] = true;
	}

	int best = 0, total = 0;
	for (int i = R; i <= 2 * R; i++)
		total += beach[i];

	std::vector<int> totals(2 * MAX + 2 * R, 0);
	totals[R] = total;
	for (int i = R + 1; i < 2 * MAX + R; i++) {
		if (beach[i - R - 1])
			total--;
		if (beach[i + R])
			total++;
		
		totals[i] = total;
		best = std::max(best, total);
	}

	std::vector<int> distances(2 * MAX + 2 * R, std::numeric_limits<int>::max());
	int closest = std::numeric_limits<int>::max();
	for (int i = R; i < 2 * MAX + R; i++) {
		if (totals[i] != best)
			continue;

		for (int j = R; j >= 0; j--) {
			if (beach[i - j] || beach[i + j]) {
				distances[i] = j;
				break;
			}
		}

		closest = std::min(closest, distances[i]);
	}

	std::vector<int> optimal;
	optimal.reserve(2 * MAX);
	for (int i = R; i < 2 * MAX + R; i++)
		if (totals[i] == best && distances[i] == closest)
			optimal.push_back(i);

	std::cout << best << " " << closest << std::endl;
	for (const int x: optimal)
		std::cout << x - MAX - R << " ";
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
