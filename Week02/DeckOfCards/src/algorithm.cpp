#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
	int n, k; std::cin >> n >> k;
	std::vector<int> v(n + 1, 0);

	for (int i = 1; i <= n; i++) {
		int a; std::cin >> a;
		v[i] = v[i - 1] + a;
	}

	int i = 1, j = 1, best_i = 0, best_j = 0, best_err = std::numeric_limits<int>::max();
	while (j <= n) {
		int err = k - (v[j] - v[i - 1]);
		int abs_err = std::abs(err);

		if (abs_err < best_err) {
			best_i = i;
			best_j = j;
			best_err = abs_err;
		}

		if (err < 0 && i < j) {
			i++;
		} else {
			j++;
		}
	}

	std::cout << best_i - 1 << " " << best_j - 1 << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
