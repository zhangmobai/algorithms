#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print(const vector<int> &v) {
	for (auto &n : v)
		cout << n << " ";

	cout << endl;
}

void perm(vector<int> &v) {
	auto n = v.size();
	while (true) {
		print(v);
		int j = n - 2, k = n - 1;
		while (j > -1 && v[j] >= v[j + 1])
			j--;

		if (j == -1)
			break;

		while (v[k] <= v[j])
			--k;

		swap(v[k], v[j]);
		++j;
		k = n - 1;
		while (j < k) {
			swap(v[j], v[k]);
			++j;
			--k;
		}
	}
}

int main(int argc, char **argv) {
	vector<int> v{1, 1, 3};
	sort(v.begin(), v.end());
	perm(v);
	return 0;
}
