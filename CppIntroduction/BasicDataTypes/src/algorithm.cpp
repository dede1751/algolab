#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
	int i;
	long l;
	std::string s;
	double d;

	std::cin >> i >> l >> s >> d;
	std::cout << std::setprecision(2)
		<< i << " "
		<< l << " "
		<< s << " "
		<< std::fixed << d << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
