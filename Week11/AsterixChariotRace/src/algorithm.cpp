#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <tuple>

std::vector<std::vector<int>> stages;
std::vector<long> costs;

std::tuple<long, long, long> solve(const int src) {
	if (stages[src].size() == 0)
		return {costs[src], costs[src], 0};
	
	long fix_me = costs[src], parent_save = 0;
	long best_child = std::numeric_limits<int>::max();

	for (const auto& s: stages[src]) {
		auto [fix_child, child_saved, save_child] = solve(s);
		long child_safe = std::min(fix_child, child_saved);

		fix_me += std::min(save_child, child_safe);
		parent_save += child_safe;
		best_child = std::min(best_child, fix_child - child_safe);
	}

	return {fix_me, parent_save + best_child, parent_save};
}

void testcase() {
	int n; std::cin >> n;
	stages = std::vector<std::vector<int>>(n, std::vector<int>());
	costs = std::vector<long>(n);

	for (int i = 0; i < n - 1; i++) {
		int a, b; std::cin >> a >> b;
		stages[a].push_back(b);
	}

	for (int i = 0; i < n; i++)
		std::cin >> costs[i];
	
	auto monza = solve(0);
	std::cout << std::min(std::get<0>(monza), std::get<1>(monza)) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
