//============================================================================
// Name        : uva10999-TouristGuide.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <limits>
using namespace std;

struct Link {
	Link(int dst, int weight) : dst(dst), weight(weight) {}
	int dst;
	int weight;
};

struct AdjList {
	AdjList(int sz) : _sz(sz),
			_links(sz, vector<Link>()) {}

	void addLink(int src, int dst, int weight) {
		assert(src >= 0 && src < _sz);
		assert(dst >= 0 && dst < _sz);
		_links[src].push_back(Link(dst, weight));
		_links[dst].push_back(Link(src, weight));
	}

	const vector<Link>& getLinks(int node) const {
		assert(node >= 0 && node < _sz);
		return _links[node];
	}

	int getSz() const {
		return _sz;
	}

private:
	int _sz;
	vector<vector<Link> > _links;
};

int solve(const int src, const int dst, const int num, const AdjList& graph) {
	int visited[graph.getSz()];
	fill (visited, visited + graph.getSz(), 0);

	queue<int> q;
	q.push(src);
	visited[src] = numeric_limits<int>::max();

	// aim to maximise visited[dst]
	while (!q.empty()) {
		const int currNode = q.front();
		const int currScore = visited[currNode];
		q.pop();

		for (size_t i = 0; i < graph.getLinks(currNode).size(); ++i) {
			const int tgtNode = graph.getLinks(currNode)[i].dst;
			const int tgtScore = min(currScore, graph.getLinks(currNode)[i].weight);

			if (visited[tgtNode] < tgtScore) {
				visited[tgtNode] = tgtScore;
				q.push(tgtNode);
			}
		}
	}

	double f = num / (double)(visited[dst] - 1);
	return f == (int) f ? f : f + 1;
}

int main() {
	int n, r;
	int scenario = 1;
	while (cin >> n >> r) {
		if (!n && !r) break;

		AdjList graph(n);
		for (int i = 0; i < r; ++i) {
			int c1, c2, p;
			cin >> c1 >> c2 >> p;
			--c1; --c2;

			graph.addLink(c1, c2, p);
		}
		int src, dst, num;
		cin >> src >> dst >> num;
		--src; --dst;

		cout << "Scenario #" << scenario++ << endl <<
				"Minimum Number of Trips = " << solve(src, dst, num, graph) << endl << endl;
	}
	return 0;
}
