//============================================================================
// Name        : uva10048-AudioPhobia.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <stack>
#include <cassert>
using namespace std;

struct Edge {
	Edge(int dst, int weight) :
		_dst(dst), _weight(weight) {}

	int dst() const {
		return _dst;
	}

	int weight() const {
		return _weight;
	}

private:
	int _dst;
	int _weight;
};

struct AdjList {
	AdjList(int n) :
		_size(n),
		_edges(n, vector<Edge>()) {}

	const vector<Edge>& getEdges(int node) const {
		assert(node >= 0 && node < _size);
		return _edges[node];
	}

	void addEdge(int src, int dst, int weight, bool isDirected = false) {
		assert(src >= 0 && src < _size);
		assert(dst >= 0 && dst < _size);
		_edges[src].push_back(Edge(dst, weight));

		if (!isDirected) {
			_edges[dst].push_back(Edge(src, weight));
		}
	}

	int getSize() const {
		return _size;
	}

private:
	int _size;
	vector<vector<Edge> > _edges;
};

int solve(const AdjList& g, int src, int dst) {
	vector<int> visited(g.getSize(), -1);

	stack<int> s;
	s.push(src);
	visited[src] = 0;

	while (!s.empty()) {
		int curr = s.top();
		s.pop();

		const vector<Edge>& edges = g.getEdges(curr);

		for (vector<Edge>::const_iterator eIt = edges.begin(), eItEnd = edges.end();
				eIt != eItEnd;
				++eIt)
		{
			int next = eIt->dst();
			if (visited[next] < 0 ||
					max(visited[curr],  eIt->weight()) < visited[next]) {
				visited[next] = max(visited[curr], eIt->weight());
				s.push(next);
			}
		}
	}

	return visited[dst];
}

int main() {
	int crossings, streets, queries;

	int mCase = 0;
	while (cin >> crossings >> streets >> queries) {
		if (!crossings && !streets && !queries) {
			break;
		}

		AdjList graph(crossings);
		for (int i = 0; i < streets; ++i) {
			int c1, c2, d;
			cin >> c1 >> c2 >> d;
			--c1; --c2;
			graph.addEdge(c1, c2, d);
		}

		if (mCase) cout << endl;

		cout << "Case #" << ++mCase << endl;
		for (int i = 0; i < queries; ++i) {
			int c1, c2;
			cin >> c1 >> c2;
			--c1; --c2;

			int s = solve(graph, c1, c2);
			if (s >= 0) {
				cout << s << endl;
			}
			else {
				cout << "no path" << endl;
			}
		}
	}
	return 0;
}
