#include <iostream>
#include <type_traits>

using namespace std;

class Foo {
public:
	int x{0};
};

class B {
public:
};

int main(int argc, char **argv) {
	cout << std::boolalpha << is_trivially_copyable<Foo>::value << endl;
	cout << std::boolalpha << is_trivially_copyable<B>::value << endl;
	return 0;
}
