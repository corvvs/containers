#include "ft_sprint.hpp"

VectorClass<std::string>    ft::sprint::chronicle;

unsigned long    get_ut(void)
{
    struct timeval	tv;

    if (gettimeofday(&tv, NULL)) { return (0); }
    return ((tv.tv_sec) * 1e6 + tv.tv_usec);
}


ft::sprint::sprint(const std::string title)
    : title_(title), time_origin_(get_ut()) {}

ft::sprint::sprint(const ft::sprint& other) {
    *this = other;
}

ft::sprint& ft::sprint::operator=(const ft::sprint& rhs) {
    time_origin_ = rhs.time_origin_;
    return *this;
}

ft::sprint::~sprint() {
    unsigned long time_current = get_ut();
    double dt_ms = (time_current - time_origin_) / 1000.0;
    std::stringstream ss;
    ss << title_ << ":\t" << dt_ms << "ms";
    ft::sprint::chronicle.push_back(ss.str());
}

void    ft::sprint::list() {
    for (VectorClass<std::string>::iterator it = chronicle.begin(); it != chronicle.end(); ++it) {
        std::cout << *it << std::endl;
    }
}
