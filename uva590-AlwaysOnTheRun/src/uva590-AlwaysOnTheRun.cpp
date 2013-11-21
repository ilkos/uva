//============================================================================
// Name        : uva590-AlwaysOnTheRun.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <limits>
using namespace std;

struct Schedule {
	Schedule() {}
	Schedule(istream& in) {
		int d;
		in >> d;
		costs = vector<int>(d);

		for (int i = 0; i < d; ++i) {
			in >> costs[i];
			costs[i];
		}
	}

	int get(int day) const {
		if (costs.empty()) return 0;
		return costs[day % costs.size()];
	}
	vector<int> costs;
};

int getMinCost(const vector<vector<Schedule> >& schedules, const int k,
		const int currentCity, const int currentDay, vector<vector<int> >& memo) {
	if (currentDay == k && currentCity == (int)schedules.size() - 1) {
		return 0;
	}
	else if (currentDay == k) {
		return numeric_limits<int>::max();
	}

	if (memo[currentCity][currentDay] >= 0) {
		return memo[currentCity][currentDay];
	}

	int mCost = numeric_limits<int>::max();
	for (int i = 0; i < (int)schedules[currentCity].size(); ++i) {
		int cost = schedules[currentCity][i].get(currentDay);
		if (!cost) {
			continue;
		}

		int fcost = getMinCost(schedules, k, i, currentDay + 1, memo);

		if (fcost != numeric_limits<int>::max())
			mCost = min(mCost, cost + fcost);
	}
	return memo[currentCity][currentDay] = mCost;
}

int main() {
	int n, k;
	int scenario = 1;
	while (cin >> n >> k) {
		if (!n && !k) break;

		vector<vector<Schedule> > matrix(n, vector<Schedule>(n));
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				if (i == j) continue;
				matrix[i][j] = Schedule(cin);
			}
		}

		vector<vector<int> > memo(n, vector<int>(k, -1));

		cout << "Scenario #" << scenario << endl;
		int res = getMinCost(matrix, k, 0, 0, memo);
		if (res == numeric_limits<int>::max()) {
			cout << "No flight possible." << endl;
		}
		else {
			cout << "The best flight costs " << res << '.' << endl;
		}
		cout << endl;
		++scenario;
	}

	return 0;
}
