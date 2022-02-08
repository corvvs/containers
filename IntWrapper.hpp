#ifndef INTWRAPPER_HPP
# define INTWRAPPER_HPP
# include <iostream>

namespace ft {
    class IntWrapper {
        public:
            IntWrapper();
            IntWrapper(int i);
            IntWrapper(const IntWrapper& other);
            ~IntWrapper();
            IntWrapper& operator=(const IntWrapper& rhs);

            int get_int() const;

        private:
            int *i_;
    };

}

std::ostream& operator<<(std::ostream& stream, const ft::IntWrapper& value);

#endif
