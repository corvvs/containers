#include "test_common.hpp"

void    insert_and_erase_random(int n) {
    VectorClass<int> ns;
    for (int i = 0; i < n; ++i) {
        int k = rand() % (100 * n);
        ns.push_back(k);
    }
    for (int i = 0; i < 10 * n; ++i) {
        int j = rand() % ns.size();
        int k = rand() % ns.size();
        std::swap(ns[j], ns[k]);
    }
    TreeClass<int>  tree;
    for (int i = 0; i < n; ++i) {
        int k = ns[i];
        DOUT() << "insert: " << k << std::endl;
        tree.insert(k);
        // print_iterative_container_elements(tree);
    }
    print_iterative_container_elements(tree);
    for (VectorClass<int>::size_type i = 0; i < ns.size(); ++i) {
        // print_iterative_container_elements(tree);
        TreeClass<int>::size_type lh = tree.debug_longest_height();
        TreeClass<int>::size_type sh = tree.debug_shortest_height();
        TreeClass<int>::size_type lb = tree.debug_longest_black_height();
        TreeClass<int>::size_type sb = tree.debug_shortest_black_height();
        if (lh > sh * 2) {
            DOUT() << "longest  height: " << tree.debug_longest_height() << std::endl;
            DOUT() << "shortest height: " << tree.debug_shortest_height() << std::endl;
        }
        if (sb != lb) {
            DOUT() << "longest  b-height: " << tree.debug_shortest_black_height() << std::endl;
            DOUT() << "shortest b-height: " << tree.debug_longest_black_height() << std::endl;
        }
        DOUT() << "erase: " << ns[i] << std::endl;
        tree.erase(ns[i]);
    }
    DOUT() << "size: " << tree.size() << std::endl;
    // for (TreeClass<int>::iterator it = tree.begin(); it != tree.end();) {
    //     // print_iterative_container_elements(tree);
    //     TreeClass<int>::size_type lh = tree.debug_longest_height();
    //     TreeClass<int>::size_type sh = tree.debug_shortest_height();
    //     TreeClass<int>::size_type lb = tree.debug_longest_black_height();
    //     TreeClass<int>::size_type sb = tree.debug_shortest_black_height();
    //     if (sh * 2 > lh) {
    //         DOUT() << "longest  height: " << tree.debug_longest_height() << std::endl;
    //         DOUT() << "shortest height: " << tree.debug_shortest_height() << std::endl;
    //     }
    //     if (sb != lb) {
    //         DOUT() << "longest  b-height: " << tree.debug_shortest_black_height() << std::endl;
    //         DOUT() << "shortest b-height: " << tree.debug_longest_black_height() << std::endl;
    //     }
    //     DOUT() << "it: " << &*it << std::endl;
    //     tree.erase(it++);
    // }
    // print_iterative_container_elements(tree);
}

void    insertion_with_hint(int n, int key) {
    TreeClass<int> tree;
    for (int i = 0; i < n; ++i) {
        tree.insert(i < key ? i : i + 1);
    }
    print_iterative_container_elements(tree);

    TreeClass<int>::iterator hint;

    DOUT() << "#1" << std::endl;
    hint = tree.lower_bound(key);
    DOUT() << *hint << std::endl;
    tree.insert(hint, key);
    print_iterative_container_elements(tree);
    DOUT() << tree.size() << std::endl;

    DOUT() << "#2" << std::endl;
    hint = tree.begin();
    DOUT() << *hint << std::endl;
    tree.insert(hint, n + 1);
    print_iterative_container_elements(tree);
    DOUT() << tree.size() << std::endl;

    DOUT() << "#3" << std::endl;
    hint = tree.end();
    DOUT() << *hint << std::endl;
    tree.insert(hint, -1000);
    print_iterative_container_elements(tree);
    DOUT() << tree.size() << std::endl;

    DOUT() << "#4" << std::endl;
    hint = tree.end();
    DOUT() << *hint << std::endl;
    tree.insert(hint, -1);
    print_iterative_container_elements(tree);
    DOUT() << tree.size() << std::endl;
}

