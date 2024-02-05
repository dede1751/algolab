#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase()
{
	int n, m; std::cin >> n >> m;
	std::vector<int> s(n);
	std::vector<int> w(m);
	std::vector<int> x(n, 0);

	for (int i = 0; i < n; i++)
		std::cin >> s[i];
	
	for (int i = 0; i < m; i++)
		std::cin >> w[i];

	std::sort(s.begin(), s.end(), std::greater<int>());
	std::sort(w.begin(), w.end(), std::greater<int>());

	if (s[0] < w[0]) {
		std::cout << "impossible" << std::endl;
		return;
	}

	for (int i = 0, j = 0; i < m; i++) {
		while (j < n && s[j] >= w[i])
			j++;

		(*std::lower_bound(x.begin(), x.begin() + j - 1, x[j - 1], std::greater<int>()))++;
	}

	std::cout << (x[0] * 3 - 1) << std::endl;
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
