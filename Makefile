CXX				:=	clang++
CXXFLAGS		:=	-Wall -Wextra -Werror -std=c++98 --pedantic
ifdef USE_STL
	CXXFLAGS	+=	-D USE_STL=1
endif

HPPS_VECTOR		:=	ft_vector.hpp
HPPS_SRCS		:=	main_vector.cpp
HPPS_OBJS		:=	$(HPPS_SRCS:.cpp=.o)
NAME_VECTOR_STL	:=	exe_vector_stl
NAME_VECTOR_FT	:=	exe_vector_ft

NAMES			:=	$(NAME_VECTOR)

OBJS			:=	$(HPPS_OBJS)

.PHONY			:	all
all				:	vector_stl

.PHONY			:	vector_clean vector_stl vector

vector_stl		:
	$(MAKE) USE_STL=1 $(NAME_VECTOR_STL)

vector			:
	$(MAKE) $(NAME_VECTOR_FT)

$(NAME_VECTOR_FT) $(NAME_VECTOR_STL)	:	$(HPPS_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(HPPS_OBJS)
