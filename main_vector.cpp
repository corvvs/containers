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

#define VC(T) typename VectorClass<T>

namespace test_int {
    template <class Iter>
    void    print_elements(Iter from, Iter to) {
        std::cout << "[";
        for (Iter it = from; it != to; ++it) {
            if (it != from) {
                std::cout << ", ";
            }
            std::cout << *it;
        }
        std::cout << "]" << std::endl;
    }

    // 構築とreserve
    template <class T>
    void    construct_and_reserve() {
        SPRINT("construct_and_reserve");
        VC(T)  vi;
        print_stats(vi);
        std::cout << "-- reserve(0)" << std::endl;
        vi.reserve(0);
        print_stats(vi);
        std::cout << "-- reserve(1)" << std::endl;
        vi.reserve(1);
        print_stats(vi);
        std::cout << "-- reserve(100000000)" << std::endl;
        vi.reserve(100000000);
        print_stats(vi);
        std::cout << "-- reserve(1)" << std::endl;
        vi.reserve(1);
        print_stats(vi);
        std::cout << "-- reserve(0)" << std::endl;
        vi.reserve(0);
        print_stats(vi);
        // std::cout << "-- reserve(-1)" << std::endl;
        // vi.reserve(-1);
        // print_stats(vi);
    }

    // swap
    template <class T>
    void    swap() {
        VC(T)  vi;
        vi.push_back(random_value_generator<T>());
        vi.push_back(random_value_generator<T>());
        VC(T)  vj;
        vj.push_back(random_value_generator<T>());
        vj.push_back(random_value_generator<T>());
        VC(T)::iterator ii = vi.begin();
        VC(T)::iterator ij = vj.begin();
        print_elements(vi.begin(), vi.end());
        print_elements(vj.begin(), vj.end());
        std::cout << *ii << " - " << *ij << std::endl;
        {
            SPRINT("swap");
            vi.swap(vj);
        }
        print_elements(vi.begin(), vi.end());
        print_elements(vj.begin(), vj.end());
        std::cout << *ii << " - " << *ij << std::endl;
    }

    template <class T>
    void    mass_assign(VC(T)::size_type n) {
        SPRINT("mass_assign") << "(" << n << ")";
        srand(n);
        VC(T)  vi;
        for (VC(T)::size_type i = 0; i < 5; ++i) {
            VC(T)::size_type cap = vi.capacity();
            vi.assign((VC(T)::size_type)((double)(rand()) / RAND_MAX * n) + 1, random_value_generator<T>());
            std::cout << "cap: " << cap << " -> " << vi.capacity() << std::endl;
            print_elements(vi.begin(), vi.end());
            print_elements(vi.rbegin(), vi.rend());
            print_stats(vi);
        }
    }

    template <class T>
    void    mass_assign_range(VC(T)::size_type n) {
        srand(n);
        VC(T) vj;
        for (VC(T)::size_type i = 0; i < n; ++i) {
            vj.push_back(random_value_generator<T>());
        }
        VC(T) vi;
        {
            SPRINT("mass_assign_range") << "(" << n << ")";
            for (VC(T)::size_type i = 0; i <= n / 100; ++i) {
                VC(T)::size_type from = VC(T)::size_type(double(rand()) / RAND_MAX * (vj.size() - 1));
                VC(T)::size_type to = VC(T)::size_type(double(rand()) / RAND_MAX * (vj.size() - from)) + from;
                std::cout << "insert [" << from << ", " << to << "), " << (to - from) << std::endl;
                VC(T)::size_type cap = vi.capacity();
                vi.assign(vj.begin() + from, vj.begin() + to);
                std::cout << "cap: " << cap << " -> " << vi.capacity() << std::endl;
                print_elements(vi.begin(), vi.end());
                print_elements(vi.rbegin(), vi.rend());
                print_stats(vi);
            }
        }
    }

