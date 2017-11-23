#include <iostream>
#include <cassert>

using namespace std;

static bool sorted(int *a, size_t n) {
	if (n == 0 || a == NULL) {
		return true;
	}

	for (size_t i = 0; i != n - 1; ++i) {
		if (a[i] > a[i+1]) {
			return false;
		}
	}

	return true;
}

static void print(const char *name, int *a, size_t n) {
	cout << name << ": ";
	for (size_t i = 0; i != n; ++i) {
		cout << a[i] << " ";
	}

	cout << endl;
}

static void
sort(int *a, int n)
{
	if (a == NULL || n <= 1) {
		return;
	}

	int pivot = a[n - 1];
	int i = -1, j = 0;
	for (; j != n - 1; ++j) {
		if (a[j] <= pivot) {
			i++;
			auto tmp = a[i];
			a[i] = a[j];
			a[j] = tmp;
		}
	}
	++i;
	auto tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
	sort(a, i);
	sort(a + i + 1, n - i - 1);
}

int *test1 = NULL;
int test2[] = { 3 };
int test3[] = { 1, 2, 3 };
int test4[] = { 5, 3, 4, 2, 8, 6, 1, 9 };
int test5[] = { 10,9,8,7,6,5,4,3,2,1,0 };
int test6[] = { 5, 3, 4, 4, 8, 8, 1, 9 };
int test7[] = { 5, 3 };

int main (int argc, char **argv) {
	assert(sorted(test1, 0));
	assert(sorted(test2, sizeof(test2)/sizeof(int)));
	assert(sorted(test3, sizeof(test3)/sizeof(int)));
	assert(!sorted(test4, sizeof(test4)/sizeof(int)));
	assert(!sorted(test5, sizeof(test5)/sizeof(int)));
	assert(!sorted(test6, sizeof(test6)/sizeof(int)));
	assert(!sorted(test7, sizeof(test7)/sizeof(int)));

	sort(test1, 0);
	print("test1", test1, 0);
	assert(sorted(test1, 0));

	sort(test2, sizeof(test2)/sizeof(int));
	print("test2", test2, sizeof(test2)/sizeof(int));
	assert(sorted(test2, sizeof(test2)/sizeof(int)));

	sort(test3, sizeof(test3)/sizeof(int));
	print("test3", test3, sizeof(test3)/sizeof(int));
	assert(sorted(test3, sizeof(test3)/sizeof(int)));

	sort(test4, sizeof(test4)/sizeof(int));
	print("test4", test4, sizeof(test4)/sizeof(int));
	assert(sorted(test4, sizeof(test4)/sizeof(int)));

	sort(test5, sizeof(test5)/sizeof(int));
	print("test5", test5, sizeof(test5)/sizeof(int));
	assert(sorted(test5, sizeof(test5)/sizeof(int)));

	sort(test6, sizeof(test6)/sizeof(int));
	print("test6", test6, sizeof(test6)/sizeof(int));
	assert(sorted(test6, sizeof(test6)/sizeof(int)));

	sort(test7, sizeof(test7)/sizeof(int));
	print("test7", test7, sizeof(test7)/sizeof(int));
	assert(sorted(test7, sizeof(test7)/sizeof(int)));

	return 0;
}
