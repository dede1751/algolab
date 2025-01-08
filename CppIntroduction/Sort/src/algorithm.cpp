#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
	int n; std::cin >> n;
	std::vector<int> v(n);

	for (int i = 0; i < n; i++)
		std::cin >> v[i];
	
	std::random_shuffle(v.begin(), v.end());

	int x; std::cin >> x;
	if (x) {
		std::sort(v.begin(), v.end(), std::greater<int>());
	} else {
		std::sort(v.begin(), v.end());
	}

	for (const auto n: v)
    	std::cout << n << " ";
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
