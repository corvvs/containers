CXX				:=	c++
CXXFLAGS		:=	-Wall -Wextra -Werror -std=c++98 --pedantic -I./include -g -fsanitize=address
ifdef USE_STL
	CXXFLAGS	+=	-D USE_STL=1
endif
ifdef FT_DEBUG
	CXXFLAGS	+=	-D FT_DEBUG=1 -g -fsanitize=address
endif


SRCS_COMMON		:=	sprint.cpp ft_common.cpp test_common.cpp IntWrapper.cpp
SRCS_COMMON		:=	$(addprefix src/, $(SRCS_COMMON))

# vector
HPPS_VECTOR		:=	vector.hpp
NAME_VECTOR_STL	:=	exe_vector_stl
NAME_VECTOR_FT	:=	exe_vector_ft
NAMES_VECTOR	:=	$(NAME_VECTOR_STL) $(NAME_VECTOR_FT)
SRCS_VECTOR		:=	src/main_vector.cpp $(SRCS_COMMON)
OBJS_VECTOR		:=	$(SRCS_VECTOR:.cpp=.o)

# stack
HPPS_STACK		:=	stack.hpp
NAME_STACK_STL	:=	exe_stack_stl
NAME_STACK_FT	:=	exe_stack_ft
NAMES_STACK		:=	$(NAME_STACK_STL) $(NAME_STACK_FT)
SRCS_STACK		:=	src/main_stack.cpp $(SRCS_COMMON)
OBJS_STACK		:=	$(SRCS_STACK:.cpp=.o)

# pair
HPPS_PAIR		:=	pair.hpp
NAME_PAIR_STL	:=	exe_pair_stl
NAME_PAIR_FT	:=	exe_pair_ft
NAMES_PAIR		:=	$(NAME_PAIR_STL) $(NAME_PAIR_FT)
SRCS_PAIR		:=	src/main_pair.cpp $(SRCS_COMMON)
OBJS_PAIR		:=	$(SRCS_PAIR:.cpp=.o)


# tree
HPPS_TREE		:=	tree.hpp
NAME_TREE_STL	:=	exe_tree_stl
NAME_TREE_FT	:=	exe_tree_ft
NAMES_TREE		:=	$(NAME_TREE_STL) $(NAME_TREE_FT)
SRCS_TREE		:=	src/main_tree.cpp $(SRCS_COMMON)
OBJS_TREE		:=	$(SRCS_TREE:.cpp=.o)

# map
HPPS_MAP		:=	map.hpp
NAME_MAP_STL	:=	exe_map_stl
NAME_MAP_FT		:=	exe_map_ft
NAMES_MAP		:=	$(NAME_MAP_STL) $(NAME_MAP_FT)
SRCS_MAP		:=	src/main_map.cpp $(SRCS_COMMON)
OBJS_MAP		:=	$(SRCS_MAP:.cpp=.o)

# set
HPPS_SET		:=	set.hpp
NAME_SET_STL	:=	exe_set_stl
NAME_SET_FT		:=	exe_set_ft
NAMES_SET		:=	$(NAME_SET_STL) $(NAME_SET_FT)
SRCS_SET		:=	src/main_set.cpp $(SRCS_COMMON)
OBJS_SET		:=	$(SRCS_SET:.cpp=.o)

# main
NAME_MAIN_STL	:=	exe_main_stl
NAME_MAIN_FT	:=	exe_main_ft
NAMES_MAIN		:=	$(NAME_MAIN_STL) $(NAME_MAIN_FT)
SRCS_MAIN		:=	src/main.cpp $(SRCS_COMMON)
OBJS_MAIN		:=	$(SRCS_MAIN:.cpp=.o)

# meta function
NAME_META_STL	:=	exe_meta_stl
NAME_META_FT	:=	exe_meta_ft
NAMES_META		:=	$(NAME_META_STL) $(NAME_META_FT)
SRCS_META		:=	src/main_meta.cpp $(SRCS_COMMON)
OBJS_META		:=	$(SRCS_META:.cpp=.o)

