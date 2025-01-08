#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>	graph;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 											P;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>     Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::pair<P,int> IPoint;
typedef Tds::Vertex_handle VH;

void testcase() {
	long n, m, r; std::cin >> n >> m >> r;

	std::vector<IPoint> points;
	points.reserve(n);
	for (int i = 0; i < n; ++i) {
		int x, y; std::cin >> x >> y;
		points.emplace_back(P(x, y), i);
	}

	Delaunay t;
	t.insert(points.begin(), points.end());

	graph g(n);
	for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
		std::set<VH> visited;
		std::queue<VH> q;

		q.push(v);
		visited.insert(v);

		while (!q.empty()) {
			auto v1 = q.front(); q.pop();

			if (v1->info() != v->info())
				boost::add_edge(v->info(), v1->info(), g);

			auto v2 = v1->incident_vertices();
			do {
				if (!t.is_infinite(v2)
					&& visited.find(v2) == visited.end()
					&& CGAL::squared_distance(v->point(), v2->point()) <= r * r
				){
					visited.insert(v2);
					q.push(v2);
				}
			} while (++v2 != v1->incident_vertices());
		}
	}

	bool interferences = !boost::is_bipartite(g);

	std::vector<int> components(n);
	boost::connected_components(g, boost::make_iterator_property_map(components.begin(), boost::get(boost::vertex_index, g))); 

	for (int i = 0; i < m; i++) {
		int x1, y1, x2, y2; std::cin >> x1 >> y1 >> x2 >> y2;
		P a(x1, y1), b(x2, y2);

		auto va = t.nearest_vertex(a), vb = t.nearest_vertex(b);
		long dist_a = (long)CGAL::squared_distance(a, va->point());
		long dist_b = (long)CGAL::squared_distance(b, vb->point());

		if (interferences)
			std::cout << "n";
		else if ((long)CGAL::squared_distance(a, b) <= r * r)
			std::cout << "y";
		else if (dist_a <= r * r && dist_b <= r * r && components[va->info()] == components[vb->info()])
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