    template <class T>
    void    mass_assignation_eq(VC(T)::size_type n) {
        srand(n);
        VC(T)  vj;
        {
            SPRINT("mass_assignation_eq") << "(" << n << ")";
            VC(T)  vi;
            for (std::size_t i = 0; i < n; i += 1 ) {
                std::size_t ca = vi.capacity();
                vi.push_back(random_value_generator<T>());
                std::size_t cb = vi.capacity();
                if (ca < cb) {
                    std::cout << "capacity updated: " << ca << " -> " << cb << std::endl;
                }
            }
            vj = vi;
            print_stats(vi);
            for (std::size_t i = 0; i < n; i += 1 ) {
                std::size_t ca = vj.capacity();
                vj.push_back(random_value_generator<T>());
                std::size_t cb = vj.capacity();
                if (ca < cb) {
                    std::cout << "capacity updated: " << ca << " -> " << cb << std::endl;
                }
            }
        }
        print_elements(vj.begin(), vj.end());
        print_elements(vj.rbegin(), vj.rend());
        print_stats(vj);
    }

    template <class T>
    void    mass_swap(VC(T)::size_type n) {
        srand(n);
        VC(T)  vj;
        {
            VC(T)  vi;
            for (std::size_t i = 0; i < n; i += 1 ) {
                std::size_t ca = vi.capacity();
                vi.push_back(random_value_generator<T>());
                std::size_t cb = vi.capacity();
                if (ca < cb) {
                    std::cout << "capacity updated: " << ca << " -> " << cb << std::endl;
                }
            }
            {
                SPRINT("mass_swap") << "(" << n << ")";
                vj.swap(vi);
            }
            print_elements(vi.begin(), vi.end());
            print_elements(vj.begin(), vj.end());
        }
    }

    template <class T>
    void    mass_repeated_allocation(VC(T)::size_type n) {
        SPRINT("mass_repeated_allocation") << "(" << n << ")";
        VC(T) vi(n, random_value_generator<T>());
        print_elements(vi.begin(), vi.end());
        print_elements(vi.rbegin(), vi.rend());
        print_stats(vi);
    }

    template <class T>
    void    mass_range_allocation(VC(T)::size_type n) {
        srand(n);
        T *arr = new T[n];
        for (std::size_t i = 0; i < n; i += 1 ) {
            arr[i] = random_value_generator<T>();
        }
        {
            SPRINT("mass_range_allocation") << "(" << n << ")";
            VC(T) vj(arr, arr + n);
            print_elements(vj.begin(), vj.end());
            print_stats(vj);
        }
        delete[] arr;
    }

    template <class T>
    void    mass_insertion_1(VC(T)::size_type n) {
        srand(n);
        VC(T) vi;
        vi.push_back(random_value_generator<T>());
        {
            SPRINT("mass_insertion_1") << "(" << n << ")";
            for (VC(T)::size_type i = 1; i <= n; ++i) {
                VC(T)::size_type  at = VC(T)::size_type(double(rand()) / RAND_MAX * (vi.size() - 1)) + 1;
                VC(T)::iterator itd = vi.insert(vi.begin() + at, random_value_generator<T>());
                std::cout << "inserted at " << itd - vi.begin() << std::endl;
            }
        }
        print_elements(vi.begin(), vi.end());
        print_stats(vi);
    }

    template <class T>
    void    mass_insertion_n(VC(T)::size_type n) {
        srand(n);
        VC(T) vi;
        vi.push_back(random_value_generator<T>());
        {
            SPRINT("mass_insertion_n") << "(" << n << ")";
            for (VC(T)::size_type i = 1; i <= n; ++i) {
                vi.insert(vi.begin() + 1, i, random_value_generator<T>());
            }
        }
        print_elements(vi.begin(), vi.end());
        print_stats(vi);
    }

    template <class T>
    void    mass_insertion_range(VC(T)::size_type n, VC(T)::size_type m) {
        srand(n);
        VC(T) vi;
        for (VC(T)::size_type i = 0; i < m; ++i) {
            vi.push_back(random_value_generator<T>());
        }
        VC(T) vj;
        {
            SPRINT("mass_insertion_range") << "(" << n << ")";
            vj.insert(vj.end(), vi.begin(), vi.end());
            for (VC(T)::size_type i = 0; i < n; ++i) {
                VC(T)::size_type  at = VC(T)::size_type(double(rand()) / RAND_MAX * (vj.size() - 1)) + 1;
                vj.insert(vj.begin() + at, vi.begin(), vi.end());
            }
        }
        print_elements(vi.begin(), vi.end());
        print_stats(vj);
    }

