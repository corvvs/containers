CXX				:=	clang++
CXXFLAGS		:=	-Wall -Wextra -Werror -std=c++98 --pedantic -O2 -g -fsanitize=address
ifdef USE_STL
	CXXFLAGS	+=	-D USE_STL=1
endif
ifdef FT_DEBUG
	CXXFLAGS	+=	-D FT_DEBUG=1 -g -fsanitize=address
endif


SRCS_COMMON		:=	sprint.cpp ft_common.cpp test_common.cpp IntWrapper.cpp

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

# pair
HPPS_PAIR		:=	pair.hpp
NAME_PAIR_STL	:=	exe_pair_stl
NAME_PAIR_FT	:=	exe_pair_ft
NAMES_PAIR		:=	$(NAME_PAIR_STL) $(NAME_PAIR_FT)
SRCS_PAIR		:=	main_pair.cpp $(SRCS_COMMON)
OBJS_PAIR		:=	$(SRCS_PAIR:.cpp=.o)


# tree
HPPS_TREE		:=	tree.hpp
NAME_TREE_STL	:=	exe_tree_stl
NAME_TREE_FT	:=	exe_tree_ft
NAMES_TREE		:=	$(NAME_TREE_STL) $(NAME_TREE_FT)
SRCS_TREE		:=	main_tree.cpp $(SRCS_COMMON)
OBJS_TREE		:=	$(SRCS_TREE:.cpp=.o)

NAMES			:=	$(NAMES_VECTOR) $(NAMES_STACK) $(NAMES_PAIR) $(NAMES_TREE)

OBJS			:=	$(OBJS_VECTOR) $(OBJS_STACK) $(OBJS_PAIR) $(OBJS_TREE)

.PHONY			:	all
all				:	stack_stl

run				:	vector_diff stack_diff

.PHONY			:	clean fclean
clean			:
	$(RM) $(OBJS)
fclean			:	clean
	$(RM) $(NAMES)

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


.PHONY			:	pair_clean pair_stl pair_ft

pair_clean	:
	$(RM) $(OBJS_PAIR)

pair_stl		:
	$(MAKE) pair_clean
	$(MAKE) USE_STL=1 $(NAME_PAIR_STL)

pair			:
	$(MAKE) pair_clean
	$(MAKE) $(NAME_PAIR_FT)

pair_diff		:	pair pair_stl
	time ./$(NAME_PAIR_STL) > outs1
	time ./$(NAME_PAIR_FT) 2> err2 > outs2
	diff outs1 outs2 || :

$(NAMES_PAIR)	:	$(OBJS_PAIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_PAIR)

.PHONY			:	tree_clean tree_stl tree_ft

tree_clean	:
	$(RM) $(OBJS_TREE)

tree_stl		:
	$(MAKE) pair_clean
	$(MAKE) USE_STL=1 $(NAME_TREE_STL)

tree			:
	$(MAKE) tree_clean
	$(MAKE) $(NAME_TREE_FT)

tree_diff		:	tree tree_stl
	time ./$(NAME_TRRE_STL) > outs1
	time ./$(NAME_TRRE_FT) 2> err2 > outs2
	diff outs1 outs2 || :

$(NAMES_TREE)	:	$(OBJS_TREE)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_TREE)
