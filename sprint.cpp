#include "sprint.hpp"

ft::vector<std::string>                 ft::sprint::chronicle;
ft::vector<ft::sprint::duration_type>   ft::sprint::durations;

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
    ft::sprint::time_type time_current = get_ut();
    ft::sprint::duration_type dt_ms = (time_current - time_origin_) / 1000.0;
    std::string t = title_ + tail_.str();
    ft::sprint::chronicle.push_back(t);
    ft::sprint::durations.push_back(dt_ms);
}
std::stringstream&  ft::sprint::get_tail() {
    return tail_;
}


void    ft::sprint::list() {
    std::size_t n = chronicle.size();
    std::size_t w = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (chronicle[i].length() > w) {
            w = chronicle[i].length();
        }
    }
    std::cout << std::left;
    for (std::size_t i = 0; i < n; ++i) {
        std::cout << std::setw(w + 1) << chronicle[i];
        std::cout << std::setw(0) << ": ";
        std::cout << durations[i] << "ms" << std::endl;
    }
}
