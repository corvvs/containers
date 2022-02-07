#ifndef FT_SPRINT_HPP
# define FT_SPRINT_HPP

# include <sys/time.h>
# include <iostream>
# include <sstream>
# include <string>
# include "test_vector.hpp"
# define SPRINT() ft::sprint sprinter(__func__)

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
