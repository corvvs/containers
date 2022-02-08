#ifndef TEST_VECTOR_HPP
# define TEST_VECTOR_HPP

# include <iostream>
# include <string>
# include <cstdlib>
# include "ft_common.hpp"
# include "vector.hpp"
# include <vector>
# include <deque>
# include <list>

# ifdef USE_STL
#  include <vector>
#  include <stack>
#  define VectorClass std::vector
#  define StackClass std::stack
# else
#  include "vector.hpp"
#  include "stack.hpp"
#  define VectorClass ft::vector
#  define StackClass ft::stack
# endif

# include "sprint.hpp"

template<class T>
T   random_value_generator();
template<>
int	random_value_generator();

#endif
