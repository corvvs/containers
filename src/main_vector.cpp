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
    print_vector_elements(vi.begin(), vi.end());
    print_vector_elements(vj.begin(), vj.end());
    std::cout << *ii << " - " << *ij << std::endl;
    {
        SPRINT("swap");
        vi.swap(vj);
    }
    print_vector_elements(vi.begin(), vi.end());
    print_vector_elements(vj.begin(), vj.end());
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
        print_vector_elements(vi.begin(), vi.end());
        print_vector_elements(vi.rbegin(), vi.rend());
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
            print_vector_elements(vi.begin(), vi.end());
            print_vector_elements(vi.rbegin(), vi.rend());
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
    print_vector_elements(vj.begin(), vj.end());
    print_vector_elements(vj.rbegin(), vj.rend());
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
        print_vector_elements(vi.begin(), vi.end());
        print_vector_elements(vj.begin(), vj.end());
    }
}

template <class T>
void    mass_repeated_allocation(VC(T)::size_type n) {
    SPRINT("mass_repeated_allocation") << "(" << n << ")";
    VC(T) vi(n, random_value_generator<T>());
    print_vector_elements(vi.begin(), vi.end());
    print_vector_elements(vi.rbegin(), vi.rend());
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
        print_vector_elements(vj.begin(), vj.end());
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
    print_vector_elements(vi.begin(), vi.end());
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
    print_vector_elements(vi.begin(), vi.end());
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
    print_vector_elements(vi.begin(), vi.end());
    print_stats(vj);
}

