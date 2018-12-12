#include <iostream>

#include "vector.h"

using namespace std;

template <typename T>
using myv = mzhang::vector<T>;

struct foo {
	foo(int xx) : x{xx} { }
	friend ostream &operator<<(ostream &os, const foo &f);
	int x;
};

ostream &operator<<(ostream &os, const foo &f) {
	return os << "foo(" << f.x << ")";
}

template <typename T>
void print(const myv<T> &v) {
	cout << "size = " << v.size() << " capactiy = " << v.capacity() << " empty = " << std::boolalpha << v.empty() << endl;
	for (size_t i = 0; i != v.size(); ++i)
		cout << v[i] << " ";
	cout << endl;
}

int main(int argc, char **argv) {
	myv<foo> v;
	print(v);

	for (auto i = 0; i != 10; ++i)
		v.push_back(foo(i));
	print(v);

	myv<foo> vv;
	vv = v;
	print(vv);

	return 0;
}
