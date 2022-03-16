#include "test_common.hpp"

int main() {
    TreeClass<int> tree;
    tree.insert(2);
    tree.insert(1);
    tree.insert(5);
    tree.insert(3);
    tree.insert(6);
    tree.insert(4);
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    TreeClass<int>::iterator it = tree.find(1);
    if (it == tree.end()) {
        std::cout << "end!!" << std::endl;
    } else {
        std::cout << "found!!: " << (*it).value() << std::endl;
    }
    for (it = tree.begin(); it != tree.end(); ++it) {
        std::cout << *((*it).value()) << std::endl;
    }
}
