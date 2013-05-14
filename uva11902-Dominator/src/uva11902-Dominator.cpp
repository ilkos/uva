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
		for (int i = 0; i < mSize; ++i) {
			if (!i) printSep();

			for (int j = 0; j < mSize; ++j) {
				if (!j) cout << '|';
				cout << (*this)(i, j) << '|';
			}
			cout << endl;

			printSep();
		}
	}

	void printSep() const {
		string sep((2 * mSize) + 1, '-');
		sep[0] = '+';
		sep[sep.size() - 1] = '+';
		cout << sep << endl;
	}

private:
	int mSize;
	vector<T> mData;
};

void dfs(int src, const AdjList& graph, vector<int>& entryTimes, int ignored) {
	entryTimes.assign(graph.getNodes(), -1);

	if (src == ignored) return;

	int time = 0;
	entryTimes[src] = time++;

	stack<int> s;
	s.push(src);

	while (!s.empty()) {
		int curr = s.top();
		s.pop();

		for (int i = 0; i < (int)graph.getLinks(curr).size(); ++i) {
			int newNode = graph.getLinks(curr)[i];
			if (newNode == ignored || entryTimes[newNode] >= 0) {
				continue;
			}

			entryTimes[newNode] = time++;
			s.push(newNode);
		}
	}
}

void solve(const AdjList& graph) {
	Matrix<char> solution(graph.getNodes());

	// dfs with all nodes
	vector<int> entryTimes;
	dfs(0, graph, entryTimes, -1);

	for (int i = 0; i < graph.getNodes(); ++i) {
		// assume that node #i is absent
		vector<int> entryTimesInAbsence;
		dfs(0, graph, entryTimesInAbsence, i);

		// diff
		for (int j = 0; j < graph.getNodes(); ++j) {
			solution(i, j) = ((entryTimes[j] >= 0 && entryTimesInAbsence[j] == -1) ? 'Y' : 'N');
		}
	}

	// print solution
	solution.print();
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
					graph.addLink(i, j, true);
				}
			}
		}

		cout << "Case " << caseNo << ":" << endl;
		solve(graph);
	}
	return 0;
}
