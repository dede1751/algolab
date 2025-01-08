#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
	int n; std::cin >> n;
	std::vector<std::pair<int, int>> b(n);

	for (int i = 0; i < n; i++) {
		int l, p; std::cin >> l >> p;
		b[i] = std::make_pair(p, l);
	}

	std::sort(b.begin(), b.end());

	int count = 1;
	for (int i = 1, p = b[0].first; i < n;) {
		int next = b[n - 1].first + b[n - 1].second;
		for (; i < n  && next > b[i].first; i++)
			next = std::min(next, std::max(p + b[i].second, b[i].first));

		p = next;
		count++;
	}

	std::cout << count << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
