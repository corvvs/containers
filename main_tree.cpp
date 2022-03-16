#include "test_common.hpp"

int main() {
    TreeClass<int> tree;
    std::cout << "empty: " << tree.empty() << ", size: " << tree.size() << ", max_size: " << tree.max_size() << std::endl;
    tree.insert(tree.end(), 2);
    tree.insert(tree.begin(), 1);
    tree.insert(tree.end(), 5);
    tree.insert(3);
    tree.insert(6);
    tree.insert(4);
    std::cout << "empty: " << tree.empty() << ", size: " << tree.size() << ", max_size: " << tree.max_size() << std::endl;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    std::cout << "empty: " << tree.empty() << ", size: " << tree.size() << ", max_size: " << tree.max_size() << std::endl;
    TreeClass<int>::iterator it = tree.find(1);
    if (it == tree.end()) {
        std::cout << "end!!" << std::endl;
    } else {
        std::cout << "found!!: " << *(*it).value() << std::endl;
    }
    it = tree.insert(it, 10);
    it = tree.insert(it, 9);
    it = tree.insert(it, 11);
    TreeClass<int>::const_iterator cit = tree.find(5);
    if (cit == tree.end()) {
        std::cout << "end!!" << std::endl;
    } else {
        std::cout << "found!!: " << *(*cit).value() << std::endl;
    }
    for (it = tree.begin(); it != tree.end(); ++it) {
        std::cout << *((*it).value()) << std::endl;
    }
    for (TreeClass<int>::reverse_iterator rit = tree.rbegin(); rit != tree.rend(); ++rit) {
        std::cout << *((*rit).value()) << std::endl;
    }


    PairClass<TreeClass<int>::iterator, TreeClass<int>::iterator> range = tree.equal_range(1);
    std::cout << "[" << *((*range.first).value()) << ", " << *((*range.second).value()) << ")"  << std::endl;
    range = tree.equal_range(5);
    std::cout << "[" << *((*range.first).value()) << ", " << *((*range.second).value()) << ")"  << std::endl;
    range = tree.equal_range(-100);
    std::cout << "[" << *((*range.first).value()) << ", " << *((*range.second).value()) << ")"  << std::endl;

    std::cout << "count: " << tree.count(1) << std::endl;
    std::cout << "count: " << tree.count(100) << std::endl;
    std::cout << "count: " << tree.count(-100) << std::endl;
}
