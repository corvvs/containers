#include "test_common.hpp"

template<>
int             random_value_generator() {
    return rand();
}

template<>
char            random_value_generator() {
    return ' ' + rand() % 95;
}

template<>
long            random_value_generator() {
    return rand();
}

template<>
double          random_value_generator() {
    return (double)rand() /  RAND_MAX;
}

template<>
simple_holder<char> random_value_generator() {
    return simple_holder<char>(random_value_generator<char>());
}

template<>
std::string     random_value_generator() {
    std::string s(1 + rand() % 10, 'a');
    for (std::string::iterator it = s.begin(); it != s.end(); ++it) {
        *it = 'a' + rand() % 26;
    }
    return s;
}

template<>
ft::IntWrapper  random_value_generator() {
    return ft::IntWrapper(rand());
}

template<>
std::vector<int>  random_value_generator() {
    std::size_t n = rand() % 10;
    std::vector<int> v;
    for (std::size_t i = 0; i < n; ++i) {
        v.push_back(random_value_generator<int>());
    }
    return v;
}

template<>
ft::vector<int>  random_value_generator() {
    std::size_t n = rand() % 10;
    ft::vector<int> v;
    for (std::size_t i = 0; i < n; ++i) {
        v.push_back(random_value_generator<int>());
    }
    return v;
}

std::ostream&   debug_out(
    const char *filename,
    const int linenumber
) {
    return (std::cout << "[" << filename << ":" << linenumber  << "] ");
}

std::ostream&   debug_err(
    const char *filename,
    const int linenumber
) {
    return (std::cerr << "[" << filename << ":" << linenumber << "] ");
}