NAMES			:=	$(NAMES_VECTOR) $(NAMES_STACK) $(NAMES_PAIR) \
					$(NAMES_MAP) $(NAMES_SET) $(NAMES_MAIN)

OBJS			:=	$(OBJS_VECTOR) $(OBJS_STACK) $(OBJS_PAIR) \
					$(OBJS_MAP) $(OBJS_SET) $(OBJS_MAIN)

.PHONY			:	all run
all				:	$(NAMES)

run				:	fclean meta_diff stack_diff vector_diff map_diff set_diff main_diff

.PHONY			:	clean fclean re
clean			:
	$(RM) $(OBJS)

fclean			:	clean
	$(RM) $(NAMES)

re				:	fclean all

# [[vector]]
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
	time ./$(NAME_VECTOR_STL) > out_vector_1
	time ./$(NAME_VECTOR_FT) 2> err2 > out_vector_2
	sed -n -e '1,/= sprints =/p'  out_vector_1 > out1
	sed -n -e '/= sprints =/,$$p' out_vector_1 > sprint1
	sed -n -e '1,/= sprints =/p'  out_vector_2 > out2
	sed -n -e '/= sprints =/,$$p' out_vector_2 > sprint2
	diff out1 out2
	ruby compare_sprint.rb sprint1 sprint2

$(NAMES_VECTOR)	:	$(OBJS_VECTOR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_VECTOR)

# [[stack]]
.PHONY			:	stack_clean stack_stl stack_ft

stack_clean		:
	$(RM) $(OBJS_STACK)

stack_stl		:
	$(MAKE) stack_clean
	$(MAKE) USE_STL=1 $(NAME_STACK_STL)

stack			:
	$(MAKE) stack_clean
	$(MAKE) $(NAME_STACK_FT)

stack_diff		:	stack stack_stl
	time ./$(NAME_STACK_STL) > out_stack_1
	time ./$(NAME_STACK_FT) 2> err2 > out_stack_2
	sed -n -e '1,/= sprints =/p'  out_stack_1 > out1
	sed -n -e '/= sprints =/,$$p' out_stack_1 > sprint1
	sed -n -e '1,/= sprints =/p'  out_stack_2 > out2
	sed -n -e '/= sprints =/,$$p' out_stack_2 > sprint2
	diff out1 out2
	ruby compare_sprint.rb sprint1 sprint2

$(NAMES_STACK)	:	$(OBJS_STACK)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_STACK)


# [[pair]]
.PHONY			:	pair_clean pair_stl pair_ft

pair_clean		:
	$(RM) $(OBJS_PAIR)

pair_stl		:
	$(MAKE) pair_clean
	$(MAKE) USE_STL=1 $(NAME_PAIR_STL)

pair			:
	$(MAKE) pair_clean
	$(MAKE) $(NAME_PAIR_FT)

pair_diff		:	pair pair_stl
	time ./$(NAME_PAIR_STL) > out_pair_1
	time ./$(NAME_PAIR_FT) 2> err2 > out_pair_2
	time ./$(NAME_MAP_STL) 2> err2 > out_map_1
	time ./$(NAME_MAP_FT) 2> err2 > out_map_2
	diff out_map_1 out_map_2 || :
	sed -n -e '1,/= sprints =/p'  out_pair_1 > out1
	sed -n -e '/= sprints =/,$$p' out_pair_1 > sprint1
	sed -n -e '1,/= sprints =/p'  out_pair_2 > out2
	sed -n -e '/= sprints =/,$$p' out_pair_2 > sprint2
	diff out1 out2
	ruby compare_sprint.rb sprint1 sprint2

$(NAMES_PAIR)	:	$(OBJS_PAIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_PAIR)

# [[tree]]
.PHONY			:	tree_clean tree_stl tree_ft

tree_clean		:
	$(RM) $(OBJS_TREE)

