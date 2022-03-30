#ifndef BLANK_HPP
# define BLANK_HPP

# include <iostream>
# include <exception>
# include <stdexcept>

namespace ft {
    template <class T>
    class blank {
        private:
            T   value_;

        public:
            blank(): value_(T()) {
                std::cout << "default()" << std::endl;
            }
            blank(const T& v): value_(v) {
                std::cout << "given()" << std::endl;
            }
            ~blank() {}
            blank&  operator=(const blank& other) {
                value_ = other.value_;
                return *this;
            }
    };
}

#endif