    template <class T>
    void    mass_pop_back(VC(T)::size_type n) {
        srand(n);
        VC(T) vi;
        for (VC(T)::size_type i = 0; i < n; ++i) {
            vi.push_back(random_value_generator<T>());
        }
        print_elements(vi.begin(), vi.end());
        print_stats(vi);
        {
            SPRINT("mass_pop_back") << "(" << n << ")";
            for (VC(T)::size_type i = 0; i < n / 2; ++i) {
                vi.pop_back();
            }
            print_elements(vi.begin(), vi.end());
            print_stats(vi);
            for (VC(T)::size_type i = 0; i < n; ++i) {
                vi.push_back(random_value_generator<T>());
            }
            print_elements(vi.begin(), vi.end());
            print_stats(vi);
            VC(T)::size_type m = vi.size();
            for (VC(T)::size_type i = 0; i < m; ++i) {
                vi.pop_back();
            }
        }
        print_elements(vi.begin(), vi.end());
        print_stats(vi);
    }

    template <class T>
    void    mass_resize(VC(T)::size_type n) {
        SPRINT("mass_resize") << "(" << n << ")";
        srand(n);
        VC(T)  vi;
        for (VC(T)::size_type i = 0; i < 5; ++i) {
            VC(T)::size_type cap = vi.capacity();
            vi.resize((VC(T)::size_type)((double)(rand()) / RAND_MAX * n) + 1, random_value_generator<T>());
            std::cout << "cap: " << cap << " -> " << vi.capacity() << std::endl;
            print_elements(vi.begin(), vi.end());
            print_elements(vi.rbegin(), vi.rend());
            print_stats(vi);
        }
    }

    template <class T>
    void    mass_erase_1(VC(T)::size_type n) {
        srand(n);
        VC(T) vi;
        for (VC(T)::size_type i = 0; i < n; ++i) {
            vi.push_back(random_value_generator<T>());
        }
        print_elements(vi.begin(), vi.end());
        print_stats(vi);
        {
            SPRINT("mass_resize") << "(" << n << ")";
            for (VC(T)::size_type i = 0; i < n; ++i) {
                VC(T)::size_type  at = VC(T)::size_type(double(rand()) / RAND_MAX * vi.size());
                VC(T)::iterator eit = vi.erase(vi.begin() + at);
                std::cout << (eit - vi.begin()) << " / " << (eit == vi.end()) << std::endl;
            }
        }
        print_elements(vi.begin(), vi.end());
        print_stats(vi);
    }

    template <class T>
    void    mass_erase_range(VC(T)::size_type n) {
        SPRINT("mass_erase_range") << "(" << n << ")";
        srand(n);
        VC(T) vi;
        for (VC(T)::size_type t = 0; t < n / 100; ++t) {
            for (VC(T)::size_type i = vi.size(); i < n; ++i) {
                vi.push_back(random_value_generator<T>());
            }
            VC(T)::size_type from = VC(T)::size_type(double(rand()) / RAND_MAX * (vi.size() - 1));
            VC(T)::size_type to = VC(T)::size_type(double(rand()) / RAND_MAX * (vi.size() - from)) + from;
            std::cout << "erase [" << from << ", " << to << ")" << std::endl;
            VC(T)::iterator eit = vi.erase(vi.begin() + from, vi.begin() + to);
            std::cout << (eit - vi.begin()) << " / " << (eit == vi.end()) << std::endl;
            print_elements(vi.begin(), vi.end());
            print_stats(vi);
        }
    }

