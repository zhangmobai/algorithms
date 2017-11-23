#include <iostream>
#include <stack>
#include <cassert>

using namespace std;

unsigned char **init(unsigned int n) {
	unsigned char **board = (unsigned char **)calloc(n, sizeof(unsigned char *));
	for (unsigned int i = 0; i != n; ++i) {
		board[i] = (unsigned char *)calloc(n, sizeof(unsigned char));
	}
	return board;
}

void print(unsigned char **board, unsigned int n) {
	for (unsigned int i = 0; i != n; ++i) {
		for (unsigned int  j = 0; j != n; ++j) {
			cout << (board[i][j] ? "X" : "O") << " ";
		}
		cout << "\n";
	}
	cout << endl;
}

bool qualify(unsigned char **board, unsigned int n, unsigned int i, unsigned int j) {
	for (unsigned int l = 0; l < i; ++l) {
		if (board[l][j]) {
			return false;
		}
		for (unsigned int k = 0; k != n; ++k) {
			if (board[l][k] == 0) {
				continue;
			}
			if (j > k && i - l == j - k) {
				return false;
			}
			if (k > j && i - l == k - j) {
				return false;
			}
		}
	}

	return true;
}

unsigned int nqueens(unsigned char **board, unsigned int n, unsigned int m) {
	unsigned int count = 0;
	for (unsigned int j = 0; j != n; ++j) {
		board[m][j] = 1;
		if (qualify(board, n, m, j)) {
			if (m == n - 1) {
				print(board, n);
				++count;
			} else {
				count += nqueens(board, n, m + 1);
			}
		}
		board[m][j] = 0;
	}

	return count;
}

int main (int argc, char **argv) {
	unsigned int n = 4;
	if (argc > 1) {
		n = atoi(argv[1]);
	}

	unsigned char **board = init(n);
	unsigned int count = nqueens(board, n, 0);
	cout << n << " Queens problem solutions: " << count << endl;

	return 0;
}
