#ifndef FT_COMMON_HPP
# define FT_COMMON_HPP

# include <iostream>
# include <string>
# include <limits>
# include <algorithm>
# define FT_PRIVATE private
// TODO: 削除
# define DOUT() debug_out(__func__, __FILE__, __LINE__)

std::ostream&   debug_out(
    const char *funcname,
    const char *filename,
    const int linenumber
);

namespace ft {
    template<class T>
    void        swap(T& a, T& b) {
        T temp(a);
        a = b;
        b = temp;
    }
}

#endif
