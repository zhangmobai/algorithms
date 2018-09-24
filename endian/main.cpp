#include <iostream>

using namespace std;

int main(int argc, char **argv) {
	unsigned int i = 1;
	char *p = reinterpret_cast<char *>(&i);
	
	if (*p)
		cout << "Little endian" << endl;
	else
		cout << "Big endian" << endl;


	return 0;
}
