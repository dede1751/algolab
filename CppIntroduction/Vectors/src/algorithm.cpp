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

	for (int i = 0; i < n; ++i)
		std::cin >> v[i];

	int d; std::cin >> d;
	v.erase(v.begin() + d);

	int a, b; std::cin >> a >> b;
	v.erase(v.begin() + a, v.begin() + b + 1);

	for (int i: v)
		std::cout << i << " ";

	if (v.size() == 0)
		std::cout << "Empty";
	
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
