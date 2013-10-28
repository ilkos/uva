//============================================================================
// Name        : uva10474-WhereIsTheMarble.cpp
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

void csort(const vector<int>& v, vector<int>& occ) {
	for (size_t i = 0; i < v.size(); ++i) {
		++occ[v[i]];
	}

	size_t vcurr = 0;
	for (size_t i = 0; i < occ.size(); ++i) {
		size_t skip = occ[i];
		occ[i] = occ[i] ? vcurr : -1;
		vcurr += skip;
	}
}

int main() {
	int n, q;

	int nCase = 1;
	while (cin >> n >> q) {
		if (!n && !q) break;

		vector<int> marbles(n);
		int maxSeen = 0;
		for (int i = 0; i < n; ++i) {
			cin >> marbles[i];
			maxSeen = max(maxSeen, marbles[i]);
		}

		vector<int> occ(maxSeen + 1);
		csort(marbles, occ);

		cout << "CASE# " << nCase << ':' << endl;
		for (int i = 0; i < q; ++i) {
			int query;
			cin >> query;

			int r = query <= maxSeen ? occ[query] : -1;
			if (r < 0) {
				cout << query << " not found" << endl;
			}
			else {
				cout << query << " found at " << r + 1 << endl;
			}
		}
		++nCase;
	}

	return 0;
}
