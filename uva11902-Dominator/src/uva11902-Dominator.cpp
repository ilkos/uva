//============================================================================
// Name        : uva11902-Dominator.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <stack>
#include <cassert>
using namespace std;

struct AdjList {
	AdjList(int nNodes) : mNodes(nNodes), mAdjacent(nNodes, vector<int>()) {}

	void addLink(int src, int dst, bool isDirected = false) {
		assert(src >= 0 && src < mNodes);
		assert(dst >= 0 && dst < mNodes);

		mAdjacent[src].push_back(dst);

		if (!isDirected) {
			addLink(dst, src, true);
		}
	}

	const vector<int>& getLinks(int node) const {
		assert(node >=0 && node < mNodes);
		return mAdjacent[node];
	}

	int getNodes() const {
		return mNodes;
	}

private:
	int mNodes;
	vector<vector<int> > mAdjacent;
};

template <typename T>
struct Matrix {
	Matrix(int size) : mSize(size), mData(size * size) {}

	T& operator() (int x, int y) {
		assert(x >= 0 && x < mSize);
		assert(y >= 0 && y < mSize);

		return mData[x * mSize + y];
	}

	const T& operator() (int x, int y) const {
		assert(x >= 0 && x < mSize);
		assert(y >= 0 && y < mSize);

		return mData[x * mSize + y];
	}

	void print() const {

	}

private:
	int mSize;
	vector<T> mData;
};

void dfs(int src, const AdjList& graph, vector<int>& entryTimes, int ignored) {

}

void solve(const AdjList& graph) {
	Matrix solution;

	// dfs with all nodes
	vector<int> entryTimes(graph.getNodes(), -1);
	dfs(0, graph, entryTimes, -1);

	// node 0 dominates by definition
	//XXX

	for (int i = 1; i < graph.getNodes(); ++i) {
		// assume that node #i is absent
		vector<int> entryTimesInAbsence(graph.getNodes(), -1);
		dfs(0, graph, entryTimesInAbsence, i);

		// diff
	}

	// print solution
}

int main() {
	int nCases;
	cin >> nCases;

	for (int caseNo = 1; caseNo <= nCases; ++caseNo) {
		int nNodes;
		cin >> nNodes;

		AdjList graph(nNodes);
		for (int i = 0; i < nNodes; ++i) {
			for (int j = 0; j < nNodes; ++j) {
				int t;
				cin >> t;
				if (t) {
					graph.addLink(i, j);
				}
			}
		}

		cout << "Case " << caseNo << ":" << endl;
		solve(matrix);
	}
	return 0;
}
