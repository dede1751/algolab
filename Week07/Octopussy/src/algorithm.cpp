#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

int take_subtree(std::vector<bool>& taken, const int root) {
	if (root >= (int)taken.size() || taken[root])
		return 0;

	taken[root] = true;
	return 1 + take_subtree(taken, root * 2 + 1) + take_subtree(taken, root * 2 + 2);
}

void testcase() {
	int n; std::cin >> n;
	std::vector<std::pair<int, int>> t(n);
	std::vector<bool> taken(n, false);

	for (int i = 0; i < n; i++) {
		int ti; std::cin >> ti;
		t[i] = std::make_pair(ti, i);
	}

	std::sort(t.begin(), t.end());
	
	int time = 0;
	for (const auto& i: t) {
		int next_time = i.first, next = i.second;

		time += take_subtree(taken, next);
		if (time > next_time) {
			std::cout << "no" << std::endl;
			return;
		}
	}
	
	std::cout << "yes" << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
