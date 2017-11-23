#include <iostream>
#include <cassert>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

static const unsigned int ALPHABET_SIZE = 256;

static int
boyer_moore (const char *string, uint32_t stringlen, const char *pat, uint32_t patlen) {
	if (stringlen < patlen) {
		return -1;
	}

	if (patlen == 0) {
		return -1;
	}

	unsigned int delta1[ALPHABET_SIZE];
	for (unsigned int i = 0; i != ALPHABET_SIZE; ++i) {
		delta1[i] = patlen;
	}

	for (unsigned int i = 0; i != patlen; ++i) {
		delta1[(unsigned char)pat[i]] = patlen - 1 - i;
	}

	vector<unsigned int> delta2(patlen - 1);

	/*
	 * How to build delta2 table in O(n) time?
	 */
	for (int i = (int)patlen - 2; i > 0; --i) {
		for (int j = patlen - 2; j >= (int)patlen - 2 - i; --j) {
			unsigned int k = 0;
			for (; k != patlen - 1 - i; ++k)
				if (pat[patlen - 1 - k] != pat[j - k])
					break;

			if (k != patlen - 1 - i)
				continue;

			if (j == (int)patlen -2 - i || pat[j - 1] != pat[i]) {
				delta2[i] = patlen * 2 - j - i - 2;
				break;
			}
		}
	}

	for (int i = patlen - 1; i < (int)stringlen;) {
		int j = patlen - 1;
		if (string[i] != pat[j]) {
			i += delta1[(unsigned char)string[i]];
			continue;
		}

		while (string[--i] == pat[--j])
			;

		if (j == -1)
			return i + 1;

		i += max(delta2[j], patlen - 1 - delta1[(unsigned char)string[i]]);
	}

	return -1;
}

int
main (int argc, char **argv)
{
	const char *s = "which finally halts, at that point";
	const char *p = "at that";
	int index = boyer_moore(s, strlen(s), p, strlen(p));
	cout << "index: " << index << endl;
	cout << s + index << endl;
	return 0;
}
