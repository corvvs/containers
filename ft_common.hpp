#ifndef FT_COMMON_HPP
# define FT_COMMON_HPP

# include <iostream>
# include <string>
# define FT_PRIVATE private
# define DOUT() debug_out(__func__, __FILE__, __LINE__)

std::ostream&   debug_out(
    const char *funcname,
    const char *filename,
    const int linenumber
) {
    return (std::cerr << "[" << filename << ":" << linenumber << " " << funcname << "] ");
}

#endif
