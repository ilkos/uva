//============================================================================
// Name        : uva11450-WeddingShopping.cpp
//============================================================================

#include <iostream>
#include <limits>
#include <vector>
#include <map>
using namespace std;

map<pair<int, size_t>, int> cache;
int solve(const int money, const vector<vector<int> >& selections, size_t csel = 0) {
	if (money < 0) {
		return cache[make_pair(money, csel)] = numeric_limits<int>::max();
	}

	if (csel >= selections.size()) {
		return cache[make_pair(money, csel)] = money;
	}

	map<pair<int, size_t>, int>::const_iterator cIt = cache.find(make_pair(money, csel));
	if (cIt != cache.end()) {
		return cIt->second;
	}

	int minrem = numeric_limits<int>::max();
	for (size_t i = 0; i < selections[csel].size(); ++i) {
		minrem = min(minrem, solve(money - selections[csel][i], selections, csel + 1));
	}

	return cache[make_pair(money, csel)] = minrem;
}

int main() {
	int nCases;
	cin >> nCases;

	while (nCases--) {
		int money, c;
		cin >> money >> c;

		vector<vector<int> > selections(c);
		for (int i = 0; i < c; ++i) {
			int k;
			cin >> k;
			selections[i].resize(k);
			for (int j = 0; j < k; ++j) {
				cin >> selections[i][j];
			}
		}

		cache.clear();
		int remaining = solve(money, selections);
		if (remaining == numeric_limits<int>::max()) {
			cout << "no solution" << endl;
		}
		else {
			cout << money - remaining << endl;
		}
	}
	return 0;
}
