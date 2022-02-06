#include "test_vector.hpp"

template<class T>
void    print_stats(VectorClass<T> &v) {
    std::cout << "capacity:     " << v.capacity() << std::endl;
    std::cout << "size:         " << v.size() << std::endl;
    std::cout << "max_size:     " << v.max_size() << std::endl;
    std::cout << "empty:        " << v.empty() << std::endl;
}

namespace test_int {
    typedef VectorClass<int>    Vec;
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

    // 構築とreserve
    void    construct_and_reserve() {
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
        vi.swap(vj);
        print_elements(vi.begin(), vi.end());
        print_elements(vj.begin(), vj.end());
        std::cout << *ii << " - " << *ij << std::endl;
    }

    void    mass_assignation(Vec::size_type n) {
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
            std::cerr << "( assign )" << std::endl;
            vj = vi;
            std::cerr << "( assign over )" << std::endl;
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
        print_stats(vj);
    }

    void    mass_swap(Vec::size_type n) {
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
            vj.swap(vi);
            print_elements(vi.begin(), vi.end());
            print_elements(vj.begin(), vj.end());
        }
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
    srand(42424242);

    // test_int::construct_and_reserve();
    // {
    //     test_int::mass_assignation(0);
    //     test_int::mass_assignation(1);
    //     test_int::mass_assignation(2);
        // test_int::mass_assignation(3);
        // test_int::mass_assignation(10000000);
        test_int::mass_swap(10000000);
    // }
}
