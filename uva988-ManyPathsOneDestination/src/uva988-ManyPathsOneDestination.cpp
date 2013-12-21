//============================================================================
// Name        : uva988-ManyPathsOneDestination.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <stdint.h>
#include <cassert>
using namespace std;

typedef uint32_t vertex_t;
struct AdjList {
	AdjList(size_t sz) :
		list_(vector<vector<vertex_t> >(sz)) {}

	size_t size() const {
		return list_.size();
	}

	void add(vertex_t src, vertex_t dst) {
		assert(src < size() && dst < size());
		list_[src].push_back(dst);
	}

	const vector<vertex_t>& edges(vertex_t node) const {
		assert(node < size());
		return list_[node];
	}

private:
	vector<vector<vertex_t> > list_;
};

// cannot have loops (otherwise we will not terminate)
uint32_t enumerate(const AdjList& graph, const vertex_t src, vector<int32_t>& enumerated) {
	if (graph.edges(src).empty()) {
		return 1;
	}

	if (enumerated[src] >= 0) {
		return enumerated[src];
	}

	const vector<vertex_t>& edges = graph.edges(src);
	uint32_t res = 0;
	for (vector<vertex_t>::const_iterator eIt(edges.begin()), eItEnd(edges.end()); eIt != eItEnd; ++eIt) {
		res += enumerate(graph, *eIt, enumerated);
	}

	return enumerated[src] = res;
}

int main() {
	int nEvents;
	bool isFirstRun = true;

	while (cin >> nEvents) {
		AdjList graph(nEvents);

		for (int i = 0; i < nEvents; ++i) {
			int nChoices;
			cin >> nChoices;

			uint32_t conn;
			for (int j = 0; j < nChoices; ++j) {
				cin >> conn;
				graph.add(i, conn);
			}
		}

		if (!isFirstRun) cout << endl;
		else isFirstRun = false;

		vector<int32_t> enumerated(graph.size(), -1);
		cout << enumerate(graph, 0, enumerated) << endl;
	}
	return 0;
}
