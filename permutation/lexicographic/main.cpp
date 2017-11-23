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

void permute(int *a, size_t n) {
	if (a == NULL || n == 0)
		return;
	if (n == 1) {
		print(a, n);
		return;
	}

	sort(a, a + n);

	for (;;) {
		print(a, n);

		int j = n - 2;
		while (a[j] >= a[j + 1])
			--j;

		if (j == -1)
			break;

		int k = n - 1;
		while (a[k] <= a[j])
			--k;
		int tmp = a[j];
		a[j] = a[k];
		a[k] = tmp;
		
		j++;
		k = n - 1;
		while (j < k) {
			tmp = a[j];
			a[j] = a[k];
			a[k] = tmp;
			++j;
			--k;
		}
	}
}

int main (int argc, char **argv) {
	int a[] = { 1, 3, 2, 2 };
	permute(a, sizeof(a) / sizeof(a[0]));
	return 0;
}
