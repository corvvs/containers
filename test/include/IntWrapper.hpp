#ifndef INTWRAPPER_HPP
# define INTWRAPPER_HPP
# include <iostream>

namespace ft {
    class IntWrapper {
        public:
            IntWrapper();
            explicit IntWrapper(int i);
            IntWrapper(const IntWrapper& other);
            ~IntWrapper();
            IntWrapper& operator=(const IntWrapper& rhs);

            int get_int() const;
            static void set_limit(int n);

            static int  limit;

        private:
            int *i_;
            static int  instantiated_;
    };

}

std::ostream& operator<<(std::ostream& stream, const ft::IntWrapper& value);

#endif
