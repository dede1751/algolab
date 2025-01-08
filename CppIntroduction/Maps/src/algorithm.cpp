#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <set>

void testcase() {
	std::map<std::string, std::set<unsigned int>> m = {};
	int q; std::cin >> q;

	while (q--) {
		unsigned int a; std::cin >> a;
		std::string b; std::cin >> b;

		if (a) {
			auto s = m.find(b);

			if (s != m.end())
				s->second.insert(a);
			else
				m.insert(std::make_pair(b, std::set<unsigned int>({a})));
		} else
			m.erase(b);
	}

	std::string s; std::cin >> s;
	auto tgt = m.find(s);

	if (tgt == m.end())
		std::cout << "Empty";
	else
		for (auto const& a: tgt->second)
			std::cout << a << " ";
	
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
