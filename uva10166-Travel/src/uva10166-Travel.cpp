//============================================================================
// Name        : uva10166-Travel.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <map>
#include <stdint.h>
#include <cassert>
#include <queue>
#include <limits>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Resolver {
	Resolver() :
		curr_(0) {}

	uint32_t resolve(const string& s) {
		map<string, uint32_t>::const_iterator it = res_.find(s);
		if (it != res_.end()) {
			return it->second;
		}

		uint32_t nval = curr_++;
		res_[s] = nval;
		return nval;
	}
private:
	uint32_t curr_;
	map<string, uint32_t> res_;
};

typedef uint32_t vertex_t;
struct Edge {
	Edge(vertex_t dst, uint32_t departure, uint32_t arrival) :
		dst(dst), departure(departure), arrival(arrival) {}

	vertex_t dst;
	uint32_t departure, arrival;
};

struct AdjList {
	AdjList(size_t sz) :
		list_(vector<vector<Edge> >(sz)) {}

	void add(vertex_t src, vertex_t dst, uint32_t departure, uint32_t arrival) {
		assert(src < size() && dst < size());
		list_[src].push_back(Edge(dst, departure, arrival));
	}

	const vector<Edge>& edges(vertex_t node) const {
		assert(node < size());
		return list_[node];
	}

	size_t size() const {
		return list_.size();
	}

private:
	vector<vector<Edge> > list_;
};

struct Path {
	Path(vertex_t node, uint32_t time) :
		node(node), time(time) {}

	bool operator< (const Path& other) const {
		return time > other.time;
	}

	vertex_t node;
	uint32_t time;
};

uint32_t dijkstra(const AdjList& graph, const vertex_t src, const vertex_t dst, const uint32_t timeAtSrc) {
	vector<uint32_t> earliestTo(graph.size(), numeric_limits<uint32_t>::max());
	priority_queue<Path> pq;
	pq.push(Path(src, timeAtSrc));
	earliestTo[src] = timeAtSrc;

	while (!pq.empty()) {
		Path c = pq.top(); pq.pop();

		if (c.time > earliestTo[c.node]) {
			continue;
		}

		const vector<Edge>& edges = graph.edges(c.node);
		for (size_t i = 0; i < edges.size(); ++i) {
			if (c.time <= edges[i].departure) {
				if (edges[i].arrival < earliestTo[edges[i].dst]) {
					earliestTo[edges[i].dst] = edges[i].arrival;
					pq.push(Path(edges[i].dst, edges[i].arrival));
				}
			}
		}
	}

	return earliestTo[dst];
}

int main() {
	int c;

	while (cin >> c) {
		if (!c) break;

		Resolver res;
		string city;
		for (int i = 0; i < c; ++i) {
			cin >> city;
			res.resolve(city);
		}

		int t;
		cin >> t;

		AdjList graph(c);
		for (int i = 0 ; i < t; ++i) {
			int ti;
			cin >> ti;

			uint32_t time;
			vector<pair<uint32_t, uint32_t> > route;
			for (int j = 0; j < ti; ++j) {
				cin >> time >> city;
				route.push_back(make_pair(time, res.resolve(city)));
			}

			sort(route.begin(), route.end());

			for (size_t j = 0; j < route.size() - 1; ++j) {
				graph.add(route[j].second, route[j + 1].second, route[j].first, route[j + 1].first);
			}
		}

		uint32_t earliestStartTime;
		string src, dst;
		cin >> earliestStartTime >> src >> dst;
		const vertex_t srcNode = res.resolve(src);
		const vertex_t dstNode = res.resolve(dst);

		uint32_t earliestArrivalTime = numeric_limits<uint32_t>::max();
		uint32_t actualStartTime = 0;
		for (size_t i = 0; i < graph.edges(srcNode).size(); ++i) {
			if (earliestStartTime <= graph.edges(srcNode)[i].departure) {
				uint32_t r = dijkstra(graph,
						graph.edges(srcNode)[i].dst,
						dstNode,
						graph.edges(srcNode)[i].arrival);

				if (r <= earliestArrivalTime) {
					if (r < earliestArrivalTime) {
						actualStartTime = graph.edges(srcNode)[i].departure;
					}
					else {
						actualStartTime = max(actualStartTime, graph.edges(srcNode)[i].departure);
					}
					earliestArrivalTime = r;
				}
			}
		}

		if (earliestArrivalTime != numeric_limits<uint32_t>::max()) {
			cout << setfill('0') << setw(4) << actualStartTime << ' ' <<
					setfill('0') << setw(4) << earliestArrivalTime << endl;
		}
		else {
			cout << "No connection" << endl;
		}
	}
	return 0;
}
