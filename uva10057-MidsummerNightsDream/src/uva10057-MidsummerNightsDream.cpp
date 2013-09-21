//============================================================================
// Name        : uva10057-MidsummerNightsDream.cpp
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

template <typename T>
T partition(T eIt, T eItEnd) {
	if (eIt == eItEnd) {
		return eIt;
	}

	{
		T selection = eIt + (eItEnd - eIt) / 2;
		swap(*selection, *eItEnd);
	}

	T firstHigh = eIt;
	for (; eIt < eItEnd; ++eIt) {
		if (*eIt < *eItEnd) {
			swap(*eIt, *firstHigh);
			++firstHigh;
		}
	}

	swap(*firstHigh, *eItEnd);
	return firstHigh;
}

template <typename T>
typename T::value_type _quickselect(T eIt, T eItEnd, int ranking) {
	T pivot = partition(eIt, eItEnd);

	int lhs = pivot - eIt;
	if (ranking < lhs) {
		return _quickselect(eIt, pivot, ranking);
	}
	else if (ranking > lhs) {
		return _quickselect(pivot + 1, eItEnd, ranking - lhs - 1);
	}
	else {
		return *pivot;
	}
}

template <typename T>
typename T::value_type quickselect(T eIt, T eItEnd, int ranking) {
	return _quickselect(eIt, eItEnd - 1, ranking);
}

int main() {
	int n;

	vector<int> exist(65536);
	while (cin >> n) {
		vector<int> numbers(n);
		fill(exist.begin(), exist.end(), 0);
		for (int i = 0; i < n; ++i) {
			cin >> numbers[i];
			++exist[numbers[i]];
		}

		if (n % 2) {
			int median = quickselect(numbers.begin(), numbers.end(), n / 2);
			cout << median << ' ' << exist[median] << ' ' << 1 << endl;
		}
		else {
			int median1 = quickselect(numbers.begin(), numbers.end(), n / 2 - 1);
			int median2 = quickselect(numbers.begin(), numbers.end(), n / 2);
			cout << median1 << ' ' << (median1 != median2 ? exist[median1] + exist[median2] : exist[median1])
					<< ' ' << median2 - median1 + 1 << endl;
		}
	}
	return 0;
}
