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
        DSOUT() << m.begin()->first << std::endl;
        DSOUT() << m.begin()->second << std::endl;
        DSOUT() << it->first << std::endl;
        DSOUT() << it->second << std::endl;
        it->second = 1000;
        DSOUT() << it->first << std::endl;
        DSOUT() << it->second << std::endl;
        DSOUT() << m.begin()->first << std::endl;
        DSOUT() << m.begin()->second << std::endl;
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
            DSOUT() << m.begin()->first << ", " << m.begin()->second << std::endl;
            DSOUT() << mm.empty() << std::endl;
            DSOUT() << mm.size() << std::endl;
            DSOUT() << (mm.begin() == m.end()) << std::endl;
            DSOUT() << mm.begin()->first << ", " << mm.begin()->second << std::endl;
            swap(m, mm);
            DSOUT() << mm.empty() << std::endl;
            DSOUT() << mm.size() << std::endl;
            DSOUT() << (mm.begin() == m.end()) << std::endl;
            DSOUT() << mm.begin()->first << ", " << mm.begin()->second << std::endl;
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        DSOUT() << m.begin()->first << ", " << m.begin()->second << std::endl;
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
        DSOUT() << m.begin()->first << ", " << m.begin()->second << std::endl;
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
            DSOUT() << it->first << ", " << it->second << std::endl;
        }
        DSOUT() << m.empty() << std::endl;
        DSOUT() << m.size() << std::endl;
        DSOUT() << (m.begin() == m.end()) << std::endl;
        DSOUT() << m.begin()->first << ", " << m.begin()->second << std::endl;
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
        DSOUT() << m.begin()->first << ", " << m.begin()->second << std::endl;
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
                DSOUT() << it->first << ", " << it->second << std::endl;
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
                DSOUT() << "found: " << key << " -> " << it->first << ", " << it->second << std::endl;
            }
            map_type::const_iterator    cit = m.lower_bound(key);
            if (cit == m.end()) {
                DSOUT() << "found: end" << std::endl;
            } else {
                DSOUT() << "found: " << key << " -> " << cit->first << ", " << cit->second << std::endl;
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
                DSOUT() << "found: " << key << " -> " << it->first << ", " << it->second << std::endl;
            }
            map_type::const_iterator    cit = m.upper_bound(key);
            if (cit == m.end()) {
                DSOUT() << "found: end" << std::endl;
            } else {
                DSOUT() << "found: " << key << " -> " << cit->first << ", " << cit->second << std::endl;
            }
        }
    }
}

int main()
{
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
    ft::sprint::list();
}
