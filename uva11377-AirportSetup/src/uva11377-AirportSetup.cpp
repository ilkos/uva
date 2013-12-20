//============================================================================
// Name        : uva11377-AirportSetup.cpp
//============================================================================

#include <iostream>
#include <stdint.h>
#include <vector>
#include <queue>
#include <limits>
#include <cassert>
using namespace std;

typedef uint32_t vertex_t;
struct Edge {
	Edge (vertex_t dst, uint32_t weight) :
		dst(dst), weight(weight) {}

	vertex_t dst;
	uint32_t weight;
};

struct AdjList {
	AdjList(size_t sz) :
		list_(vector<vector<Edge> >(sz, vector<Edge>())) { }

	size_t size() const {
		return list_.size();
	}

	void add(vertex_t src, vertex_t dst, uint32_t weight) {
		assert(src < size() && dst < size());
		list_[src].push_back(Edge(dst, weight));
	}

	const vector<Edge>& edges(vertex_t node) const {
		assert(node < size());
		return list_[node];
	}

private:
	vector<vector<Edge> > list_;
};

struct Path {
	Path(vertex_t child, int total) :
		node(child), total(total) {}

	bool operator< (const Path& other) const {
		return total > other.total;
	}
	vertex_t node;
	uint32_t total;
};

int dijkstra(const AdjList& graph, const vertex_t src, const vertex_t dst) {
	vector<uint32_t> minCostTo(graph.size(), numeric_limits<uint32_t>::max());

	priority_queue<Path> pq;
	pq.push(Path(src, 0));
	minCostTo[src] = 0;

	while (!pq.empty()) {
		Path p = pq.top();
		pq.pop();

		if (p.total > minCostTo[p.node]) {
			continue;
		}

		if (p.node == dst) {
			return p.total;
		}

		const vector<Edge>& edges = graph.edges(p.node);
		for (vector<Edge>::const_iterator eIt(edges.begin()), eItEnd(edges.end()); eIt != eItEnd; ++eIt) {
			const vertex_t tgt = eIt->dst;
			if (p.total + eIt->weight < minCostTo[tgt]) {
				minCostTo[tgt] = p.total + eIt->weight;
				pq.push(Path(tgt, minCostTo[tgt]));
			}
		}
	}

	return -1;
}

int main() {
	int nCases;
	cin >> nCases;

	for (int current = 1; current <= nCases; ++current) {
		int n, m, k;
		cin >> n >> m >> k;

		vector<bool> isAirport(n + 1, false);
		for (int i = 0; i < k; ++i) {
			uint32_t t;
			cin >> t;
			isAirport[t] = true;
		}

		AdjList graph(n + 1);
		for (int i = 0; i < m; ++i) {
			uint32_t src, dst;
			cin >> src >> dst;

			graph.add(src, dst, isAirport[dst] ? 0 : 1);
			graph.add(dst, src, isAirport[src] ? 0 : 1);
		}

		cout << "Case " << current << ':' << endl;
		int q;
		cin >> q;
		for (int i = 0; i < q; ++i) {
			uint32_t src, dst;
			cin >> src >> dst;

			if (src == dst) {
				cout << '0' << endl;
			}
			else {
				int r = dijkstra(graph, src, dst);
				if (r >= 0) {
					r += isAirport[src] ? 0 : 1;
				}
				cout << r << endl;
			}
		}

		cout << endl;
	}
	return 0;
}
