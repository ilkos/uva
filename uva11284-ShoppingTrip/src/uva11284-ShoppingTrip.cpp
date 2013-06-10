//============================================================================
// Name        : uva11284-ShoppingTrip.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <cassert>
#include <limits>
#include <bitset>
#include <map>
#include <iomanip>
using namespace std;

struct AdjList {
	struct Edge {
		Edge() :
				target(-1), cost(-1) {
		}
		Edge(int t, int c) :
				target(t), cost(c) {
		}
		int target;
		int cost;
	};

	AdjList(int nodes) :
			mNodes(nodes), mList(vector<vector<Edge> >(nodes)) {
	}

	void addEdge(int src, int dst, int cost, bool isDirected = false) {
		assert(src >= 0 && src < mNodes);
		assert(dst >= 0 && dst < mNodes);

		mList[src].push_back(Edge(dst, cost));

		if (!isDirected) {
			addEdge(dst, src, cost, true);
		}
	}

	const vector<Edge>& getEdges(const int node) const {
		assert(node >= 0 && node < mNodes);
		return mList[node];
	}

	int getNodes() const {
		return mNodes;
	}

	void calculateAllShortestPaths() {
		mShortestPaths = vector<vector<int> >(mNodes,
				vector<int>(mNodes, numeric_limits<int>::max() / 2));

		// initialise from adj list
		for (int i = 0; i < mNodes; ++i) {
			mShortestPaths[i][i] = 0;
			for (int j = 0; j < (int) mList[i].size(); ++j) {
				mShortestPaths[i][mList[i][j].target] = min(
						mShortestPaths[i][mList[i][j].target], mList[i][j].cost);
			}
		}

		for (int k = 0; k < mNodes; ++k) {
			for (int i = 0; i < mNodes; ++i) {
				for (int j = 0; j < mNodes; ++j) {
					mShortestPaths[i][j] = min(
							mShortestPaths[i][j],
							mShortestPaths[i][k] + mShortestPaths[k][j]);
				}
			}
		}
	}

	void printAllShortestPaths() const {
		for (int i = 0; i < getNodes(); ++i) {
			for (int j = 0; j < getNodes(); ++j) {
				cout << getShortestPath(i, j) << ' ';
			}
			cout << endl;
		}
	}

	int getShortestPath(int src, int dst) const {
		assert(src >= 0 && src < mNodes);
		assert(dst >= 0 && dst < mNodes);

		return mShortestPaths[src][dst];
	}

private:
	int mNodes;
	vector<vector<Edge> > mList;
	vector<vector<int> > mShortestPaths;
};

struct Comparator {
	bool operator()(const pair<int, bitset<64> >& b1
			, const pair<int, bitset<64> >& b2) const {
		if (b1.first == b2.first) {
			return b1.second.to_ulong() < b2.second.to_ulong();
		}
		return b1.first < b2.first;
	}
};

int solve(const AdjList& graph, const map<int, int>& stores,
           int srcNode, bitset<64> isVisited, // current node, current state
		   map<pair<int, bitset<64> >, int, Comparator>& cache) {

	map<pair<int, bitset<64> >, int, Comparator>::const_iterator it =
			cache.find(make_pair(srcNode, isVisited));
	if (it != cache.end()) {
		return it->second;
	}

	isVisited[srcNode] = true;
	int amountGained = -graph.getShortestPath(srcNode, 0);

	for (map<int, int>::const_iterator storeIt = stores.begin();
			storeIt != stores.end();
			++storeIt) {
		if (isVisited[storeIt->first]) { // do not repeat nodes
			continue;
		}

		int potentialBenefit = solve(graph, stores, storeIt->first, isVisited, cache) +
				storeIt->second -
				graph.getShortestPath(srcNode, storeIt->first);

		amountGained = max(amountGained, potentialBenefit);
	}

	isVisited[srcNode] = false;
	return cache[make_pair(srcNode, isVisited)] = amountGained;
}

int main() {
	int nCases;
	cin >> nCases;

	for (int caseNum = 1; caseNum <= nCases; ++caseNum) {
		int n, m;
		cin >> n >> m;

		AdjList graph(n + 1);
		for (int i = 0; i < m; ++i) {
			int p1, p2;
			double cost;
			cin >> p1 >> p2 >> cost;
			graph.addEdge(p1, p2, (int) ((cost * 100) + 0.5));
		}

		int p;
		cin >> p;
		map<int, int> stores;
		for (int i = 0; i < p; ++i) {
			int place;
			double save;
			cin >> place >> save;
			stores[place] += (int) ((save * 100) + 0.5);
		}

		graph.calculateAllShortestPaths();
		// graph.printAllShortestPaths();

		map<pair<int, bitset<64> > , int, Comparator> cache;
		bitset<64> isVisited;
		int byShopping = solve(graph, stores, 0, isVisited, cache);

		if (byShopping > 0) {
			cout << setprecision(2) << fixed << "Daniel can save $"
					<< byShopping / 100.0 << endl;
		} else {
			cout << "Don't leave the house" << endl;
		}
	}
	return 0;
}