    template <class T>
    void    mass_compare(VC(T)::size_type n) {
        srand(n);
        VectorClass<VC(T)> vs;
        // DOUT() << "vs is " << &vs << std::endl;
        int N = 10;
        for (int i = 0; i < N; ++i) {
            VC(T) vi;
            // DOUT() << " ======== subvector go ================================================" << std::endl;
            // DOUT() << "vi is " << &vi << std::endl;
            for (VC(T)::size_type j = 0; j < n; ++j) {
                vi.push_back(random_value_generator<T>());
            }
            print_elements(vi.begin(), vi.end());
            // DOUT() << " ======= subvector done ===============================================" << std::endl;
            // DOUT() << "pushing back: " << i << std::endl;
            vs.push_back(vi);
            // DOUT() << "done." << std::endl;
        }
        {
            SPRINT("mass_compare") << "(" << n << ")";
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    VC(T)& vi = vs[i];
                    VC(T)& vj = vs[j];
                    std::cout << "==: " << (vi == vj) << std::endl;
                    std::cout << "!=: " << (vi != vj) << std::endl;
                    std::cout << "<=: " << (vi <= vj) << std::endl;
                    std::cout << "< : " << (vi < vj) << std::endl;
                    std::cout << ">=: " << (vi >= vj) << std::endl;
                    std::cout << "> : " << (vi > vj) << std::endl;
                }
            }
        }
        print_stats(vs, false);
    }

    template <class T>
    void    mass_test() {
        mass_assign<T>(100000);
        mass_assign_range<T>(30000);
        mass_assignation_eq<T>(100000);
        mass_swap<T>(100000);
        mass_range_allocation<T>(50000);
        mass_repeated_allocation<T>(10000000);
        mass_insertion_1<T>(100000);
        mass_insertion_n<T>(1000);
        mass_insertion_range<T>(1000, 1000);
        mass_pop_back<T>(100000);
        mass_resize<T>(100000);
        mass_erase_1<T>(100000);
        mass_erase_range<T>(10000);
    }
}

namespace test_string {
    typedef VectorClass<std::string>    Vec;
    void    print_elements(Vec::iterator from, Vec::iterator to) {
        std::cout << "[";
        for (Vec::iterator it = from; it < to; ++it) {
            if (it != from) {
                std::cout << ", ";
            }
            std::cout << *it;
        }
        std::cout << "]" << std::endl;
    }

    void    assignation() {
        Vec  vj;
        {
            Vec  vi;
            vi.push_back("a");
            vi.push_back("b");
            std::cout << "print vi" << std::endl;
            print_elements(vi.begin(), vi.end());
            std::cout << "print vj" << std::endl;
            print_elements(vj.begin(), vj.end());
            vj = vi;
            std::cout << "print vj" << std::endl;
            print_elements(vj.begin(), vj.end());
            vi.push_back("c");
            vi.push_back("d");
            std::cout << "print vi" << std::endl;
            print_elements(vi.begin(), vi.end());
            std::cout << "print vj" << std::endl;
            print_elements(vj.begin(), vj.end());
            vj = vi;
        }
        std::cout << "print vj" << std::endl;
        print_elements(vj.begin(), vj.end());
    }
}

int main() {

    // test_int::construct_and_reserve();
    // {
        // test_int::mass_assignation_eq(0);
        // test_int::mass_assignation_eq(1);
        // test_int::mass_assignation_eq(2);
        // test_int::mass_assignation_eq(3);
        // test_int::mass_range_allocation(0);
        // test_int::mass_range_allocation(1);
        // test_int::mass_range_allocation(2);
        // test_int::mass_range_allocation(3);
        // test_int::mass_repeated_allocation(0, 999);
        // test_int::mass_repeated_allocation(1, 999);
        // test_int::mass_repeated_allocation(2, 999);
        // test_int::mass_repeated_allocation(3, 999);
        // test_int::mass_insertion(0);
        // test_int::mass_insertion_n(0);
        // test_int::mass_insertion_n(1);
        // test_int::mass_insertion_n(2);
        // test_int::mass_insertion_n(3);
        // test_int::mass_insertion_n(4);
        // test_int::mass_insertion_range(0, 10);
        // test_int::mass_insertion_range(10, 0);
        // test_int::mass_insertion_range(1, 1);
        // test_int::mass_insertion_range(10, 10);
        // test_int::mass_insertion_range(100, 100);
        // test_int::mass_pop_back(0);
        // test_int::mass_pop_back(1);
        // test_int::mass_pop_back(2);

        // test_int::mass_erase_1(1);
        // test_int::mass_erase_1(100);

        // test_int::mass_resize(100000);

        // test_int::mass_erase_range(100000);

        // test_int::mass_assign(0);
        // test_int::mass_assign(1);
        // test_int::mass_assign(2);


        // test_int::mass_compare<int>(1000);

        // test_int::mass_assign(100000);
        // test_int::mass_assign_range(30000);
        test_int::mass_test<std::string>();
    // }
    ft::sprint::list();
}
