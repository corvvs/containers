#include "test_common.hpp"

template <class U, class V>
void    print_pair(const PairClass<U, V>& p)
{
    std::cout << p.first << ", " << p.second << std::endl;
}

int main() {
    PairClass<int, int> pair1(10, 20);
    print_pair(pair1);
    PairClass<std::string, double> pair2("hello 42", 42);
    print_pair(pair2);
    PairClass<std::string, double> pair3 = pair2;
    print_pair(pair3);
    PairClass<std::string, double> pair4;
    print_pair(pair4);
    pair4 = pair3;
    print_pair(pair4);

    std::cout << (pair3 == pair4) << std::endl;

    PairClass<double, unsigned int> pair5(pair1.first, pair1.second);
    print_pair(pair5);

    PairClass<const std::string, double> pair6 = pair4;
    PairClass<const std::string, double> pair7 = pair6;
    std::cout << (pair6 == pair7) << std::endl;
    std::cout << std::equal_to< PairClass<const std::string, double> >()(pair6, pair7) << std::endl;
}
