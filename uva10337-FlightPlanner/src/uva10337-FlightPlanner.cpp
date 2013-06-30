//============================================================================
// Name        : uva10337-FlightPlanner.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <cassert>
using namespace std;

template <typename T>
struct MaybeVal {
	MaybeVal() : initialised(false) {
	}

	MaybeVal(const T t) : initialised(true), val(t) {
	}

	operator T () {
		return get();
	}

	T get() const {
		assert(initialised);
		return val;
	}

	MaybeVal<T> operator+ (const T t) const {
		if (!initialised) {
			return MaybeVal<T>();
		}
		return MaybeVal<T>(val + t);
	}

	MaybeVal<T> operator- (const T t) const {
		if (!initialised) {
			return MaybeVal<T>();
		}
		return MaybeVal<T>(val - t);
	}

	bool operator!() const {
		return !initialised;
	}

	string show() const {
		if (initialised) {
			ostringstream conv;
			conv << val;
			return conv.str();
		}
		else {
			return "maybe";
		}
	}
private:
	bool initialised;
	T val;
};

template <typename T> MaybeVal<T> min3(MaybeVal<T> a, MaybeVal<T> b, MaybeVal<T> c) {
	vector<MaybeVal<T> > v(3); v[0] = a; v[1] = b; v[2] = c;
	MaybeVal<T> res;
	for (int i = 0; i < (int)v.size(); ++i) {
		if (!!v[i]) {
			if (!res || v[i].get() < res.get()) {
				res = v[i];
			}
		}
	}
	return res;
}

map<pair<int, int>, MaybeVal<int> > cache;
MaybeVal<int> solve(const int fullDist, const vector<vector<int> >& windMap, int currDist = 0, int currHeight = 0) {
	if (currDist == fullDist) {
		return currHeight != 0 ? MaybeVal<int>() : MaybeVal<int>(0);
	}

	if (currHeight < 0 || currHeight > 9 || fullDist - currDist < currHeight) {
		return MaybeVal<int>();
	}

	map<pair<int, int>, MaybeVal<int> >::const_iterator it = cache.find(make_pair(currDist, currHeight));
	if (it != cache.end()) {
		return it->second;
	}

	MaybeVal<int> res = cache[make_pair(currDist, currHeight)] = min3(
			solve(fullDist, windMap, currDist + 1, currHeight + 1) + 60 - windMap[9 - currHeight][currDist],
			solve(fullDist, windMap, currDist + 1, currHeight) + 30 - windMap[9 - currHeight][currDist],
			solve(fullDist, windMap, currDist + 1, currHeight - 1) + 20 - windMap[9 - currHeight][currDist]
			);

	return res;
}

int main() {
	int nCases;
	cin >> nCases;

	for (int curr = 1; curr <= nCases; ++curr) {
		int dist;
		cin >> dist;

		vector<vector<int> > windMap(10, vector<int>(dist / 100));
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < dist / 100; ++j) {
				cin >> windMap[i][j];
			}
		}

		cache.clear();
		MaybeVal<int> res = solve(dist / 100, windMap);
		if (!!res) {
			cout << res.get() << endl << endl;
		}
		else {
			cout << "bad luck?" << endl;
		}
	}
	return 0;
}
