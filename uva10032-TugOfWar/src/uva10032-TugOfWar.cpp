//============================================================================
// Name        : uva10032-TugOfWar.cpp
//               Subset sum with an extra constraint
//============================================================================

#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <bitset>
#include <algorithm>
using namespace std;

// returns weight of nPeople out of weights that is as close as possible to totalWeight / 2
int solve(const int totalWeight, const int nPeople, const vector<int>& weights) {
	if (!nPeople) {
		return 0;
	}

	// A[i] bit j being on means that we can attain weight i using j people
	typedef unsigned long long dp_t;
	dp_t A[totalWeight + 1];
	fill (A, A + sizeof(A) / sizeof(dp_t), 0);

	for (int i = 0; i < (int) weights.size(); ++i) {
		// reverse order so that we don't count ourselves more than once
		for (int j = totalWeight; j > weights[i]; --j) {
			A[j] |= A[j - weights[i]] << 1; // add one person to the hops
		}
		A[weights[i]] |= 1;
	}

	int closest = numeric_limits<int>::max();
	int closest_val;

	for (int i = totalWeight; i >= 0; --i) {
		if (A[i] & (1LL << (nPeople - 1))) { // beware of the shifting.. 1L for long
			// cout << i << " " << bitset<64>(A[i]) << endl;
			int dist = abs(totalWeight - 2 * i);
			if (dist < closest) {
				closest = dist;
				closest_val = i;
			}
		}
	}

	return closest_val;
}

int main() {
	int nCases;
	cin >> nCases;

	while (nCases--) {
		int nPeople;
		cin >> nPeople;

		int totalWeight = 0;
		vector<int> weights(nPeople);
		for (int i = 0; i < nPeople; ++i) {
			cin >> weights[i];
			totalWeight += weights[i];
		}

		sort(weights.begin(), weights.end());
		int result = solve(totalWeight, nPeople / 2, weights);

		int complement = totalWeight - result;
		cout << (result < complement ? result : complement) << ' ' <<
				(result < complement ? complement : result) << endl;

		if (nCases) cout << endl;
	}
	return 0;
}
