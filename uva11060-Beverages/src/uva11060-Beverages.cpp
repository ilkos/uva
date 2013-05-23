//============================================================================
// Name        : uva11060-Beverages.cpp
//============================================================================

#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct AdjList {
	AdjList(const int nNodes) :
		mNodes(nNodes),
		mList(vector<vector<int> >(nNodes, vector<int>())),
		mInbound(vector<int>(nNodes)) {}

	void addLink(const int src, const int dst, bool isDirected = true) {
		mList[src].push_back(dst);
		++mInbound[dst];

		if (!isDirected) {
			addLink(dst, src, true);
		}
	}

	const vector<int>& getOutboundLinks(const int node) const {
		return mList[node];
	}

	int& getInboundLinks(const int node) {
		return mInbound[node];
	}

	int getNodes() const {
		return mNodes;
	}

private:
	int mNodes;

	vector<vector<int> > mList;
	vector<int> mInbound;
};

#ifdef DFS
void dfs(const AdjList& graph, int src, vector<int>& output, vector<bool>& isVisited) {
	for (int i = 0; i < (int) graph.getOutboundLinks(src).size(); ++i) {
		int target = graph.getOutboundLinks(src)[i];
		if (!isVisited[target]) {
			dfs(graph, target, output, isVisited);
		}
	}

	isVisited[src] = true;
	output.push_back(src);
}

// Version based on DFS, requires the links to be stronger -> weaker
void toposort(const AdjList& graph, vector<int>& output) {
	vector<bool> isVisited(graph.getNodes());

	for (int i = 0; i < graph.getNodes(); ++i) {
		if (!isVisited[i]) {
			dfs(graph, i, output, isVisited);
		}
	}
}
#endif

// Version based on Kahn's algorithm, requires the links to be weaker -> stronger
void toposort(AdjList& graph, vector<int>& output) {
	priority_queue<int, vector<int>, greater<int> > q;

	// identify first batch without incoming links
	for (int i = 0; i < graph.getNodes(); ++i) {
		if (graph.getInboundLinks(i) == 0) {
			q.push(i);
		}
	}

	while (!q.empty()) {
		int curr = q.top();
		q.pop();

		output.push_back(curr);

		// search others
		for (int i = 0; i < (int) graph.getOutboundLinks(curr).size(); ++i) {
			int target = graph.getOutboundLinks(curr)[i];

			--graph.getInboundLinks(target);
			if (graph.getInboundLinks(target) == 0) {
				q.push(target);
			}
		}
	}
}

int main() {
	int numCase = 1;
	int nBeverages;

	while (cin >> nBeverages) {
		AdjList graph(nBeverages);

		vector<string> names;
		map<string, int> namesToNums;

		string name;
		for (int i = 0; i < nBeverages; ++i) {
			cin >> name;
			names.push_back(name);
			namesToNums[name] = i;
		}

		int nRelations;
		cin >> nRelations;
		string weaker, stronger;
		for (int i = 0; i < nRelations; ++i) {
			cin >> weaker >> stronger;
			graph.addLink(namesToNums[weaker], namesToNums[stronger]);
		}

		vector<int> topoSorted;
		toposort(graph, topoSorted);

		cout << "Case #" << numCase++ << ": Dilbert should drink beverages in this order:";
		for (int i = 0; i < (int) topoSorted.size(); ++i) {
			cout << ' ' << names[topoSorted[i]];
		}
		cout << '.' << endl << endl;
	}
	return 0;
}
