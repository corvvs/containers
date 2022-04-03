#ifndef STACK_HPP
# define STACK_HPP

# include "ft_common.hpp"
# include "ft_meta_functions.hpp"
# include "vector.hpp"
# include <memory>

namespace ft {
    template< class T, class Container = ft::vector<T> >
    class stack {
        public:
            typedef Container                           container_type;
            typedef typename Container::value_type      value_type;
            typedef typename Container::size_type       size_type;
            typedef typename Container::reference       reference;
            typedef typename Container::const_reference const_reference;

            explicit stack(const Container& cont = Container())
                : c(cont) {}

            stack(const stack& other)
                : c(other.c) {}

            ~stack() {}

            inline stack& operator=(const stack& other) {
                if (this != &other) { c = other.c; }
                return *this;
            }

            inline reference top() {
                return c.back();
            }

            inline const_reference top() const {
                return top();
            }

            inline bool empty() const {
                return c.empty();
            }

            inline size_type size() const {
                return c.size();
            }

            inline void push(const value_type& value) {
                c.push_back(value);
            }

            inline void pop() {
                c.pop_back();
            }

        protected:
            Container  c;
    };

    // [比較演算子]
    template< class T, class Container >
    inline bool operator==(
        const ft::stack<T, Container>& lhs,
        const ft::stack<T, Container>& rhs
    ) {
        return lhs.c == rhs.c;
    }

    template< class T, class Container >
    inline bool operator!=(
        const ft::stack<T, Container>& lhs,
        const ft::stack<T, Container>& rhs
    ) {
        return lhs.c != rhs.c;
    }

    template< class T, class Container >
    inline bool operator<(
        const ft::stack<T, Container>& lhs,
        const ft::stack<T, Container>& rhs
    ) {
        return lhs.c < rhs.c;
    }

    template< class T, class Container >
    inline bool operator<=(
        const ft::stack<T, Container>& lhs,
        const ft::stack<T, Container>& rhs
    ) {
        return lhs.c <= rhs.c;
    }

    template< class T, class Container >
    inline bool operator>(
        const ft::stack<T, Container>& lhs,
        const ft::stack<T, Container>& rhs
    ) {
        return lhs.c > rhs.c;
    }

    template< class T, class Container >
    inline bool operator>=(
        const ft::stack<T, Container>& lhs,
        const ft::stack<T, Container>& rhs
    ) {
        return lhs.c >= rhs.c;
    }

    template< class T, class Container >
    inline void swap(
        ft::stack<T, Container>& lhs,
        ft::stack<T, Container>& rhs
    ) {
        lhs.swap(rhs);
    }
}

#endif
