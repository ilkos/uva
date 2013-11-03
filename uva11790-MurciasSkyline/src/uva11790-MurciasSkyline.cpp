//============================================================================
// Name        : uva11790-MurciasSkyline.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
using namespace std;

template <typename Comp>
int _LIS(const vector<int>& elements, const vector<int>& lengths, Comp comp) {
	vector<int> A(lengths.begin(), lengths.end());

	for (size_t i = 1; i < elements.size(); ++i) {
		for (size_t j = 0; j < i; ++j) {
			if (comp(elements[j], elements[i]) && A[j] + lengths[i] > A[i]) {
				A[i] = A[j] + lengths[i];
			}
		}
	}

	return *max_element(A.begin(), A.end());
}

pair<int, int> solve(const vector<int>& heights, const vector<int>& widths) {
	int lis = _LIS(heights, widths, less<int>());
	int lds = _LIS(heights, widths, greater<int>());
	return make_pair(lis, lds);
}

int main() {
	int nCases;
	cin >> nCases;

	for (int curr = 1; curr <= nCases; ++curr) {
		int nBuildings;
		cin >> nBuildings;

		vector<int> heights(nBuildings), widths(nBuildings);
		for (int i = 0; i < nBuildings; ++i) {
			cin >> heights[i];
		}

		for (int i = 0; i < nBuildings; ++i) {
			cin >> widths[i];
		}

		pair<int, int> s = solve(heights, widths);
		cout << "Case " << curr << ". ";
		if (s.first >= s.second) {
			cout << "Increasing (" << s.first << "). Decreasing (" << s.second << ")." << endl;
		}
		else {
			cout << "Decreasing (" << s.second << "). Increasing (" << s.first << ")." << endl;
		}
	}
	return 0;
}
