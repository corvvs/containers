#ifndef SPRINT_HPP
# define SPRINT_HPP

# include <sys/time.h>
# include <iostream>
# include <iomanip>
# include <sstream>
# include <string>
# include "test_common.hpp"
# include "vector.hpp"
# define NSPRINT(title) ft::sprint sprinter(title); sprinter.get_tail()
# define SPRINT() NSPRINT(__func__)

namespace ft {
    // 時間を測るためのクラス
    // 構築時と破壊時の時間の差分を記録し、staticメンバ関数 list により出力する。
    class sprint {
        public:
            typedef unsigned long   time_type;
            typedef double          duration_type;

            sprint(const std::string = "<no title>");
            sprint(const sprint& other);
            sprint&  operator=(const sprint &rhs);
            ~sprint();

            std::stringstream&  get_tail();

            static void list();

        private:
            std::stringstream                   tail_;
            const std::string                   title_;
            unsigned long                       time_origin_;
            static ft::vector<std::string>      chronicle;
            static ft::vector<duration_type>    durations;
    };
}

#endif
