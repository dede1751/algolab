#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
	int n; std::cin >> n;
	int sum = 0;

	while (n--) {
		int x; std::cin >> x;
		sum += x;
	}
	
	std::cout << sum << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
