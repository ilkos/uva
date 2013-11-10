//============================================================================
// Name        : uva507-JillRidesAgain.cpp
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

int maxsum(const vector<int>& route, int& from, int& to) {
	int optimal = 0;
	from = 0;
	to = 0;

	int current = 0;
	int lo = 0;

	for (int i = 0; i < (int) route.size(); ++i) {
		current += route[i];

		if (current > optimal) {
			from = lo;
			to = i;
			optimal = current;
		}
		else if (current == optimal && to - from < i - lo) {
			from = lo;
			to = i;
		}

		if (current < 0) {
			current = 0;
			lo = i + 1;
		}
	}
	return optimal;
}

int main() {
	int b;
	cin >> b;

	for (int i = 0; i < b; ++i) {
		int nStops;
		cin >> nStops;

		vector<int> scores(nStops - 1);
		for (int j = 0; j < nStops - 1; ++j) {
			cin >> scores[j];
		}

		int from, to;
		int s = maxsum(scores, from, to);
		if (s <= 0) {
			cout << "Route " << i + 1 << " has no nice parts" << endl;
		}
		else {
			cout << "The nicest part of route " << i + 1 << " is between stops "
					<< from + 1 << " and " << to + 2 << endl;
		}
	}
	return 0;
}
