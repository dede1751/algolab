#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
	std::string a, b;
	std::cin >> a >> b;

	std::string c = a, d = b;
	std::reverse(c.begin(), c.end());
	std::reverse(d.begin(), d.end());
	
	char tmp = c[0];
	c[0] = d[0];
	d[0] = tmp;

	std::cout << a.length() << " " << b.length() << std::endl;
	std::cout << a + b << std::endl;
	std::cout << c << " " << d << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
