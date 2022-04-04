#include "test_common.hpp"

namespace fill {
    void    constructor() {
        SPRINT("fill::constructor");
        PairClass<int, int> pair1(10, 20);
        DSOUT() << pair1 << std::endl;
        PairClass<std::string, double> pair2("hello 42", 42);
        DSOUT() << pair2 << std::endl;
        PairClass<std::string, double> pair3 = pair2;
        DSOUT() << pair3 << std::endl;
        PairClass<std::string, double> pair4;
        DSOUT() << pair4 << std::endl;
        pair4 = pair3;
        DSOUT() << pair4 << std::endl;

        DSOUT() << (pair3 == pair4) << std::endl;

        PairClass<double, unsigned int> pair5(pair1.first, pair1.second);
        DSOUT() << pair5 << std::endl;

        PairClass<const std::string, double> pair6 = pair4;
        PairClass<const std::string, double> pair7 = pair6;
        DSOUT() << (pair6 == pair7) << std::endl;
        DSOUT() << std::equal_to< PairClass<const std::string, double> >()(pair6, pair7) << std::endl;
    }

    void    make_pair() {
        SPRINT("fill::make_pair");
        PairClass<int, int> pair1 = NS::make_pair(10, 20);
        PairClass<double, double> pair2 = NS::make_pair(pair1.first, pair1.second);
        DSOUT() << pair2 << std::endl;
    }

    void    swap() {
        SPRINT("fill::swap");
        typedef SetClass<int>                   set_type;
        typedef MapClass<int, int>              map_type;
        typedef PairClass< set_type, map_type > pair_type;
        set_type    s1;
        set_type    s2;
        s1.insert(100);
        s1.insert(150);
        s2.insert(200);
        s2.insert(250);
        map_type    m1;
        map_type    m2;
        m1[300] = 400;
        m1[350] = 450;
        m2[500] = 600;
        m2[550] = 650;
        pair_type   p1(s1, m1);
        pair_type   p2(s2, m2);
        DSOUT() << p1 << std::endl;
        DSOUT() << p2 << std::endl;
        ft::swap(p1, p2);
        DSOUT() << p1 << std::endl;
        DSOUT() << p2 << std::endl;
        std::swap(p1, p2);
        DSOUT() << p1 << std::endl;
        DSOUT() << p2 << std::endl;
        p1.swap(p2);
        DSOUT() << p1 << std::endl;
        DSOUT() << p2 << std::endl;
    }

    void    comparation() {
        SPRINT("fill::comparation");
        typedef PairClass<int, int> st;
        typedef VectorClass<st>     vt;
        vt    v;
        int n = 3;
        for (int i = 0; i < (1 << (n + 2)); ++i) {
            st  s(i % n, rand() % n);
            v.push_back(s);
        }
        for (vt::size_type i = 0; i < v.size(); ++i) {
            for (vt::size_type j = 0; j < v.size(); ++j) {
                DSOUT() << "(" << i << " vs " << j << ") "
                        << "==:" << (v[i] == v[j]) << " "
                        << "!=:" << (v[i] != v[j]) << " "
                        << "<:" << (v[i] < v[j]) << " "
                        << "<=:" << (v[i] <= v[j]) << " "
                        << ">:" << (v[i] > v[j]) << " "
                        << ">=:" << (v[i] >= v[j]) << std::endl;
            }
        }
    }

    void    test() {
        constructor();
        make_pair();
        swap();
        comparation();
    }
}

int main() {
    fill::test();
    ft::sprint::list();
}
