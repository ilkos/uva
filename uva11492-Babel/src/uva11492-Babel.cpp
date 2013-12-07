//============================================================================
// Name        : uva11492-Babel.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <stdint.h>
#include <cassert>
#include <map>
#include <queue>
#include <limits>
using namespace std;

typedef uint32_t vertex_t;

struct Edge {
	Edge() {}
	Edge(vertex_t src, vertex_t dst, uint32_t weight, char initial) :
		src_(src), dst_(dst), weight_(weight), initial_(initial) {
	}

	vertex_t getPoint() const {
		return src_;
	}

	vertex_t getOtherPoint(vertex_t point) const {
		return point == src_ ? dst_ : src_;
	}

	vertex_t src_;
	vertex_t dst_;
	uint32_t weight_;
	char initial_;
};

struct AdjList {
	AdjList(size_t len = 0) :
		list_(vector<vector<Edge> >(len, vector<Edge>())) {
	}

	void addEdge(const Edge& e, bool isDirected = false) {
		size_t ms = max(e.src_, e.dst_);
		if (ms >= size()) {
			list_.resize(ms * 2);
		}

		list_[e.src_].push_back(e);
		if (!isDirected) {
			list_[e.dst_].push_back(e);
		}
	}

	const vector<Edge>& getEdges(const vertex_t v) const {
		assert(v < size());
		return list_[v];
	}

	size_t size() const {
		return list_.size();
	}

private:
	vector<vector<Edge> > list_;
};

struct Resolver {
	int add(const string& s) {
		map<string, int>::const_iterator it = strToInt_.find(s);
		if (it != strToInt_.end()) {
			return it->second;
		}

		strs_.push_back(s);
		return strToInt_[s] = strs_.size() - 1;
	}

	const string& get(int n) const {
		assert(n >= 0 && n < (int)strs_.size());
		return strs_[n];
	}

private:
	map<string, int> strToInt_;
	vector<string> strs_;
};

struct Path {
	Path(vertex_t v, uint32_t cost, char pi) :
		v(v), totalcost(cost), prevInitial(pi) {}

	vertex_t v;
	uint32_t totalcost;
	char prevInitial;

	bool operator> (const Path& other) const {
		return totalcost > other.totalcost;
	}
};

struct Costs {
	Costs(size_t len) :
		costs_(vector<vector<uint32_t> >(len, vector<uint32_t>(32, numeric_limits<uint32_t>::max()))) {
	}

	uint32_t& operator() (vertex_t v, char c) {
		assert(v < costs_.size());
		assert(c - 'a' < 32);

		return costs_[v][c - 'a'];
	}

private:
	vector<vector<uint32_t> > costs_;
};

int findShortest(vertex_t src, vertex_t dst, const AdjList& graph) {
	priority_queue<Path, vector<Path>, greater<Path> > pq;

	// map [vertex_t, char] to minimal cost
	Costs costs(graph.size());
	pq.push(Path(src, 0, 'z' + 1));

	while (!pq.empty()) {
		Path curr = pq.top();
		pq.pop();

		if (curr.v == dst) {
			return curr.totalcost;
		}

		costs(curr.v, curr.prevInitial) = curr.totalcost;

		const vector<Edge>& edges = graph.getEdges(curr.v);
		for (vector<Edge>::const_iterator it(edges.begin()), itEnd(edges.end()); it != itEnd; ++it) {
			if (it->initial_ == curr.prevInitial) {
				continue;
			}

			if (curr.totalcost + it->weight_ < costs(it->getOtherPoint(curr.v), it->initial_)) {
				costs(it->getOtherPoint(curr.v), it->initial_) = curr.totalcost + it->weight_;
				pq.push(Path(it->getOtherPoint(curr.v), curr.totalcost + it->weight_, it->initial_));
			}
		}
	}

	return -1;
}

int main() {
	int m;

	while (cin >> m) {
		if (!m) break;

		Resolver resolver;

		string src, dst;
		cin >> src >> dst;

		resolver.add(src);
		resolver.add(dst);

		AdjList graph;

		for (int i = 0; i < m; ++i) {
			string l1, l2, word;
			cin >> l1 >> l2 >> word;

			graph.addEdge(Edge(resolver.add(l1), resolver.add(l2), word.size(), word[0]));
		}

		int s = findShortest(resolver.add(src), resolver.add(dst), graph);
		if (s < 0) {
			cout << "impossivel" << endl;
		}
		else {
			cout << s << endl;
		}
	}
	return 0;
}
