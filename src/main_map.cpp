#include <iostream>
#include <string>
#include <list>
#include "test_common.hpp"

// 各機能を1つずつ潰していくテスト。
namespace fill {
    typedef MapClass<int, int>          map_type;
    typedef map_type::iterator          iterator;
    typedef map_type::const_iterator    const_iterator;

    // デフォルトコンストラクタ
    void    constructor_default(int n) {
        SPRINT("constructor_default") << "(" << n << ")";
        map_type    m;
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            m[rand()] = rand();
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    // コンパレータ指定コンストラクタ
    void    constructor_comparator(int n) {
        SPRINT("constructor_comparator") << "(" << n << ")";
        MapClass<int, int, std::greater<int> >    m((std::greater<int>()));
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            m[rand()] = rand();
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    // コンパレータ・アロケータ指定コンストラクタ
    void    constructor_comparator_allocator(int n) {
        SPRINT("constructor_comparator_allocator") << "(" << n << ")";
        MapClass<int, int>    m(
            (std::less<int>()), std::allocator<map_type::value_type>()
        );
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            m[rand()] = rand();
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    // イテレータ指定コンストラクタ
    void    constructor_iterator(int n) {
        SPRINT("constructor_iterator") << "(" << n << ")";
        VectorClass< PairClass<int, int> >  v;
        for (int i = 0; i < n; ++i) {
            v.push_back(NS::make_pair(rand(), rand()));
        }
        MapClass<int, int>    m((v.begin()), v.end());
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    // コピーコンストラクタ
    void    constructor_copy(int n) {
        SPRINT("constructor_copy") << "(" << n << ")";
        map_type    m;
        for (int i = 0; i < n; ++i) {
            m[rand()] = rand();
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        {
            map_type    mm((m));
            DSOUT() << mm.empty() << std::endl;
            DSOUT() << mm.size() << std::endl;
            DSOUT() << (mm.begin() == mm.end()) << std::endl;
        }
        m[rand()] = rand();
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    oprator_assignation(int n) {
        SPRINT("oprator_assignation") << "(" << n << ")";
        map_type    m;
        for (int i = 0; i < n; ++i) {
            m[rand()] = rand();
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        {
            map_type    mm;
            DSOUT() << mm.empty() << std::endl;
            DSOUT() << mm.size() << std::endl;
            DSOUT() << (mm.begin() == mm.end()) << std::endl;
            mm = m;
            DSOUT() << mm.empty() << std::endl;
            DSOUT() << mm.size() << std::endl;
            DSOUT() << (mm.begin() == mm.end()) << std::endl;
        }
        m[rand()] = rand();
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    get_allocator() {
        SPRINT("get_allocator");
        map_type    m;
        std::allocator<PairClass<int, int> >  alloc;
        DSOUT() << (m.get_allocator() == alloc) << std::endl;
    }

    void    key_comp() {
        SPRINT("key_comp");
        map_type    m;
        DSOUT() << m.key_comp()(1, 1) << std::endl;
        DSOUT() << m.key_comp()(0, 1) << std::endl;
        DSOUT() << m.key_comp()(1, 0) << std::endl;
    }

    void    value_comp() {
        SPRINT("value_comp");
        map_type    m;
        DSOUT() << m.value_comp()(NS::make_pair(1, 1), NS::make_pair(1, -10000)) << std::endl;
        DSOUT() << m.value_comp()(NS::make_pair(1, 1), NS::make_pair(2, -10000)) << std::endl;
        DSOUT() << m.value_comp()(NS::make_pair(2, -10000), NS::make_pair(3, 1)) << std::endl;
    }

    void    begin_end_variable() {
        SPRINT("begin_end_variable");
        map_type    m;
        DSOUT() << (m.begin() == m.end());
        m[1] = 10;
        DSOUT() << (m.begin() == m.end());
        map_type::iterator  it = m.end();
        --it;
        DSOUT() << (m.begin() == m.end());
        DSOUT() << *(m.begin()) << std::endl;
        DSOUT() << *it << std::endl;
        it++;
        DSOUT() << *it << std::endl;
        it--;
        DSOUT() << *it << std::endl;
        it->second = 1000;
        DSOUT() << *it << std::endl;
        DSOUT() << *(m.begin()) << std::endl;
        // it->first = 0; // error: cannot assign to non-static data member 'first' with const-qualified type 'ft::pair<const int, int>::first_type' (aka 'const int')
        ++it;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    clear(int n) {
        SPRINT("clear") << "(" << n << ")";
        map_type    m;
        for (int i = 0; i < n; ++i) {
            m[rand()] = rand();
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        m.clear();
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            m[rand()] = rand();
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    swap(int n) {
        SPRINT("swap") << "(" << n << ")";
        map_type    m;
        for (int i = 0; i < n; ++i) {
            m[rand()] = rand();
        }
        {
            map_type    mm;
            for (int i = 0; i < n; ++i) {
                mm[rand()] = rand();
            }
            DSOUT() << m.empty() << std::endl;
            DSOUT() << m.size() << std::endl;
            DSOUT() << (m.begin() == m.end()) << std::endl;
            DSOUT() << *(m.begin()) << std::endl;
            DSOUT() << mm.empty() << std::endl;
            DSOUT() << mm.size() << std::endl;
            DSOUT() << (mm.begin() == m.end()) << std::endl;
            DSOUT() << *(mm.begin()) << std::endl;
            std::swap(m, mm);
            DSOUT() << mm.empty() << std::endl;
            DSOUT() << mm.size() << std::endl;
            DSOUT() << (mm.begin() == m.end()) << std::endl;
            DSOUT() << *(mm.begin()) << std::endl;
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        DSOUT() << *(m.begin()) << std::endl;
    }

    void    insert_value(int n) {
        SPRINT("insert_value") << "(" << n << ")";
        map_type    m;
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            PairClass<map_type::iterator, bool> result = m.insert(NS::make_pair(rand(), rand()));
            DSOUT() << result.second << ", " << result.first->first << ", " << result.first->second << std::endl;
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        DSOUT() << *(m.begin()) << std::endl;
    }

    void    insert_value_with_hint(int n) {
        SPRINT("insert_value_with_hint") << "(" << n << ")";
        map_type    m;
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        map_type::iterator it = m.begin();
        for (int i = 0; i < n; ++i) {
            it = m.insert(it, NS::make_pair(rand(), rand()));
            DSOUT() << *it << std::endl;
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        DSOUT() << *(m.begin()) << std::endl;
    }

    void    insert_value_with_range(int n) {
        SPRINT("insert_value_with_range") << "(" << n << ")";
        std::list<NS::pair<int, int> >  l;
        for (int i = 0; i < n; ++i) {
            l.push_back(NS::make_pair(rand(), rand()));
        }
        map_type    m;
        m.insert(l.begin(), l.end());
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        DSOUT() << *(m.begin()) << std::endl;
    }

    void    erase_by_position(int n) {
        SPRINT("erase_by_position") << "(" << n << ")";
        map_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(NS::make_pair(rand() % n, rand()));
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            map_type::iterator  it = m.find(rand() % n);
            if (it != m.end()) {
                m.erase(it);
            }
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    erase_by_key(int n) {
        SPRINT("erase_by_key") << "(" << n << ")";
        map_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(NS::make_pair(rand() % n, rand()));
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            int key = rand() % n;
            DSOUT() << key << " -> " << m.erase(key) << std::endl;
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    erase_by_range(int n) {
        SPRINT("erase_by_key") << "(" << n << ")";
        map_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(NS::make_pair(rand() % n, rand()));
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        map_type::iterator  b = m.begin();
        map_type::iterator  e = m.end();
        m.erase(++(++b), --(--e));
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    find_variable(int n) {
        SPRINT("find_variable") << "(" << n << ")";
        map_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(NS::make_pair(rand() % n, rand()));
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            map_type::iterator  it = m.find(rand() % n);
            if (it != m.end()) {
                it->second = rand();
            }
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    find_constant(int n) {
        SPRINT("find_constant") << "(" << n << ")";
        map_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(NS::make_pair(rand() % n, rand()));
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            map_type::const_iterator  it = m.find(rand() % n);
            if (it != m.end()) {
                DSOUT() << *it << std::endl;
            }
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    equal_range(int n) {
        SPRINT("equal_range") << "(" << n << ")";
        map_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(NS::make_pair(rand() % n, rand()));
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n * 5; ++i) {
            NS::pair<map_type::iterator, map_type::iterator>    range = m.equal_range(rand() % n);
            if (range.first == range.second) {
                // not found
                if (range.first == m.end()) {
                    DSOUT() << "not found and end" << std::endl;
                } else {
                    DSOUT() << "not found: " << range.first->first << ", " << range.first->second << std::endl;
                }
            } else {
                if (range.second == m.end()) {
                    DSOUT() << "found: " << range.first->first << " - end" << std::endl;
                } else {
                    DSOUT() << "found: " << range.first->first << " - " << range.second->first << std::endl;
                }
            }
        }
    }

    void    lower_bound(int n) {
        SPRINT("lower_bound") << "(" << n << ")";
        map_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(NS::make_pair(rand() % n, rand()));
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n * 5; ++i) {
            int key = rand() % n;
            map_type::iterator    it = m.lower_bound(key);
            if (it == m.end()) {
                DSOUT() << "found: end" << std::endl;
            } else {
                DSOUT() << "found: " << key << " -> " << *it << std::endl;
            }
            map_type::const_iterator    cit = m.lower_bound(key);
            if (cit == m.end()) {
                DSOUT() << "found: end" << std::endl;
            } else {
                DSOUT() << "found: " << key << " -> " << *cit << std::endl;
            }
        }
    }

    void    upper_bound(int n) {
        SPRINT("upper_bound") << "(" << n << ")";
        map_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(NS::make_pair(rand() % n, rand()));
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n * 5; ++i) {
            int key = rand() % n;
            map_type::iterator    it = m.upper_bound(key);
            if (it == m.end()) {
                DSOUT() << "found: end" << std::endl;
            } else {
                DSOUT() << "found: " << key << " -> " << *it << std::endl;
            }
            map_type::const_iterator    cit = m.upper_bound(key);
            if (cit == m.end()) {
                DSOUT() << "found: end" << std::endl;
            } else {
                DSOUT() << "found: " << key << " -> " << *cit << std::endl;
            }
        }
    }

    void    comparation() {
        SPRINT("comparation");
        typedef MapClass<int, int>   st;
        typedef VectorClass<st> vt;
        vt    v;
        int n = 3;
        for (int i = 0; i < (1 << (n + 2)); ++i) {
            st  s;
            for (int j = 0; j < n; ++j) {
                if ((1 << j) & i) {
                    s[j] = i % 3;
                }
            }
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

    void    iterator_comparation() {
        SPRINT("iterator_comparation");
        typedef MapClass<int, int>  st;
        st                          s;
        s[1] = 1;
        s[2] = 2;
        st::iterator        vit1 = s.begin();
        st::iterator        vit2 = s.begin();
        ++vit2;
        st::const_iterator  cit1 = s.begin();
        st::const_iterator  cit2 = s.begin();
        ++cit2;
        DSOUT() << (vit1 == vit1) << " " << (vit1 != vit1) << std::endl;
        DSOUT() << (vit1 == vit2) << " " << (vit1 != vit2) << std::endl;
        DSOUT() << (vit1 == cit1) << " " << (vit1 != cit1) << std::endl;
        DSOUT() << (vit1 == cit2) << " " << (vit1 != cit2) << std::endl;
        DSOUT() << (vit2 == vit1) << " " << (vit2 != vit1) << std::endl;
        DSOUT() << (vit2 == vit2) << " " << (vit2 != vit2) << std::endl;
        DSOUT() << (vit2 == cit1) << " " << (vit2 != cit1) << std::endl;
        DSOUT() << (vit2 == cit2) << " " << (vit2 != cit2) << std::endl;
        DSOUT() << (cit1 == vit1) << " " << (cit1 != vit1) << std::endl;
        DSOUT() << (cit1 == vit2) << " " << (cit1 != vit2) << std::endl;
        DSOUT() << (cit1 == cit1) << " " << (cit1 != cit1) << std::endl;
        DSOUT() << (cit1 == cit2) << " " << (cit1 != cit2) << std::endl;
        DSOUT() << (cit2 == vit1) << " " << (cit2 != vit1) << std::endl;
        DSOUT() << (cit2 == vit2) << " " << (cit2 != vit2) << std::endl;
        DSOUT() << (cit2 == cit1) << " " << (cit2 != cit1) << std::endl;
        DSOUT() << (cit2 == cit2) << " " << (cit2 != cit2) << std::endl;
    }

    void    test() {
        constructor_default(100);
        constructor_comparator(100);
        constructor_comparator_allocator(100);
        constructor_iterator(100);
        constructor_copy(100);
        oprator_assignation(100);
        get_allocator();
        key_comp();
        value_comp();
        begin_end_variable();
        clear(100);
        swap(100);
        insert_value(100);
        insert_value_with_hint(100);
        insert_value_with_range(100);
        erase_by_position(100);
        erase_by_key(100);
        erase_by_range(100);
        find_variable(100);
        find_constant(100);
        equal_range(100);
        lower_bound(100);
        upper_bound(100);
        comparation();
        iterator_comparation();
    }
}

namespace logic {
    typedef std::string             Key;
    typedef int                     Value;
    typedef MapClass<Key, Value>    set_type;

    template <class T>
    struct size_compare {
        bool    operator()(const T& lhs, const T& rhs) const {
            return lhs.size() < rhs.size();
        }
    };

    // - 特殊な comparatorを 使用できること
    // - comparatorにより挿入済みと判定された場合は挿入を行わないこと
    void    specify_comparator() {
        SPRINT("specify_comparator");
        MapClass<Key, Value, size_compare<Key> >     m;
        m.insert(NS::make_pair("apple", 1));
        DSOUT() << *(m.begin()) << std::endl;
        DSOUT() << m.size() << std::endl;
        m.insert(NS::make_pair("hello", 86));
        DSOUT() << *(m.begin()) << std::endl;
        DSOUT() << m.size() << std::endl;
        m.insert(NS::make_pair("42", 42));
        DSOUT() << *(m.begin()) << std::endl;
        DSOUT() << m.size() << std::endl;
    }

    // - 内部にポインタを持つオブジェクトを持てること
    // - clear後にinsertしても状態が整合すること
    void    destroy_and_create() {
        SPRINT("destroy_and_create");
        MapClass< std::list<int>, int >  m;
        int n = 10;
        for (int i = 0; i < n; ++i) {
            std::list<int>  lst;
            for (int j = 0; j < n; ++j) {
                lst.push_back(rand() % 3);
            }
            m.insert(NS::make_pair(lst, 1));
        }
        DSOUT() << m.size() << std::endl;
        {
            MapClass< std::list<int>, int >  mm;
            int n = 10;
            for (int i = 0; i < n; ++i) {
                std::list<int>  lst;
                for (int j = 0; j < n; ++j) {
                    lst.push_back(rand() % 3);
                }
                mm.insert(NS::make_pair(lst, 2));
            }
            DSOUT() << (m == mm) << std::endl;
            m = mm;
            DSOUT() << (m == mm) << std::endl;
        }
        DSOUT() << m.size() << std::endl;
        m.clear();
        for (int i = 0; i < n; ++i) {
            std::list<int>  lst;
            for (int j = 0; j < n; ++j) {
                lst.push_back(rand() % 3);
            }
            m.insert(NS::make_pair(lst, 20));
        }
        DSOUT() << m.size() << std::endl;
    }

    // // [いろんなものを載せてみるコーナー]

    // mapにmapをのせる
    void    map_on_map() {
        SPRINT("map_on_map");
        typedef MapClass< MapClass< int, int >, int > map_type;
        map_type    s;
        for (int i = 0; i < 10; ++i) {
            map_type::key_type  t;
            for (int j = 0; j < 3; ++j) {
                NS::pair<map_type::key_type::iterator, bool> result = t.insert(NS::make_pair(rand() % 3, j));
                DSOUT() << "inner(" << j << "): " << (result.second ? "ok" : "FAIL") << std::endl;
            }
            DSOUT() << "insert outer(" << i << ") -> " << t << std::endl;
            NS::pair<map_type::iterator, bool> result = s.insert(NS::make_pair(t, i));
            DSOUT() << "outer(" << i << "): " << (result.second ? "ok" : "FAIL") << std::endl;
        }
        for (map_type::const_iterator it = s.begin(); it != s.end(); ++it) {
            DSOUT() << *it << std::endl;
        }
    }

    // setにmapをのせる
    void    map_on_set() {
        SPRINT("map_on_set");
        typedef SetClass< MapClass< int, int > > set_type;
        set_type    s;
        for (int i = 0; i < 10; ++i) {
            set_type::key_type  t;
            for (int j = 0; j < 3; ++j) {
                NS::pair<set_type::key_type::iterator, bool> result = t.insert(NS::make_pair(rand() % 3, j));
                DSOUT() << "inner(" << j << "): " << (result.second ? "ok" : "FAIL") << std::endl;
            }
            NS::pair<set_type::iterator, bool> result = s.insert(t);
            DSOUT() << "outer(" << i << "): " << (result.second ? "ok" : "FAIL") << std::endl;
        }
        for (set_type::const_iterator it = s.begin(); it != s.end(); ++it) {
            DSOUT () << *it << std::endl;
        }
    }

    // mapにsetをのせる
    void    set_on_map() {
        SPRINT("set_on_map");
        typedef MapClass< SetClass< int >, int > map_type;
        map_type    s;
        for (int i = 0; i < 10; ++i) {
            map_type::key_type  t;
            for (int j = 0; j < 3; ++j) {
                NS::pair<map_type::key_type::iterator, bool> result = t.insert(rand() % 3);
                DSOUT() << "inner(" << j << "): " << (result.second ? "ok" : "FAIL") << std::endl;
            }
            NS::pair<map_type::iterator, bool> result = s.insert(NS::make_pair(t, i));
            DSOUT() << "outer(" << i << "): " << (result.second ? "ok" : "FAIL") << std::endl;
        }
        for (map_type::const_iterator it = s.begin(); it != s.end(); ++it) {
            DSOUT() << *it << std::endl;
        }
    }

    // mapにvectorをのせる
    void    vector_on_map() {
        SPRINT("vector_on_map");
        typedef MapClass< VectorClass< int >, int > map_type;
        map_type    s;
        for (int i = 0; i < 10; ++i) {
            map_type::key_type  t;
            for (int j = 0; j < 4; ++j) {
                t.push_back(rand() % 2);
            }
            s.insert(NS::make_pair(t, i));
        }
        for (map_type::const_iterator it = s.begin(); it != s.end(); ++it) {
            DSOUT() << *it << std::endl;
        }
    }

    // mapにreferenceをのせる
    // setだと無理だけどmapはいけてしまう・・・
    void    ref_is_key() {
        SPRINT("ref_is_key");
        MapClass< int&, int >    s;
        // s[1] = 2; // これは無理
        DSOUT() << s.size() << std::endl;
    }
    void    ref_is_val() {
        SPRINT("ref_is_val");
        MapClass< int, int& >    s;
        // s[1] = 2; // ftではコンパイルできない。stlではコンパイルできるが実行時に死ぬ。
        // 「ftではコンパイルできない」のはおそらく以下のような理由:
        // ↑を正しくコンパイルするためには引数が"1個"のstd::allocator::constructが使える必要がある。
        // が, C++03までは引数が"2個"で固定で, C++11から"1個以上"になった.
        // (C++11以降ではコンパイルだけできるが、実行時に結局 T& という型の値をデフォルト構築することになるので, その時に死ぬ)
        DSOUT() << s.size() << std::endl;
        DSOUT() << s.count(1) << std::endl;
    }

    void    pointer_is_key() {
        SPRINT("pointer_is_key");
        MapClass< int*, int  >  s;
        {
            int i = 0;
            int j = 0;
            s[&i] = 0;
            s[&j] = 1;
        }
        DSOUT() << s.size() << std::endl;
        DSOUT() << s.begin()->second << std::endl; // diffでるかも。でないと思うけど。
    }

    void    pointer_is_val() {
        SPRINT("pointer_is_val");
        MapClass< int, int*  >  s;
        {
            int i = 0;
            int j = 0;
            s[0] = &i;
            s[1] = &j;
        }
        DSOUT() << s.size() << std::endl;
        DSOUT() << s.begin()->first << std::endl;
    }

    void    const_is_key() {
        SPRINT("const_is_key");
        MapClass< const int, int >  s;
        {
            s[1] = 2;
            DSOUT() << s.begin()->first << ", " << s.begin()->second << std::endl;
            s.erase(s.begin());
        }
        DSOUT() << s.size() << std::endl;
    }

    void    const_is_val() {
        SPRINT("const_is_val");
        MapClass< int, const int >  s;
        {
            // s[1] = 2; // これは再代入になるのでコンパイルエラー
            s.insert(NS::make_pair(1, 2));
            DSOUT() << s.begin()->first << ", " << s.begin()->second << std::endl;
            s.erase(s.begin());
        }
        DSOUT() << s.size() << std::endl;
    }

    void    map_iterator_on_map() {
        SPRINT("map_iterator_on_map");
        MapClass<int, int>   m;
        MapClass< MapClass<int, int>::iterator, int >   s;
        // s[m.begin()] = 1; // 値を入れようとするとエラー
        DSOUT() << s.size() << std::endl;
    }

    void    test() {
        specify_comparator();
        destroy_and_create();
        map_on_map();
        map_on_set();
        set_on_map();
        vector_on_map();
        ref_is_key();
        ref_is_val();
        pointer_is_key();
        pointer_is_val();
        const_is_key();
        const_is_val();
    }
}

int main()
{
    fill::test();
    logic::test();
    ft::sprint::list();
}
