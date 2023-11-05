#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using P = K::Point_2;
using Triangulation = CGAL::Delaunay_triangulation_2<K>;
using Face_iterator = Triangulation::Finite_faces_iterator;

void testcase(int n) {
	Triangulation t;

	for (int i = 0; i < n; i++) {
		int x, y; std::cin >> x >> y;
		t.insert(P(x, y));
	}

	int m; std::cin >> m;
	std::cout << std::setprecision(0) << std::fixed;
	for (int i = 0; i < m; i++) {
		int x, y; std::cin >> x >> y;
		P p(x, y);

		auto v = t.nearest_vertex(p);
		std::cout << CGAL::squared_distance(p, v->point()) << std::endl;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int n;
	std::cin >> n;
	while (n) {
		testcase(n);
		std::cin >> n;
	}
}
