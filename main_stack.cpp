#include "test_common.hpp"

template<class T>
void    print_stats(VectorClass<T> &v, bool with_stats = true) {
    std::cout << "capacity:     " << v.capacity() << std::endl;
    std::cout << "size:         " << v.size() << std::endl;
    std::cout << "empty:        " << v.empty() << std::endl;
    if (with_stats) {
        std::cout << "max_size:     " << v.max_size() << std::endl;
    }
}

template <class T, class Container>
void    print_elements(StackClass<T, Container> st) {
    StackClass<T, Container> receiver;
    std::cout << "[";
    while (!st.empty()) {
        std::cout << st.top();
        receiver.push(st.top());
        st.pop();
        if (st.empty()) {
            break;
        }
        std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    while (!receiver.empty()) {
        st.push(receiver.top());
        receiver.pop();
    }
}

template <class T, class Container>
void    mass_push_and_pop(unsigned long n) {
    srand(n);
    StackClass<T, Container> s;
    {
        SPRINT("mass_push_and_pop") << "(" << n << ")";
        for (unsigned long i = 0; i < n; ++i) {
            if (rand() % 3 == 0 && !s.empty()) {
                s.pop();
            } else {
                s.push(random_value_generator<T>());
            }
        }
    }
    print_elements(s);
    std::cout << s.size() << std::endl;
}

template <class T, class Container>
void    mass_copy(unsigned long n) {
    srand(n);
    StackClass<T, Container> s;
    for (unsigned long i = 0; i < n; ++i) {
        s.push(random_value_generator<T>());
    }
    print_elements(s);
    {
        SPRINT("mass_copy") << "(" << n << ")";
        StackClass<T, Container> t(s);
        print_elements(t);
    }
}

template <class T, class Container>
void    mass_assign(unsigned long n) {
    srand(n);
    StackClass<T, Container> s;
    StackClass<T, Container> t;
    for (unsigned long i = 0; i < n; ++i) {
        s.push(random_value_generator<T>());
    }
    print_elements(s);
    {
        SPRINT("mass_assign") << "(" << n << ")";
        t = s;
        print_elements(t);
    }
}

template <class T, class Container>
void    mass_test(const std::string& sub_title) {
    ft::sprint::insert_comment(sub_title);
    mass_push_and_pop<T, Container>(1000000);
    mass_copy<T, Container>(1000000);
    mass_assign<T, Container>(1000000);
}

#define TP(T, C) T, C<T>

template <class T>
void    run_mass_test(const std::string& main_title) {
    ft::sprint::insert_comment(main_title);
    mass_test<TP(T, ft::vector)>(">> ft::vector");
    mass_test<TP(T, std::vector)>(">> std::vector");
    mass_test<TP(T, std::deque)>(">> std::deque");
    mass_test<TP(T, std::list)>(">> std::list");
}

int main() {
    run_mass_test<int>("[int]");
    run_mass_test<ft::IntWrapper>("[IntWrapper]");
    run_mass_test<std::string>("[std::string]");
    ft::sprint::list();
}
