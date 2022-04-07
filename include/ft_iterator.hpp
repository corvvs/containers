#ifndef FT_ITERATOR_HPP
# define FT_ITERATOR_HPP
# include "ft_meta_functions.hpp"

namespace ft {
    // [[iterator_traits]]
    // イテレータと思われる型から、イテレータ的振る舞いに関する型情報を引き出すための
    // 統一的なインターフェース。
    // イテレータはこのインターフェースを実装していることが求められる。

    // 普通のイテレータ

    template <class T>
    class has_difference_type {
        private: 
            template <class U>
            static ft::yes_type f(typename U::difference_type* = NULL);
            template <class U>
            static ft::no_type  f(...);
        public:
            static const bool   value = sizeof(f<T>(NULL)) == sizeof(ft::yes_type);
    };

    template <class T>
    class has_value_type {
        private: 
            template <class U>
            static ft::yes_type f(typename U::value_type* = NULL);
            template <class U>
            static ft::no_type  f(...);
        public:
            static const bool   value = sizeof(f<T>(NULL)) == sizeof(ft::yes_type);
    };

    template <class T>
    class has_pointer {
        private: 
            template <class U>
            static ft::yes_type f(typename U::pointer* = NULL);
            template <class U>
            static ft::no_type  f(...);
        public:
            static const bool   value = sizeof(f<T>(NULL)) == sizeof(ft::yes_type);
    };

    template <class T>
    class has_reference {
        private: 
            template <class U>
            static ft::yes_type f(typename U::reference* = NULL);
            template <class U>
            static ft::no_type  f(...);
        public:
            static const bool   value = sizeof(f<T>(NULL)) == sizeof(ft::yes_type);
    };

    template <class T>
    class has_iterator_category {
        private: 
            template <class U>
            static ft::yes_type f(typename U::iterator_category* = NULL);
            template <class U>
            static ft::no_type  f(...);
        public:
            static const bool   value = sizeof(f<T>(NULL)) == sizeof(ft::yes_type);
    };


    template <class Iterator,
        bool = has_iterator_category<Iterator>::value
            && has_value_type<Iterator>::value
            && has_pointer<Iterator>::value
            && has_difference_type<Iterator>::value
            // これを入れるとおかしなことになる
            // && has_reference<Iterator>::value
    > struct iterator_traits;

    template <class Iterator>
    struct iterator_traits<Iterator, true> {
        typedef typename Iterator::difference_type   difference_type;
        typedef typename Iterator::value_type        value_type;
        typedef typename Iterator::pointer           pointer;
        typedef typename Iterator::reference         reference;
        typedef typename Iterator::iterator_category iterator_category;
    };

    // ポインタもイテレータの一種とみなす

    template <class T>
    struct iterator_traits<T*, false> {
        typedef ptrdiff_t                       difference_type; // ポインタの差はptr_diff_t
        typedef T                               value_type;
        typedef T*                              pointer;
        typedef T&                              reference;
        typedef std::random_access_iterator_tag iterator_category; // ポインタはランダムアクセス可
    };

    // constポインタ

    template <class T>
    struct iterator_traits<const T*, false> {
        typedef ptrdiff_t                       difference_type;
        typedef T                               value_type;
        typedef const T*                        pointer;
        typedef const T&                        reference;
        typedef std::random_access_iterator_tag iterator_category;
    };

    template <>
    struct iterator_traits<class Iterator, false> {};


    // [iterator_category predicates]

    template <class T, class To,
        bool = has_iterator_category<iterator_traits<T> >::value>
    struct has_iterator_category_convertible_to:
        public integral_constant<bool, is_convertible<typename iterator_traits<T>::iterator_category, To>::value> {};

    template <class T, class To>
    struct has_iterator_category_convertible_to<T, To, false> : false_type {};

    template <class T>
    struct is_input_iterator:
        public has_iterator_category_convertible_to<T, std::input_iterator_tag> {};

    template <class T>
    struct is_forward_iterator:
        public has_iterator_category_convertible_to<T, std::forward_iterator_tag> {};

    template <class T>
    struct is_bidirectional_iterator:
        public has_iterator_category_convertible_to<T, std::bidirectional_iterator_tag> {};

