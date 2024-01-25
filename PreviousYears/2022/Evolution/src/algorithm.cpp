#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_map>

void testcase() {
	int n, q; std::cin >> n >> q;
	std::unordered_map<std::string, int> s2i;
	std::vector<std::string> i2s(n);
	std::vector<bool> leaf(n, true);
	std::vector<int> ages(n),pred_map(n, -1);

	std::vector<std::vector<std::pair<int, int>>> lists;
	std::vector<std::pair<int, int>> lookup(n);

	for (int i = 0; i < n; i++) {
		std::string s; std::cin >> s;
		int a; std::cin >> a;

		i2s[i] = s;
		s2i[s] = i;
		ages[i] = a;
	}

	for (int i = 1; i < n; i++) {
		std::string s, p; std::cin >> s >> p;
		pred_map[s2i[s]] = s2i[p];
		leaf[s2i[p]] = false;
	}

	for (int i = 0; i < n; i++) {
		if (!leaf[i])
			continue;

		int list_idx = lists.size();
		lookup[i] = {list_idx, 0};
		lists.push_back({{ages[i], i}});

		for (int curr = pred_map[i]; curr != -1; curr = pred_map[curr]) {
			lookup[curr] = {list_idx, lists[list_idx].size()};
			lists[list_idx].push_back({ages[curr], curr});
		}
	}

	for (int i = 0; i < q; i++) {
		std::string s; std::cin >> s;
		int b; std::cin >> b;

		auto l = lookup[s2i[s]];
		int start = l.second, end = lists[l.first].size();
		while (start < end) {
			int mid = start + (end - start) / 2;

			if (lists[l.first][mid].first <= b)
				start = mid + 1;
			else
				end = mid;
		}

		std::cout << i2s[lists[l.first][start - 1].second] << " ";
	}
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
