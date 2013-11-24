//============================================================================
// Name        : uva341-NonStopTravel.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <queue>
#include <cassert>
#include <stdint.h>
using namespace std;

struct Edge {
	Edge(int dst, uint32_t weight) : dst(dst), weight(weight) {}
	int dst;
	uint32_t weight;
};

struct GreaterWeight {
	bool operator() (const Edge& lhs, const Edge& rhs) const {
		return lhs.weight > rhs.weight;
	}
};

struct AdjList {
	AdjList(int nodes) :
		nodes_(nodes),
		graph_(vector<vector<Edge> >(nodes_)) {}

	void addEdge(int src, int dst, int weight) {
		assert(src >= 0 && src < nodes_);
		assert(dst >= 0 && dst < nodes_);
		graph_[src].push_back(Edge(dst, weight));
	}

	const vector<Edge>& edges(const int src) const {
		assert(src >= 0 && src < nodes_);
		return graph_[src];
	}

	const int size() const {
		return nodes_;
	}

private:
	int nodes_;
	vector<vector<Edge> > graph_;
};

int dijkstra(const AdjList& graph, int src, int dst, vector<int>& out) {
	priority_queue<Edge, vector<Edge>, GreaterWeight> pq;
	vector<int> parent(graph.size(), -1);
	vector<uint32_t> shortestPath(graph.size(), numeric_limits<uint32_t>::max() / 2);

	parent[src] = src;
	shortestPath[src] = 0;
	pq.push(Edge(src, 0));

	while (!pq.empty()) {
		const Edge curr = pq.top();
		pq.pop();

		if (curr.weight > shortestPath[curr.dst]) continue;

		const vector<Edge>& outgoing = graph.edges(curr.dst);
		for (int i = 0; i < (int) outgoing.size(); ++i) {

			if (shortestPath[curr.dst] + outgoing[i].weight < shortestPath[outgoing[i].dst]) {
				shortestPath[outgoing[i].dst] = shortestPath[curr.dst] + outgoing[i].weight;
				parent[outgoing[i].dst] = curr.dst;

				pq.push(Edge(outgoing[i].dst, shortestPath[outgoing[i].dst]));
			}

		}
	}

	int curr = dst;
	while (parent[curr] != curr) {
		out.push_back(curr);
		curr = parent[curr];
	}
	out.push_back(src);
	reverse(out.begin(), out.end());
	return shortestPath[dst];
}

int main() {
	int ni;
	int testCase = 1;
	while (cin >> ni) {
		if (!ni) break;

		AdjList graph(ni);
		for (int i = 0; i < ni; ++i) {
			int nstreets;
			cin >> nstreets;

			for (int j = 0; j < nstreets; ++j) {
				int dst, weight;
				cin >> dst >> weight;
				--dst;

				graph.addEdge(i, dst, weight);
			}
		}

		int src, dst;
		cin >> src >> dst;
		--src; --dst;

		vector<int> path;
		int mindelay = dijkstra(graph, src, dst, path);
		cout << "Case " << testCase << ": Path = ";
		for (int i = 0; i < (int)path.size(); ++i) {
			if (i) cout << ' ';
			cout << path[i] + 1;
		}
		cout << "; " << mindelay << " second delay" << endl;
		++testCase;
	}
	return 0;
}
