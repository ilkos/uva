//============================================================================
// Name        : uva10600-ACMBlackout.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <stdint.h>
#include <algorithm>
#include <set>
#include <limits>
using namespace std;

typedef uint32_t vertex_t;
struct Edge {
	Edge(vertex_t src, vertex_t dst, uint32_t weight) :
		src(src), dst(dst), weight(weight) {}

	vertex_t getOther(vertex_t one) const {
		return one == src ? dst : src;
	}

	bool operator< (const Edge& other) const {
		if (src != other.src) {
			return src < other.src;
		}

		if (dst != other.dst) {
			return dst < other.dst;
		}

		return weight < other.weight;
	}

	bool operator== (const Edge& other) const {
		return src == other.src && dst == other.dst && weight == other.weight;
	}

	vertex_t src;
	vertex_t dst;
	uint32_t weight;
};

struct AdjList {
	AdjList(size_t n) :
		lists_(vector<vector<Edge> >(n, vector<Edge>())) {
	}

	void add(vertex_t src, vertex_t dst, uint32_t weight) {
		lists_[src].push_back(Edge(src, dst, weight));
	}

	const vector<Edge>& getEdges(vertex_t node) {
		return lists_[node];
	}

	void getAllEdges(vector<Edge>& edges) const {
		for (vector<vector<Edge> >::const_iterator it(lists_.begin()), itEnd(lists_.end());
				it != itEnd;
				++it) {
			edges.insert(edges.end(), it->begin(), it->end());
		}
	}
	size_t size() const {
		return lists_.size();
	}

private:
	vector<vector<Edge> > lists_;
};

struct UnionFind {
	UnionFind(size_t n) :
		parent_(vector<uint32_t>(n)),
		rank_(vector<uint32_t>(n)) {
		reset();
	}

	void link(uint32_t lhs, uint32_t rhs) {
		uint32_t lhsRoot = root(lhs);
		uint32_t rhsRoot = root(rhs);

		if (lhsRoot == rhsRoot) return;

		if (rank_[lhsRoot] < rank_[rhsRoot]) {
			do_link(lhsRoot, rhsRoot);
		}
		else {
			do_link(rhsRoot, lhsRoot);
		}
	}

	bool isLinked(uint32_t lhs, uint32_t rhs) const {
		return root(lhs) == root(rhs);
	}

	uint32_t getNumForests() const {
		set<uint32_t> ids;
		for (size_t i = 0; i < parent_.size(); ++i) {
			ids.insert(root(i));
		}
		return ids.size();
	}

	void reset() {
		for (size_t i = 0; i < parent_.size(); ++i) {
			parent_[i] = i;
			rank_[i] = 0;
		}
	}
private:
	uint32_t root(uint32_t node) const {
		while (parent_[node] != node) {
			node = parent_[node];
		}
		return node;
	}

	void do_link(uint32_t parentNode, uint32_t childNode) {
		parent_[childNode] = parentNode;
		++rank_[parentNode];
	}

	vector<uint32_t> parent_;
	vector<uint32_t> rank_;
};

bool compWeight(const Edge& lhs, const Edge& rhs) {
	return lhs.weight < rhs.weight;
}

void spanningTree(const AdjList& graph) {
	UnionFind uf(graph.size());

	vector<Edge> edges;
	graph.getAllEdges(edges);

	sort(edges.begin(), edges.end(), ::compWeight);

	set<Edge> processed;
	uint32_t minCost = 0;
	for (vector<Edge>::const_iterator it(edges.begin()), itEnd(edges.end()); it != itEnd; ++it) {
		if (!uf.isLinked(it->src, it->dst)) {
			minCost += it->weight;
			processed.insert(*it);
			uf.link(it->src, it->dst);
		}
	}

	uint32_t secondMinCost = numeric_limits<uint32_t>::max();
	for (set<Edge>::const_iterator skipEdgeIt(processed.begin()), skipEdgeItEnd(processed.end());
			skipEdgeIt != skipEdgeItEnd; ++skipEdgeIt) {
		uf.reset();
		uint32_t sr = 0;
		for (vector<Edge>::const_iterator it(edges.begin()), itEnd(edges.end()); it != itEnd; ++it) {
			if (*skipEdgeIt == *it) continue;

			if (!uf.isLinked(it->src, it->dst)) {
				sr += it->weight;
				uf.link(it->src, it->dst);
			}
		}

		// ensure we're still fully connected
		if (uf.getNumForests() == 1) {
			secondMinCost = min(secondMinCost, sr);
		}
	}
	cout << minCost << ' ' << secondMinCost << endl;
}

int main() {
	int nCases;
	cin >> nCases;

	while (nCases--) {
		int n, m;
		cin >> n >> m;

		AdjList graph(n);
		for (int i = 0; i < m; ++i) {
			int a, b, w;
			cin >> a >> b >> w;
			--a; --b;

			graph.add(a, b, w);
		}

		spanningTree(graph);
	}
	return 0;
}
