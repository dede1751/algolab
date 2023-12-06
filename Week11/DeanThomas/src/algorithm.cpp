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
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel   K;
typedef K::Point_2 											P;
typedef std::size_t                                         Index;
typedef std::pair<bool, K::FT>                              Info;
typedef CGAL::Triangulation_vertex_base_2<K>                   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Info,K>      Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

void testcase() {
	long n, m, d; std::cin >> n >> m >> d;

	std::vector<P> points;
	points.reserve(n);
	for (int i = 0; i < n; ++i) {
		long x, y; std::cin >> x >> y;
		points.emplace_back(P(x, y));
	}

	Delaunay t;
  	t.insert(points.begin(), points.end());

	std::priority_queue<std::pair<K::FT, Delaunay::Face_handle>> q;
	for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); f++) {
		f->info() = {false, CGAL::squared_distance(f->vertex(0)->point(), t.dual(f))};

		for (int i = 0; i < 3; i++) {
			if (!t.is_infinite(f->neighbor(i)))
				continue;

			f->info().second = std::max(f->info().second, t.segment(f, i).squared_length());
		}
		q.push({f->info().second, f});
	}

	while (!q.empty()) {
		auto curr = q.top(); q.pop();
		auto face = curr.second;

		if (!face->info().first) {
			for (int i = 0; i < 3; i++) {
				auto neighbor = face->neighbor(i);
				if (t.is_infinite(neighbor))
					continue;

				if (!neighbor->info().first) {
					auto capacity = std::min(t.segment(face, i).squared_length(), face->info().second);
					neighbor->info().second = std::max(neighbor->info().second, capacity);
					q.push(std::make_pair(neighbor->info().second, neighbor));
				}
			}

			face->info().first = true;
		}
	}

	for (int i = 0; i < m; i++) {
		long x, y, s; std::cin >> x >> y >> s;
		P p(x, y);
		K::FT r = CGAL::squared_distance(P(0, 0), P(s + d, 0));

		if (CGAL::squared_distance(p, t.nearest_vertex(p)->point()) < r) {
			std::cout << "n";
			continue;
		}

		auto f = t.locate(p);
		if (t.is_infinite(f))
			std::cout << "y";
		else if (f->info().second < 4 * r)
			std::cout << "n";
		else
			std::cout << "y";
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
