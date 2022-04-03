#include <iostream>
#include <string>
#include <set>
#include "set.hpp"
#include "ft_meta_functions.hpp"
#include "test_common.hpp"
// #include <forward_list> // C++11!!


namespace is_same {
#define VICE_VERSA(type_a, type_b) (DSOUT() << (#type_a " is " #type_b ": ") << NS::is_same<type_a, type_b>::value << std::endl, DSOUT() << (#type_b " is " #type_a ": ") << NS::is_same<type_b, type_a>::value << std::endl)
    void    is_same() {
        VICE_VERSA(int, int);
        VICE_VERSA(int, char);
        VICE_VERSA(int, unsigned int);
        VICE_VERSA(int, long);
        VICE_VERSA(int, int*);
        VICE_VERSA(int, int&);
        VICE_VERSA(int, volatile int);
        VICE_VERSA(int, const int);
        VICE_VERSA(int*, int[]);
    }
#undef VICE_VERSA
}

namespace is_convertible {
    template <class T>
    struct convertible_implicit {
        T   i;
        convertible_implicit(const T& n) {
            i = n;
        }
    };

#define VICE_VERSA(type_a, type_b) (DSOUT() << (#type_a " -> " #type_b ": ") << NS::is_convertible<type_a, type_b>::value << std::endl, DSOUT() << (#type_b " -> " #type_a ": ") << NS::is_convertible<type_b, type_a>::value << std::endl)
    void    is_convertible() {
        VICE_VERSA(int, char);
        VICE_VERSA(int, bool);
        VICE_VERSA(int, std::size_t);
        VICE_VERSA(int, double);
        VICE_VERSA(int, convertible_implicit<int>);
        VICE_VERSA(int, convertible_implicit<bool>);
        VICE_VERSA(int, std::string);
        VICE_VERSA(int, convertible_implicit<std::string>);
        VICE_VERSA(int, const int);
        VICE_VERSA(int, int*);
        VICE_VERSA(int, int**);
        VICE_VERSA(int*, int**);
        // 検出限界
        VICE_VERSA(int, int&);
        VICE_VERSA(int, const int&);
        VICE_VERSA(const int, int&);
        VICE_VERSA(const int*, int*);
        VICE_VERSA(const int*, void*);
        VICE_VERSA(volatile int, int);
        VICE_VERSA(volatile int*, int*);
        VICE_VERSA(volatile int*, void*);
        // 検出限界
        VICE_VERSA(volatile int&, int);
        // 検出限界
        VICE_VERSA(const volatile int&, int);
        VICE_VERSA(const int&, volatile int);
        VICE_VERSA(volatile int&, const int);
        VICE_VERSA(int (*)(void), int);
        VICE_VERSA(int (*)(void), int (*)(void));
        VICE_VERSA(int (*)(void), int (*)(int));
        VICE_VERSA(int (*)(void), long (*)(void));
        VICE_VERSA(int (*)(void), void (*)(void));
        VICE_VERSA(int (*)(int), int (*)(int, int));
        VICE_VERSA(int (*)(int), char);
        VICE_VERSA(char, std::vector<char>);
        VICE_VERSA(NS::vector<int>, NS::vector<char>);
        VICE_VERSA(std::vector<int>, ft::vector<int>);
        VICE_VERSA(char*, std::string);
        VICE_VERSA(const char*, std::string);
        VICE_VERSA(const char*, const std::string);
        VICE_VERSA(const char*, const std::string&);
        VICE_VERSA(int[], int);
        // 検出限界
        VICE_VERSA(int[], int*);
        VICE_VERSA(long[], std::size_t[]);
    }
#undef VICE_VERSA
}

