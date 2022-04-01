#ifndef PAIR_HPP
# define PAIR_HPP

# include "ft_common.hpp"
# include "ft_meta_functions.hpp"
# include <memory>
# include <iostream>
# include <iterator>
# include <exception>
# include <stdexcept>

namespace ft {
    template <class First, class Second>
    class pair {
        public:
            typedef First   first_type;
            typedef Second  second_type;

            first_type  first;
            second_type second;

        public:
            pair()
                : first(), second() {}
            pair(const pair<First, Second>& other)
                : first(other.first), second(other.second) {}
            pair(const First& f, const Second& s)
                : first(f), second(s) {}
            template <class U, class V>
            pair(const pair<U, V>& other)
                : first(other.first), second(other.second) {}
            ~pair() {}

            pair<First, Second>& operator=(const pair<First, Second> &rhs) {
                first = rhs.first;
                second = rhs.second;
                return *this;
            }

            void    swap(pair<First, Second>& other) {
                ft::swap(first, other.first);
                ft::swap(second, other.second);
            }
    };

    template<class First, class Second>
    pair<First, Second> make_pair(First x, Second y) {
        return pair<First, Second>(x, y);
    }

    template <class First, class Second>
    bool operator==(
        const ft::pair<First, Second>& lhs,
        const ft::pair<First, Second>& rhs
    ) {
        return (lhs.first == rhs.first) && (lhs.second == rhs.second);
    }

    template <class First, class Second>
    bool operator!=(
        const ft::pair<First, Second>& lhs,
        const ft::pair<First, Second>& rhs
    ) {
        return !(lhs == rhs);
    }

    template <class First, class Second>
    bool operator<(
        const ft::pair<First, Second>& lhs,
        const ft::pair<First, Second>& rhs
    ) {
        if (lhs.first == rhs.first) {
            return lhs.second < rhs.second;
        }
        return (lhs.first < rhs.first);
    }

    template <class First, class Second>
    bool operator>(
        const ft::pair<First, Second>& lhs,
        const ft::pair<First, Second>& rhs
    ) {
        return (rhs < lhs);
    }

    template <class First, class Second>
    bool operator<=(
        const ft::pair<First, Second>& lhs,
        const ft::pair<First, Second>& rhs
    ) {
        return !(lhs > rhs);
    }

    template <class First, class Second>
    bool operator>=(
        const ft::pair<First, Second>& lhs,
        const ft::pair<First, Second>& rhs
    ) {
        return !(lhs < rhs);
    }
}

template <class First, class Second>
void    swap(ft::pair<First, Second>& x, ft::pair<First, Second>& y) {
    x.swap(y);
}

#endif
