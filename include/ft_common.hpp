#ifndef FT_COMMON_HPP
# define FT_COMMON_HPP

# include <iostream>
# include <string>
# include <limits>
# include <algorithm>
# define FT_PRIVATE private
# define DSOUT() debug_out(__FILE__, __LINE__)
# define DOUT()  debug_err(__FILE__, __LINE__)

std::ostream&   debug_out(
    const char *filename,
    const int linenumber
);

std::ostream&   debug_err(
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
