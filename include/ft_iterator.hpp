#ifndef FT_ITERATOR_HPP
# define FT_ITERATOR_HPP
# include "ft_meta_functions.hpp"

namespace ft {
    // [[iterator_traits]]
    // イテレータと思われる型から、イテレータ的振る舞いに関する型情報を引き出すための
    // 統一的なインターフェース。
    // イテレータはこのインターフェースを実装していることが求められる。

    // 普通のイテレータ

    template <class Iterator>
    struct iterator_traits {
        typedef typename Iterator::difference_type   difference_type;
        typedef typename Iterator::value_type        value_type;
        typedef typename Iterator::pointer           pointer;
        typedef typename Iterator::reference         reference;
        typedef typename Iterator::iterator_category iterator_category;
    };

    // ポインタもイテレータの一種とみなす

    template <class T>
    struct iterator_traits<T*> {
        typedef ptrdiff_t                   difference_type; // ポインタの差はptr_diff_t
        typedef T                           value_type;
        typedef T*                          pointer;
        typedef T&                          reference;
        typedef std::random_access_iterator_tag  iterator_category; // ポインタはランダムアクセス可
    };

    // constポインタ

    template <class T>
    struct iterator_traits<const T*> {
        typedef ptrdiff_t                   difference_type;
        typedef T                           value_type;
        typedef const T*                    pointer;
        typedef const T&                    reference;
        typedef std::random_access_iterator_tag  iterator_category;
    };

    // [iterator_category predicates]
    template <class T>
    struct has_iterator_category {
        private:
            struct __two {
                char __lx; char __lxx;
            };
            template <class To>
            static __two __test(...);
            template <class To>
            static char __test(typename To::iterator_category* = nullptr);
        public:
            static const bool value = sizeof(__test<T>(nullptr)) == 1;
    };

    template <class T, class To, bool = has_iterator_category<iterator_traits<T> >::value>
    struct has_iterator_category_convertible_to
        : is_convertible<typename iterator_traits<T>::iterator_category, To> {};

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
            typedef typename ft::iterator_traits<NormalIter>::value_type        value_type;
            typedef typename ft::iterator_traits<NormalIter>::pointer           pointer;
            typedef typename ft::iterator_traits<NormalIter>::reference         reference;
            typedef typename ft::iterator_traits<NormalIter>::iterator_category iterator_category;
        
            reverse_iterator(): current(NormalIter()) {}
            explicit reverse_iterator(NormalIter iter): current(iter) {}
            reverse_iterator(const reverse_iterator<NormalIter>& other) {
                *this = other;
            }
            virtual ~reverse_iterator() {}

            iterator_type       base() const { return current; }

            reverse_iterator&   operator=(const reverse_iterator& rhs) {
                current = rhs.current;
                return *this;
            }

            // 通常イテレータと異なり, mutableバージョンはない。
            reference           operator*() const {
                NormalIter  rv = current;
                return *(--rv);
            }

            pointer             operator->() const {
                return &*(*this);
            }

            reverse_iterator&   operator++() {
                --current;
                return *this;
            }

            reverse_iterator    operator++(int) {
                reverse_iterator    rv(*this);
                --*this;
                return rv;
            }

            reverse_iterator&   operator--() {
                ++current;
                return *this;
            }

            reverse_iterator    operator--(int) {
                reverse_iterator    rv(*this);
                ++*this;
                return rv;
            }

            reverse_iterator&  operator+=(difference_type n) {
                current -= n;
                return *this;
            }

            reverse_iterator&    operator-=(difference_type n) {
                current += n;
                return *this;
            }

            reverse_iterator    operator+(difference_type n) const {
                return reverse_iterator(current - n);
            }

            reverse_iterator    operator-(difference_type n) const {
                return reverse_iterator(current + n);
            }

            reference           operator[](difference_type n) const {
                return *(*this - n);
            }
    };
}

template <class It1, class It2>
bool    operator==(const ft::reverse_iterator<It1>& x, const ft::reverse_iterator<It2>& y)
{
    return x.base() == y.base();
}

template <class It1, class It2>
bool    operator!=(const ft::reverse_iterator<It1>& x, const ft::reverse_iterator<It2>& y)
{
    return x.base() != y.base();
}

template <class It1, class It2>
bool    operator<(const ft::reverse_iterator<It1>& x, const ft::reverse_iterator<It2>& y)
{
    return x.base() < y.base();
}

template <class It1, class It2>
bool    operator>(const ft::reverse_iterator<It1>& x, const ft::reverse_iterator<It2>& y)
{
    return x.base() > y.base();
}

template <class It1, class It2>
bool    operator<=(const ft::reverse_iterator<It1>& x, const ft::reverse_iterator<It2>& y)
{
    return x.base() <= y.base();
}

template <class It1, class It2>
bool    operator>=(const ft::reverse_iterator<It1>& x, const ft::reverse_iterator<It2>& y)
{
    return x.base() >= y.base();
}

template <class It1, class It2>
typename ft::reverse_iterator<It1>::difference_type operator-(
    const ft::reverse_iterator<It1>& x,
    const ft::reverse_iterator<It2>& y
) {
    return y.base() - x.base();
}

template <class It>
ft::reverse_iterator<It>    operator+(
    typename ft::reverse_iterator<It>::difference_type n,
    const ft::reverse_iterator<It>& x
) {
    return ft::reverse_iterator<It>(x.base() - n);
}

#endif
