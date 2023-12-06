#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
	int n; std::cin >> n;
	int even = 1, odd = 0, sum = 0;

	while (n--) {
		int a; std::cin >> a;
		sum += a;

		if (sum % 2) {
			odd++;
		} else {
			even++;
		}
	}

	std::cout << (even * (even - 1)) / 2 + (odd * (odd - 1)) / 2 << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
