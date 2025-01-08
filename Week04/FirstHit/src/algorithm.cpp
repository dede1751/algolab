#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using K = CGAL::Exact_predicates_exact_constructions_kernel;
using P = K::Point_2;
using S = K::Segment_2;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase(long n)
{
	long x, y, a, b; std::cin >> x >> y >> a >> b;
	P p1(x, y), p2(a, b);
	S best;
	K::Ray_2 ray(p1, p2);
	bool found = false;
	std::vector<S> segments(n);
	
	for (int i = 0; i < n; i++) {
		long r, s, t, u; std::cin >> r >> s >> t >> u;
		P start(r, s), end(t, u);
		segments[i] = S(start, end);
	}
	
	std::random_shuffle(segments.begin(), segments.end());
	for (const auto segment: segments) {
		auto result = found ? CGAL::intersection(best, segment) : CGAL::intersection(ray, segment);
		
		if (result) {
			found = true;

			if (const P* op = boost::get<P>(&*result))
				best = S(p1, *op);
			else if (const S* os = boost::get<S>(&*result)) {
				P d1 = os->start(), d2 = os->end();

				if (CGAL::has_smaller_distance_to_point(p1, d1, d2))
				best = S(p1, d1);
				else
				best = S(p1, d2);
			}
		}
	}

	if (!found)
		std::cout << "no" << std::endl;
	else {
		std::cout << std::setprecision(0) << std::fixed;
		std::cout << floor_to_double(best.end().x()) << " " << floor_to_double(best.end().y()) << std::endl;
	}
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	long n; std::cin >> n;
	while (n) {
		testcase(n);
		std::cin >> n;
	}
}
