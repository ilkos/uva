//============================================================================
// Name        : uva10364-Square.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
using namespace std;

bool isTargetPossible(const int target, const vector<int>& lengths, bitset<20> used,
		const size_t pos, const int remaining, const int remainingSticks) {
	if (remainingSticks == 0) {
		return true;
	}
	if (remaining == 0) {
		return isTargetPossible(target, lengths, used, 0, target, remainingSticks - 1);
	}
	if (pos >= lengths.size()) {
		return false;
	}

	if (isTargetPossible(target, lengths, used, pos + 1, remaining, remainingSticks)) {
		return true;
	}
	if (!used[pos]) {
		used[pos] = true;
		return isTargetPossible(target, lengths, used, pos + 1, remaining - lengths[pos], remainingSticks);
	}
	return false;
}

// is it possible to split in 4 subsequences with exactly the same length
bool isPossible(const vector<int>& lengths) {
	int sum = 0;
	for (size_t i = 0; i < lengths.size(); ++i) {
		sum += lengths[i];
	}
	if (sum % 4 != 0) {
		return false;
	}
	const int target = sum / 4;
	return isTargetPossible(target, lengths, bitset<20>(), 0, target, 4);
}

int main() {
	int nCases;
	cin >> nCases;

	while (nCases--) {
		int m;
		cin >> m;

		vector<int> lengths(m);
		for (int i = 0; i < m; ++i) {
			cin >> lengths[i];
		}
		sort(lengths.begin(), lengths.end(), greater<int>());
		cout << (isPossible(lengths) ? "yes" : "no") << endl;
	}
	return 0;
}
