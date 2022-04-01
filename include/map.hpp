#ifndef MAP_HPP
# define MAP_HPP
# include "tree.hpp"
# include "pair.hpp"
# include "ft_algorithm.hpp"

namespace ft {

    template <
        class Key,
        class Value,
        class KeyComparator = std::less<Key>,
        class PairAllocator = std::allocator<ft::pair<const Key, Value> >
    >
    class map {
        public:
            typedef Key                                         key_type;
            typedef Value                                       mapped_type;
            // これがtreeに渡される
            typedef ft::pair<const key_type, mapped_type>       value_type;
            typedef KeyComparator                               key_compare;
            typedef PairAllocator                               allocator_type;
            typedef map<key_type, mapped_type, key_compare, allocator_type>
                                                                self_type;
            typedef value_type&                                 reference;
            typedef const value_type&                           const_reference;
            typedef typename allocator_type::pointer            pointer;
            typedef typename allocator_type::const_pointer      const_pointer;
            typedef typename allocator_type::size_type          size_type;
            typedef typename allocator_type::difference_type    difference_type;

            template <class, class, class>
            class map_value_compare;
            typedef map_value_compare<key_type, value_type, key_compare>
                                                                map_value_compare_type;
            // 内部tree
            typedef ft::tree<value_type, map_value_compare_type, allocator_type>
                                                                base;
            class const_iterator;

            // [[(variable) iterator]]
            class iterator {
                FT_PRIVATE:
                    typedef typename base::node_type                node_type;
                    typedef typename base::iterator                 tree_iterator;
                    typedef typename base::const_iterator           tree_const_iterator;
                    typedef iterator                                iterator_type;
                    typedef const_iterator                          const_iterator_type;

                protected:
                    tree_iterator                                   tree_it_;

                public:
                    typedef std::bidirectional_iterator_tag         iterator_category;
                    typedef map::value_type                         value_type;
                    typedef typename base::iterator::difference_type
                                                                    difference_type;
                    typedef value_type&                             reference;
                    typedef value_type*                             pointer;

                    iterator() {}
                    explicit iterator(tree_const_iterator tit)
                        : tree_it_(tit) {}
                    iterator(const iterator_type& other) {
                        *this = other;
                    }
                    iterator(const_iterator_type variable)
                        : tree_it_(variable.tree_iter()) {}
                    virtual ~iterator() {}
                    iterator&   operator=(const iterator& rhs) {
                        tree_it_ = rhs.tree_it_;
                        return *this;
                    }

                    tree_const_iterator tree_iter() const {
                        return tree_it_;
                    }

                    reference   operator*() const {
                        return *(tree_it_->value());
                    }
                    pointer     operator->() const {
                        return tree_it_->value();
                    }

                    iterator&   operator++() {
                        ++tree_it_;
                        return *this;
                    }

                    iterator    operator++(int) {
                        iterator    it = *this;
                        ++*this;
                        return it;
                    }

                    iterator&   operator--() {
                        --tree_it_;
                        return *this;
                    }

                    iterator    operator--(int) {
                        iterator    it = *this;
                        --*this;
                        return it;
                    }

                    bool        operator==(const iterator& rhs) const {
                        return operator->() == rhs.operator->();
                    }
                    bool        operator!=(const iterator& rhs) const {
                        return !(*this == rhs);
                    }
            };

            class const_iterator {
                FT_PRIVATE:
                    typedef typename base::node_type    node_type;
                    typedef typename base::iterator     tree_iterator;
                    typedef typename base::const_iterator
                                                        tree_const_iterator;
                    typedef iterator                    iterator_type;
                    typedef const_iterator              const_iterator_type;

                protected:
                    tree_const_iterator                 tree_it_;

                public:
                    typedef std::bidirectional_iterator_tag  iterator_category;
                    typedef map::value_type             value_type;
                    typedef typename base::iterator::difference_type
                                                        difference_type;
                    typedef const value_type&           reference;
                    typedef const value_type*           pointer;

                    const_iterator() {}
                    explicit const_iterator(tree_const_iterator tit)
                        : tree_it_(tit) {}
                    const_iterator(const const_iterator_type& other) {
                        *this = other;
                    }
                    const_iterator(iterator_type variable)
                        : tree_it_(variable.tree_iter()) {}
                    virtual ~const_iterator() {}
                    const_iterator&   operator=(const const_iterator& rhs) {
                        tree_it_ = rhs.tree_it_;
                        return *this;
                    }

                    tree_const_iterator tree_iter() const {
                        return tree_it_;
                    }

                    reference   operator*() const {
                        return *(tree_it_->value());
                    }
                    pointer     operator->() const {
                        return tree_it_->value();
                    }

                    const_iterator&   operator++() {
                        ++tree_it_;
                        return *this;
                    }

                    const_iterator    operator++(int) {
                        iterator    it = *this;
                        ++*this;
                        return it;
                    }

