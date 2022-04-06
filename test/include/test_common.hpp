#ifndef TEST_COMMON_HPP
# define TEST_COMMON_HPP

# include <iostream>
# include <string>
# include <cstdlib>
# include "ft_common.hpp"
# include "vector.hpp"
# include "noncopyable.hpp"
# include <vector>
# include <deque>
# include <list>

# include "pair.hpp"
# ifdef USE_STL
#  include <vector>
#  include <stack>
#  include <map>
#  include <set>
#  include "tree.hpp"
#  define StackContainer std::deque
#  define NS std
# else
#  include "vector.hpp"
#  include "stack.hpp"
#  include "pair.hpp"
#  include "tree.hpp"
#  include "map.hpp"
#  include "set.hpp"
#  define StackContainer ft::vector
#  define NS ft
# endif
# define TreeClass   ft::tree
# define VectorClass NS::vector
# define StackClass  NS::stack
# define PairClass   NS::pair
# define MapClass    NS::map
# define SetClass    NS::set
# define DSOUT() debug_out(__FILE__, __LINE__)
# define DOUT()  debug_err(__FILE__, __LINE__)

# include "sprint.hpp"
# include "IntWrapper.hpp"

template <class T>
struct simple_holder {
    T   val_;

    simple_holder(T val = T()) FT_NOEXCEPT: val_(val) {}
    simple_holder(const simple_holder& other) FT_NOEXCEPT {
        *this = other;
    }
    ~simple_holder() {}
    // ↓これをコメントアウトすると std::is_trivially_copy_assignable となる.
    inline simple_holder&   operator=(const simple_holder& rhs) FT_NOEXCEPT {
        val_ = rhs.val_;
        return *this;
    }
};

template<class T>
T                   random_value_generator();
template<>
char                random_value_generator();
template<>
int                 random_value_generator();
template<>
long                random_value_generator();
template<>
double              random_value_generator();
template<>
simple_holder<char> random_value_generator();
template<>
std::string         random_value_generator();
template<>
ft::IntWrapper      random_value_generator();
template<>
std::vector<int>    random_value_generator();
template<>
ft::vector<int>     random_value_generator();

std::ostream&   debug_out(
    const char *filename,
    const int linenumber
);

std::ostream&   debug_err(
    const char *filename,
    const int linenumber
);

// [[ << overloads ]]
// [declarations]

template <class T, class Allocator>
std::ostream& operator<<(
    std::ostream& stream,
    const std::vector<T, Allocator>& value
);

template <class T, class Allocator>
std::ostream& operator<<(
    std::ostream& stream,
    const ft::vector<T, Allocator>& value
);

template <class Key, class KeyComparator, class KeyAllocator>
std::ostream& operator<<(
    std::ostream& stream,
    const NS::set<Key, KeyComparator, KeyAllocator>& value
);

template <class Key, class Value, class KeyComparator, class PairAllocator>
std::ostream& operator<<(
    std::ostream& stream,
    const NS::map<Key, Value, KeyComparator, PairAllocator>& value
);

template <class First, class Second>
std::ostream& operator<<(
    std::ostream& stream,
    const NS::pair<First, Second>& value
);

// [implementations]

template <class T, class Allocator>
std::ostream& operator<<(
    std::ostream& stream,
    const std::vector<T, Allocator>& value
) {
    stream << "[";
    for (typename std::vector<T, Allocator>::const_iterator it = value.begin(); it != value.end(); ++it) {
        if (it != value.begin()) {
            stream << ", ";
        }
        stream << *it;
    }
    stream << "]";
    return stream;
}

template <class T, class Allocator>
std::ostream& operator<<(
    std::ostream& stream,
    const ft::vector<T, Allocator>& value
) {
    stream << "[";
    for (typename ft::vector<T, Allocator>::const_iterator it = value.begin(); it != value.end(); ++it) {
        if (it != value.begin()) {
            stream << ", ";
        }
        stream << *it;
    }
    stream << "]";
    return stream;
}

template <class Key, class KeyComparator, class KeyAllocator>
std::ostream& operator<<(
    std::ostream& stream,
    const NS::set<Key, KeyComparator, KeyAllocator>& value
) {
    typedef NS::set<Key, KeyComparator, KeyAllocator>   c_type;
    stream << "(";
    for (typename c_type::const_iterator it = value.begin(); it != value.end(); ++it) {
        if (it != value.begin()) {
            stream << ", ";
        }
        stream << *it;
    }
    stream << ")";
    return stream;
}


template <class Key, class Value, class KeyComparator, class PairAllocator>
std::ostream& operator<<(
    std::ostream& stream,
    const NS::map<Key, Value, KeyComparator, PairAllocator>& value
) {
    typedef NS::map<Key, Value, KeyComparator, PairAllocator>   c_type;
    stream << "{";
    for (typename c_type::const_iterator it = value.begin(); it != value.end(); ++it) {
        if (it != value.begin()) {
            stream << ", ";
        }
        stream << "(" << it->first << " => " << it->second << ")";
    }
    stream << "}";
    return stream;
}

template <class First, class Second>
std::ostream& operator<<(
    std::ostream& stream,
    const NS::pair<First, Second>& value
) {
    stream << "(" << value.first << ", " << value.second << ")";
    return stream;
}

template <class T>
std::ostream& operator<<(
    std::ostream& stream,
    const simple_holder<T>& value
) {
    stream << "[" << value.val_ << "]";
    return stream;
}

// [[print funcs]]

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
