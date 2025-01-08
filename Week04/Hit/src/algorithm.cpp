#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;

void testcase(long n) {
	long x, y, a, b; std::cin >> x >> y >> a >> b;
	K::Point_2 p1(x, y), p2(a, b);
	K::Ray_2 ray(p1, p2);

	for (int i = 0; i < n; i++) {
		long r, s, t, u; std::cin >> r >> s >> t >> u;
		K::Point_2 start(r, s), end(t, u);
		K::Segment_2 segment(start, end);

		if (CGAL::do_intersect(ray, segment)) {
			for (; i < n - 1; i++)
				std::cin >> r >> s >> t >> u;
			
			std::cout << "yes" << std::endl;
			return;
		}
	}
	std::cout << "no" << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	long n; std::cin >> n;
	while (n) {
		testcase(n);
		std::cin >> n;
	}
}
