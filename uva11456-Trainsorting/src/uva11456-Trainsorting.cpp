//============================================================================
// Name        : uva11456-Trainsorting.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <set>
using namespace std;

// Longest increasing - decreasing subsequence
// Calculate length for each starting point and optimise of LIS + LDS over the starting points
template <typename Comp>
void _LIS(const vector<int>& elements, vector<int>& out, Comp comp) {
	out.assign(elements.size(), 1);

	// we want the resulting array out[i] to contain the length of the longest subsequence *starting*
	// from i and not ending in i
	// as such, we run the algorithm in the reverse order (the comparator for LIS is less<> and the one for LDS is greater<>)
	for (int i = elements.size() - 1; i >= 0; --i) {
		for (int j = elements.size() - 1; j > i; --j) {
			if (comp(elements[j], elements[i]) && out[i] < out[j] + 1) {
				out[i] = out[j] + 1;
			}
		}
	}
}

int solve(const vector<int>& weights) {
	vector<int> lis, lds;

	_LIS(weights, lis, less<int>());
	_LIS(weights, lds, greater<int>());

	int m = 0;
	for (size_t i = 0; i < weights.size(); ++i) {
		// cout << weights[i] << ": " << lis[i] << " - " << lds[i] << endl;
		m = max(m, lis[i] + lds[i] - 1);
	}
	return m;
}

int main() {
	int nCases;
	cin >> nCases;

	while (nCases--) {
		int nCars;
		cin >> nCars;

		vector<int> weights(nCars);
		for (int i = 0; i < nCars; ++i) {
			cin >> weights[i];
		}

		cout << solve(weights) << endl;
	}
	return 0;
}
