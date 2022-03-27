#ifndef TEST_VECTOR_HPP
# define TEST_VECTOR_HPP

# include <iostream>
# include <string>
# include <cstdlib>
# include "ft_common.hpp"
# include "vector.hpp"
# include <vector>
# include <deque>
# include <list>

# include "pair.hpp"
# ifdef USE_STL
#  include <vector>
#  include <stack>
#  include <map>
#  include "tree.hpp"
#  define VectorClass std::vector
#  define StackClass std::stack
#  define PairClass std::pair
#  define TreeClass ft::tree
#  define MapClass std::map
#  define NS std
// #  define TreeClass std::__tree
# else
#  include "vector.hpp"
#  include "stack.hpp"
#  include "pair.hpp"
#  include "tree.hpp"
#  include "map.hpp"
#  define VectorClass ft::vector
#  define StackClass ft::stack
#  define PairClass ft::pair
#  define TreeClass ft::tree
#  define MapClass ft::map
#  define NS ft
# endif

# include "sprint.hpp"
# include "IntWrapper.hpp"

template<class T>
T                   random_value_generator();
template<>
int                 random_value_generator();
template<>
std::string         random_value_generator();
template<>
ft::IntWrapper      random_value_generator();
template<>
std::vector<int>    random_value_generator();
template<>
ft::vector<int>     random_value_generator();

template <class T>
std::ostream& operator<<(std::ostream& stream, std::vector<T>& value) {
    stream << "[";
    for (typename std::vector<T>::iterator it = value.begin(); it != value.end(); ++it) {
        if (it != value.begin()) {
            stream << ", ";
        }
        stream << *it;
    }
    stream << "]";
    return stream;
}

template <class T>
std::ostream& operator<<(std::ostream& stream, ft::vector<T>& value) {
    stream << "[";
    for (typename ft::vector<T>::iterator it = value.begin(); it != value.end(); ++it) {
        if (it != value.begin()) {
            stream << ", ";
        }
        stream << *it;
    }
    stream << "]";
    return stream;
}

template <class Iter>
void    print_vector_elements(Iter from, Iter to) {
    std::cout << "[";
    for (Iter it = from; it != to; ++it) {
        if (it != from) {
            std::cout << ", ";
        }
        std::cout << *it;
    }
    std::cout << "]" << std::endl;
}

template <class Container>
void    print_iterative_container_elements(Container& container) {
    DOUT() << &*(container.begin()) << std::endl;
    std::cout << "[";
    for (typename Container::iterator it = container.begin(); it != container.end(); ++it) {
        if (it != container.begin()) {
            std::cout << ", ";
        }
        // std::cout << *it;
        std::cout << *(it->value());
    }
    std::cout << "]" << std::endl;
}

#endif
