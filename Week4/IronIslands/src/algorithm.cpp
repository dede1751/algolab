#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_map>

void testcase() {
	int n, k, w; std::cin >> n >> k >> w;
	std::vector<int> c(n);
	std::vector<std::vector<int>> ww(w);
	std::unordered_map<int, std::vector<std::pair<int, int>>> wwm;

	for (auto &i: c)
		std::cin >> i;
	
	for (int i = 0; i < w; i++) {
		int l; std::cin >> l;
		std::vector<int> r(l + 1, 0);

		for (int j = 1; j <= l; j++) {
			int x; std::cin >> x;
			r[j] = r[j - 1] + c[x];

			auto best = wwm.find(r[j]);
			if (best == wwm.end()) {
				auto v = std::vector<std::pair<int, int>>(2, std::make_pair(-1, -1));
				v[0].first = i;
				v[0].second = j;
				wwm.insert(std::make_pair(r[j], v));
			} else if (j > best->second[0].second) {
				best->second[1] = best->second[0];
				best->second[0].first = i;
				best->second[0].second = j;
			} else if (j > best->second[1].second) {
				best->second[1].first = i;
				best->second[1].second = j;
			}
		}
		ww[i] = r;
	}

	int max = c[0] == k ? 1 : 0;
	for (int w1 = 0; w1 < w; w1++) {
		int i = 2, j = 2;
		while (j < (int)ww[w1].size()) {
			int err = k - (ww[w1][j] - ww[w1][i - 1]);
			if (err == 0)
				max = std::max(max, j - i + 1);

			if (err < 0 && i < j)
				i++;
			else
				j++;
		}

		for (int i = 2; i < (int)ww[w1].size(); i++) {
			int rest = k - (ww[w1][i] - c[0]);
			auto tgt = wwm.find(rest);
			
			if (tgt != wwm.end()) {
				int w2 = tgt->second[0].first, w3 = tgt->second[1].first;
				if (w1 != w2)
					max = std::max(max, i - 1 + tgt->second[0].second);
				else if (w3 != -1 && w1 != w3)
					max = std::max(max, i - 1 + tgt->second[1].second);
			}
		}
	}

	std::cout << max << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
