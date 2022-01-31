#include "test_vector.hpp"

int main() {
	
	VectorClass<int>	vi;

	std::cout << "[default constructor]" << std::endl;
	std::cout << "capacity:     " << vi.capacity() << std::endl;
	std::cout << "size:         " << vi.size() << std::endl;
	std::cout << "max_size:     " << vi.max_size() << std::endl;
	std::cout << "empty:        " << vi.empty() << std::endl;
	// std::cout << "begin == end: " << (vi.begin() == vi.end()) << std::endl;
}
