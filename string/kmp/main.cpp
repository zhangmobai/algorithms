#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <stack>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>

using namespace std;

int find(const char *s, const char *p) {
	unsigned int i = 0;
	vector<int> q;
	for (; i != strlen(p); ++i) {
		q.push_back(0);
		if (i == 0) {
			continue;
		}

		unsigned int j = q[i - 1];
		while (j != 0 && p[j] != p[i]) {
			j = q[j];
		}

		if (p[i] == p[j]) {
			q[i] = j + 1;
		}
	}

	i = 0;
	for (unsigned int j = 0; j < strlen(p) && i < strlen(s);) {
		while (j < strlen(p) && i + j < strlen(s) && s[i + j] == p[j])
			++j;

		if (j == strlen(p)) {
			return i;
		}

		if (j == 0)
			i++;
		else
			i += (j - q[j - 1]);
	}

	return -1;
}

int main (int argc, char **argv) {
	const char *s = "ABC ABCDAB ABCDABCDABDE";
	const char *p = "ABCDABD";

	int i = find(s, p);
	if (i == -1) {
		cout << "Not Found" << endl;
	} else {
		cout << "Index " << i << ": " << s + i << endl;
	}

	return 0;
}
