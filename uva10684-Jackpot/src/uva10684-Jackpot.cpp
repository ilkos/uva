//============================================================================
// Name        : uva10684-Jackpot.cpp
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

int solve(const vector<int>& array) {
	int result = 0;

	int running = 0;
	for (int i = 0; i < (int) array.size(); ++i) {
		if (running + array[i] > 0) {
			running += array[i];
			result = max(result, running);
		}
		else {
			running = 0;
		}
	}

	return result;
}

int main() {
	int n;

	while (cin >> n) {
		if (!n) break;

		vector<int> array(n);
		for (int i = 0; i < n; ++i) {
			cin >> array[i];
		}

		int solution = solve(array);
		if (solution > 0)
			cout << "The maximum winning streak is " << solution << '.' << endl;
		else
			cout << "Losing streak." << endl;
	}
	return 0;
}
