//============================================================================
// Name        : uva11695-FlightPlanning.cpp
//               A connected graph with n nodes and n - 1 edges is a *tree*
//============================================================================

#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <queue>
#include <limits>
using namespace std;

struct AdjList {
	AdjList(int n) : mLinks(vector<set<int> >(n)) {}

	void addLink(const int src, const int dst) {
		mLinks[src].insert(dst);
		mLinks[dst].insert(src);
	}

	void removeLink(const int src, const int dst) {
		mLinks[src].erase(dst);
		mLinks[dst].erase(src);
	}

	int getSize() const {
		return mLinks.size();
	}

	struct edge_iterator {
		friend class AdjList;

		int src() const {
			return srcIt - adjList.mLinks.begin();
		}

		int dst() const {
			return *dstIt;
		}

		void operator++() {
			getNextValid();
		}

		bool isValid() const {
			return srcIt != adjList.mLinks.end() && dstIt != srcIt->end();
		}

	private:
		edge_iterator(const AdjList& adjList, vector<set<int> >::const_iterator srcIt) :
			adjList(adjList), srcIt(srcIt), dstIt(srcIt->begin()) {
			if (!isValid()) {
				getNextValid();
			}
		}

		void getNextValid() {
			while (srcIt != adjList.mLinks.end()) {
				if (dstIt != srcIt->end()) {
					++dstIt;
				}
				if (dstIt == srcIt->end()) {
					++srcIt;
					dstIt = srcIt->begin();
				}
				if (isValid()) return;
			}
		}

		const AdjList& adjList;
		vector<set<int> >::const_iterator srcIt;
		set<int>::const_iterator dstIt;
	};

	edge_iterator begin() const {
		return edge_iterator(*this, mLinks.begin());
	}

	vector<set<int> > mLinks;
};

template <typename T>
void dfs(const AdjList& graph, T visitor, const int src) {
	vector<int> parent(graph.getSize(), -2);
	stack<int> s;

	parent[src] = -1;
	s.push(src);

	while (!s.empty()) {
		int current = s.top();
		s.pop();

		visitor.visitNode(current);

		for (set<int>::const_iterator it = graph.mLinks[current].begin(), itEnd = graph.mLinks[current].end();
				it != itEnd;
				++it) {
			visitor.visitEdge(current, *it);

			if (parent[*it] != -2) { // visited
				continue;
			}

			parent[*it] = current;
			s.push(*it);
		}
	}
}

struct CreateGraph {
	CreateGraph(AdjList& newGraph) : newGraph(newGraph) {}

	void visitNode(int node) {}
	void visitEdge(int src, int dst) {
		newGraph.addLink(src, dst); // bidirectional
	}

private:
	AdjList& newGraph;
};

struct GetLeafNodes {
	GetLeafNodes(const AdjList& graph, queue<int>& leaves) : graph(graph), leaves(leaves) {}

	void visitNode(int node) {
		if (graph.mLinks[node].size() <= 1) {
			leaves.push(node);
		}
	}

	void visitEdge(int src, int dst) {}

private:
	const AdjList& graph;
	queue<int>& leaves;
};

int identifyCenter(const AdjList& igraph, int root) {
	AdjList graph(igraph.getSize());
	dfs(igraph, CreateGraph(graph), root);

	queue<int> leaves;
	dfs(graph, GetLeafNodes(graph, leaves), root);
	int node = -1;
	while (!leaves.empty()) {
		node = leaves.front();
		leaves.pop();

		const set<int>& linked = graph.mLinks[node];

		if (linked.size() > 0) {
			int parent = *linked.begin();
			graph.removeLink(node, parent);

			if (graph.mLinks[parent].size() == 1) {
				leaves.push(parent);
			}
		}
	}
	return node;
}

int getTreeDiameter(const AdjList& graph, int root, int& height, vector<bool>& isVisited) {
	isVisited[root] = true;

	int fHeight = 0, sHeight = 0, maxDiameter = 0;
	int numChildren = 0;
	for (set<int>::const_iterator it = graph.mLinks[root].begin(), itEnd = graph.mLinks[root].end();
			it != itEnd;
			++it) {
		if (isVisited[*it]) continue;

		++numChildren;

		// child node
		int diameter = getTreeDiameter(graph, *it, height, isVisited);
		if (height > fHeight) {
			sHeight = fHeight;
			fHeight = height;
		}
		else if (height > sHeight) {
			sHeight = height;
		}
		maxDiameter = max(maxDiameter, diameter);
	}

	if (!numChildren) {
		height = 0;
		return 0;
	}
	else if (numChildren == 1) {
		height = fHeight + 1;
		return max(maxDiameter, height);
	}
	else {
		height = fHeight + 1;
		return max(maxDiameter, fHeight + sHeight + 2);
	}
}

void solve (AdjList& graph) {
	int minDiameter = numeric_limits<int>::max();
	pair<int, int> toRemove, toAdd;

	vector<pair<int, int> > edges;
	for (AdjList::edge_iterator it = graph.begin(); it.isValid(); ++it) {
		if (it.src() > it.dst()) edges.push_back(make_pair(it.src(), it.dst()));
	}

	for (vector<pair<int, int> >::const_iterator it = edges.begin(), itEnd = edges.end(); it != itEnd; ++it) {
		// by removing current edge we are left with two trees
		graph.removeLink(it->first, it->second);

		// identify the centre of each tree
		int lCentral = identifyCenter(graph, it->first);
		int rCentral = identifyCenter(graph, it->second);

		graph.addLink(lCentral, rCentral);

		vector<bool> isVisited(graph.getSize());
		int height;
		int d = getTreeDiameter(graph, it->first, height, isVisited);

		if (d < minDiameter) {
			minDiameter = d;
			toRemove = make_pair(it->first, it->second);
			toAdd = make_pair(lCentral, rCentral);
		}

		// restore original graph
		graph.removeLink(lCentral, rCentral);
		graph.addLink(it->first, it->second);
	}

	cout << minDiameter << endl <<
			toRemove.first << " " << toRemove.second << endl <<
			toAdd.first << " " << toAdd.second << endl;
}

int main() {
	int nCases = 1;
	// cin >> nCases;

	for (int current = 1; current <= nCases; ++current) {
		int n;
		cin >> n;

		AdjList graph(n + 1);
		int a, b;
		for (int i = 0; i < n - 1; ++i) {
			cin >> a >> b;
			graph.addLink(a, b);
		}

		solve(graph);
	}
	return 0;
}
