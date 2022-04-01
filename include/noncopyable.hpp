#ifndef NONCOPYABLE_HPP
# define NONCOPYABLE_HPP

namespace ft {
    template <class T>
    class noncopyable {
        private:
            T   value_;
        public:
            noncopyable(): value_(T()) {}
            noncopyable(const T& v): value_(v) {}
            ~noncopyable() {}
            const T&    get_value() const {
                return value_;
            }
        private:
            noncopyable(const noncopyable& other) {
                (void)other;
            }
            noncopyable&  operator=(const noncopyable& other){
                (void)other;
                return *this;
            }
    };
}

#endif
