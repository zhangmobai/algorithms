#include <iostream>
#include <cassert>
#include <algorithm>

using namespace std;

void print(const int *a, size_t n) {
	for (size_t i = 0; i != n; ++i) {
		cout << a[i] << " ";
	}
	cout << endl;
}

void swap(int *a, size_t i, size_t j) {
	if (i != j) {
		int tmp = a[i];
		a[i] = a[j];
		a[j] = tmp;
	}
}

void permute_helper(int *a, size_t i, size_t n) {
	if (n - i == 1) {
		print(a, n);
		return;
	}

	for (size_t j = i; j != n; ++j) {
		swap(a, i, j);
		permute_helper(a, i + 1, n);
		swap(a, i, j);
	}
}

void permute(int *a, size_t n) {
	if (a == NULL || n == 0)
		return;

	permute_helper(a, 0, n);
}

int main (int argc, char **argv) {
	int a[] = { 1, 2, 3 };
	permute(a, sizeof(a) / sizeof(a[0]));
	return 0;
}
