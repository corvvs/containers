#include "test_common.hpp"

template<>
int         random_value_generator() {
    return rand();
}

template<>
std::string random_value_generator() {
    std::string s(1 + rand() % 10, 'a');
    for (std::string::iterator it = s.begin(); it != s.end(); ++it) {
        *it = 'a' + rand() % 26;
    }
    return s;
}
