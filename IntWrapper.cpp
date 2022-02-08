#include "IntWrapper.hpp"

ft::IntWrapper::IntWrapper() {
    i_ = new int;
    *i_ = 0;
}

ft::IntWrapper::IntWrapper(int i) {
    i_ = new int;
    *i_ = i;
}

ft::IntWrapper::IntWrapper(const IntWrapper& other) {
    i_ = new int;
    *this = other;
}

ft::IntWrapper& ft::IntWrapper::operator=(const IntWrapper& rhs) {
    if (this != &rhs) {
        *i_ = *rhs.i_;
    }
    return *this;
}

ft::IntWrapper::~IntWrapper() {
    delete i_;
}

int ft::IntWrapper::get_int() const {
    return *i_;
}

std::ostream& operator<<(std::ostream& stream, const ft::IntWrapper& value) {
    return stream << value.get_int();
}
