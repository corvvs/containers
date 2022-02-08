#ifndef SPRINT_HPP
# define SPRINT_HPP

# include <sys/time.h>
# include <iostream>
# include <sstream>
# include <string>
# include "test_common.hpp"
# define SPRINT() ft::sprint sprinter(__func__)
# define NSPRINT(title) ft::sprint sprinter(title)

namespace ft {
    class sprint {
        public:
            sprint(const std::string = "<no title>");
            sprint(const sprint& other);
            sprint&  operator=(const sprint &rhs);
            ~sprint();

            static void list();

        private:
            const std::string               title_;
            unsigned long                   time_origin_;
            static VectorClass<std::string> chronicle;
    };
}

#endif
