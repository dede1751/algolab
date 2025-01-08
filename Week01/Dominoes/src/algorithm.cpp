#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
	int n; std::cin >> n;
	int covered = 1;
	std::vector<int> d(n, 0);

	for (int i = 0; i < n; i++) {
		int h; std::cin >> h;

		if (covered > i) {
			covered = std::max(covered, i + h);
		}
	}

	std::cout << std::min(covered, n) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
