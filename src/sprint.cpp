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
    : title_(title) {
        std::cout << ">> sprint start: " << title_ << std::endl;
        time_origin_ = get_ut();
    }

ft::sprint::sprint(const ft::sprint& other) {
    *this = other;
}

ft::sprint& ft::sprint::operator=(const ft::sprint& rhs) {
    time_origin_ = rhs.time_origin_;
    return *this;
}

ft::sprint::~sprint() {
    time_type time_current = get_ut();
    duration_type dt_ms = (time_current - time_origin_) / 1000.0;
    std::string t = title_ + tail_.str();
    chronicle.push_back(t);
    durations.push_back(dt_ms);
    std::cout << "<< sprint end: " << title_ << std::endl;
}
std::stringstream&  ft::sprint::get_tail() {
    return tail_;
}

void    ft::sprint::insert_comment(const std::string& comment) {
    chronicle.push_back(comment);
    durations.push_back(-1);
}

void    ft::sprint::list() {
    std::size_t n = chronicle.size();
    std::size_t w = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (chronicle[i].length() > w) {
            w = chronicle[i].length();
        }
    }
    std::string sep(w / 2, '=');
    std::cout << sep << " sprints " << sep << std::endl;
    std::cout << std::left;
    for (std::size_t i = 0; i < n; ++i) {
        if (durations[i] >= 0) {
            std::cout << std::setw(w + 1) << chronicle[i];
            std::cout << std::setw(0) << ": ";
            std::cout << durations[i] << "ms" << std::endl;
        } else {
            std::cout << chronicle[i] << std::endl;;
        }
    }
}
