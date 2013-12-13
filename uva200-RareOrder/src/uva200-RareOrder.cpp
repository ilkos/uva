//============================================================================
// Name        : uva200-RareOrder.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <stdint.h>
#include <algorithm>
using namespace std;

typedef uint32_t vertex_t;
struct AdjList {
	AdjList(size_t sz) :
		lists_(vector<vector<vertex_t> >(sz, vector<vertex_t>())),
		indegree_(vector<uint32_t>(sz)),
		exists_(vector<bool>(sz, false))
	{}

	size_t size() const {
		return lists_.size();
	}

	void add(vertex_t src, vertex_t dst) {
		lists_[src].push_back(dst);
		++indegree_[dst];

		exists_[src] = true;
		exists_[dst] = true;
	}

	const vector<vertex_t>& edges(vertex_t i) const {
		return lists_[i];
	}

	uint32_t indegree(vertex_t i) const {
		return indegree_[i];
	}

	bool exists(vertex_t i) const {
		return exists_[i];
	}

private:
	vector<vector<vertex_t> > lists_;
	vector<uint32_t> indegree_;
	vector<bool> exists_;
};

void dfs(const AdjList& graph, const vertex_t src, vector<bool>& isVisited, vector<vertex_t>& toposort) {
	if (isVisited[src]) {
		return;
	}
	isVisited[src] = true;

	for (size_t i = 0; i < graph.edges(src).size(); ++i) {
		dfs(graph, graph.edges(src)[i], isVisited, toposort);
	}

	toposort.push_back(src);
}

void identify(AdjList& graph, const string& lo, const string& hi) {
	size_t curr = 0;
	while (curr < min(lo.size(), hi.size()) && lo[curr] == hi[curr]) ++curr;
	if (curr == min(lo.size(), hi.size())) return;
	graph.add(lo[curr] - 'A', hi[curr] - 'A');
}

string process(const vector<string>& strings) {
	AdjList graph(26);

	for (size_t i = 0; i < strings.size() - 1; ++i) {
		identify(graph, strings[i], strings[i + 1]);
	}

	vertex_t root = 0;
	for (size_t i = 0; i < graph.size(); ++i) {
		if (graph.exists(i) && !graph.indegree(i)) {
			root = i;
			break;
		}
	}

	vector<vertex_t> toposort;
	vector<bool> isVisited(graph.size());
	dfs(graph, root, isVisited, toposort);
	reverse(toposort.begin(), toposort.end());

	string res;
	for (size_t i = 0; i < toposort.size(); ++i) {
		res.push_back('A' + toposort[i]);
	}
	return res;
}

int main() {
	vector<string> strings;
	string line;
	while (getline(cin, line)) {
		if (line == "#") {
			break;
		}
		strings.push_back(line);
	}

	cout << process(strings) << endl;
	return 0;
}
