#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <stack>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cassert>
#include <climits>

using namespace std;

enum label {
	END = -1,
	MATCHED = (1 << CHAR_BIT),
	SPLIT,
	CAT
};

struct NFAState {
	int c;
	NFAState *out;
	NFAState *out1;
};

struct DFAState {
	set<NFAState *> nfastates;
	map<char, DFAState *> next;
};

struct RegComp {
	RegComp(const char *r) : re(r), index(-1), token(-1), start(NULL), d(NULL) {
		start = parse();
	}

	/*
	 * regexp: regexp OR branch
	 *         branch
	 * branch: branch closure
	 *         closure
	 * closure: closure +
	 *          closure ?
	 *          closure *
	 *          atom
	 * atom: normal_char
	 *       .
	 *       ( regexp )
	 *       empty
	 */
	NFAState *parse(void) {
		lex();
		regexp();
		return NULL;
	}

	void regexp(void) {
		branch();
		while (token == '|') {
			lex();
			branch();
			addToken('|');
		}
	}

	void branch(void) {
		closure();
		while (token != '|' && token != END) {
			lex();
			closure();
			addToken(CAT);
		}
	}

	int lex(void) {
		return -1;
	}
	
	void addToken(int t) {
	}

	string re;
	int index;
	int token;
	NFAState *start;
	DFAState *d;
};

static bool
match(const char *s, RegComp &re)
{
	return true;
}

int main (int argc, char **argv) {
	const char *s = "abbbba";
	RegComp re("a(bb)+a");

	if (match(s, re))
		cout << "Matched" << endl;
	else
		cout << "Not Matched" << endl;

	return 0;
}
