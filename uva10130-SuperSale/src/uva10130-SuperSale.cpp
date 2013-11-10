//============================================================================
// Name        : uva10130-SuperSale.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

struct Object {
	int weight, value;
};

void knapsack(const vector<Object>& objects, vector<int>& output, int maxWeight) {
	output.resize(maxWeight + 1);

	// let A[i][j] denote the maximum value using objects [0, i) to with an exact weight of j
	int A[objects.size() + 1][maxWeight + 1];
	memset(A, 0, sizeof(A));

	for (int i = 1; i < (int) objects.size() + 1; ++i) { // evaluate i - 1
		for (int j = 0; j < maxWeight + 1; ++j) {
			A[i][j] = max(A[i - 1][j],
					(j >= objects[i - 1].weight ?
							A[i - 1][j - objects[i - 1].weight] + objects[i - 1].value :
							0));
		}
	}

	for (int i = 0; i < maxWeight + 1; ++i) {
		output[i] = A[objects.size()][i];
	}
}

int main() {
	int nTests;
	cin >> nTests;

	while (nTests--) {
		int nObjects;
		cin >> nObjects;

		vector<Object> objects(nObjects);
		for (int i = 0; i < nObjects; ++i) {
			cin >> objects[i].value >> objects[i].weight;
		}

		int nPeople;
		cin >> nPeople;
		vector<int> maxWeight(nPeople);
		int maxWeightVal = 0;
		for (int i = 0; i < nPeople; ++i) {
			cin >> maxWeight[i];
			maxWeightVal = max(maxWeightVal, maxWeight[i]);
		}

		// calculate the 0 - 1 value for weight up to maxVal
		vector<int> maxValues;
		knapsack(objects, maxValues, maxWeightVal);

		int total = 0;
		for (int i = 0; i < nPeople; ++i) {
			total += maxValues[maxWeight[i]];
		}
		cout << total << endl;
	}
	return 0;
}
