#ifndef ITERATOR_WRAPPER_HPP
# define ITERATOR_WRAPPER_HPP

namespace ft {
    template <class Iter>
    class iterator_wrapper {
        public:
            typedef Iter                                        iterator_type;
            typedef iterator_wrapper<iterator_type>             self_type;
            typedef ft::iterator_traits<iterator_type>          iter_traits;
            typedef typename iter_traits::value_type            value_type;
            typedef typename iter_traits::difference_type       difference_type;
            typedef typename iter_traits::pointer               pointer;
            typedef typename iter_traits::reference             reference;
            typedef typename iter_traits::iterator_category     iterator_category;

        protected:
            iterator_type    base_;

        public:
            iterator_wrapper() FT_NOEXCEPT : base_(NULL) {}
            iterator_wrapper(iterator_type ptr_) FT_NOEXCEPT : base_(ptr_) {}
            iterator_wrapper(const iterator_wrapper& other) FT_NOEXCEPT : base_(other.base_) {}
            template <class U>
            iterator_wrapper(const iterator_wrapper<U>& u,
                typename ft::enable_if<
                    ft::is_convertible<U, iterator_type>::value
                >::type* = NULL) FT_NOEXCEPT
                : base_(u.operator->()) {}
            // ~iterator_wrapper() {}
            // iterator_wrapper&   operator=(const iterator_wrapper& other) FT_NOEXCEPT {
            //     base_ = other.base_;
            //     return *this;
            // }

            iterator_type   base() const FT_NOEXCEPT { return base_; }

            reference   operator*() const FT_NOEXCEPT {
                return *base_;
            }
            pointer     operator->() const FT_NOEXCEPT {
                return base_;
            }

            iterator_wrapper&   operator++() FT_NOEXCEPT {
                ++base_;
                return *this;
            }

            iterator_wrapper    operator++(int) FT_NOEXCEPT {
                iterator_wrapper    it = *this;
                ++*this;
                return it;
            }

            iterator_wrapper&   operator--() FT_NOEXCEPT {
                --base_;
                return *this;
            }

            iterator_wrapper    operator--(int) FT_NOEXCEPT {
                iterator_wrapper    it = *this;
                --*this;
                return it;
            }

            iterator_wrapper  operator+ (difference_type n) const FT_NOEXCEPT {
                iterator_wrapper it(*this);
                it += n;
                return it;
            }
            iterator_wrapper& operator+=(difference_type n) FT_NOEXCEPT {
                base_ += n;
                return *this;
            }
            iterator_wrapper  operator- (difference_type n) const FT_NOEXCEPT {
                return *this + (-n);
            }
            iterator_wrapper& operator-=(difference_type n) FT_NOEXCEPT {
                *this += -n;
                return *this;
            }
            reference    operator[](difference_type n) const FT_NOEXCEPT {
                return base_[n];
            }

            bool operator==(const iterator_wrapper& rhs) const FT_NOEXCEPT {
                return base_ == rhs.base_;
            }
            bool operator!=(const iterator_wrapper& rhs) const FT_NOEXCEPT {
                return base_ != rhs.base_;
            }
    };
}

#endif
