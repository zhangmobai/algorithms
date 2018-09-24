#include <iostream>
#include <bitset>


using namespace std;

template<typename T, unsigned int N, unsigned int M>
class BloomFilter {
public:
	void add(const T &k) {
	}

	bool mayContain(const T &k) {
		return true;
	}
private:
	bitset<N> set;
};

int main(int argc, char **argv) {
	return 0;
}
