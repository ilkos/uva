//============================================================================
// Name        : uva11292-DragonOfLoowater.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <map>
using namespace std;

void solve(const vector<int>& heads, map<int, int>& heights) {
	int cost = 0;

	for (size_t i = 0; i < heads.size(); ++i) {
		map<int, int>::iterator h = heights.find(heads[i]);
		if (h == heights.end()) {
			h = heights.upper_bound(heads[i]);
			if (h == heights.end()) {
				cout << "Loowater is doomed!" << endl;
				return;
			}
		}

		cost += h->first;
		h->second--;
		if (!h->second) {
			heights.erase(h);
		}
	}

	cout << cost << endl;
}

int main() {
	int n, m;

	while (cin >> n >> m) {
		if (!n && !m) break;

		vector<int> heads(n);
		map<int, int> heights;

		for (int i = 0; i < n; ++i) {
			cin >> heads[i];
		}

		{
			int t;
			for (int i = 0; i < m; ++i) {
				cin >> t;
				++heights[t];
			}
		}

		solve(heads, heights);
	}
	return 0;
}
