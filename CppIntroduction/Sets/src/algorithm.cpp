#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>

void testcase() {
	std::set<int> s = {};
	int q; std::cin >> q;

	while (q--) {
		int a, b; std::cin >> a >> b;

		if (a) {
			auto rm = s.find(b);
			if (rm != s.end())
				s.erase(rm);
		} else 
			s.insert(b);
	}

	for (int i: s)
			std::cout << i << " ";
		
		if (s.empty())
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