    template <class T>
    struct is_random_access_iterator:
        public has_iterator_category_convertible_to<T, std::random_access_iterator_tag> {};

    // [[reverse_iterator]]

    template <class NormalIter>
    class reverse_iterator {
        protected:
            // 元のイテレータを保持する(composition)

            NormalIter  current;

        public:
            // iterator_traits は元のイテレータを受け継ぐ

            typedef NormalIter                                                  iterator_type;
            typedef typename ft::iterator_traits<NormalIter>::difference_type   difference_type;
            typedef typename ft::iterator_traits<NormalIter>::pointer           pointer;
            typedef typename ft::iterator_traits<NormalIter>::value_type        value_type;
            typedef typename ft::iterator_traits<NormalIter>::iterator_category iterator_category;
            typedef typename ft::iterator_traits<NormalIter>::reference         reference;

            reverse_iterator(): current(NormalIter()) {}
            explicit reverse_iterator(NormalIter iter): current(iter) {}
            // reverse_iterator -> const_reverse_iterator の変換をやるにはこれが必要
            template <class U>
            reverse_iterator(const reverse_iterator<U>& u)
                : current(u.base()) {}
            reverse_iterator(const reverse_iterator<NormalIter>& other): current(other.current) {}
            // ~reverse_iterator() {}

            reverse_iterator&   operator=(const reverse_iterator& rhs) {
                current = rhs.base();
                return *this;
            }

            iterator_type       base() const { return current; }

            // 通常イテレータと異なり, mutableバージョンはない。
            reference           operator*() const {
                NormalIter  rv = current;
                return *(--rv);
            }

            pointer             operator->() const {
                return &(operator*());
            }

            reverse_iterator&   operator++() {
                --current;
                return *this;
            }

            reverse_iterator    operator++(int) {
                reverse_iterator    rv(*this);
                ++*this;
                return rv;
            }

            reverse_iterator&   operator--() {
                ++current;
                return *this;
            }

            reverse_iterator    operator--(int) {
                reverse_iterator    rv(*this);
                --*this;
                return rv;
            }

            reverse_iterator    operator+(difference_type n) const {
                return reverse_iterator(current - n);
            }

            reverse_iterator&   operator+=(difference_type n) {
                current -= n;
                return *this;
            }

            reverse_iterator    operator-(difference_type n) const {
                return reverse_iterator(current + n);
            }

            reverse_iterator&   operator-=(difference_type n) {
                current += n;
                return *this;
            }

            reference           operator[](difference_type n) const {
                return *(*this + n);
            }
    };
}

template <class It1, class It2>
inline bool operator==(const ft::reverse_iterator<It1>& x, const ft::reverse_iterator<It2>& y)
{
    return x.base() == y.base();
}

template <class It1, class It2>
inline bool operator!=(const ft::reverse_iterator<It1>& x, const ft::reverse_iterator<It2>& y)
{
    return x.base() != y.base();
}

template <class It1, class It2>
inline bool operator<(const ft::reverse_iterator<It1>& x, const ft::reverse_iterator<It2>& y)
{
    return x.base() > y.base();
}

template <class It1, class It2>
inline bool operator>(const ft::reverse_iterator<It1>& x, const ft::reverse_iterator<It2>& y)
{
    return x.base() < y.base();
}

template <class It1, class It2>
inline bool operator<=(const ft::reverse_iterator<It1>& x, const ft::reverse_iterator<It2>& y)
{
    return x.base() >= y.base();
}

template <class It1, class It2>
inline bool operator>=(const ft::reverse_iterator<It1>& x, const ft::reverse_iterator<It2>& y)
{
    return x.base() <= y.base();
}

template <class It1, class It2>
inline typename ft::reverse_iterator<It1>::difference_type operator-(
    const ft::reverse_iterator<It1>& x,
    const ft::reverse_iterator<It2>& y
) {
    return y.base() - x.base();
}

template <class It>
inline ft::reverse_iterator<It>    operator+(
    typename ft::reverse_iterator<It>::difference_type n,
    const ft::reverse_iterator<It>& x
) {
    return ft::reverse_iterator<It>(x.base() - n);
}

#endif