template <class T>
void    mass_pop_back(VC(T)::size_type n) {
    srand(n);
    VC(T) vi;
    for (VC(T)::size_type i = 0; i < n; ++i) {
        vi.push_back(random_value_generator<T>());
    }
    print_vector_elements(vi.begin(), vi.end());
    print_stats(vi);
    {
        SPRINT("mass_pop_back") << "(" << n << ")";
        for (VC(T)::size_type i = 0; i < n / 2; ++i) {
            vi.pop_back();
        }
        print_vector_elements(vi.begin(), vi.end());
        print_stats(vi);
        for (VC(T)::size_type i = 0; i < n; ++i) {
            vi.push_back(random_value_generator<T>());
        }
        print_vector_elements(vi.begin(), vi.end());
        print_stats(vi);
        VC(T)::size_type m = vi.size();
        for (VC(T)::size_type i = 0; i < m; ++i) {
            vi.pop_back();
        }
    }
    print_vector_elements(vi.begin(), vi.end());
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
        print_vector_elements(vi.begin(), vi.end());
        print_vector_elements(vi.rbegin(), vi.rend());
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
    print_vector_elements(vi.begin(), vi.end());
    print_stats(vi);
    {
        SPRINT("mass_resize") << "(" << n << ")";
        for (VC(T)::size_type i = 0; i < n; ++i) {
            VC(T)::size_type  at = VC(T)::size_type(double(rand()) / RAND_MAX * vi.size());
            VC(T)::iterator eit = vi.erase(vi.begin() + at);
            std::cout << (eit - vi.begin()) << " / " << (eit == vi.end()) << std::endl;
        }
    }
    print_vector_elements(vi.begin(), vi.end());
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
        print_vector_elements(vi.begin(), vi.end());
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
        print_vector_elements(vi.begin(), vi.end());
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
void    mass_test(const std::string& sub_title, std::size_t n) {
    ft::sprint::insert_comment(sub_title);
    mass_assign<T>(100 * n);
    mass_assign_range<T>(30 * n);
    mass_assignation_eq<T>(100 * n);
    mass_swap<T>(100 * n);
    mass_range_allocation<T>(50 * n);
    mass_repeated_allocation<T>(10000 * n);
    mass_insertion_1<T>(100 * n);
    mass_insertion_n<T>(n);
    mass_insertion_range<T>(n, n);
    mass_pop_back<T>(100 * n);
    mass_resize<T>(100 * n);
    mass_erase_1<T>(100 * n);
    mass_erase_range<T>(10 * n);
}

namespace fill {
    void    constructors() {
        VectorClass<int> vv((20), 1234);

        {
            SPRINT("fill::constructors default");
            VectorClass<int> v;
            print_stats(v);
        }
        {
            SPRINT("fill::constructors size");
            VectorClass<int> v((10));
            print_stats(v);
        }
        {
            SPRINT("fill::constructors allocator");
            VectorClass<int> v((std::allocator<int>()));
            print_stats(v);
        }
        {
            SPRINT("fill::constructors size & default value");
            VectorClass<int> v((10), 999);
            print_stats(v);
            DSOUT() << v.at(0) << std::endl;
        }
        {
            SPRINT("fill::constructors size & default value & allocator");
            VectorClass<int> v((10), 888, std::allocator<int>());
            print_stats(v);
            DSOUT() << v.at(0) << std::endl;
        }

        {
            SPRINT("fill::constructors range");
            VectorClass<int> v((vv.begin() + 10), vv.end());
            print_stats(v);
            DSOUT() << v.at(0) << std::endl;
        }

        {
            SPRINT("fill::constructors range & allocator, and operator=");
            VectorClass<int> v((vv.begin() + 5), (vv.end() - 5), std::allocator<int>());
            print_stats(v);
            DSOUT() << v.at(0) << std::endl;
            v = vv;
            print_stats(v);
            DSOUT() << v.at(0) << std::endl;
        }

        {
            SPRINT("fill::constructors copy");
            VectorClass<int> v((vv));
            print_stats(v);
            DSOUT() << v.at(0) << std::endl;
        }
    }

    void    assign() {
        VectorClass<int>    vv(10, 1234);
        {
            SPRINT("fill::assign size & value");
            VectorClass<int>    v;
            v.assign(10, 20);
        }
        {
            SPRINT("fill::assign range");
            VectorClass<int>    v;
            v.assign(vv.begin() + 4, vv.end() - 4);
        }
        {
            SPRINT("fill::assign pointer & pointer");
            VectorClass<int>    v;
            v.assign(&vv[1], &vv[5]);
        }
    }

    void    at(int n) {
        SPRINT("fill::at") << "(" << n << ")";
        VectorClass<int> v;
        for (int i = 0; i < n; ++i) {
            v.push_back(i);
        }
        DSOUT() << v.at(0) << std::endl;
        DSOUT() << v.at(n - 1) << std::endl;
        v.at(n - 1) = -100;
        DSOUT() << v.at(n - 1) << std::endl;
        try {
            DSOUT() << v.at(n) << std::endl;
        } catch(std::out_of_range& e) {
            DSOUT() << "exception: " << e.what() << std::endl;
        }
    }

    void    assign(int n) {
        SPRINT("fill::assign") << "(" << n << ")";
        VectorClass<int> v1(n);
        VectorClass<int> v2(0);
        VectorClass<int> v3(0);

        print_stats(v1);
        print_stats(v2);
        print_stats(v3);
        v2 = v1;
        print_stats(v2);
        v2 = v3;
        print_stats(v2);
    }
}

namespace logic {
    void    equal_same_size(int n) {
        VectorClass<int> v1(n);
        VectorClass<int> v2(n);
        v2.end()[-1] += 1;
        {
            SPRINT("logic::equal_same_size") << "(" << n << ")";
            DSOUT() << (v1 == v2) << std::endl;
        }
    }

    void    equal_diff_size(int n) {
        VectorClass<int> v1(n);
        VectorClass<int> v2(n + 1);
        {
            SPRINT("logic::equal_diff_size") << "(" << n << ")";
            DSOUT() << (v1 == v2) << std::endl;
        }
    }

    void    strong_insertion() {
        // push_back の例外安全性をテストする
        SPRINT("logic::strong_insertion");
        VectorClass<ft::IntWrapper> v(2);
        try {
            print_stats(v);
            ft::IntWrapper::set_limit(1);
            v.push_back(ft::IntWrapper(2));
        } catch(std::runtime_error& e) {
            DSOUT() << "exception: " << e.what() << std::endl;
        }
        ft::IntWrapper::set_limit(-1);
        print_stats(v);
    }

    void    iterator_for_empty_container() {
        VectorClass<int> v1;
        VectorClass<int> v2;

        DSOUT() << (v1.begin() == v1.end()) << std::endl;
        DSOUT() << (v2.begin() == v2.end()) << std::endl;
        DSOUT() << (v1.end() == v2.end()) << std::endl;
        v2.push_back(1);
        DSOUT() << (v1.begin() == v1.end()) << std::endl;
        DSOUT() << (v2.begin() == v2.end()) << std::endl;
        DSOUT() << (v1.end() == v2.end()) << std::endl;
        v2.pop_back();
        DSOUT() << (v1.begin() == v1.end()) << std::endl;
        DSOUT() << (v2.begin() == v2.end()) << std::endl;
        DSOUT() << (v1.end() == v2.end()) << std::endl;
    }
}

#include "blank.hpp"

int main() {
    // mass_test<int>("[int]", 1000);
    // mass_test<ft::IntWrapper>("[ft::IntWrapper]", 100);
    // mass_test<std::string>("[std::string]", 100);
    // mass_test<std::vector<int> >("[std::vector<int>]", 20);
    // mass_test<ft::vector<int> >("[ft::vector<int>]", 20);

    fill::constructors();
    fill::assign();
    fill::at(10);

    logic::equal_same_size(100);
    logic::equal_same_size(1000);
    logic::equal_same_size(10000);
    logic::equal_same_size(100000);
    logic::equal_diff_size(100);
    logic::equal_diff_size(1000);
    logic::equal_diff_size(10000);
    logic::equal_diff_size(100000);
    logic::iterator_for_empty_container();
    logic::strong_insertion();

    {
        VectorClass< ft::blank<int> >   v((0));
    }

    ft::sprint::list();
}
