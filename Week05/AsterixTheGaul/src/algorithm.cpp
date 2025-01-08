#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <tuple>

bool reaches(const std::vector<std::pair<int, int>>& mov, const int s, const long D, const long T) {
	int n = (int) mov.size(), s1 = n / 2, s2 = n - s1;
	std::vector<std::pair<long, long>> l2;
	l2.reserve(1 << s2);

	for (int idx = 1; idx < (1 << s2); idx++) {
		long t = 0, d = 0;
		for (int i = 0; i < n; i++) {
			if (idx & (1 << i)) {
				t += mov[i + s1].first;
				d += mov[i + s1].second + s;
			}

			if (t >= T)
				continue;
			if (d >= D)
				return true;
		}

		l2.push_back(std::make_pair(t, d));
	}

	std::sort(l2.begin(), l2.end());

	std::vector<std::pair<long, long>> x2;
	x2.reserve(l2.size());
	x2.push_back(*l2.begin());
	for (std::size_t i = 1; i < l2.size(); i++)
		if (l2[i].second > (x2.end() - 1)->second)
			x2.push_back(l2[i]);
	
	for (int idx = 1; idx < (1 << s1); idx++) {	
		long t = 0, d = 0;
		for (int i = 0; i < n; i++) {
			if (idx & (1 << i)) {
				t += mov[i].first;
				d += mov[i].second + s;
			}

			if (t >= T)
				continue;
			if (d >= D)
				return true;
		}

		int start = 0, end = x2.size() - 1, boundary = 0;
		while (start <= end) {
			int mid = (start + end) / 2;

			if (t + x2[mid].first < T) {
				boundary = mid;
				start = mid + 1;
			} else
				end = mid - 1;
		}

		auto val = x2[boundary];
		if (t + val.first < T && d + val.second >= D)
			return true;
	}

	return false;
}

void testcase() {
	int n, m; long D, T; std::cin >> n >> m >> D >> T;
	std::vector<std::pair<int, int>> mov(n);
	std::vector<int> s(m + 1, 0);

	for (int i = 0; i < n; i++) {
		int d, t; std::cin >> d >> t;
		mov[i] = std::make_pair(t, d);
	}

	for (int i = 1; i <= m; i++)
		std::cin >> s[i];

	int start = 0, end = m, boundary = -1;
	while (start <= end) {
		int mid = (start + end) / 2;

		if (reaches(mov, s[mid], D, T)) {
			boundary = mid;
			end = mid - 1;
		} else
			start = mid + 1;
	}

	if (boundary == -1)
		std::cout << "Panoramix captured" << std::endl;
	else
		std::cout << boundary << std::endl;

}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
