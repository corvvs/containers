#include "test_common.hpp"

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

template <class T>
void    performance(const std::string& t) {
    ft::sprint::insert_comment("default container");
    ft::sprint::push_bread("stack<" + t + ">");
    mass_push_and_pop<T, StackContainer<T> >(300000);
    mass_copy<T, StackContainer<T> >(300000);
    mass_assign<T, StackContainer<T> >(300000);
    ft::sprint::pop_bread();
}

template <class T, class Container>
void    performance(const std::string& t, const std::string& container) {
    ft::sprint::insert_comment(container);
    ft::sprint::push_bread("stack<" + t + ", " + container + ">");
    mass_push_and_pop<T, Container>(300000);
    mass_copy<T, Container>(300000);
    mass_assign<T, Container>(300000);
    ft::sprint::pop_bread();
}

#define TP(T, C) T, C<T>

template <class T>
void    run_mass_test(const std::string& main_title) {
    ft::sprint::insert_comment(">> " + main_title);
    performance<T>(main_title);
    performance<TP(T, ft::vector)>(main_title, "ft::vector");
    performance<TP(T, std::vector)>(main_title, "std::vector");
    performance<TP(T, std::deque)>(main_title, "std::deque");
    performance<TP(T, std::list)>(main_title, "std::list");
}

int main() {
    run_mass_test<int>("int");
    run_mass_test<ft::IntWrapper>("ft::IntWrapper");
    run_mass_test<std::string>("std::string");
    ft::sprint::list();
}
