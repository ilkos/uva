//============================================================================
// Name        : uva11003-Boxes.cpp
//============================================================================

#include <iostream>
#include <limits>
#include <vector>
using namespace std;

struct Box {
	int weight;
	int maxload;
};

int solve(const vector<Box>& boxes) {
	// weight[i][j] is a diagonal table of the minimum weight attained
	// by using exactly j elements from the subsequence[0, i)
	vector<vector<int> > weight(boxes.size() + 1, vector<int>(boxes.size() + 1, numeric_limits<int>::max()));

	for (size_t i = 0; i < boxes.size() + 1; ++i) {
		weight[i][0] = 0;
	}

	for (size_t i = 1; i < boxes.size() + 1; ++i) { // introduce boxes[i - 1]
		for (size_t j = 1; j <= i; ++j) {
			if (weight[i - 1][j - 1] != numeric_limits<int>::max()) {
				if (boxes[i - 1].maxload >= weight[i - 1][j - 1]) {
					// either stack the box, or don't -> minimise weight
					weight[i][j] = min(weight[i - 1][j - 1] + boxes[i - 1].weight, weight[i - 1][j]);
				}
				else {
					weight[i][j] = weight[i - 1][j];
				}
			}
		}
	}

	int longest = 1;
	for (int j = boxes.size(); j >= 0; --j) {
		if (weight[boxes.size()][j] != numeric_limits<int>::max()) {
			longest = j;
			break;
		}
	}
	return longest;
}

int main() {
	int n;

	while (cin >> n) {
		if (!n) break;

		vector<Box> boxes(n);
		for (int i = n - 1; i >= 0; --i) { // store reversed
			cin >> boxes[i].weight >> boxes[i].maxload;
		}

		cout << solve(boxes) << endl;
	}
	return 0;
}
