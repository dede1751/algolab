#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel    K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>    Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef K::Point_2 P;
typedef std::tuple<long,long,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<P,Index> IPoint;

void testcase() {
	long m, n; std::cin >> m >> n;
	
	std::vector<std::pair<P, K::FT>> players(m);
	for (int i = 0; i < m; ++i) {
		K::FT x, y, r; std::cin >> x >> y >> r;
		players[i] = {P(x, y), r};
	}

	long h; std::cin >> h;
	std::vector<IPoint> points(n);
	for (Index i = 0; i < n; ++i) {
		K::FT x, y; std::cin >> x >> y;
		points[i] = {P(x, y), i};
	}

	Delaunay t;
	t.insert(points.begin(), points.end());
	std::vector<int> winners;

	for (long i = 0; i < m; i++) {
		K::FT min_dist = CGAL::squared_distance(P(0,0), P(0, h + players[i].second));
		P closest = t.nearest_vertex(players[i].first)->point();
		if (CGAL::squared_distance(players[i].first, closest) >= min_dist)
			winners.push_back(i);
	}

	if (winners.size() > 0) {
		for (const int i: winners)
			std::cout << i << " ";
		std::cout << std::endl;
		return;
	}

	int start = 1, end = n + 1;
	while (start < end) {
		int mid = (start + end) / 2;

		Delaunay t;
		t.insert(points.begin(), points.begin() + mid);

		std::vector<int> w;
		for (long i = 0; i < m; i++) {
			K::FT min_dist = CGAL::squared_distance(P(0,0), P(0, h + players[i].second));
			P closest = t.nearest_vertex(players[i].first)->point();
			if (CGAL::squared_distance(players[i].first, closest) >= min_dist)
				w.push_back(i);
		}

		if (w.size() > 0) {
			start = mid + 1;
			winners = w;
		} else
			end = mid;
	}

	for (const int i: winners)
		std::cout << i << " ";
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
