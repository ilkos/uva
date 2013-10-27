//============================================================================
// Name        : uva11512-GATTACA.cpp
//============================================================================

#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct SuffixArray {
	SuffixArray(const string& data) : mData(data) {}

	struct Suffix {
		Suffix() : idx(-1) {}
		Suffix(size_t idx) : idx(idx) {}
		size_t idx;
	};

	struct Cmp {
		Cmp (const vector<int>& ranks, int gap) : ranks(ranks), gap(gap) {}

		bool operator() (const Suffix& lhs, const Suffix& rhs) {
			if (ranks[lhs.idx] != ranks[rhs.idx]) {
				return ranks[lhs.idx] < ranks[rhs.idx];
			}

			size_t l = lhs.idx + gap;
			size_t r = rhs.idx + gap;
			if (l < ranks.size() && r < ranks.size()) {
				return ranks[l] < ranks[r];
			}
			else {
				return l > r; // equiv to lhs.idx > rhs.idx, i.e. smallest suffix wins
			}
		}

		const vector<int>& ranks;
		const int gap;
	};

	void prepare() {
		// sa is the suffix array
		sa.resize(mData.size());

		// ranks denotes the ranking of the n-grams as per the last round of calculation
		//       i.e as per the comparison of the first gap characters
		// ranks[i] is the ranking of the suffix starting at mData[i]
		vector<int> ranks(mData.size());

		// temporary vector
		// tmp[i] denotes the ranking of sa[i] (i.e. the ith suffix as per the current n-gram order)
		vector<int> tmp(mData.size());
		tmp[0] = 0; // fixed at 0, sa[0] has the best ranking

		for (size_t i = 0; i < mData.size(); ++i) {
			sa[i] = Suffix(i);
			ranks[i] = mData[i];
		}

		for (int gap = 1; ; gap *= 2) {
			sort(sa.begin(), sa.end(), Cmp(ranks, gap));

			// rank the current version of the suffix array
			for (size_t i = 0; i < tmp.size() - 1; ++i) {
				tmp[i + 1] = tmp[i] + Cmp(ranks, gap)(sa[i], sa[i + 1]);
			}

			// rank the suffixes pointed to by the suffix array
			for (size_t i = 0; i < ranks.size(); ++i) {
				ranks[sa[i].idx] = tmp[i];
			}

			if (tmp[tmp.size() - 1] == (int)tmp.size() - 1) break;
		}
	}

	const vector<Suffix>& get() const {
		return sa;
	}

	void calcLCP() {
		lcp.resize(sa.size());

		vector<int> rank(sa.size());
		for (size_t i = 0; i < sa.size(); ++i) {
			rank[sa[i].idx] = i;
		}

		lcp[0] = 0;
		size_t height = 0;
		// iterate through all suffixes
		for (size_t i = 0; i < mData.size(); ++i) {
			if (rank[i] > 0) {
				// find previous suffix in the suffix array
				size_t j = sa[rank[i] - 1].idx;

				// lower bound for search is height (Kasai et al)
				while (i + height < mData.size() && j + height < mData.size() &&
						mData[i + height] == mData[j + height]) {
					++height;
				}
				// save in lcp (i is the actual suffix, rank[i] will yield the suffix array order)
				lcp[rank[i]] = height;
				if (height > 0) --height;
			}
		}
	}

	const vector<int>& getLCP() const {
		return lcp;
	}

	void print() const {
		cout << "Suffix array for: \"" << mData << "\"" << endl;
		for (size_t i = 0; i < sa.size(); ++i) {
			cout << mData.substr(sa[i].idx) << endl;
		}
	}

private:
	const string mData;
	vector<Suffix> sa;
	vector<int> lcp;
};

int main() {
	int nCases;
	cin >> nCases;

	string data;
	while (nCases--) {
		cin >> data;

		SuffixArray sa(data);
		sa.prepare();
		sa.calcLCP();

		int maxsize = 0;
		for (size_t i = 0; i < sa.getLCP().size(); ++i) {
			maxsize = max(maxsize, sa.getLCP()[i]);
		}

		string maxstr;
		size_t nRept = 0;
		if (maxsize > 0) {
			int c = 0;
			while (sa.getLCP()[c] != maxsize) ++c;
			maxstr = data.substr(sa.get()[c].idx, maxsize);
			while (c + nRept < sa.getLCP().size() && sa.getLCP()[c + nRept] == maxsize) ++nRept;
		}

		if (!maxstr.empty()) {
			cout << maxstr << ' ' << nRept + 1 << endl;
		}
		else {
			cout << "No repetitions found!" << endl;
		}
	}
	return 0;
}
