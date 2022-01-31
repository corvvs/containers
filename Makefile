CXX				:=	clang++
CXXFLAGS		:=	-Wall -Wextra -Werror -std=c++98 --pedantic

HPPS_VECTOR		:=	ft_vector.hpp
HPPS_SRCS		:=	main_vector.cpp
HPPS_OBJS		:=	$(HPPS_SRCS:.cpp=.o)
NAME_VECTOR		:=	exe_vector

NAMES			:=	$(NAME_VECTOR)

OBJS			:=	$(HPPS_OBJS)

.PHONY			:	all clean fclean re


all				:	$(NAME_VECTOR)

$(NAME_VECTOR)	:	$(HPPS_OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAMNAME_VECTOR) $(HPPS_OBJS)

clean		:
	$(RM) -rf $(OBJS)

fclean		:	clean
	$(RM) $(NAMES)

re			:	fclean all
