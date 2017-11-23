#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>

using namespace std;

void print(const vector<unsigned int> &a) {
	for_each(a.begin(), a.end(), [](const unsigned int &v) { cout << v; });
	cout << endl;
}

void print_binary(unsigned int n) {
	for (unsigned int i = 0; i != 8; ++i) {
		cout << ((n >> i) & 1);
	}
}

void gray_code(unsigned int n) {
	vector<unsigned int> a;
	vector<unsigned int> f;
	for (unsigned int i = 0; i != n; ++i) {
		f.push_back(i);
		a.push_back(0);
	}
	f.push_back(n);

	unsigned int i = 0;
	(void)i;
	for(;;) {
		cout << " f: ";
		print(f);
		cout << "a: ";
		print(a);

		auto j = f[0];
		if (j == n) {
			break;
		}

#if 1
		cout << "i: ";
		print_binary(++i);
#endif

		f[0] = 0;

		f[j] = f[j + 1];
		f[j + 1] = j + 1;

		a[j] = 1 - a[j];

	}
}

int main (int argc, char **argv) {
	gray_code(4);
	return 0;
}