                    const_iterator&   operator--() {
                        --tree_it_;
                        return *this;
                    }

                    const_iterator    operator--(int) {
                        const_iterator    it = *this;
                        --*this;
                        return it;
                    }

                    bool        operator==(const const_iterator& rhs) const {
                        return operator->() == rhs.operator->();
                    }
                    bool        operator!=(const const_iterator& rhs) const {
                        return !(*this == rhs);
                    }
            };

        // [[インナークラス群]]

        // map と tree を接続するコンパレータ
        // (KeyCompareから作られる)
        template <
            // mapのKey
            class MVCKey,
            // ツリーノードに載っている値の型. つまり pair<Key, Value>.
            class MVCNodeValue,
            // Keyのコンパレータ
            class MVCKeyCompare
        >
        class map_value_compare {

            FT_PRIVATE:

                MVCKeyCompare key_compare_;

            public:

                map_value_compare()
                    : key_compare_() {}

                explicit map_value_compare(MVCKeyCompare c)
                    : key_compare_(c) {}

                // Keyのコンパレータを返す
                const MVCKeyCompare& key_comp() const { return key_compare_; }

                // Keyはそのまま、ノードはKeyを引っ張ってきてコンパレータに渡す
                bool operator()(const MVCNodeValue& x, const MVCNodeValue& y) const {
                    return key_compare_(x.first, y.first);
                }

                bool operator()(const MVCNodeValue& x, const MVCKey& y) const {
                    return key_compare_(x.first, y);
                }

                bool operator()(const MVCKey& x, const MVCNodeValue& y) const {
                    return key_compare_(x, y.first);
                }

                void swap(map_value_compare& y) {
                    swap(key_compare_, y.key_compare_);
                }
        };

        // pair<MK, MV> のコンパレータ
        // (KeyCompareから作られる)
        class value_compare {
            // map::value_compare() が value_compare コンストラクタを呼ぶが,
            // これは protected なのでそのままでは map から呼べない。
            friend class map;

            protected:

                key_compare comp;

                value_compare(key_compare c): comp(c) {}

            public:

                // 2変数関数向けメタ関数に適合させるための型定義
                typedef bool        result_type;
                typedef value_type  first_argument_type;
                typedef value_type  second_argument_type;

                bool operator()(const value_type& x, const value_type& y) const {
                    return comp(x.first, y.first);
                }
        };

        FT_PRIVATE:

            // 内部ツリー
            base    tree_;

        public:

            // [[コンストラクタ群]]

            map(): tree_(map_value_compare_type(KeyComparator()), PairAllocator()) {}

            explicit map(
                const KeyComparator& comp
            ): tree_(map_value_compare_type(comp), PairAllocator()) {}

            explicit map(
                const KeyComparator& comp,
                const PairAllocator& alloc
            ): tree_(map_value_compare_type(comp), alloc) {}


            template <class InputIterator>
            map(InputIterator first,
                InputIterator last,
                const KeyComparator& comp = KeyComparator(),
                const PairAllocator& alloc = PairAllocator()
            ): tree_(map_value_compare_type(comp), alloc) {
                insert(first, last);
            }

            map(const self_type& other) {
                *this = other;
            }

            ~map() {
                // 破壊処理はtreeのデストラクタがやる
            }

            self_type&      operator=(const self_type& rhs) {
                if (this == &rhs) { return *this; }
                tree_ = rhs.tree_;
                return *this;
            }

            // [[getter群]]

            allocator_type      get_allocator() const {
                return allocator_type(tree_.get_allocator());
            }

            // Key同士を取って比較するコンパレータを返す
            key_compare         key_comp() const {
                // tree_.value_compare() は map_value_comp を返す.
                // map_value_comp.key_comp() は key_compare(KeyComparator) を返す.
                return tree_.value_compare().key_comp();
            }

            // pair<Key, Value>を取り, first同士を比較するコンパレータを返す
            value_compare       value_comp() const {
                // 一見 tree_.value_compare() でよさそうだが,
                // 型としては value_compare を返す必要があるので,
                // 面倒だが以下のようにする:
                return value_compare(tree_.value_compare().key_comp());
            }

            inline bool         empty() const { return tree_.empty(); }
            inline size_type    size() const {return tree_.size();}
            inline size_type    max_size() const {return tree_.max_size();}

            iterator            begin() { return iterator(tree_.begin()); }
            const_iterator      begin() const { return const_iterator(tree_.begin()); }
            iterator            end() { return iterator(tree_.end()); }
            const_iterator      end() const { return const_iterator(tree_.end()); }

            // [[ライフサイクル]]

            inline void         clear() { tree_.clear(); }
            void                swap(self_type& other) {
                tree_.swap(other.tree_);
            }

            // [[挿入系関数群]]

