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
    typedef VectorClass<int>    Vec;
    void    print_elements(Vec::iterator from, Vec::iterator to) {
        std::cout << "[";
        for (Vec::iterator it = from; it != to; ++it) {
            if (it != from) {
                std::cout << ", ";
            }
            std::cout << *it;
        }
        std::cout << "]" << std::endl;
    }
    void    print_elements(Vec::reverse_iterator from, Vec::reverse_iterator to) {
        std::cout << "[";
        for (Vec::reverse_iterator it = from; it != to; ++it) {
            if (it != from) {
                std::cout << ", ";
            }
            std::cout << *it;
        }
        std::cout << "]" << std::endl;
    }

    // 構築とreserve
    void    construct_and_reserve() {
        SPRINT();
        Vec  vi;
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
    void    swap() {
        Vec  vi;
        vi.push_back(1);
        vi.push_back(2);
        Vec  vj;
        vj.push_back(3);
        vj.push_back(4);
        Vec::iterator ii = vi.begin();
        Vec::iterator ij = vj.begin();
        print_elements(vi.begin(), vi.end());
        print_elements(vj.begin(), vj.end());
        std::cout << *ii << " - " << *ij << std::endl;
        {
            SPRINT();
            vi.swap(vj);
        }
        print_elements(vi.begin(), vi.end());
        print_elements(vj.begin(), vj.end());
        std::cout << *ii << " - " << *ij << std::endl;
    }

    void    mass_assign(Vec::size_type n) {
        SPRINT() << "(" << n << ")";
        srand(n);
        Vec  vi;
        for (Vec::size_type i = 0; i < 5; ++i) {
            Vec::size_type cap = vi.capacity();
            vi.assign((Vec::size_type)((double)(rand()) / RAND_MAX * n) + 1, rand());
            std::cout << "cap: " << cap << " -> " << vi.capacity() << std::endl;
            print_elements(vi.begin(), vi.end());
            print_elements(vi.rbegin(), vi.rend());
            print_stats(vi);
        }
    }

    void    mass_assign_range(Vec::size_type n) {
        srand(n);
        Vec vj;
        for (Vec::size_type i = 0; i < n; ++i) {
            vj.push_back(rand());
        }
        Vec vi;
        {
            SPRINT() << "(" << n << ")";
            for (Vec::size_type i = 0; i <= n / 100; ++i) {
                Vec::size_type from = Vec::size_type(double(rand()) / RAND_MAX * (vj.size() - 1));
                Vec::size_type to = Vec::size_type(double(rand()) / RAND_MAX * (vj.size() - from)) + from;
                std::cout << "insert [" << from << ", " << to << "), " << (to - from) << std::endl;
                Vec::size_type cap = vi.capacity();
                vi.assign(vj.begin() + from, vj.begin() + to);
                std::cout << "cap: " << cap << " -> " << vi.capacity() << std::endl;
                print_elements(vi.begin(), vi.end());
                print_elements(vi.rbegin(), vi.rend());
                print_stats(vi);
            }
        }
    }

    void    mass_assignation_eq(Vec::size_type n) {
        srand(n);
        Vec  vj;
        {
            SPRINT() << "(" << n << ")";
            Vec  vi;
            for (std::size_t i = 0; i < n; i += 1 ) {
                std::size_t ca = vi.capacity();
                vi.push_back(rand());
                std::size_t cb = vi.capacity();
                if (ca < cb) {
                    std::cout << "capacity updated: " << ca << " -> " << cb << std::endl;
                }
            }
            vj = vi;
            print_stats(vi);
            for (std::size_t i = 0; i < n; i += 1 ) {
                std::size_t ca = vj.capacity();
                vj.push_back(rand());
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

    void    mass_swap(Vec::size_type n) {
        srand(n);
        Vec  vj;
        {
            Vec  vi;
            for (std::size_t i = 0; i < n; i += 1 ) {
                std::size_t ca = vi.capacity();
                vi.push_back(rand());
                std::size_t cb = vi.capacity();
                if (ca < cb) {
                    std::cout << "capacity updated: " << ca << " -> " << cb << std::endl;
                }
            }
            {
                SPRINT() << "(" << n << ")";
                vj.swap(vi);
            }
            print_elements(vi.begin(), vi.end());
            print_elements(vj.begin(), vj.end());
        }
    }

    void    mass_repeated_allocation(Vec::size_type n, const Vec::value_type& i) {
        SPRINT() << "(" << n << ")";
        Vec vi(n, i);
        print_elements(vi.begin(), vi.end());
        print_elements(vi.rbegin(), vi.rend());
        print_stats(vi);
    }

    void    mass_range_allocation(Vec::size_type n) {
        srand(n);
        int *arr = new int[n];
        for (std::size_t i = 0; i < n; i += 1 ) {
            arr[i] = (rand());
        }
        {
            SPRINT() << "(" << n << ")";
            Vec vj(arr, arr + n);
            print_elements(vj.begin(), vj.end());
            print_stats(vj);
        }
        delete[] arr;
    }

    void    mass_insertion_1(Vec::size_type n) {
        srand(n);
        Vec vi;
        vi.push_back(0);
        {
            SPRINT() << "(" << n << ")";
            for (Vec::size_type i = 1; i <= n; ++i) {
                Vec::size_type  at = Vec::size_type(double(rand()) / RAND_MAX * (vi.size() - 1)) + 1;
                Vec::iterator itd = vi.insert(vi.begin() + at, i);
                std::cout << "inserted at " << itd - vi.begin() << std::endl;
            }
        }
        print_elements(vi.begin(), vi.end());
        print_stats(vi);
    }

    void    mass_insertion_n(Vec::size_type n) {
        srand(n);
        Vec vi;
        vi.push_back(rand());
        {
            SPRINT() << "(" << n << ")";
            for (Vec::size_type i = 1; i <= n; ++i) {
                vi.insert(vi.begin() + 1, i, i);
            }
        }
        print_elements(vi.begin(), vi.end());
        print_stats(vi);
    }

    void    mass_insertion_range(Vec::size_type n, Vec::size_type m) {
        srand(n);
        Vec vi;
        for (Vec::size_type i = 0; i < m; ++i) {
            vi.push_back(i);
        }
        Vec vj;
        {
            SPRINT() << "(" << n << ")";
            vj.insert(vj.end(), vi.begin(), vi.end());
            for (Vec::size_type i = 0; i < n; ++i) {
                Vec::size_type  at = Vec::size_type(double(rand()) / RAND_MAX * (vj.size() - 1)) + 1;
                vj.insert(vj.begin() + at, vi.begin(), vi.end());
            }
        }
        print_elements(vi.begin(), vi.end());
        print_stats(vj);
    }

    void    mass_pop_back(Vec::size_type n) {
        srand(n);
        Vec vi;
        for (Vec::size_type i = 0; i < n; ++i) {
            vi.push_back(rand());
        }
        print_elements(vi.begin(), vi.end());
        print_stats(vi);
        {
            SPRINT() << "(" << n << ")";
            for (Vec::size_type i = 0; i < n / 2; ++i) {
                vi.pop_back();
            }
            print_elements(vi.begin(), vi.end());
            print_stats(vi);
            for (Vec::size_type i = 0; i < n; ++i) {
                vi.push_back(rand());
            }
            print_elements(vi.begin(), vi.end());
            print_stats(vi);
            Vec::size_type m = vi.size();
            for (Vec::size_type i = 0; i < m; ++i) {
                vi.pop_back();
            }
        }
        print_elements(vi.begin(), vi.end());
        print_stats(vi);
    }

    void    mass_resize(Vec::size_type n) {
        SPRINT() << "(" << n << ")";
        srand(n);
        Vec  vi;
        for (Vec::size_type i = 0; i < 5; ++i) {
            Vec::size_type cap = vi.capacity();
            vi.resize((Vec::size_type)((double)(rand()) / RAND_MAX * n) + 1, rand());
            std::cout << "cap: " << cap << " -> " << vi.capacity() << std::endl;
            print_elements(vi.begin(), vi.end());
            print_elements(vi.rbegin(), vi.rend());
            print_stats(vi);
        }
    }

    void    mass_erase_1(Vec::size_type n) {
        srand(n);
        Vec vi;
        for (Vec::size_type i = 0; i < n; ++i) {
            vi.push_back(i);
        }
        print_elements(vi.begin(), vi.end());
        print_stats(vi);
        {
            SPRINT() << "(" << n << ")";
            for (Vec::size_type i = 0; i < n; ++i) {
                Vec::size_type  at = Vec::size_type(double(rand()) / RAND_MAX * vi.size());
                Vec::iterator eit = vi.erase(vi.begin() + at);
                std::cout << (eit - vi.begin()) << " / " << (eit == vi.end()) << std::endl;
            }
        }
        print_elements(vi.begin(), vi.end());
        print_stats(vi);
    }

    void    mass_erase_range(Vec::size_type n) {
        SPRINT() << "(" << n << ")";
        srand(n);
        Vec vi;
        for (Vec::size_type t = 0; t < n / 100; ++t) {
            for (Vec::size_type i = vi.size(); i < n; ++i) {
                vi.push_back(rand());
            }
            Vec::size_type from = Vec::size_type(double(rand()) / RAND_MAX * (vi.size() - 1));
            Vec::size_type to = Vec::size_type(double(rand()) / RAND_MAX * (vi.size() - from)) + from;
            std::cout << "erase [" << from << ", " << to << ")" << std::endl;
            Vec::iterator eit = vi.erase(vi.begin() + from, vi.begin() + to);
            std::cout << (eit - vi.begin()) << " / " << (eit == vi.end()) << std::endl;
            print_elements(vi.begin(), vi.end());
            print_stats(vi);
        }
    }

    void    mass_compare(Vec::size_type n) {
        srand(n);
        VectorClass<Vec> vs;
        // DOUT() << "vs is " << &vs << std::endl;
        int N = 10;
        for (int i = 0; i < N; ++i) {
            Vec vi;
            // DOUT() << " ======== subvector go ================================================" << std::endl;
            // DOUT() << "vi is " << &vi << std::endl;
            for (Vec::size_type j = 0; j < n; ++j) {
                vi.push_back(rand());
            }
            print_elements(vi.begin(), vi.end());
            // DOUT() << " ======= subvector done ===============================================" << std::endl;
            // DOUT() << "pushing back: " << i << std::endl;
            vs.push_back(vi);
            // DOUT() << "done." << std::endl;
        }
        {
            SPRINT() << "(" << n << ")";
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    Vec& vi = vs[i];
                    Vec& vj = vs[j];
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

    void    mass_test() {
        test_int::mass_assign(100000);
        test_int::mass_assign_range(30000);
        test_int::mass_assignation_eq(100000);
        test_int::mass_swap(100000);
        test_int::mass_range_allocation(50000);
        test_int::mass_repeated_allocation(10000000, 999);
        test_int::mass_insertion_1(100000);
        test_int::mass_insertion_n(1000);
        test_int::mass_insertion_range(1000, 1000);
        test_int::mass_pop_back(100000);
        test_int::mass_resize(100000);
        test_int::mass_erase_1(100000);
        test_int::mass_erase_range(10000);
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


        test_int::mass_compare(1000);

        // test_int::mass_assign(100000);
        // test_int::mass_assign_range(30000);
        test_int::mass_test();
    // }
    ft::sprint::list();
}
