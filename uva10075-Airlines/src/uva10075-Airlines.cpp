//============================================================================
// Name        : uva10075-Airlines.cpp
//============================================================================

#include <iostream>
#include <map>
#include <vector>
#include <cassert>
#include <cmath>
#include <queue>
using namespace std;

#define PI 3.141592653589793

struct Edge {
	Edge (const string& itarget, double iweight) : target(itarget), weight(iweight) {}
	string target;
	double weight;
};

struct AdjacencyList {

	void addLink(const string& src, string& dst, double weight) {
		mList[src].push_back(Edge(dst, weight));
	}

	const vector<Edge>& getLinks(const string& node) const {
		map<string, vector<Edge> >::const_iterator it = mList.find(node);
		if (it != mList.end()) return it->second;

		static vector<Edge> empty;
		return empty;
	}
	map<string, vector<Edge> > mList;
};

double getDistance(const string& src, const string& dst,
		const map<string, pair<double, double> >& locations) {
	const pair<double, double>& l1 = locations.find(src)->second;
	const pair<double, double>& l2 = locations.find(dst)->second;

	double ds = acos(sin(l1.first * PI/180.0) * sin(l2.first * PI/180.0) + cos(l1.first * PI/180.0) * cos(l2.first * PI/180.0) * cos(abs(l1.second - l2.second) * PI/180.0));
	double result = floor((6378.0 * ds) + 0.5);

	return result;
}

struct Path {
	Path(const string& loc, double dist) : loc(loc), dist(dist) {}
	bool operator> (const Path& other) const {
		return dist > other.dist;
	}

	string loc;
	double dist;
};

double getMinPath(const string& src, const string& dst, const AdjacencyList& graph) {
	priority_queue<Path, vector<Path>, greater<Path> > pq;
	pq.push(Path(src, 0));

	map<string, bool> isVisited;

	while (!pq.empty()) {
		Path curr = pq.top();
		pq.pop();

		isVisited[curr.loc] = true;
		if (curr.loc == dst) return curr.dist;

		for (int i = 0; i < (int)graph.getLinks(curr.loc).size(); ++i) {
			const Edge& edge = graph.getLinks(curr.loc)[i];

			if (!isVisited[edge.target]) {
				pq.push(Path(edge.target, curr.dist + edge.weight));
			}
		}
	}

	return -1.0;
}

int main() {
	int n, m, q;

	int nCase = 1;

	while (cin >> n >> m >> q) {
		if (!n && !m && !q) break;

		string city;
		double longitude, latitude;
		map<string, pair<double, double> > cityLocations;
		for (int i = 0; i < n; ++i) {
			cin >> city >> latitude >> longitude;
			cityLocations[city] = make_pair(latitude, longitude);
		}

		AdjacencyList graph;
		for (int i = 0; i < m; ++i) {
			string src, dst;
			cin >> src >> dst;
			graph.addLink(src, dst, getDistance(src, dst, cityLocations));
		}

		if (nCase != 1) cout << endl;
		cout << "Case #" << nCase++ << endl;

		for (int i = 0; i < q; ++i) {
			string src, dst;
			cin >> src >> dst;

			double sol = getMinPath(src, dst, graph);
			if (sol >= 0) {
				cout << sol << " km" << endl;
			}
			else {
				cout << "no route exists" << endl;
			}
		}
	}
	return 0;
}
