//============================================================================
// Name        : uva196-Spreadsheet.cpp
//============================================================================

#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>
using namespace std;

class Expr {
public:
	virtual ~Expr() {
	}

	virtual void eval() =0;
	virtual int get() const =0;
};

class Num: public Expr {
public:
	Num(int val) :
			mVal(val) {
	}

	void eval() {
	}
	int get() const {
		return mVal;
	}
private:
	int mVal;
};

class SumV: public Expr {
public:
	SumV(const vector<Expr**>& sum) :
			mSum(sum), mVal(-1), isEval(false) {
	}

	void eval() {
		if (isEval) {
			return;
		}
		mVal = 0;
		for (int i = 0; i < (int) mSum.size(); ++i) {
			if (*mSum[i]) {
				(*mSum[i])->eval();
				mVal += (*mSum[i])->get();
			}
		}
		isEval = true;
	}

	int get() const {
		assert(isEval);
		return mVal;
	}

private:
	vector<Expr**> mSum;
	int mVal;
	bool isEval;
};

int convertAlphaToNum(const std::string& alpha) {
	int result = 0;
	for (int i = 0; i < (int)alpha.size(); ++i) {
		result *= 26;
		result += alpha[i] - 'A' + 1;
	}
	return result - 1;
}

pair<int, int> parseSprFmt(const std::string& pos) {
	int curr = 0;
	while (curr < (int) pos.size() && isalpha(pos[curr]))
		++curr;

	return std::make_pair(atoi(pos.substr(curr).c_str()) - 1,
			convertAlphaToNum(pos.substr(0, curr)));
}

class Table {
public:
	Table(int rows, int columns) :
			mData(vector<vector<Expr*> >(rows, vector<Expr*>(columns))) {
	}

	~Table() {
		for (int i = 0; i < (int) mData.size(); ++i) {
			for (int j = 0; j < (int) mData[0].size(); ++j) {
				delete mData[i][j];
				mData[i][j] = 0;
			}
		}
	}

	void assign(int row, int column, const string& expr) {
		assert(row >= 0 && row < (int)mData.size());
		assert(column >= 0 && column < (int)mData[0].size());

		mData[row][column] = getExpression(expr);
	}

	int get(int row, int column) {
		mData[row][column]->eval();
		return mData[row][column]->get();
	}

private:
	Expr* getExpression(const string& expr) {
		if (expr[0] != '=') {
			return new Num(atoi(expr.c_str()));
		}

		// = expr
		vector<Expr**> sum;

		int prevPos = 1;
		int currPos = 1;
		while (1) {
			if (currPos >= (int) expr.size() || expr[currPos] == '+') {
				sum.push_back(
						getExprPtr(expr.substr(prevPos, currPos - prevPos)));
				if (currPos >= (int) expr.size()) {
					break;
				}

				prevPos = ++currPos;
			} else {
				++currPos;
			}
		}

		return new SumV(sum);
	}

	Expr** getExprPtr(const std::string& pos) {
		pair<int, int> rc = parseSprFmt(pos);
		assert(rc.first >= 0 && rc.first < (int)mData.size());
		assert(rc.second >=0 && rc.second < (int)mData[0].size());
		return &(mData[rc.first][rc.second]);
	}

	vector<vector<Expr*> > mData;
};

int main() {
	int nCases;
	cin >> nCases;

	for (int ccase = 1; ccase <= nCases; ++ccase) {
		int nColumns, nRows;
		cin >> nColumns >> nRows;

		Table table(nRows, nColumns);
		for (int i = 0; i < nRows; ++i) {
			for (int j = 0; j < nColumns; ++j) {
				string s;
				cin >> s;
				table.assign(i, j, s);
			}
		}

		for (int i = 0; i < nRows; ++i) {
			for (int j = 0; j < nColumns; ++j) {
				if (j) cout << ' ';
				cout << table.get(i, j);
			}
			cout << endl;
		}
	}

	return 0;
}
