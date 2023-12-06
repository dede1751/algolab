#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using P = K::Point_2;
using Vb = CGAL::Triangulation_vertex_base_2<K>;
using Fb = CGAL::Triangulation_face_base_with_info_2<std::pair<bool, K::FT>, K>;
using Tds = CGAL::Triangulation_data_structure_2<Vb, Fb>;
using Triangulation = CGAL::Delaunay_triangulation_2<K, Tds>;
using FH = Triangulation::Face_handle;

void testcase(int n) {
	Triangulation t;
	std::vector<P> points(n);
	std::priority_queue<std::pair<K::FT, FH>> q;

	for (int i = 0; i < n; i++) {
		long x, y; std::cin >> x >> y;
		points[i] = P(x, y);
	}

	t.insert(points.begin(), points.end());

	for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); f++) {
		f->info() = std::make_pair(false, 0);

		for (int i = 0; i < 3; i++)
			if (t.is_infinite(f->neighbor(i))) {
				auto edge_len = t.segment(f, i).squared_length();

				f->info().second = std::max(f->info().second, edge_len);
				q.push(std::make_pair(edge_len, f));
			}
	}

	while (!q.empty()) {
		auto curr = q.top();
		q.pop();

		if (!curr.second->info().first) {
			for (int i = 0; i < 3; i++) {
				auto neighbor = curr.second->neighbor(i);
				auto capacity = std::min(t.segment(curr.second, i).squared_length(), curr.second->info().second);

				if (!t.is_infinite(neighbor) && !neighbor->info().first) {
					neighbor->info().second = std::max(neighbor->info().second, capacity);
					q.push(std::make_pair(neighbor->info().second, neighbor));
				}
			}

			curr.second->info().first = true;
		}
	}

	int m; std::cin >> m;
	for (int i = 0; i < m; i++) {
		long x, y, d; std::cin >> x >> y >> d;
		P p(x, y);

		if (d > std::ceil(CGAL::squared_distance(p, t.nearest_vertex(p)->point())))
			std::cout << "n";
		else {
			FH fh = t.locate(p);
			
			if (t.is_infinite(fh))
				std::cout << "y";
			else
				std::cout << ((std::floor(fh->info().second) >= 4 * d) ? "y" : "n");
		}
	}

	std::cout << std::endl;
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
