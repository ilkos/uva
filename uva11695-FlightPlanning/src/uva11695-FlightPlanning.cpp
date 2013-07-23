//============================================================================
// Name        : uva11695-FlightPlanning.cpp
//               A connected graph with n nodes and n - 1 edges is a *tree*
//============================================================================

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <cassert>
#include <limits>
using namespace std;

#ifdef ONLINE_JUDGE
#define assert(x)
#endif

struct AdjList {
	AdjList(int n) : mLinks(vector<vector<int> >(n)) {}

	void addLink(const int src, const int dst, bool isDirected = false) {
		assert(src >= 0 && src < (int) mLinks.size() && dst >= 0 && dst < (int) mLinks.size());

		mLinks[src].push_back(dst);
		if (!isDirected) {
			mLinks[dst].push_back(src);
		}
	}

	void removeLink(const int src, const int dst, bool isDirected = false) {
		assert(src >= 0 && src < (int) mLinks.size() && dst >= 0 && dst < (int) mLinks.size());
		mLinks[src].erase(remove(mLinks[src].begin(), mLinks[src].end(), dst),
				mLinks[src].end());

		if (!isDirected) {
			mLinks[dst].erase(remove(mLinks[dst].begin(), mLinks[dst].end(), src),
					mLinks[dst].end());
		}
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
		edge_iterator(const AdjList& adjList, vector<vector<int> >::const_iterator srcIt) :
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
		vector<vector<int> >::const_iterator srcIt;
		vector<int>::const_iterator dstIt;
	};

	edge_iterator begin() const {
		return edge_iterator(*this, mLinks.begin());
	}

	vector<vector<int> > mLinks;
};

template <typename T>
void dfs(const AdjList& graph, T visitor, const int src) {
	vector<int> isVisited(graph.getSize());
	stack<int> s;

	isVisited[src] = true;
	s.push(src);

	while (!s.empty()) {
		int current = s.top();
		s.pop();

		visitor.visitNode(current);

		for (int i = 0; i < (int) graph.mLinks[current].size(); ++i) {
			int tgt = graph.mLinks[current][i];
			if (isVisited[tgt]) {
				continue;
			}

			isVisited[tgt] = true;
			s.push(tgt);
		}
	}
}

struct GetOutDegreeNodes {
	GetOutDegreeNodes(const AdjList& graph, vector<vector<int> >& links, queue<int>& leaves) :
		graph(graph), links(links), leaves(leaves) {}

	void visitNode(int node) {
		links[node] = graph.mLinks[node];

		if (links[node].size() <= 1) {
			leaves.push(node);
		}
	}

private:
	const AdjList& graph;
	vector<vector<int> >& links;
	queue<int>& leaves;
};

int identifyCenter(const AdjList& graph, int root) {
	queue<int> leaves;
	vector<vector<int> > links(graph.getSize());
	dfs(graph, GetOutDegreeNodes(graph, links, leaves), root);

	int node;
	while (!leaves.empty()) {
		node = leaves.front();
		leaves.pop();

		if (links[node].size() > 0) {
			int parent = links[node].front();
			links[parent].erase(remove(links[parent].begin(), links[parent].end(), node),
					links[parent].end());

			if (links[parent].size() == 1) {
				leaves.push(parent);
			}
		}
	}
	return node;
}

int getTreeDiameter(const AdjList& graph, int root, int& height, vector<int>& isVisited) {
	isVisited[root] = true;

	int fHeight = 0, sHeight = 0, maxDiameter = 0;
	int numChildren = 0;
	for (int i = 0; i < (int) graph.mLinks[root].size(); ++i) {
		int tgt = graph.mLinks[root][i];
		if (isVisited[tgt]) continue;
		++numChildren;

		// child node
		int d = getTreeDiameter(graph, tgt, height, isVisited);
		if (height > fHeight) {
			sHeight = fHeight;
			fHeight = height;
		}
		else if (height > sHeight) {
			sHeight = height;
		}
		maxDiameter = max(maxDiameter, d);
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
		edges.push_back(make_pair(it.src(), it.dst()));
	}

	for (vector<pair<int, int> >::const_iterator it = edges.begin(), itEnd = edges.end(); it != itEnd; ++it) {
		// by removing current edge we are left with two trees
		graph.removeLink(it->first, it->second);

		int height;
		vector<int> visited(graph.getSize());
		int lDiameter = getTreeDiameter(graph, it->first, height, visited);
		int rDiameter = getTreeDiameter(graph, it->second, height, visited);

		int d = max(max(lDiameter, rDiameter), (lDiameter + 1) / 2 + (rDiameter + 1) / 2 + 1);

		if (d < minDiameter) {
			// identify the centre of each tree
			int lCentral = identifyCenter(graph, it->first);
			int rCentral = identifyCenter(graph, it->second);

			minDiameter = d;
			toRemove = make_pair(it->first, it->second);
			toAdd = make_pair(lCentral, rCentral);
		}

		// restore original graph
		graph.addLink(it->first, it->second);
	}

	cout << minDiameter << endl <<
			toRemove.first << " " << toRemove.second << endl <<
			toAdd.first << " " << toAdd.second << endl;
}

int main() {
	int nCases = 1;
#ifdef ONLINE_JUDGE
	cin >> nCases;
#endif

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
