#ifndef TEST_VECTOR_HPP
# define TEST_VECTOR_HPP

# include <iostream>
# include <string>
# include <cstdlib>

# ifdef USE_STL
#  include <vector>
#  define VectorClass std::vector
# else
#  include "ft_vector.hpp"
#  define VectorClass ft::vector
# endif

#endif
