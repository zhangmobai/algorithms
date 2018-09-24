#include <iostream>
#include <string>
#include <deque>
#include <stack>
#include <map>

using namespace std;

bool is_operator(char c) {
	switch (c) {
	case '+':
	case '-':
	case 'x':
	case '/':
		return true;

	default:
		return false;
	}
}

void rpn(const string &in, map<string, int> &pre) {
	deque<string> oprand;
	stack<string> op;
	for (string::size_type i = 0; i != in.size();) {
		if (in[i] == ' ') {
			++i;
			continue;
		}

		string o;
		if (in[i] >= '0' && in[i] <= '9') {
			do {
				o.push_back(in[i]);
				++i;
			} while (i != in.size() && in[i] >= '0' && in[i] <= '9');
			oprand.push_back(o);
			continue;
		}

		if (in[i] == '(') {
			o.push_back(in[i]);
			op.push(o);
			++i;
			continue;
		}

		if (in[i] == ')') {
			while (!op.empty() && op.top() != "(") {
				oprand.push_back(op.top());
				op.pop();
			}

			if (op.empty()) {
				cout << "Invalid parenthese!" << endl;
				return;
			}

			op.pop();
			++i;
			continue;
		}

		if (!is_operator(in[i])) {
			cout << "Unrecognized character " << in[i] << endl;
			return;
		}
		
		do {
			o.push_back(in[i]);
			++i;
		} while (i != in.size() && is_operator(in[i]));
		
		if (op.empty() || pre[op.top()] <= pre[o])
			op.push(o);
		else {
			oprand.push_back(op.top());
			op.pop();
			op.push(o);
		}
	}

	while (!op.empty()) {
		oprand.push_back(op.top());
		op.pop();
	}

	for (auto &s : oprand)
		cout << s << " ";
	cout << endl;
}

int main(int argc, char **argv) {
	map<string, int> pre;
	pre["+"] = 1;
	pre["-"] = 1;
	pre["x"] = 2;
	pre["/"] = 2;

	//string infix{"2 + 3 x 4 - 5"};
	string infix{"3 + 4 x (2 - 1)"};
	rpn(infix, pre);
	return 0;
}
