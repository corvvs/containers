#include <iostream>
#include "vector.hpp"

int main() {
    ft::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    std::cout << v.back() << std::endl;
}
