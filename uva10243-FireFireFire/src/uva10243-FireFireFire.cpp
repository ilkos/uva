//============================================================================
// Name        : uva10243-FireFireFire.cpp
//               Vertex cover in tree
//============================================================================

#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <cassert>
using namespace std;

typedef unsigned int vertex_t;
struct AdjList {
	AdjList(size_t n) : sz(n), links(n, vector<int>()) {}

	void addLink(vertex_t src, vertex_t dst) {
		assert(src < sz && dst < sz);
		links[src].push_back(dst);
	}

	const vector<int>& getLinks(vertex_t vertex) const {
		assert(vertex < sz);
		return links[vertex];
	}

	size_t getSize() const {
		return sz;
	}

private:
	const size_t sz;
	vector<vector<int> > links;
};

int vertex_cover(const AdjList& graph, vertex_t root, bool include, map<pair<vertex_t, bool>, int>& eval) {
	map<pair<vertex_t, bool>, int>::const_iterator it = eval.find(make_pair(root, include));
	if (it != eval.end()) {
		return it->second;
	}

	const vector<int>& links = graph.getLinks(root);
	int s;
	if (!include) { // have to include child
		s = 0;
		for (size_t i = 0; i < links.size(); ++i) {
			s += vertex_cover(graph, links[i], true, eval);
		}
	}
	else { // may or may not include child
		s = 1;
		for (size_t i = 0; i < links.size(); ++i) {
			s += min(vertex_cover(graph, links[i], true, eval), vertex_cover(graph, links[i], false, eval));
		}
	}

	return eval[make_pair(root, include)] = s;
}

template <typename T>
void dfs(const AdjList& graph, vertex_t root, T func) {
	vector<int> isVisited(graph.getSize());
	stack<int> s;
	s.push(root);
	isVisited[root] = true;

	while (!s.empty()) {
		vertex_t curr = s.top();
		s.pop();

		const vector<int>& links = graph.getLinks(curr);
		for (size_t i = 0; i < links.size(); ++i) {
			vertex_t next = links[i];
			if (!isVisited[next]) {
				isVisited[next] = true;
				s.push(next);
				func.visitNode(curr, next);
			}
		}
	}
}

struct CreateSpanningTree {
	CreateSpanningTree(AdjList& tree) : tree(tree) {}
	void visitNode(vertex_t parent, vertex_t node) {
		tree.addLink(parent, node);
	}
private:
	AdjList& tree;
};

int solve(const AdjList& graph) {
	if (graph.getSize() == 1) {
		return 1;
	}

	AdjList sTree(graph.getSize());
	dfs(graph, 0, CreateSpanningTree(sTree));

	map<pair<vertex_t, bool>, int> eval;
	return min(vertex_cover(sTree, 0, true, eval), vertex_cover(sTree, 0, false, eval));
}

int main() {
	int n;
	while (cin >> n) {
		if (!n) break;

		AdjList graph(n);
		int links;
		for (int i = 0; i < n; ++i) {
			cin >> links;
			int dst;
			for (int j = 0; j < links; ++j) {
				cin >> dst;
				graph.addLink(i, dst - 1);
			}
		}

		cout << solve(graph) << endl;
	}
	return 0;
}
