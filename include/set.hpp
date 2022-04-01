#ifndef SET_HPP
# define SET_HPP
# include "tree.hpp"
# include "pair.hpp"
# include "ft_algorithm.hpp"

namespace ft {

    template <
        class Key,
        class KeyComparator = std::less<Key>,
        class KeyAllocator = std::allocator<Key>
    >
    class set {
        public:
            typedef Key                                         key_type;
            typedef Key                                         value_type;
            typedef KeyComparator                               key_compare;
            typedef KeyComparator                               value_compare;
            typedef KeyAllocator                                allocator_type;
            typedef set<key_type, key_compare, allocator_type>  self_type;
            typedef value_type&                                 reference;
            typedef const value_type&                           const_reference;
            typedef typename allocator_type::pointer            pointer;
            typedef typename allocator_type::const_pointer      const_pointer;
            typedef typename allocator_type::size_type          size_type;
            typedef typename allocator_type::difference_type    difference_type;

            // 内部tree
            typedef ft::tree<value_type, key_compare, allocator_type>
                                                                base;
            class const_iterator {
                FT_PRIVATE:
                    typedef typename base::node_type    node_type;
                    typedef typename base::iterator     tree_iterator;
                    typedef typename base::const_iterator
                                                        tree_const_iterator;
                    typedef const_iterator              const_iterator_type;

                    tree_const_iterator                 tree_it_;

                public:
                    typedef std::bidirectional_iterator_tag
                                                        iterator_category;
                    typedef set::value_type             value_type;
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
                    virtual ~const_iterator() {}

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
            typedef const_iterator  iterator;

        // [[インナークラス群]]

        FT_PRIVATE:

            // 内部ツリー
            base    tree_;

        public:

            // [[コンストラクタ群]]

            set(): tree_(KeyComparator(), KeyAllocator()) {}

            explicit set(
                const KeyComparator& comp
            ): tree_(comp, KeyAllocator()) {}

            explicit set(
                const KeyComparator& comp,
                const KeyAllocator& alloc
            ): tree_(comp, alloc) {}


            template <class InputIterator>
            set(InputIterator first,
                InputIterator last,
                const KeyComparator& comp = KeyComparator(),
                const KeyAllocator& alloc = KeyAllocator()
            ): tree_(comp, alloc) {
                insert(first, last);
            }

            set(const self_type& other) {
                *this = other;
            }

            ~set() {
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
                return tree_.value_compare();
            }

            // setではkey_compと同じ.
            value_compare       value_comp() const {
                return key_comp();
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
                typename base::const_iterator  e = tree_.end();
                for (; first != last; ++first) {
                    tree_.insert(e, *first);
                }
            }

            // [[削除]]

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
    template <class Key, class KeyComparator, class KeyAllocator>
    bool operator==(
        const ft::set<Key, KeyComparator, KeyAllocator>& lhs,
        const ft::set<Key, KeyComparator, KeyAllocator>& rhs
    ) {
        // ランダムアクセスイテレータを持つコンテナなら equal 側でサイズ比較をするが、
        // set はそうではないので, 自前でサイズ比較をやる.
        return lhs.size() == rhs.size()
            && ft::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class Key, class KeyComparator, class KeyAllocator>
    bool operator!=(
        const ft::set<Key, KeyComparator, KeyAllocator>& lhs,
        const ft::set<Key, KeyComparator, KeyAllocator>& rhs
    ) {
        return !(lhs == rhs);
    }

    // lhs と rhs の内容を辞書的に比較します。 比較は std::lexicographical_compare と同等の関数によって行われます。 
    // lhs の内容が rhs の内容より辞書的に小さい場合は true、そうでなければ false。
    template <class Key, class KeyComparator, class KeyAllocator>
    bool operator<(
        const ft::set<Key, KeyComparator, KeyAllocator>& lhs,
        const ft::set<Key, KeyComparator, KeyAllocator>& rhs
    ) {
        // lhs の内容が rhs の内容より辞書的に小さいまたは等しい場合は true、そうでなければ false。
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class Key, class KeyComparator, class KeyAllocator>
    bool operator<=(
        const ft::set<Key, KeyComparator, KeyAllocator>& lhs,
        const ft::set<Key, KeyComparator, KeyAllocator>& rhs
    ) {
        return !(lhs > rhs);
    }

    template <class Key, class KeyComparator, class KeyAllocator>
    bool operator>(
        const ft::set<Key, KeyComparator, KeyAllocator>& lhs,
        const ft::set<Key, KeyComparator, KeyAllocator>& rhs
    ) {
        // lhs の内容が rhs の内容より辞書的に大きいまたは等しい場合は true、そうでなければ false。
        return rhs < lhs;
    }

    template <class Key, class KeyComparator, class KeyAllocator>
    bool operator>=(
        const ft::set<Key, KeyComparator, KeyAllocator>& lhs,
        const ft::set<Key, KeyComparator, KeyAllocator>& rhs
    ) {
        return !(lhs < rhs);
    }

}

template <class Key, class KeyComparator, class KeyAllocator>
void swap(
    ft::set<Key, KeyComparator, KeyAllocator>& lhs,
    ft::set<Key, KeyComparator, KeyAllocator>& rhs
) {
    lhs.swap(rhs);
}

#endif