            pair<iterator, bool>    insert(const value_type& v) {
                ft::pair<typename base::iterator, bool>   result = tree_.insert(v);
                return ft::make_pair(iterator(result.first), result.second);
            }
            iterator                insert(const_iterator hint, const value_type& v) {
                typename base::iterator  result = tree_.insert(hint.tree_iter(), v);
                return iterator(result);
            }
            template <class InputIterator>
            void                    insert(InputIterator first, InputIterator last)
            {
                for (; first != last; ++first) {
                    tree_.insert(tree_.end(), *first);
                }
            }

            // [[削除]]

            void        erase(iterator position) {
                tree_.erase(position.tree_iter());
            }
            void        erase(const_iterator position) {
                tree_.erase(position.tree_iter());
            }
            size_type   erase(const key_type& x) {
                return tree_.erase_by_key(x);
            }
            void        erase(iterator first, iterator last) {
                return tree_.erase(first.tree_iter(), last.tree_iter());
            }

            // [[検索系関数]]

            Value&                      operator[](const key_type& x) {
                ft::pair<iterator, bool> result = insert(ft::make_pair(x, mapped_type()));
                return (result.first)->second;
            }

            inline iterator             find(const key_type& x) {
                return iterator(tree_.find(x));
            }
            inline const_iterator       find(const key_type& x) const {
                return const_iterator(tree_.find(x));
            }

            inline size_type            count(const key_type& x) const {
                return tree_.count(x);
            }

            pair<iterator, iterator>    equal_range(const key_type& x) {
                return tree_.equal_range(x);
            }

            iterator                    lower_bound(const key_type& x) {
                return iterator(tree_.lower_bound(x));
            }
            const_iterator              lower_bound(const key_type& x) const {
                return const_iterator(tree_.lower_bound(x));
            }

            iterator                    upper_bound(const key_type& x) {
                return iterator(tree_.upper_bound(x));
            }
            const_iterator              upper_bound(const key_type& x) const {
                return const_iterator(tree_.upper_bound(x));
            }

    };

    // [比較演算子]
    // lhs と rhs の内容が等しいかどうか調べます。 つまり、それらが同じ個数の要素を持ち、 lhs 内のそれぞれの要素が rhs 内の同じ位置の要素と等しいかどうか比較します。
    template <class Key, class Value, class KeyComparator, class PairAllocator>
    bool operator==(
        const ft::map<Key, Value, KeyComparator, PairAllocator>& lhs,
        const ft::map<Key, Value, KeyComparator, PairAllocator>& rhs
    ) {
        // ランダムアクセスイテレータを持つコンテナなら equal 側でサイズ比較をするが、
        // map はそうではないので, 自前でサイズ比較をやる.
        return lhs.size() == rhs.size()
            && ft::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class Key, class Value, class KeyComparator, class PairAllocator>
    bool operator!=(
        const ft::map<Key, Value, KeyComparator, PairAllocator>& lhs,
        const ft::map<Key, Value, KeyComparator, PairAllocator>& rhs
    ) {
        return !(lhs == rhs);
    }

    // lhs と rhs の内容を辞書的に比較します。 比較は std::lexicographical_compare と同等の関数によって行われます。 
    // lhs の内容が rhs の内容より辞書的に小さい場合は true、そうでなければ false。
    template <class Key, class Value, class KeyComparator, class PairAllocator>
    bool operator<(
        const ft::map<Key, Value, KeyComparator, PairAllocator>& lhs,
        const ft::map<Key, Value, KeyComparator, PairAllocator>& rhs
    ) {
        // lhs の内容が rhs の内容より辞書的に小さいまたは等しい場合は true、そうでなければ false。
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class Key, class Value, class KeyComparator, class PairAllocator>
    bool operator<=(
        const ft::map<Key, Value, KeyComparator, PairAllocator>& lhs,
        const ft::map<Key, Value, KeyComparator, PairAllocator>& rhs
    ) {
        return !(lhs > rhs);
    }

    template <class Key, class Value, class KeyComparator, class PairAllocator>
    bool operator>(
        const ft::map<Key, Value, KeyComparator, PairAllocator>& lhs,
        const ft::map<Key, Value, KeyComparator, PairAllocator>& rhs
    ) {
        // lhs の内容が rhs の内容より辞書的に大きいまたは等しい場合は true、そうでなければ false。
        return rhs < lhs;
    }

    template <class Key, class Value, class KeyComparator, class PairAllocator>
    bool operator>=(
        const ft::map<Key, Value, KeyComparator, PairAllocator>& lhs,
        const ft::map<Key, Value, KeyComparator, PairAllocator>& rhs
    ) {
        return !(lhs < rhs);
    }
}

template <class Key, class Value, class KeyComparator, class PairAllocator>
void swap(
    ft::map<Key, Value, KeyComparator, PairAllocator>& lhs,
    ft::map<Key, Value, KeyComparator, PairAllocator>& rhs
) {
    lhs.swap(rhs);
}

#endif
