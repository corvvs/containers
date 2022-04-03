#include <iostream>
#include <string>
#include <list>
#include "test_common.hpp"

// 各機能を1つずつ潰していくテスト。
namespace fill {
    typedef SetClass<int>               set_type;
    typedef set_type::iterator          iterator;
    typedef set_type::const_iterator    const_iterator;

    // デフォルトコンストラクタ
    void    constructor_default(int n) {
        SPRINT("constructor_default") << "(" << n << ")";
        set_type    m;
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            m.insert(rand());
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    // コンパレータ指定コンストラクタ
    void    constructor_comparator(int n) {
        SPRINT("constructor_comparator") << "(" << n << ")";
        SetClass<int, std::greater<int> >    m((std::greater<int>()));
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            m.insert(rand());
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        DSOUT();
        SetClass<int, std::greater<int> >::iterator it = m.begin();
        for (int i = 0; i < n / 10; ++i) {
            std::cout << *(it++) << " ";
        }
        std::cout << std::endl;
        DSOUT();
        it = m.begin();
        for (int i = 0; i < n / 10; ++i) {
            std::cout << *it << " ";
            ++it;
        }
        std::cout << std::endl;
    }

    // コンパレータ・アロケータ指定コンストラクタ
    void    constructor_comparator_allocator(int n) {
        SPRINT("constructor_comparator_allocator") << "(" << n << ")";
        SetClass<int>    m(
            (std::less<int>()), std::allocator<set_type::value_type>()
        );
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            m.insert(rand());
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    // イテレータ指定コンストラクタ
    void    constructor_iterator(int n) {
        SPRINT("constructor_iterator") << "(" << n << ")";
        VectorClass<int>  v;
        for (int i = 0; i < n; ++i) {
            v.push_back(rand());
        }
        set_type    m((v.begin()), v.end());
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    // コピーコンストラクタ
    void    constructor_copy(int n) {
        SPRINT("constructor_copy") << "(" << n << ")";
        set_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(rand());
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        {
            set_type    mm((m));
            DSOUT() << mm.empty() << std::endl;
            DSOUT() << mm.size() << std::endl;
            DSOUT() << (mm.begin() == mm.end()) << std::endl;
        }
        m.insert(rand());
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    oprator_assignation(int n) {
        SPRINT("oprator_assignation") << "(" << n << ")";
        set_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(rand());
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        {
            set_type    mm;
            DSOUT() << mm.empty() << std::endl;
            DSOUT() << mm.size() << std::endl;
            DSOUT() << (mm.begin() == mm.end()) << std::endl;
            mm = m;
            DSOUT() << mm.empty() << std::endl;
            DSOUT() << mm.size() << std::endl;
            DSOUT() << (mm.begin() == mm.end()) << std::endl;
        }
        m.insert(rand());
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    get_allocator() {
        SPRINT("get_allocator");
        int k = 0;
        for (int i = 0; i < 1000; ++i) {
            set_type    m;
            std::allocator< set_type::value_type >  alloc;
            k += (m.get_allocator() == alloc);
        }
        DSOUT() << k << std::endl;
    }

    void    key_comp() {
        SPRINT("key_comp");
        set_type    m;
        DSOUT() << m.key_comp()(1, 1) << std::endl;
        DSOUT() << m.key_comp()(0, 1) << std::endl;
        DSOUT() << m.key_comp()(1, 0) << std::endl;
    }

    void    value_comp() {
        SPRINT("value_comp");
        set_type    m;
        DSOUT() << m.value_comp()(1, 1) << std::endl;
        DSOUT() << m.value_comp()(0, 1) << std::endl;
        DSOUT() << m.value_comp()(1, 0) << std::endl;
    }

    void    begin_end_variable() {
        SPRINT("begin_end_variable");
        set_type    m;
        DSOUT() << (m.begin() == m.end());
        m.insert(1);
        DSOUT() << (m.begin() == m.end());
        set_type::iterator  it = m.end();
        --it;
        DSOUT() << (m.begin() == m.end());
        DSOUT() << *(m.begin()) << std::endl;
        DSOUT() << *it << std::endl;
        DSOUT() << *(it++) << std::endl;
        it--;
        DSOUT() << *(m.begin()) << std::endl;
        ++it;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    clear(int n) {
        SPRINT("clear") << "(" << n << ")";
        set_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(rand());
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        m.clear();
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            m.insert(rand());
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    swap(int n) {
        SPRINT("swap") << "(" << n << ")";
        set_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(rand());
        }
        {
            set_type    mm;
            for (int i = 0; i < n; ++i) {
                mm.insert(rand());
            }
            DSOUT() << m.empty() << std::endl;
            DSOUT() << m.size() << std::endl;
            DSOUT() << (m.begin() == m.end()) << std::endl;
            DSOUT() << *(m.begin()) << std::endl;
            DSOUT() << mm.empty() << std::endl;
            DSOUT() << mm.size() << std::endl;
            DSOUT() << (mm.begin() == m.end()) << std::endl;
            DSOUT() << *(mm.begin()) << std::endl;
            m.swap(mm);
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
        set_type    m;
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            PairClass<set_type::iterator, bool> result = m.insert(rand());
            DSOUT() << result.second << ", " << *(result.first) << std::endl;
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        DSOUT() << *(m.begin()) << std::endl;
    }

    void    insert_value_with_hint(int n) {
        SPRINT("insert_value_with_hint") << "(" << n << ")";
        set_type    m;
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        set_type::iterator it = m.begin();
        for (int i = 0; i < n; ++i) {
            it = m.insert(it, rand());
            DSOUT() << *it << std::endl;
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        DSOUT() << *(m.begin()) << std::endl;
    }

    void    insert_value_with_range(int n) {
        SPRINT("insert_value_with_range") << "(" << n << ")";
        std::list<set_type::value_type>  l;
        for (int i = 0; i < n; ++i) {
            l.push_back(rand());
        }
        set_type    m;
        m.insert(l.begin(), l.end());
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        DSOUT() << *(m.begin()) << std::endl;
    }

    void    erase_by_position(int n) {
        SPRINT("erase_by_position") << "(" << n << ")";
        set_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(rand());
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            set_type::iterator  it = m.find(rand() % n);
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
        set_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(rand());
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
        set_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(rand());
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        set_type::iterator  b = m.begin();
        set_type::iterator  e = m.end();
        m.erase(++(++b), --(--e));
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    find_variable(int n) {
        SPRINT("find_variable") << "(" << n << ")";
        set_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(rand());
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        // *(m.begin()) = 1;
    }

    void    find_constant(int n) {
        SPRINT("find_constant") << "(" << n << ")";
        set_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(rand());
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n; ++i) {
            set_type::const_iterator  it = m.find(rand() % n);
            if (it != m.end()) {
                DSOUT() << *it<< std::endl;
            }
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
    }

    void    equal_range(int n) {
        SPRINT("equal_range") << "(" << n << ")";
        set_type    m;
        for (int i = 0; i < n; ++i) {
            int k = rand() % n;
            DSOUT() << "insert: " << k << std::endl;
            m.insert(k);
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n * 5; ++i) {
            int k = rand() % n;
            DSOUT() << "test for: " << k << std::endl;
            NS::pair<set_type::iterator, set_type::iterator>    range = m.equal_range(k);
            if (range.first == range.second) {
                // not found
                if (range.first == m.end()) {
                    DSOUT() << "not found and end" << std::endl;
                } else {
                    DSOUT() << "not found: " << *range.first << std::endl;
                }
            } else {
                if (range.second == m.end()) {
                    DSOUT() << "found: " << *range.first << " - end" << std::endl;
                } else {
                    DSOUT() << "found: " << *range.first << " - " << *range.second << std::endl;
                }
            }
        }
    }

    void    lower_bound(int n) {
        SPRINT("lower_bound") << "(" << n << ")";
        set_type    m;
        for (int i = 0; i < n; ++i) {
            int k = rand() % n;
            DSOUT() << "insert: " << k << std::endl;
            m.insert(k);
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n * 5; ++i) {
            int key = rand() % n;
            set_type::iterator    it = m.lower_bound(key);
            if (it == m.end()) {
                DSOUT() << "found: end" << std::endl;
            } else {
                DSOUT() << "found: " << key << " -> " << *it << std::endl;
            }
            set_type::const_iterator    cit = m.lower_bound(key);
            if (cit == m.end()) {
                DSOUT() << "found: end" << std::endl;
            } else {
                DSOUT() << "found: " << key << " -> " << *cit << std::endl;
            }
        }
    }

    void    upper_bound(int n) {
        SPRINT("upper_bound") << "(" << n << ")";
        set_type    m;
        for (int i = 0; i < n; ++i) {
            m.insert(rand() % n);
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        for (int i = 0; i < n * 5; ++i) {
            int key = rand() % n;
            set_type::iterator    it = m.upper_bound(key);
            if (it == m.end()) {
                DSOUT() << "found: end" << std::endl;
            } else {
                DSOUT() << "found: " << key << " -> " << *it << std::endl;
            }
            set_type::const_iterator    cit = m.upper_bound(key);
            if (cit == m.end()) {
                DSOUT() << "found: end" << std::endl;
            } else {
                DSOUT() << "found: " << key << " -> " << *cit << std::endl;
            }
        }
    }

    void    comparation() {
        SPRINT("comparation");
        typedef SetClass<int>   st;
        typedef VectorClass<st> vt;
        vt    v;
        int n = 3;
        for (int i = 0; i < (1 << (n + 2)); ++i) {
            st  s;
            for (int j = 0; j < n; ++j) {
                if ((1 << j) & i) {
                    s.insert(j);
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
        typedef SetClass<int>   st;
        st                      s;
        s.insert(1);
        s.insert(2);
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
        constructor_default(1000);
        constructor_comparator(1000);
        constructor_comparator_allocator(1000);
        constructor_iterator(1000);
        constructor_copy(1000);
        oprator_assignation(1000);
        get_allocator();
        key_comp();
        value_comp();
        begin_end_variable();
        clear(1000);
        swap(1000);
        insert_value(1000);
        insert_value_with_hint(1000);
        insert_value_with_range(1000);
        erase_by_position(1000);
        erase_by_key(1000);
        erase_by_range(1000);
        find_variable(1000);
        find_constant(1000);
        equal_range(1000);
        lower_bound(1000);
        upper_bound(1000);
        comparation();
        iterator_comparation();
    }
}

namespace logic {
    typedef std::string     Key;
    typedef SetClass<Key>   set_type;

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
        // SetClass<Key>                   m; // これはコンパイルできない
        SetClass<Key, size_compare<Key> >     m;
        m.insert("apple");
        DSOUT() << *(m.begin()) << std::endl;
        DSOUT() << m.size() << std::endl;
        m.insert("hello");
        DSOUT() << *(m.begin()) << std::endl;
        DSOUT() << m.size() << std::endl;
        m.insert("42");
        DSOUT() << *(m.begin()) << std::endl;
        DSOUT() << m.size() << std::endl;
    }

    // - 内部にポインタを持つオブジェクトを持てること
    // - clear後にinsertしても状態が整合すること
    void    destroy_and_create() {
        SPRINT("destroy_and_create");
        SetClass< std::list<int> >  m;
        int n = 10;
        for (int i = 0; i < n; ++i) {
            std::list<int>  lst;
            for (int j = 0; j < n; ++j) {
                lst.push_back(rand());
            }
            m.insert(lst);
        }
        DSOUT() << m.size() << std::endl;
        {
            SetClass< std::list<int> >  mm;
            int n = 10;
            for (int i = 0; i < n; ++i) {
                std::list<int>  lst;
                for (int j = 0; j < n; ++j) {
                    lst.push_back(rand());
                }
                mm.insert(lst);
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
                lst.push_back(rand());
            }
            m.insert(lst);
        }
        DSOUT() << m.size() << std::endl;
    }

    // [いろんなものを載せてみるコーナー]

    // setにsetをのせる
    void    set_on_set() {
        SPRINT("set_on_set");
        typedef SetClass< SetClass< int > > set_type;
        set_type    s;
        for (int i = 0; i < 1000; ++i) {
            set_type::key_type  t;
            for (int j = 0; j < 4; ++j) {
                NS::pair<set_type::key_type::iterator, bool> result = t.insert(rand() % 3);
                DSOUT() << result.second << std::endl;
                DSOUT() << "inner" << j << ": " << result.second << std::endl;
            }
            NS::pair<set_type::iterator, bool> result = s.insert(t);
            DSOUT() << "outer(" << i << "): " << result.second << std::endl;
        }
        for (set_type::const_iterator it = s.begin(); it != s.end(); ++it) {
            DSOUT() << *it << std::endl;
        }
    }

    // setにvectorをのせる
    void    vector_on_set() {
        SPRINT("vector_on_set");
        typedef SetClass< VectorClass< int > > set_type;
        set_type    s;
        for (int i = 0; i < 1000; ++i) {
            set_type::key_type  t;
            for (int j = 0; j < 4; ++j) {
                t.push_back(rand() % 3);
            }
            s.insert(t);
        }
        for (set_type::const_iterator it = s.begin(); it != s.end(); ++it) {
            DSOUT() << *it << std::endl;
        }
    }

    // // setにreferenceをのせる -> コンパイルエラーになるはず
    // void    ref_on_set() {
    //     SetClass< int& >    s;
    // }

    // setにポインタをのせる
    void    pointer_on_set() {
        SPRINT("pointer_on_set");
        SetClass< int* >    s;
        {
            int i = 0;
            int j = 0;
            s.insert(&i);
            s.insert(&j);
        }
        DSOUT() << s.size() << std::endl;
    }

    // setにconstをのせる
    void    const_on_set() {
        SPRINT("const_on_set");
        SetClass< const std::string >    s;
        {
            s.insert("1");
            s.insert("2");
        }
        DSOUT() << s.size() << std::endl;
    }

    // // setにvolatileをのせる -> hmmmmmmmmmmm
    // void    volatile_on_set() {
    //     SPRINT("volatile_on_set");
    //     SetClass< volatile int >    s;
    //     {
    //         volatile int    i = 1;
    //         s.insert(i);
    //     }
    //     DSOUT() << s.size() << std::endl;
    // }

    void    set_iterator_on_set() {
        SPRINT("set_iterator_on_set");
        SetClass<int>   m;
        SetClass< SetClass<int>::iterator >    s;
        // s.insert(m.begin()); // 値を入れようとするとエラー
        DSOUT() << s.size() << std::endl;
    }

    void    test() {
        specify_comparator();
        destroy_and_create();
        set_on_set();
        vector_on_set();
        pointer_on_set();
        const_on_set();
        set_iterator_on_set();
    }
}

namespace performance {
    void    insertion(int n, int m) {
        SetClass<int>   s;
        for (int i = 0; i < n; ++i) {
            s.insert(rand());
        }
        {
            SPRINT("performance::insertion") << "(" << n << ", " << m << ")";
            for (int i = 0; i < m; ++i) {
                s.insert(rand());
            }
        }
    }

    void    hint_aided_insertion(int n, int m) {
        SetClass<int>   s;
        for (int i = 0; i < n; ++i) {
            s.insert(i);
        }
        {
            SPRINT("performance::hint_aided_insertion") << "(" << n << ", " << m << ")";
            for (int i = 0; i < m; ++i) {
                s.insert(s.end(), s.size());
            }
        }
    }

    void    copy(int n) {
        SetClass<int>   s;
        for (int i = 0; i < n; ++i) {
            s.insert(rand());
        }
        {
            SPRINT("performance::copy") << "(" << n << ")";
            SetClass<int>   ss((s));
        }
    }

    void    ranged_insertion(int n) {
        SetClass<int>   s;
        for (int i = 0; i < n; ++i) {
            s.insert(rand());
        }
        {
            SPRINT("performance::ranged_insertion") << "(" << n << ")";
            SetClass<int>   ss;
            ss.insert(s.begin(), s.end());
        }
    }

    // でかい(=compareに時間がかかる)キーを使った処理
    void    heavy_key(int n, int m) {
        typedef SetClass< VectorClass< int > > set_type;
        set_type::const_iterator    it;
        set_type    s;
        {
            SPRINT("performance::heavy_key initialize") << "(" << n << ", " << m << ")";
            for (int i = 0; i < n; ++i) {
                set_type::key_type  t;
                for (int j = 0; j < m; ++j) {
                    t.push_back(rand());
                }
                s.insert(t);
            }
            DSOUT() << *(s.begin()->begin()) << std::endl;
            it = s.begin();
            for (int i = 0; i < n / 2; ++i, ++it);
        }
        {
            SPRINT("performance::heavy_key find") << "(" << n << ", " << m << ")";
            for (int i = 0; i < n; ++i) {
                set_type::const_iterator result = s.find(*it);
                DSOUT() << result->back() << std::endl;
            }
        }
        {
            SPRINT("performance::heavy_key erasure") << "(" << n << ", " << m << ")";
            while (s.size() > 0) {
                if (rand() % 2 == 0) {
                    s.erase(s.begin());
                } else {
                    set_type::const_iterator    it = s.end();
                    --it;
                    s.erase(it);
                }
            }
        }
    }

    void    test() {
        insertion(1000, 1000);
        insertion(10000, 1000);
        insertion(100000, 1000);
        insertion(1000000, 1000);
        hint_aided_insertion(1000, 1000);
        hint_aided_insertion(10000, 1000);
        hint_aided_insertion(100000, 1000);
        hint_aided_insertion(1000000, 1000);
        copy(1000);
        copy(10000);
        copy(100000);
        copy(1000000);
        ranged_insertion(1000);
        ranged_insertion(10000);
        ranged_insertion(100000);
        ranged_insertion(1000000);
        heavy_key(100, 10);
        heavy_key(1000, 10);
        heavy_key(10000, 10);
        heavy_key(100, 100);
        heavy_key(1000, 100);
        heavy_key(10000, 100);
        heavy_key(100, 1000);
        heavy_key(1000, 1000);
        heavy_key(10000, 1000);
    }
}

int main()
{
    fill::test();
    logic::test();
    performance::test();
    ft::sprint::list();
}
