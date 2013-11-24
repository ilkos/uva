//============================================================================
// Name        : uva10926-HowManyDependencies.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <stack>
#include <stdint.h>
#include <cassert>
using namespace std;

struct AdjList {
	AdjList(uint32_t sz) :
		size_(sz),
		graph_(vector<vector<uint32_t> >(sz)) {
	}

	void addEdge(uint32_t src, uint32_t dst) {
		assert(src < size_ && dst < size_);
		graph_[src].push_back(dst);
	}

	const vector<uint32_t>& edges(uint32_t node) const {
		assert(node < size_);
		return graph_[node];
	}

	uint32_t size() const {
		return size_;
	}

private:
	uint32_t size_;
	vector<vector<uint32_t> > graph_;
};

int getNumDeps(const AdjList& graph, int task) {
	stack<int> s;
	vector<bool> isVisited(graph.size());

	int cnt = 0;
	s.push(task);
	isVisited[task] = true;

	while (!s.empty()) {
		int node  = s.top();
		s.pop();

		const vector<uint32_t>& outgoing = graph.edges(node);
		for (size_t i = 0; i < outgoing.size(); ++i) {
			if (isVisited[outgoing[i]]) continue;

			s.push(outgoing[i]);
			isVisited[outgoing[i]] = true;
			++cnt;
		}
	}

	return cnt;
}

int getMaxNumDependencies(const AdjList& graph) {
	int mval = 0;
	int task = -1;

	for (uint32_t i = 0; i < graph.size(); ++i) {
		int numDependencies = getNumDeps(graph, i);
		if (numDependencies > mval) {
			mval = numDependencies;
			task = i;
		}
	}
	return task;
}

int main() {
	int n;
	while (cin >> n) {
		if (!n) break;

		AdjList graph(n);
		for (int task = 0; task < n; ++task) {
			int nDepends;
			cin >> nDepends;

			for (int dependency = 0; dependency < nDepends; ++dependency) {
				int dependent;
				cin >> dependent;
				--dependent;

				graph.addEdge(task, dependent);
			}
		}

		cout << getMaxNumDependencies(graph) + 1 << endl;
	}

	return 0;
}
