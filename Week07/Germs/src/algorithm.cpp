#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using P = K::Point_2;
using Triangulation = CGAL::Delaunay_triangulation_2<K>;

int time_of_death(K::FT& square_dist)
{
	if (square_dist <= 1)
		return 0;

	K::FT d = CGAL::sqrt(square_dist);
	return (int) std::ceil(CGAL::sqrt((d - 1) / 2));
}

void testcase(int n)
{
	long l, b, r, t; std::cin >> l >> b >> r >> t;
	Triangulation tr;
	std::vector<P> points(n);

	for (int i = 0; i < n; i++) {
		int x, y; std::cin >> x >> y;
		points[i] = P(x, y);
	}

	tr.insert(points.begin(), points.end());
	
	std::vector<K::FT> dists;
	for (auto vh: tr.finite_vertex_handles()) {
		auto neighbor = CGAL::nearest_neighbor(tr, vh);
		long n_dist = CGAL::squared_distance(vh->point(), neighbor->point());

		long x = vh->point().x(), y = vh->point().y();
		long r_dist = std::min(r - x, std::min(x - l, std::min(t - y, y - b)));
		r_dist = 4 * r_dist * r_dist;

		dists.push_back(CGAL::min(r_dist, n_dist));
	}

	std::sort(dists.begin(), dists.end());

	int first = time_of_death(dists[0]);
	int half = time_of_death(dists[n / 2]);
	int last = time_of_death(dists[n - 1]);

	std::cout << first << " " << half << " " << last << std::endl;
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
