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

    // [[reverse_iterator]]

    template <class NormalIter>
    class reverse_iterator {
        public:
            typedef NormalIter                                                  iterator_type;
            typedef typename ft::iterator_traits<NormalIter>::difference_type   difference_type;
            typedef typename ft::iterator_traits<NormalIter>::value_type        value_type;
            typedef typename ft::iterator_traits<NormalIter>::pointer           pointer;
            typedef typename ft::iterator_traits<NormalIter>::reference         reference;
            typedef typename ft::iterator_traits<NormalIter>::iterator_category iterator_category;
        
        protected:
            NormalIter  current_;

        public:
            reverse_iterator(): current_(NormalIter()) {}
            reverse_iterator(NormalIter iter): current_(iter) {}
            reverse_iterator(const reverse_iterator<NormalIter>& other) {
                *this = other;
            }
            virtual ~reverse_iterator() {}

            iterator_type       base() const { return current_; }

            reverse_iterator&   operator=(const reverse_iterator& rhs) {
                current_ = rhs.current_;
                return *this;
            }

            // 通常イテレータと異なり, mutableバージョンはない。
            reference           operator*() const {
                NormalIter  rv = current_;
                return *(--rv);
            }

            pointer             operator->() const {
                return &*(*this);
            }

            reverse_iterator&   operator++() {
                --current_;
                return *this;
            }

            reverse_iterator    operator++(int) {
                reverse_iterator    rv(*this);
                --*this;
                return rv;
            }

            reverse_iterator&   operator--() {
                ++current_;
                return *this;
            }

            reverse_iterator    operator--(int) {
                reverse_iterator    rv(*this);
                ++*this;
                return rv;
            }

            reverse_iterator&  operator+=(difference_type n) {
                current_ -= n;
                return *this;
            }

            reverse_iterator&    operator-=(difference_type n) {
                current_ += n;
                return *this;
            }

            reverse_iterator    operator+(difference_type n) const {
                return reverse_iterator(current_ - n);
            }

            reverse_iterator    operator-(difference_type n) const {
                return reverse_iterator(current_ + n);
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
