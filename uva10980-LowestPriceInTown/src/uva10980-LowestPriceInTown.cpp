//============================================================================
// Name        : uva10980-LowestPriceInTown.cpp
//============================================================================

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <limits>
#include <iomanip>
using namespace std;

struct Offer {
	Offer(const pair<int, int>& in) :
		amount(in.first), price(in.second) {
	}
	int amount;
	int price;
};

void solve(const int amount, const vector<Offer>& offers, const vector<int>& queries) {
	// let A[i] denote the min amount for which you can get i items
	// then A[i] = min (A[i - offers[j].amount] + offers[j].price) for all j

	//cout << amount << endl;
	int A[amount + 1];
	fill(A, A + amount + 1, numeric_limits<int>::max());
	A[0] = 0;

	for (int i = 1; i < (int) amount + 1; ++i) {
		for (int j = 0; j < (int) offers.size(); ++j) {
			int prevAmount = (i - offers[j].amount < 0) ?
					0 :
					A[i - offers[j].amount];
			A[i] = min(A[i], prevAmount + offers[j].price);
		}
	}

	for (int i = 0; i < (int) queries.size(); ++i) {
		cout << fixed << setprecision(2) << "Buy " << queries[i] << " for $" << A[queries[i]] / 100.0 << endl;
	}
}

int main() {
	double price;
	int nOffers;
	int curr = 1;

	while (cin >> price >> nOffers >> ws) {
		map<int, int> offers;
		offers[1] = price * 100 + 0.5;
		for (int i = 0; i < nOffers; ++i) {
			int amount;
			double price;
			cin >> amount >> price >> ws;
			if (!offers.count(amount) || price * 100 + 0.5 < offers[amount]) {
				offers[amount] = price * 100 + 0.5;
			}
		}

		vector<Offer> offersV (offers.begin(), offers.end());

		string line; getline(cin, line);
		istringstream is(line);

		int amount;
		vector<int> queries;
		int maxAmount = 0;
		while (is >> amount) {
			maxAmount = max(maxAmount, amount);
			queries.push_back(amount);
		}
		cout << "Case " << curr << ":" << endl;
		solve(maxAmount, offersV, queries);
		++curr;
	}

	return 0;
}
