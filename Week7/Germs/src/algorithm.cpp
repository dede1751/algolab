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
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using P = K::Point_2;
using Vb = CGAL::Triangulation_vertex_base_with_info_2<int, K>;
using Fb = CGAL::Triangulation_face_base_2<K>;
using Tds = CGAL::Triangulation_data_structure_2<Vb,Fb>;
using Triangulation = CGAL::Delaunay_triangulation_2<K, Tds>;

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
	std::vector<std::pair<P, int>> points(n);

	for (int i = 0; i < n; i++) {
		int x, y; std::cin >> x >> y;
		points[i] = std::make_pair(P(x, y), i);
	}

	tr.insert(points.begin(), points.end());
	
	std::vector<std::tuple<K::FT, int, int>> dists;
	for (auto vh: tr.finite_vertex_handles()) {
		auto neighbor = CGAL::nearest_neighbor(tr, vh);
		auto n_dist = CGAL::squared_distance(vh->point(), neighbor->point());

		long x = vh->point().x(), y = vh->point().y();
		long r_dist = std::min(r - x, std::min(x - l, std::min(t - y, y - b)));
		r_dist = 4 * r_dist * r_dist;

		if (r_dist > n_dist)
			dists.push_back(std::make_tuple(n_dist, vh->info(), neighbor->info()));
		else
			dists.push_back(std::make_tuple(r_dist, vh->info(), n));
	}

	std::sort(dists.begin(), dists.end());

	int eliminated = 0, first = -1, half = -1, last = -1;
	std::vector<bool> alive(n + 1, true);
	alive[n] = false;

	for (auto &i: dists) {
		auto d = std::get<0>(i);
		int src = std::get<1>(i), tgt = std::get<2>(i);

		if (eliminated == 0)
			first = time_of_death(d);

		eliminated += alive[src] + alive[tgt];
		alive[src] = alive[tgt] = false;

		if (eliminated > n / 2 && half == -1)
			half = time_of_death(d);
		else if (eliminated == n)
			last = time_of_death(d);
	}

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
