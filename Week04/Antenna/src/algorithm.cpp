#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

using K = CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt;
using P = K::Point_2;
using Traits = CGAL::Min_circle_2_traits_2<K>;
using MC = CGAL::Min_circle_2<Traits>;

double ceil_to_double(const K::FT& x)
{
	double a = std::ceil(CGAL::to_double(x));
	while (a + 1 > x) a--; 
	while (a < x) a++;
	return a;
}

void testcase(int n)
{
	std::vector<P> points(n);

	for (int i = 0; i < n; i++) {
		long x, y; std::cin >> x >> y;
		points[i] = P(x, y);
	}

	MC mc(points.begin(), points.end(), true);
	Traits::Circle c = mc.circle();
	std::cout << std::setprecision(0) << std::fixed;
	std::cout << ceil_to_double(CGAL::sqrt(c.squared_radius())) << std::endl;
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int n; std::cin >> n;
	while (n) {
		testcase(n);
		std::cin >> n;
	}
}
