#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

void print(const vector<int> &a) {
	for_each(a.begin(), a.end(), [](const int &v) { cout << v << " "; });
	cout << endl;
}

void plain_changes(vector<int> &a) {
	vector<int> c;
	vector<int> o;
	for (auto i = 0U; i != a.size(); ++i) {
		c.push_back(0);
		o.push_back(1);
	}

	for (;;) {
		cout << "c: ";
		print(c);
		cout << "o: ";
		print(o);

		cout << "a: ";
		print(a);

		int j = a.size() - 1;
		int s = 0;

step4:
		int q = c[j] + o[j];
		if (q < 0) {
			o[j] = -o[j];
			j--;
			goto step4;
		} else if (q == j + 1) {
			if (j == 1) {
				break;
			} else {
				s++;
				o[j] = -o[j];
				j--;
				goto step4;
			}
		}

		auto tmp = a[j - c[j] + s];
		a[j - c[j] + s] = a[j - q + s];
		a[j -q + s] = tmp;

		c[j] = q;
	}
}

int main (int argc, char **argv) {
	vector<int> a{ 1, 2, 3 ,4 };
	plain_changes(a);
	return 0;
}
