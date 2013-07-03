//============================================================================
// Name        : uva1220-PartyAtHaliBula.cpp
//               Maximum independent set in tree graph
//============================================================================

#include <iostream>
#include <map>
#include <vector>
#include <cassert>
using namespace std;

struct AdjList {
	void link(const string& src, const string& dst) {
		mAdjList[src].push_back(dst);
	}

	bool hasLinked(const string& src) const {
		return mAdjList.find(src) != mAdjList.end();
	}

	const vector<string>& getLinked(const string& src) const {
		map<string, vector<string> >::const_iterator it = mAdjList.find(src);
		if (it == mAdjList.end()) {
			throw;
		}
		return it->second;
	}

private:
	map<string, vector<string> > mAdjList;
};

map<pair<string, bool>, pair<int, bool> > cache;
pair<int, bool> countMaxIndepSet(const string& root, bool canSelect, const AdjList& graph) {
	if (!graph.hasLinked(root)) {
		return canSelect ? make_pair(1, true) : make_pair(0, true);
	}

	map<pair<string, bool>, pair<int, bool> >::const_iterator it = cache.find(make_pair(root, canSelect));
	if (it != cache.end()) {
		return it->second;
	}

	pair<int, bool> resultWithout = make_pair(0, true);
	// assume not selected
	for (int i = 0; i < (int)graph.getLinked(root).size(); ++i) {
		pair<int, bool> c = countMaxIndepSet(graph.getLinked(root)[i], true, graph);
		resultWithout.first += c.first;
		resultWithout.second &= c.second;
	}

	pair<int, bool> resultWith = make_pair(0, true);
	if (canSelect) {
		++resultWith.first;
		for (int i = 0; i < (int)graph.getLinked(root).size(); ++i) {
			pair<int, bool> c = countMaxIndepSet(graph.getLinked(root)[i], false, graph);
			resultWith.first += c.first;
			resultWith.second &= c.second;
		}
	}

	if (resultWithout.first > resultWith.first) {
		return cache[make_pair(root, canSelect)] = resultWithout;
	}
	else if (resultWithout.first < resultWith.first) {
		return cache[make_pair(root, canSelect)] = resultWith;
	}
	else { // same
		return cache[make_pair(root, canSelect)] = make_pair(resultWith.first, false);
	}
}

int main() {
	int nEmployees;

	while (cin >> nEmployees) {
		if (!nEmployees) break;

		string bigBoss;
		cin >> bigBoss;

		AdjList graph;
		string name, boss;
		for (int i = 1; i < nEmployees; ++i) {
			cin >> name >> boss;
			graph.link(boss, name);
		}

		cache.clear();
		pair<int, bool> result = countMaxIndepSet(bigBoss, true, graph);
		cout << result.first << " " << (result.second ? "Yes" : "No") << endl;
	}
	return 0;
}
