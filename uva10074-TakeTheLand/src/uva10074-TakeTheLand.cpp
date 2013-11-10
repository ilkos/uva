//============================================================================
// Name        : uva10074-TakeTheLand.cpp
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

int find_area(const vector<vector<int> >& amap) {
	vector<vector<int> > vprefix(amap.size(), vector<int>(amap[0].size()));

	for (size_t i = 0; i < amap[0].size(); ++i) {
		vprefix[0][i] = amap[0][i];
	}

	for (size_t i = 1; i < amap.size(); ++i) {
		for (size_t j = 0; j < amap[i].size(); ++j) {
			vprefix[i][j] = vprefix[i - 1][j] + amap[i][j];
		}
	}

	int r = 0;

	for (int i = 0; i < (int) amap.size(); ++i) {
		for (int j = 0; j < (int) amap.size(); ++j) {
			int lo = 0;
			for (int k = 0; k < (int) amap[i].size(); ++k) {
				if (vprefix[j][k] - (i > 0 ? vprefix[i - 1][k] : 0)) {
					lo = k + 1;
				}
				else {
					r = max(r, (j - i + 1) * (k - lo + 1));
				}
			}
		}
	}
	return r;
}

int main() {
	int m, n;

	while (cin >> m >> n) {
		if (!m && !n) break;

		vector<vector<int> > areamap(m, vector<int>(n));
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				cin >> areamap[i][j];
			}
		}

		cout << find_area(areamap) << endl;
	}
	return 0;
}