tree_stl		:
	$(MAKE) tree_clean
	$(MAKE) USE_STL=1 $(NAME_TREE_STL)

tree			:
	$(MAKE) tree_clean
	$(MAKE) $(NAME_TREE_FT)

tree_diff		:	tree tree_stl
	time ./$(NAME_TREE_STL) 2> err2 > out_tree_1
	time ./$(NAME_TREE_FT) 2> err2 > out_tree_2
	diff out_tree_1 out_tree_2 || :

$(NAMES_TREE)	:	$(OBJS_TREE)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_TREE)

# [[map]]
.PHONY			:	map_clean map_stl map_ft

map_clean		:
	$(RM) $(OBJS_MAP)

map_stl			:
	$(MAKE) map_clean
	$(MAKE) USE_STL=1 $(NAME_MAP_STL)

map				:
	$(MAKE) map_clean
	$(MAKE) $(NAME_MAP_FT)

map_diff		:	map map_stl
	time ./$(NAME_MAP_STL) 2> err2 > out_map_1
	time ./$(NAME_MAP_FT) 2> err2 > out_map_2
	sed -n -e '1,/= sprints =/p'  out_map_1 > out1
	sed -n -e '/= sprints =/,$$p' out_map_1 > sprint1
	sed -n -e '1,/= sprints =/p'  out_map_2 > out2
	sed -n -e '/= sprints =/,$$p' out_map_2 > sprint2
	diff out1 out2
	ruby compare_sprint.rb sprint1 sprint2

$(NAMES_MAP)	:	$(OBJS_MAP)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_MAP)


# [[set]]
.PHONY			:	set_clean set_stl set_ft

set_clean		:
	$(RM) $(OBJS_SET)

set_stl			:
	$(MAKE) set_clean
	$(MAKE) USE_STL=1 $(NAME_SET_STL)

set				:
	$(MAKE) set_clean
	$(MAKE) $(NAME_SET_FT)

set_diff		:	set set_stl
	time ./$(NAME_SET_STL) 2> err2 > out_set_1
	time ./$(NAME_SET_FT) 2> err2 > out_set_2
	sed -n -e '1,/= sprints =/p'  out_set_1 > out1
	sed -n -e '/= sprints =/,$$p' out_set_1 > sprint1
	sed -n -e '1,/= sprints =/p'  out_set_2 > out2
	sed -n -e '/= sprints =/,$$p' out_set_2 > sprint2
	diff out1 out2
	ruby compare_sprint.rb sprint1 sprint2

$(NAMES_SET)	:	$(OBJS_SET)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_SET)

# [[main(subject)]]
.PHONY			:	main_clean main_stl main_ft

main_clean		:
	$(RM) $(OBJS_MAP)

main_stl		:
	$(MAKE) main_clean
	$(MAKE) USE_STL=1 $(NAME_MAIN_STL)

main			:
	$(MAKE) main_clean
	$(MAKE) $(NAME_MAIN_FT)

main_diff		:	main main_stl
	time ./$(NAME_MAIN_STL) 100 2> err2 > out_main_1
	time ./$(NAME_MAIN_FT) 100 2> err2 > out_main_2
	diff out_main_1 out_main_2

$(NAMES_MAIN)	:	$(OBJS_MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_MAIN)


# [[meta]]
.PHONY			:	meta_clean meta_stl meta_ft

meta_clean		:
	$(RM) $(OBJS_META)

meta_stl		:
	$(MAKE) meta_clean
	$(MAKE) USE_STL=1 $(NAME_META_STL)

meta			:
	$(MAKE) meta_clean
	$(MAKE) $(NAME_META_FT)

meta_diff		:	meta meta_stl
	time ./$(NAME_META_STL) 100 2> err2 > out_meta_1
	time ./$(NAME_META_FT) 100 2> err2 > out_meta_2
	diff out_meta_1 out_meta_2 || :

$(NAMES_META)	:	$(OBJS_META)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_META)