void    traverse_by_iterator(int n) {
    TreeClass<int> tree;
    for (int i = 0; i < n; ++i) {
        tree.insert(rand() % (n * 10));
    }
    {
        SPRINT("traverse by normal iterator") << "(" << n << ")";
        TreeClass<int>::iterator it = tree.begin();
        std::cout << "[normal-traverse by normal iterator]" << std::endl;
        for (; it != tree.end(); ++it) {
            std::cout << *(it->value()) << " ";
        }
        std::cout << std::endl;
        std::cout << "[reversal-traverse by normal iterator]" << std::endl;
        for (;it != tree.begin();) {
            --it;
            std::cout << *(it->value()) << " ";
        }
        std::cout << std::endl;
    }
    {
        SPRINT("traverse by reverse iterator") << "(" << n << ")";
        TreeClass<int>::reverse_iterator it = tree.rbegin();
        std::cout << "[reversal-traverse by reverse iterator]" << std::endl;
        for (; it != tree.rend(); ++it) {
            std::cout << *(it->value()) << " ";
        }
        std::cout << std::endl;
        std::cout << "[normal-traverse by normal iterator]" << std::endl;
        for (;it != tree.rbegin();) {
            --it;
            std::cout << *(it->value()) << " ";
        }
        std::cout << std::endl;
    }
}

void    compare_iterators()
{
    // "constでないiterator"と"const_iterator"が比較できることを確かめる.
    ft::tree<int> v;
    v.insert(1);
    ft::tree<int>::const_iterator	cit = v.begin();
    ft::tree<int>::iterator 		vit = v.begin();
    std::cout << (vit == cit) << std::endl;
    ++vit;
    std::cout << (vit == cit) << std::endl;
    ++cit;
    std::cout << (vit == cit) << std::endl;
}

int main() {
    compare_iterators();
    insert_and_erase_random(100);
    insertion_with_hint(1000, 27);
    traverse_by_iterator(10);

    VectorClass<int> v(10);
    for (VectorClass<int>::size_type i = 0; i < v.size(); ++i) {
        v[i] = i * 5;
    }

    TreeClass<int> tree;
    std::cout << "empty: " << tree.empty() << ", size: " << tree.size() << ", max_size: " << tree.max_size() << std::endl;
    tree.insert(tree.end(), 2);
    tree.insert(tree.begin(), 1);
    tree.insert(tree.end(), 5);
    tree.insert(3);
    tree.insert(6);
    tree.insert(4);
    tree.insert(v.begin(), v.end());
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
    // it = tree.insert(it, 10);
    // it = tree.insert(it, 9);
    // it = tree.insert(it, 11);
    // TreeClass<int>::const_iterator cit = tree.find(5);
    // if (cit == tree.end()) {
    //     std::cout << "end!!" << std::endl;
    // } else {
    //     std::cout << "found!!: " << *(*cit).value() << std::endl;
    // }
    // for (it = tree.find(10); it != tree.end(); ++it) {
    //     std::cout << *((*it).value()) << std::endl;
    // }
    // for (TreeClass<int>::reverse_iterator rit = tree.rbegin(); rit != tree.rend(); ++rit) {
    //     std::cout << *((*rit).value()) << std::endl;
    // }


    // PairClass<TreeClass<int>::iterator, TreeClass<int>::iterator> range = tree.equal_range(1);
    // std::cout << "[" << *((*range.first).value()) << ", " << *((*range.second).value()) << ")"  << std::endl;
    // range = tree.equal_range(5);
    // std::cout << "[" << *((*range.first).value()) << ", " << *((*range.second).value()) << ")"  << std::endl;
    // range = tree.equal_range(-100);
    // std::cout << "[" << *((*range.first).value()) << ", " << *((*range.second).value()) << ")"  << std::endl;

    // std::cout << "count: " << tree.count(1) << std::endl;
    // std::cout << "count: " << tree.count(100) << std::endl;
    // std::cout << "count: " << tree.count(-100) << std::endl;

    // tree.erase(tree.find(10));
    // for (TreeClass<int>::iterator rit = tree.begin(); rit != tree.end(); ++rit) {
    //     std::cout << *((*rit).value()) << std::endl;
    // }
    // while (tree.size() > 0) {
    //     std::cout << "size is " << tree.size() << std::endl;
    //     tree.erase(tree.begin());
    // }
    ft::sprint::list();
}
