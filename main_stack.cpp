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

#define SC(Container) StackClass<int, Container>

namespace test_int {

    template <class Container>
    void    print_elements(SC(Container) st) {
        SC(Container) receiver;
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

    template <class Container>
    void    mass_push_and_pop(unsigned long n) {
        srand(n);
        SC(Container) s;
        {
            SPRINT() << "(" << n << ")";
            for (unsigned long i = 0; i < n; ++i) {
                if (random_value_generator<int>() % 3 == 0 && !s.empty()) {
                    s.pop();
                } else {
                    s.push(random_value_generator<int>());
                }
            }
        }
        print_elements(s);
        std::cout << s.size() << std::endl;
    }

    template <class Container>
    void    mass_copy(unsigned long n) {
        srand(n);
        SC(Container) s;
        for (unsigned long i = 0; i < n; ++i) {
            s.push(random_value_generator<int>());
        }
        print_elements(s);
        {
            SPRINT() << "(" << n << ")";
            SC(Container) t(s);
            print_elements(t);
        }
    }

    template <class Container>
    void    mass_assign(unsigned long n) {
        srand(n);
        SC(Container) s;
        SC(Container) t;
        for (unsigned long i = 0; i < n; ++i) {
            s.push(random_value_generator<int>());
        }
        print_elements(s);
        {
            SPRINT() << "(" << n << ")";
            t = s;
        }
        print_elements(t);
    }

    template <class Container>
    void    mass_test() {
        mass_push_and_pop<Container>(1000000);
        mass_copy<Container>(1000000);
        mass_assign<Container>(1000000);
    }
}


int main() {
    test_int::mass_test<ft::vector<int> >();
    test_int::mass_test<std::vector<int> >();
    test_int::mass_test<std::deque<int> >();
    test_int::mass_test<std::list<int> >();
    ft::sprint::list();
}
