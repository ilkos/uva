//============================================================================
// Name        : uva11159-FactorsAndMultiples.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <stdint.h>
#include <limits>
#include <cassert>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
using namespace std;

const uint32_t INFINITY = numeric_limits<uint32_t>::max() / 2;
template <typename T>
T fromStream(istream& is) {
	T val;
	is >> val;
	return val;
}

typedef uint32_t vertex_t;
struct Edge {
	Edge(vertex_t dst, uint32_t weight) : dst(dst), weight(weight) {}

	bool operator==(const Edge& other) const {
		return dst == other.dst;
	}

	vertex_t dst;
	uint32_t weight;
};

struct AdjList {
	AdjList(size_t sz) :
		list_(vector<vector<Edge> >(sz)) {}

	size_t size() const {
		return list_.size();
	}

	void addEdge(vertex_t src, vertex_t dst, uint32_t weight) {
		assert(src < size() && dst < size());
		list_[src].push_back(Edge(dst, weight));
	}

	const vector<Edge>& edges(vertex_t node) const {
		assert(node < size());
		return list_[node];
	}

	Edge* getEdge(vertex_t from, vertex_t to) {
		for (vector<Edge>::iterator eIt(list_[from].begin()), eItEnd(list_[from].end());
				eIt != eItEnd;
				++eIt) {
			if (eIt->dst == to) {
				return &(*eIt);
			}
		}

		return 0;
	}

private:
	vector<vector<Edge> > list_;
};

uint32_t bfs(const AdjList& graph, const vertex_t src, const vertex_t dst,
		vector<uint32_t>& parent) {
	fill(parent.begin(), parent.end(), numeric_limits<uint32_t>::max());
	queue<pair<vertex_t, uint32_t> > q;
	parent[src] = src;
	q.push(make_pair(src, INFINITY));

	while (!q.empty()) {
		const pair<vertex_t, uint32_t> node = q.front();
		q.pop();

		if (node.first == dst) {
			return node.second;
		}

		const vector<Edge>& edges = graph.edges(node.first);
		for (vector<Edge>::const_iterator eIt(edges.begin()), eItEnd(edges.end()); eIt != eItEnd; ++eIt) {
			if (parent[eIt->dst] != numeric_limits<uint32_t>::max()) { // visited
				continue;
			}

			if (!eIt->weight) {
				continue;
			}
			assert(eIt->weight > 0);

			parent[eIt->dst] = node.first;
			q.push(make_pair(eIt->dst, min(node.second, eIt->weight)));
		}
	}

	return 0;
}

void augmentPath(AdjList& graph, const vertex_t sink,
		const vector<uint32_t>& parent, uint32_t flow) {
	vertex_t curr = sink;

	while (parent[curr] != curr) {
		Edge* e = graph.getEdge(parent[curr], curr);
		assert(e);
		e->weight -= flow;

		e = graph.getEdge(curr, parent[curr]);
		assert(e);
		e->weight += flow;

		curr = parent[curr];
	}
}

// calculate flow on the graph
void edmonds_karp(AdjList& graph, const vertex_t source, const vertex_t sink) {
	vector<uint32_t> parent(graph.size());
	while (true) {
		uint32_t augment = bfs(graph, source, sink, parent);
		if (!augment) {
			break;
		}

		augmentPath(graph, sink, parent, augment);
	}
}

uint32_t findMinCut(const AdjList& graph, const vertex_t src, const vertex_t sink) {
	// dfs from the src, find uncrossable edges
	stack<vertex_t> s;
	vector<bool> isVisited(graph.size(), false);
	s.push(src);
	isVisited[src] = true;

	set<pair<vertex_t, vertex_t> > vedges;
	while (!s.empty()) {
		const vertex_t curr = s.top(); s.pop();
		const vector<Edge>& edges = graph.edges(curr);
		for (vector<Edge>::const_iterator it(edges.begin()), itEnd(edges.end()); it != itEnd; ++it) {
			if (!it->weight) {
				vedges.insert(make_pair(curr < it->dst ? curr : it->dst,
						curr < it->dst ? it->dst : curr));
				continue;
			}

			if (isVisited[it->dst]) {
				continue;
			}
			isVisited[it->dst] = true;
			s.push(it->dst);
		}
	}

	uint32_t r = 0;
	for (set<pair<vertex_t, vertex_t> >::const_iterator it(vedges.begin()), itEnd(vedges.end());
			it != itEnd;
			++it) {
		if (isVisited[it->first] + isVisited[it->second] == 1 &&
				(it->second == src || it->second == sink)) { // check to not process back edges
			++r;
		}
	}

	return r;
}

int main() {
	uint32_t nCases;
	cin >> nCases;

	for (uint32_t current = 1; current <= nCases; ++current) {
		const int n = fromStream<int>(cin);

		vector<int> setA(n);
		for (int i = 0; i < n; ++i) {
			cin >> setA[i];
		}

		const int m = fromStream<int>(cin);
		vector<int> setB(m);
		for(int i = 0; i < m; ++i) {
			cin >> setB[i];
		}

		AdjList graph(n + m + 2);

		for (size_t i = 0; i < setA.size(); ++i) {
			for (size_t j = 0; j < setB.size(); ++j) {
				if ((setA[i] && (setB[j] % setA[i] == 0)) || !setB[j]) {
					graph.addEdge(i, n + j, INFINITY);
					graph.addEdge(n + j, i, 0);
				}
			}
		}

		const vertex_t src = n + m;
		const vertex_t sink = src + 1;
		for (size_t i = 0; i < setA.size(); ++i) {
			graph.addEdge(src, i, 1);
			graph.addEdge(i, src, 0);
		}

		for (size_t i = 0; i < setB.size(); ++i) {
			graph.addEdge(n + i, sink, 1);
			graph.addEdge(sink, n + i, 0);
		}

		edmonds_karp(graph, src, sink);
		uint32_t minCut = findMinCut(graph, src, sink);

		cout << "Case " << current << ": " << minCut << endl;
	}
	return 0;
}
