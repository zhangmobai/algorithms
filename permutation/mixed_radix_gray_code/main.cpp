#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>

using namespace std;

void print(const vector<unsigned int> &a) {
	for_each(a.begin(), a.end(), [](const unsigned int &v) { cout << v; });
	cout << endl;
}

void gray_code(const vector<unsigned int> &radixes) {
	vector<unsigned int> a;
	vector<unsigned int> f;
	vector<int> o;

	for (unsigned int i = 0; i != radixes.size(); ++i) {
		a.push_back(0);
		f.push_back(i);
		o.push_back(1);
	}
	f.push_back(radixes.size());

	for (;;) {
		cout << "a: ";
		print(a);

		auto j = f[0];
		if (j == radixes.size()) {
			break;
		}

		f[0] = 0;

		a[j] = a[j] + o[j];

		if (a[j] == radixes[j] - 1 || a[j] == 0) {
			o[j] = -o[j];
			f[j] = f[j + 1];
			f[j + 1] = j + 1;
		}
	}
}

int main (int argc, char **argv) {
	gray_code({ 2, 3, 4 });
	return 0;
}
