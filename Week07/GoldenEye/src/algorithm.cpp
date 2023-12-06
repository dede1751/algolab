#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <tuple>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using P = K::Point_2;

using Index = std::size_t;
using Vb = CGAL::Triangulation_vertex_base_with_info_2<Index,K>;
using Fb = CGAL::Triangulation_face_base_2<K>;
using Tds = CGAL::Triangulation_data_structure_2<Vb,Fb>;
using Delaunay = CGAL::Delaunay_triangulation_2<K,Tds>;

using Edge = std::tuple<Index,Index,K::FT>;
using EdgeV = std::vector<Edge>;
using IPoint = std::pair<P,Index>;
using Mission = std::pair<P,P>;
using MissionV = std::vector<Mission>;

long min_power(long n, EdgeV& edges, Delaunay& tr, MissionV& missions) {
	boost::disjoint_sets_with_storage<> uf(n);
	Index n_components = n;
	long min_p = 0;

	for (const auto& [s, t]: missions) {
		auto v = tr.nearest_vertex(s), w = tr.nearest_vertex(t);
		long p1 = 4*CGAL::squared_distance(s, v->point()), p2 = 4*CGAL::squared_distance(t, w->point());
		min_p = std::max(min_p, std::max(p1, p2));
	}

	EdgeV::const_iterator e = edges.begin();
	for (const auto& [s, t]: missions) {
		auto v = tr.nearest_vertex(s)->info(), w = tr.nearest_vertex(t)->info();

		if (uf.find_set(v) != uf.find_set(w)) {
			for (; e != edges.end() && n_components > 1; ++e) {
				Index c1 = uf.find_set(std::get<0>(*e)), c2 = uf.find_set(std::get<1>(*e));
				
				if (c1 != c2) {
					uf.link(c1, c2);
					n_components--;
					min_p = std::max(min_p, (long)std::ceil(std::get<2>(*e)));

					if (uf.find_set(v) == uf.find_set(w))
						break;
				}
			}
		}
	}

	return min_p;
}

void testcase() {
	long n, m, p; std::cin >> n >> m >> p;
	std::vector<IPoint> points(n);
	MissionV missions(m);
	MissionV possible;

	for (Index i = 0; i < (Index)n; i++) {
		long x, y; std::cin >> x >> y;
		points[i] = std::make_pair(P(x, y), i);
	}

	for (Index i = 0; i < (Index)m; i++) {
		long x0, y0, x1, y1; std::cin >> x0 >> y0 >> x1 >> y1;
		missions[i] = std::make_pair(P(x0, y0), P(x1, y1));
	}

	Delaunay tr;
	tr.insert(points.begin(), points.end());

	EdgeV edges;
	edges.reserve(3*n);
	for (auto e = tr.finite_edges_begin(); e != tr.finite_edges_end(); ++e) {
		Index i1 = e->first->vertex((e->second + 1) % 3)->info();
		Index i2 = e->first->vertex((e->second + 2) % 3)->info();

		if (i1 > i2) std::swap(i1, i2);
		edges.emplace_back(i1, i2, tr.segment(e).squared_length());
	}

	std::sort(edges.begin(), edges.end(),
		[](const Edge& e1, const Edge& e2) -> bool {
			return std::get<2>(e1) < std::get<2>(e2);
		});

	boost::disjoint_sets_with_storage<> uf(n);
	Index n_components = n;

	for (EdgeV::const_iterator e = edges.begin(); e != edges.end() && n_components > 1; ++e) {
		if (std::get<2>(*e) > p)
			break;

		Index c1 = uf.find_set(std::get<0>(*e)), c2 = uf.find_set(std::get<1>(*e));
		if (c1 != c2) {
			uf.link(c1, c2);
			n_components--;
		}
	}
	
	for (const auto& [s, t]: missions) {
		auto v = tr.nearest_vertex(s), w = tr.nearest_vertex(t);
		long p1 = 4*CGAL::squared_distance(s, v->point()), p2 = 4*CGAL::squared_distance(t, w->point());

		if (p1 <= p && p2 <= p) {
			if (uf.find_set(v->info()) == uf.find_set(w->info())) {
				possible.push_back(std::make_pair(s, t));
				std::cout << "y";
			} else
				std::cout << "n";
		}
		else
			std::cout << "n";
	}

	std::cout << std::endl << min_power(n, edges, tr, missions);
	std::cout << std::endl << min_power(n, edges, tr, possible) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
