CXX				:=	clang++
CXXFLAGS		:=	-Wall -Wextra -Werror -std=c++98 --pedantic -O2 -g -fsanitize=address
ifdef USE_STL
	CXXFLAGS	+=	-D USE_STL=1
endif
ifdef FT_DEBUG
	CXXFLAGS	+=	-D FT_DEBUG=1 -g -fsanitize=address
endif


SRCS_COMMON		:=	sprint.cpp ft_common.cpp test_common.cpp

# vector
HPPS_VECTOR		:=	vector.hpp
NAME_VECTOR_STL	:=	exe_vector_stl
NAME_VECTOR_FT	:=	exe_vector_ft
NAMES_VECTOR	:=	$(NAME_VECTOR_STL) $(NAME_VECTOR_FT)
SRCS_VECTOR		:=	main_vector.cpp $(SRCS_COMMON)
OBJS_VECTOR		:=	$(SRCS_VECTOR:.cpp=.o)

# stack
HPPS_STACK		:=	stack.hpp
NAME_STACK_STL	:=	exe_stack_stl
NAME_STACK_FT	:=	exe_stack_ft
NAMES_STACK		:=	$(NAME_STACK_STL) $(NAME_STACK_FT)
SRCS_STACK		:=	main_stack.cpp $(SRCS_COMMON)
OBJS_STACK		:=	$(SRCS_STACK:.cpp=.o)

NAMES			:=	$(NAMES_VECTOR) $(NAMES_STACK)

OBJS			:=	$(HPPS_OBJS)

.PHONY			:	all
all				:	stack_stl

run				:	vector_diff stack_diff

.PHONY			:	vector_clean vector_stl vector

vector_clean	:
	$(RM) $(OBJS_VECTOR)

vector_stl		:
	$(MAKE) vector_clean
	$(MAKE) USE_STL=1 $(NAME_VECTOR_STL)

vector			:
	$(MAKE) vector_clean
	$(MAKE) $(NAME_VECTOR_FT)

vector_diff		:	vector vector_stl
	time ./$(NAME_VECTOR_STL) > outv1
	time ./$(NAME_VECTOR_FT) 2> err2 > outv2
	diff outv1 outv2 || :

$(NAMES_VECTOR)	:	$(OBJS_VECTOR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_VECTOR)

.PHONY			:	stack_clean stack_stl stack_ft

stack_clean	:
	$(RM) $(OBJS_STACK)

stack_stl		:
	$(MAKE) stack_clean
	$(MAKE) USE_STL=1 $(NAME_STACK_STL)

stack			:
	$(MAKE) stack_clean
	$(MAKE) $(NAME_STACK_FT)

stack_diff		:	stack stack_stl
	time ./$(NAME_STACK_STL) > outs1
	time ./$(NAME_STACK_FT) 2> err2 > outs2
	diff outs1 outs2 || :

$(NAMES_STACK)	:	$(OBJS_STACK)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_STACK)
