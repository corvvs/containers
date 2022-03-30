#include "IntWrapper.hpp"

int ft::IntWrapper::limit = -1;
int ft::IntWrapper::instantiated_ = 0;

ft::IntWrapper::IntWrapper() {
    i_ = new int;
    *i_ = 0;
}

ft::IntWrapper::IntWrapper(int i) {
    i_ = new int;
    *i_ = i;
    instantiated_ += 1;
    if (limit >= 0 && limit <= instantiated_) {
        throw std::runtime_error("over instantiation-limit");
    }
}

ft::IntWrapper::IntWrapper(const IntWrapper& other) {
    i_ = new int;
    *i_ = 0;
    *this = other;
    instantiated_ += 1;
    if (limit >= 0 && limit <= instantiated_) {
        throw std::runtime_error("over instantiation-limit");
    }
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

void ft::IntWrapper::set_limit(int n) {
    limit = n;
    instantiated_ = 0;
}


std::ostream& operator<<(std::ostream& stream, const ft::IntWrapper& value) {
    return stream << value.get_int();
}