namespace iterator_category {
    template <class Iter>
    void subtest() {
        DSOUT() << "is_input_iterator: " << ft::is_input_iterator< Iter >::value << std::endl;
        DSOUT() << "is_forward_iterator: " << ft::is_forward_iterator< Iter >::value << std::endl;
        DSOUT() << "is_bidirectional_iterator: " << ft::is_bidirectional_iterator< Iter >::value << std::endl;
        DSOUT() << "is_random_access_iterator: " << ft::is_random_access_iterator< Iter >::value << std::endl;
        DSOUT() << "has_iterator_category: " << ft::has_iterator_category< Iter >::value  << std::endl;
        DSOUT() << "std::__has_iterator_category: " << std::__has_iterator_category< Iter >::value  << std::endl;
    }

    void test() {
        std::cout << "[std::vector<int>::iterator]" << std::endl;
        subtest<std::vector<int>::iterator >();
        std::cout << "[ft::vector<int>::iterator]" << std::endl;
        subtest<ft::vector<int>::iterator >();
        std::cout << "[NS::map<std::string, int>::iterator]" << std::endl;
        subtest<NS::map<std::string, int>::iterator >();
        std::cout << "[NS::set<int>::iterator]" << std::endl;
        subtest<NS::set<int>::iterator >();

        // forward_iterator の例だけど C++11!!
        // std::cout << "[std::forward_list<int>::iterator]" << std::endl;
        // subtest<std::forward_list<int>::iterator >();

        std::cout << "[std::istream_iterator<int>]" << std::endl;
        subtest<std::istream_iterator<int> >();
        std::cout << "[std::ostream_iterator<int>]" << std::endl;
        subtest<std::ostream_iterator<int> >();
        std::cout << "[int*]" << std::endl;
        subtest<int* >();
        std::cout << "[int]" << std::endl;
        subtest<int>();
    }
}

int main() {
    is_same::is_same();
    is_convertible::is_convertible();
    iterator_category::test();

    {
        typedef std::vector<int>::iterator Iter;
        std::cout << "[std::vector<int>::iterator]" << std::endl;
        std::cout << "std::__has_iterator_category: " << std::__has_iterator_category< Iter >::value  << std::endl;
        std::cout << "ft::has_iterator_category: " << ft::has_iterator_category< Iter >::value  << std::endl;
        std::cout << "ft::is_random_access_iterator: " << ft::is_random_access_iterator< Iter >::value  << std::endl;
        std::cout << "ft::has_iterator_category_convertible_to: " << ft::has_iterator_category_convertible_to< Iter, std::random_access_iterator_tag >::value  << std::endl;
        std::cout << "ft::has_iterator_category: " << ft::has_iterator_category< Iter >::value  << std::endl;
        std::cout << "std::is_convertible: " << std::is_convertible< Iter::iterator_category, std::random_access_iterator_tag >::value  << std::endl;
        std::cout << "ft::is_convertible: " << ft::is_convertible< Iter::iterator_category, std::random_access_iterator_tag >::value  << std::endl;
    }
    {
        typedef ft::vector<int>::iterator Iter;
        std::cout << "[ft::vector<int>::iterator]" << std::endl;
        std::cout << "std::__has_iterator_category: " << std::__has_iterator_category< Iter >::value  << std::endl;
        std::cout << "ft::has_iterator_category: " << ft::has_iterator_category< Iter >::value  << std::endl;
        std::cout << "ft::is_random_access_iterator: " << ft::is_random_access_iterator< Iter >::value  << std::endl;
        std::cout << "ft::has_iterator_category_convertible_to: " << ft::has_iterator_category_convertible_to< Iter, std::random_access_iterator_tag >::value  << std::endl;
        std::cout << "ft::has_iterator_category: " << ft::has_iterator_category< Iter >::value  << std::endl;
        std::cout << "std::is_convertible: " << std::is_convertible< Iter::iterator_category, std::random_access_iterator_tag >::value  << std::endl;
        std::cout << "ft::is_convertible: " << ft::is_convertible< Iter::iterator_category, std::random_access_iterator_tag >::value  << std::endl;
    }
}
