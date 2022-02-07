#ifndef TEST_VECTOR_HPP
# define TEST_VECTOR_HPP

# include <iostream>
# include <string>
# include <cstdlib>
# include "ft_common.hpp"

# ifdef USE_STL
#  include <vector>
#  define VectorClass std::vector
# else
#  include "vector.hpp"
#  define VectorClass ft::vector
# endif

# include "sprint.hpp"

#endif
