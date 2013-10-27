//============================================================================
// Name        : uva10369-ArcticNetwork.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <cmath>
#include <iomanip>
#include <cassert>
using namespace std;

struct Coord {
	int x, y;

	long sdist(const Coord& other) const {
		return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
	}
};

// oracle
bool isGood(const long vsq, int sat, const vector<vector<long> >& distsq) {
	vector<bool> isVisited(distsq.size());
	typedef pair<long, size_t> Tgt;
	priority_queue<Tgt, vector<Tgt>, greater<Tgt> > q;

	q.push(make_pair(0, 0));
	unsigned int nVisited = 0;

	while (!q.empty() && nVisited != distsq.size()) {
		const Tgt currTgt = q.top();
		const size_t curr = currTgt.second;
		q.pop();

		if (isVisited[curr]) continue;

		if (currTgt.first > vsq) {
			if (!sat) return false;
			--sat;
		}
		isVisited[curr] = true;
		++nVisited;

		for (size_t i = 0; i < distsq[curr].size(); ++i) {
			if (isVisited[i]) continue;
			q.push(make_pair(distsq[curr][i], i));
		}
	}

	assert(nVisited == distsq.size());
	return true;
}

double solve(const int sat, const vector<Coord>& coords, const long maxdim) {
	vector<vector<long> > distsq(coords.size(), vector<long>(coords.size()));
	for (size_t i = 0; i < coords.size(); ++i) { // O(n^2)
		for (size_t j = 0; j < coords.size(); ++j) {
			distsq[i][j] = coords[i].sdist(coords[j]);
		}
	}

	long lo = 0;
	long hi = 2 * maxdim * maxdim;
	long last = -1;
	while (lo <= hi) { // O(logk)
		long mid = lo + (hi - lo) / 2;

		if (isGood(mid, sat, distsq)) {
			last = mid;
			hi = mid - 1;
		}
		else {
			lo = mid + 1;
		}
	}

	return sqrt(last);
}

int main() {
	int nCases;
	cin >> nCases;

	while (nCases--) {
		int sat;
		cin >> sat;

		if (sat) --sat; // takes two to comm

		int nOutposts;
		cin >> nOutposts;
		vector<Coord> coords(nOutposts);
		int maxdim = 0;
		for (int i = 0; i < nOutposts; ++i) {
			cin >> coords[i].x >> coords[i].y;
			maxdim = max(maxdim, max(coords[i].x, coords[i].y));
		}

		cout << fixed << setprecision(2) << solve(sat, coords, maxdim) << endl;
	}
	return 0;
}
