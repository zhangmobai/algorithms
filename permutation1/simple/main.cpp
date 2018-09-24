#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print(const vector<int> &v) {
	for (auto &n : v)
		cout << n << " ";

	cout << endl;
}

void perm(vector<int> &v, vector<int>::size_type start = 0) {
	if (start == v.size() - 1) {
		print(v);
		return;
	}
			
	for (vector<int>::size_type i = start; i != v.size(); ++i) {
		swap(v[start], v[i]);
		perm(v, start + 1);
		swap(v[start], v[i]);
	}
}

int main(int argc, char **argv) {
	vector<int> v{1, 2, 3};
	sort(v.begin(), v.end());
	perm(v);
	return 0;
}
