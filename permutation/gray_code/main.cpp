#include <iostream>
#include <cassert>
#include <algorithm>

using namespace std;

void generate_gray_code(size_t n) {
	for (int i = 0; i != (1 << n); ++i) {
		size_t m = i ^ (i >> 1);
		for (int j = n - 1; j >= 0; --j) {
			if ((1 << j) & m)
				cout << "1";
			else
				cout << "0";
		}
		cout << endl;
	}
}

int main (int argc, char **argv) {
	generate_gray_code(3);
	return 0;
}
