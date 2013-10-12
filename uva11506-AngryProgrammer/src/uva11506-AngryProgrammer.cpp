//============================================================================
// Name        : uva11506-AngryProgrammer.cpp
//               min-cut => max-flow
//============================================================================

#include <iostream>
#include <cassert>
#include <limits>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
using namespace std;

template <typename T, typename U>
struct Tie {
	Tie (T& t, U& u) : t(t), u(u) {}
	void operator= (const pair<T, U>& p) {
		t = p.first;
		u = p.second;
	}
	T& t;
	U& u;
};

template <typename T, typename U>
Tie<T, U> tie(T& t, U& u) {
	return Tie<T, U>(t, u);
}

typedef unsigned int vertex_t;
struct Edge {
	Edge(vertex_t dst, int capacity) : dst(dst), capacity(capacity) {}
	vertex_t dst;
	int capacity;
};

struct EdgePoints {
	EdgePoints(vertex_t dst) : dst(dst) {}
	bool operator() (const Edge& edge) const {
		return edge.dst == dst;
	}
	vertex_t dst;
};
struct AdjList {
	AdjList(size_t n) : sz(n), links(n, vector<Edge>()) {}

	void addLink(vertex_t src, vertex_t dst, int capacity, bool isUndirected = false) {
		assert(src < sz && dst < sz);
		links[src].push_back(Edge(dst, capacity));

		if (isUndirected) {
			links[dst].push_back(Edge(src, capacity));
		}
	}

	const vector<Edge>& getLinks(vertex_t vertex) const {
		assert(vertex < sz);
		return links[vertex];
	}

	vector<Edge>& getLinks(vertex_t vertex) {
		assert(vertex < sz);
		return links[vertex];
	}

	size_t getSize() const {
		return sz;
	}

private:
	const size_t sz;
	vector<vector<Edge> > links;
};

#define NOTVISITED -2
#define ROOT -1
int identifyPath(const vertex_t src, const vertex_t dst, const AdjList& graph, vector<int>& visited) {
	queue<pair<vertex_t, int> > q;
	q.push(make_pair(src, numeric_limits<int>::max()));

	fill (visited.begin(), visited.end(), NOTVISITED);
	visited[src] = ROOT;

	while (!q.empty()) {
		vertex_t currV;
		int pathCapacity;
		tie(currV, pathCapacity) = q.front();
		q.pop();

		if (currV == dst) {
			return pathCapacity;
		}

		const vector<Edge>& cEdges = graph.getLinks(currV);
		for (size_t i = 0; i < cEdges.size(); ++i) {
			if (cEdges[i].capacity > 0 && visited[cEdges[i].dst] == NOTVISITED) {
				q.push(make_pair(cEdges[i].dst, min(cEdges[i].capacity, pathCapacity)));
				visited[cEdges[i].dst] = currV;
			}
		}
	}

	return 0;
}

void augment(const vertex_t src, const vertex_t dst, AdjList& graph, const vector<int>& visited, const int aug) {
	vertex_t curr = dst;

	while (curr != src) {
		assert(visited[curr] >= 0);

		// cout << "augmenting " << visited[curr] << " - " << curr << " by " << aug << endl;
		vector<Edge>& outbound = graph.getLinks(visited[curr]);
		vector<Edge>::iterator it = find_if(outbound.begin(), outbound.end(), EdgePoints(curr));
		assert(it != outbound.end());
		it->capacity -= aug;

		vector<Edge>& inbound = graph.getLinks(curr);
		it = find_if(inbound.begin(), inbound.end(), EdgePoints(visited[curr]));
		assert(it != inbound.end());
		it->capacity += aug;

		curr = visited[curr];
	}
}

int solve(const vertex_t src, const vertex_t dst, AdjList& graph) {
	int totalFlow = 0;

	while (true) {
		vector<int> visited(graph.getSize());
		int augmentBy = identifyPath(src, dst, graph, visited);
		if (!augmentBy) {
			break;
		}
		totalFlow += augmentBy;
		augment(src, dst, graph, visited, augmentBy);
	}

	return totalFlow;
}

inline int in(int machine) {
	return machine * 2;
}
inline int out(int machine) {
	return in(machine) + 1;
}

int main() {
	int machines, wires;
	while (cin >> machines >> wires) {
		if (!machines && !wires) {
			break;
		}
		int id, cost;

		// expand each node N into Nin, Nout
		// index N -> (N + 1) / 2, (N + 1) / 2 + 1
		AdjList graph(machines * 2);
		for (int i = 0; i < machines - 2; ++i) {
			cin >> id >> cost;
			--id;
			graph.addLink(in(id), out(id), cost);
			graph.addLink(out(id), in(id), cost);
		}

		for (int i = 0; i < wires; ++i) {
			int j, k, d;
			cin >> j >> k >> d;
			--j; --k;

			graph.addLink(out(j), in(k), d);
			graph.addLink(in(k), out(j), 0);

			graph.addLink(out(k), in(j), d);
			graph.addLink(in(j), out(k), 0);
		}

		cout << solve(out(0), in(machines - 1), graph) << endl;
	}
	return 0;
}
