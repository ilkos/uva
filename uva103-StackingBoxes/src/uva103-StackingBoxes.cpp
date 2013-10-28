//============================================================================
// Name        : uva103-StackingBoxes.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <cassert>
using namespace std;

struct Box {
	Box(size_t dimensions) : dim(vector<int>(dimensions)) {}

	bool operator< (const Box& other) const {
		return dim < other.dim;
	}

	bool isNesting(const Box& other) const {
		for (size_t i = 0; i < dim.size(); ++i) {
			if (dim[i] >= other.dim[i]) {
				return false;
			}
		}
		return true;
	}

	int label;
	vector<int> dim;
};

// LIS in n^2
void solve(const vector<Box>& boxes) {
	// s[i] is the length of the longest chain using boxes[1..i]
	vector<int> s(boxes.size(), 1);
	vector<int> parent(boxes.size(), -1);

	int longest = 1;
	int longestEnd = 0;

	for (size_t i = 1; i < boxes.size(); ++i) {
		for (size_t j = 0; j < i; ++j) {
			if (s[j] >= s[i] && boxes[j].isNesting(boxes[i])) {
				s[i] = s[j] + 1;
				parent[i] = j;

				if (s[i] > longest) {
					longest = s[i];
					longestEnd = i;
				}
			}
		}
	}

	cout << longest << endl;

	stack<int> labels;

	int curr = longestEnd;
	while (curr >= 0) {
		labels.push(boxes[curr].label);
		curr = parent[curr];
	}

	assert((int)labels.size() == longest);
	while (!labels.empty()) {
		cout << labels.top();
		if (!labels.empty()) cout << ' ';
		labels.pop();
	}
	cout << endl;
}

int main() {
	int nBoxes, nDiamensions;

	while (cin >> nBoxes >> nDiamensions) {
		vector<Box> boxes(nBoxes, Box(nDiamensions));
		for (int i = 0; i < nBoxes; ++i) {
			for (int j = 0; j < nDiamensions; ++j) {
				cin >> boxes[i].dim[j];
			}
			boxes[i].label = i + 1;
			sort(boxes[i].dim.begin(), boxes[i].dim.end());
		}

		sort(boxes.begin(), boxes.end());
		solve(boxes);
	}
	return 0;
}
