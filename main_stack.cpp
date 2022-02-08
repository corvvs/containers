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

namespace test_int {
    typedef StackClass<int, ft::vector<int> >    Sta;
    void    print_elements(Sta st) {
        Sta receiver;
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

    void    mass_push_and_pop(unsigned long n) {
        srand(n);
        Sta s;
        {
            SPRINT();
            for (unsigned long i = 0; i < n; ++i) {
                if (rand() % 3 == 0 && !s.empty()) {
                    s.pop();
                } else {
                    s.push(rand());
                }
            }
        }
        print_elements(s);
        std::cout << s.size() << std::endl;
    }

    void    mass_copy(unsigned long n) {
        srand(n);
        Sta s;
        for (unsigned long i = 0; i < n; ++i) {
            s.push(rand());
        }
        print_elements(s);
        {
            SPRINT();
            Sta t(s);
            print_elements(t);
        }
    }

    void    mass_assign(unsigned long n) {
        srand(n);
        Sta s;
        Sta t;
        for (unsigned long i = 0; i < n; ++i) {
            s.push(rand());
        }
        print_elements(s);
        {
            SPRINT();
            t = s;
            print_elements(t);
        }
    }

    void    mass_test() {
    }
}


int main() {
    // test_int::mass_push_and_pop(100000);
    // test_int::mass_copy(1000000);
    test_int::mass_assign(1000000);
    ft::sprint::list();
}
