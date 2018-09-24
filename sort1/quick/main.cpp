#include <iostream>
#include <vector>
#include <utility>

using namespace std;

void print(const vector<int> &v) {
	for (auto i: v)
		cout << i << " ";

	cout << endl;
}

void quick_sort(vector<int> &v, vector<int>::iterator start, vector<int>::iterator end) {
	if (end - start <= 1)
		return;

	vector<int>::iterator p = start + 1;
	vector<int>::iterator q = start;
	auto &key = *start;

	for (; p != end; ++p) {
		if (*p <= key) {
			++q;
			swap(*p, *q);
		}
	}

	swap(key, *q);
	quick_sort(v, start, q);
	quick_sort(v, q + 1, end);
}

int main(int argc, char **argv) {
	vector<int> v{3, 4, 1, 9, 5, 3, 7, 2};
	//vector<int> v{3, 3, 3, 3, 3, 3};
	quick_sort(v, v.begin(), v.end());
	print(v);
	return 0;
}
