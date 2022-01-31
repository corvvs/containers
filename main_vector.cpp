#include "test_vector.hpp"

template<class T>
void	print_stats(VectorClass<T> &v) {
	std::cout << "capacity:     " << v.capacity() << std::endl;
	std::cout << "size:         " << v.size() << std::endl;
	std::cout << "max_size:     " << v.max_size() << std::endl;
	std::cout << "empty:        " << v.empty() << std::endl;
}


int main() {
	
	VectorClass<int>	vi;
	VectorClass<int>	vj;

	std::cout << "[default constructor]" << std::endl;
	print_stats(vi);
	std::cout << "done" << std::endl;
	vi.push_back(1);
	vi.push_back(2);
	vi.push_back(3);
	vi.push_back(4);
	vi.push_back(5);
	vi.push_back(6);
	vi.push_back(7);
	vi.push_back(8);
	print_stats(vi);
	vi = vj;
	print_stats(vj);
}
