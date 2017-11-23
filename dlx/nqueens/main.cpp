#include <iostream>
#include <stack>
#include <cassert>

using namespace std;

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
