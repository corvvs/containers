#ifndef ITERATOR_WRAPPER_HPP
# define ITERATOR_WRAPPER_HPP

namespace ft {
    template <class Iter>
    class iterator_wrapper {
        public:
            typedef Iter                                        iterator_type;
            typedef iterator_wrapper<iterator_type>             self_type;
            typedef ft::iterator_traits<iterator_type>          _iter_traits;
            typedef typename _iter_traits::value_type           value_type;
            typedef typename _iter_traits::difference_type      difference_type;
            typedef typename _iter_traits::pointer              pointer;
            typedef typename _iter_traits::reference            reference;
            typedef typename _iter_traits::iterator_category    iterator_category;

        protected:
            iterator_type    base_;

        public:
            iterator_wrapper(): base_(NULL) {}
            iterator_wrapper(iterator_type ptr_): base_(ptr_) {}
            iterator_wrapper(const iterator_wrapper& other) {
                *this = other;
            }
            template <class _Up>
            iterator_wrapper(const iterator_wrapper<_Up>& __u,
                typename std::enable_if<std::is_convertible<_Up, iterator_type>::value>::type* = NULL)
                : base_(__u.operator->()) {}
            virtual ~iterator_wrapper() {}
            iterator_wrapper&   operator=(const iterator_wrapper& other) {
                base_ = other.base_;
                return *this;
            }

            iterator_type   base() const { return base_; }

            reference   operator*() const {
                return *base_;
            }
            pointer     operator->() const {
                return base_;
            }

            iterator_wrapper&   operator++() {
                ++base_;
                return *this;
            }

            iterator_wrapper    operator++(int) {
                iterator_wrapper    it = *this;
                ++*this;
                return it;
            }

            iterator_wrapper&   operator--() {
                --base_;
                return *this;
            }

            iterator_wrapper    operator--(int) {
                iterator_wrapper    it = *this;
                --*this;
                return it;
            }

            iterator_wrapper  operator+ (difference_type __n) const {
                iterator_wrapper __w(*this);
                __w += __n;
                return __w;
            }
            iterator_wrapper& operator+=(difference_type __n) {
                base_ += __n;
                return *this;
            }
            iterator_wrapper  operator- (difference_type __n) const {
                return *this + (-__n);
            }
            iterator_wrapper& operator-=(difference_type __n) {
                *this += -__n;
                return *this;
            }
            reference    operator[](difference_type __n) const {
                return base_[__n];
            }

            inline bool operator==(const iterator_wrapper& rhs) const {
                return base_ == rhs.base_;
            }
            inline bool operator!=(const iterator_wrapper& rhs) const {
                return !(*this == rhs);
            }
    };
}

#endif
