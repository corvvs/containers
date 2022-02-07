#include "ft_common.hpp"

std::ostream&   debug_out(
    const char *funcname,
    const char *filename,
    const int linenumber
) {
    return (std::cerr << "[" << filename << ":" << linenumber << " " << funcname << "] ");
}
