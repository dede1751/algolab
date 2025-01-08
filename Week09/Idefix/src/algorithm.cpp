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
using Bones = std::vector<std::pair<Index, long>>;

int max_bones(EdgeV& edges, Bones& bones, long n, long s) {
	boost::disjoint_sets_with_storage<> uf(n);
	Index n_components = n;

	for (EdgeV::const_iterator e = edges.begin(); e != edges.end() && n_components > 1; ++e) {
		if (std::get<2>(*e) > s)
			break;
		
		Index c1 = uf.find_set(std::get<0>(*e));
		Index c2 = uf.find_set(std::get<1>(*e));
		if (c1 != c2) {
			uf.link(c1, c2);
			--n_components;
		}
	}

	std::vector<int> covered_bones(n, 0);
	for (const auto [i, dist]: bones)
		if (4 * dist <= s)
			covered_bones[uf.find_set(i)]++;
	
	return *std::max_element(covered_bones.begin(), covered_bones.end());
}

void testcase() {
	long n, m, s, k; std::cin >> n >> m >> s >> k;
	std::vector<IPoint> trees(n);

	for (int i = 0; i < n; i++) {
		int x, y; std::cin >> x >> y;
		trees[i] = std::make_pair(P(x, y), i);
	}

	Delaunay tr;
	tr.insert(trees.begin(), trees.end());

	Bones bones(m);
	for (int i = 0; i < m; i++) {
		int x, y; std::cin >> x >> y;
		P p(x, y);
		auto nearest = tr.nearest_vertex(p);
		long dist = CGAL::squared_distance(p, nearest->point());

		bones[i] = std::make_pair(nearest->info(), dist);
	}

	EdgeV edges;
	edges.reserve(3*n);
	for (auto e = tr.finite_edges_begin(); e != tr.finite_edges_end(); ++e) {
		Index i1 = e->first->vertex((e->second+1)%3)->info();
		Index i2 = e->first->vertex((e->second+2)%3)->info();

		if (i1 > i2) std::swap(i1, i2);
		edges.emplace_back(i1, i2, tr.segment(e).squared_length());
	}
	std::sort(edges.begin(), edges.end(),
		[](const Edge& e1, const Edge& e2) -> bool {
			return std::get<2>(e1) < std::get<2>(e2);
		});
	
	long start = 0, end = 1L << 51;
	while (start < end) {
		long mid = start + (end - start) / 2;

		if (max_bones(edges, bones, n, mid) >= k)
			end = mid;
		else
			start = mid + 1;
	}

	std::cout << max_bones(edges, bones, n, s) << " " << end << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
