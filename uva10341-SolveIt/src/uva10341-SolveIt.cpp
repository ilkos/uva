//============================================================================
// Name        : uva10341-SolveIt.cpp
//               First derivative in [0..1] is negative
//               Monotonically decreasing
//============================================================================

#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

#define EPS 0.0000001

double f(double x, int p, int q, int r, int s, int t, int u) {
	return p * exp(-x) + q * sin(x) + r * cos(x) + s * tan(x) + t * x * x + u;
}

void solve(int p, int q, int r, int s, int t, int u) {
	double lo = 0.0;
	double hi = 1.0;

	if (f(0, p, q, r, s, t, u) < -EPS || f(1.0, p, q, r, s, t, u) > EPS) {
		cout << "No solution" << endl;
		return;
	}

	double res;
	while (lo <= hi) {
		double mid = lo + (hi - lo) / 2;

		double fmid = f(mid, p, q, r, s, t, u);
		if (abs(fmid) <= EPS) {
			res = mid;
			break;
		}
		else if (fmid > EPS) {
			lo = mid;
		}
		else {
			hi = mid;
		}
	}

	cout << fixed << setprecision(4) << res << endl;
}

int main() {
	int p, q, r, s, t, u;

	while (cin >> p >> q >> r >> s >> t >> u) {
		solve(p, q, r, s, t, u);
	}
	return 0;
}
