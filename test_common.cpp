#include "test_common.hpp"

template<>
int   random_value_generator() {
    return rand();
}
