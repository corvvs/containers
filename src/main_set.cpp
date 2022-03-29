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
            std::cout << *(it++) << " ";
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
        set_type    m;
        std::allocator< set_type::value_type >  alloc;
        DSOUT() << (m.get_allocator() == alloc) << std::endl;
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
            swap(m, mm);
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

    void    fill_test() {
        fill::constructor_default(100);
        fill::constructor_comparator(100);
        fill::constructor_comparator_allocator(100);
        fill::constructor_iterator(100);
        fill::constructor_copy(100);
        fill::oprator_assignation(100);
        fill::get_allocator();
        fill::key_comp();
        fill::value_comp();
        fill::begin_end_variable();
        fill::clear(100);
        fill::swap(100);
        fill::insert_value(100);
        fill::insert_value_with_hint(100);
        fill::insert_value_with_range(100);
        fill::erase_by_position(100);
        fill::erase_by_key(100);
        fill::erase_by_range(100);
        fill::find_variable(100);
        fill::find_constant(100);
        fill::equal_range(100);
        fill::lower_bound(100);
        fill::upper_bound(100);
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
}

namespace performance {
    void    insertion(int n, int m) {
        SetClass<int>   s;
        for (int i = 0; i < n; ++i) {
            s.insert(rand());
        }
        {
            SPRINT("performance::insertion") << "(" << n << ")";
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
            SPRINT("performance::hint_aided_insertion") << "(" << n << ")";
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
}

int main()
{
    // fill::fill_test();
    // logic::specify_comparator();
    // logic::destroy_and_create();
    // performance::insertion(1000, 100);
    // performance::insertion(10000, 100);
    // performance::insertion(100000, 100);
    // performance::insertion(1000000, 100);
    // performance::hint_aided_insertion(1000, 100);
    // performance::hint_aided_insertion(10000, 100);
    // performance::hint_aided_insertion(100000, 100);
    // performance::hint_aided_insertion(1000000, 100);
    performance::copy(1000);
    performance::copy(10000);
    performance::copy(100000);
    performance::copy(1000000);
    performance::ranged_insertion(1000);
    performance::ranged_insertion(10000);
    performance::ranged_insertion(100000);
    performance::ranged_insertion(1000000);
    ft::sprint::list();

}
