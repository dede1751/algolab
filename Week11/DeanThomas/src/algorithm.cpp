#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <tuple>
#include <queue>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel   K;
typedef K::Point_2 											P;
typedef std::size_t                                         Index;
typedef std::pair<bool, K::FT>                              Info;
typedef CGAL::Triangulation_vertex_base_2<K>                   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Info,K>      Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

void testcase() {
	long n, m, d; std::cin >> n >> m >> d;
	std::vector<P> points(n);
	std::priority_queue<std::pair<K::FT, Delaunay::Face_handle>> q;

	for (int i = 0; i < n; ++i) {
		long x, y; std::cin >> x >> y;
		points[i] = P(x, y);
	}

	Delaunay t;
  	t.insert(points.begin(), points.end());

	for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f){
		f->info() = {false, CGAL::squared_distance(t.dual(f), f->vertex(0)->point())};

		for (int i = 0; i < 3; i++)
			if (t.is_infinite(f->neighbor(i)))
				f->info().second = CGAL::max(f->info().second, t.segment(f, i).squared_length());

		q.push({f->info().second, f});
	}

	while (!q.empty()) {
		auto curr = q.top(); q.pop();
		auto f1 = curr.second;

		if (!f1->info().first) {
			for (int i = 0; i < 3; i++) {
				auto f2 = f1->neighbor(i);

				if (!t.is_infinite(f2) && !f2->info().first) {
					auto d = CGAL::min(f1->info().second, t.segment(f1, i).squared_length());

					if (d > f2->info().second) {
						f2->info().second = d;
						q.push({f2->info().second, f2});
					}
				}
			}

			f1->info().first = true;
		}
	}

	for (int i = 0; i < m; i++) {
		long x, y, s; std::cin >> x >> y >> s;
		K::FT r = CGAL::squared_distance(P(0, 0), P(0, d + s));
		P p(x, y);

		if (r > CGAL::squared_distance(p, t.nearest_vertex(p)->point())) {
			std::cout << "n";
			continue;
		}

		auto f = t.locate(p);
		if (t.is_infinite(f) || 4*r <= f->info().second)
			std::cout << "y";
		else
			std::cout << "n";
	}
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
