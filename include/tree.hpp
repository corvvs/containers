#ifndef TREE_HPP
# define TREE_HPP

# include "ft_common.hpp"
# include "ft_meta_functions.hpp"
# include "ft_iterator.hpp"
# include "ft_algorithm.hpp"
# include "pair.hpp"
# include <memory>
# include <iostream>
# include <iterator>
# include <exception>
# include <stdexcept>

namespace ft {

    // tree (Red-Black Tree)
    template <
        // ノードに載る値の型
        // - map<MK, MV, MKC, MA>   -> pair<MK, MV>
        // - set<SK, SKC, SA>       -> SK
        // ※Valueはノードではない！
        class Value,
        // Valueのコンパレータ
        // - map<MK, MV, MKC, MA>
        //  - Value同士を取り、MK同士として比較するコンパレータ
        //  - bool compare(const pair<MK, MV>&, const pair<MK, MV>&);
        //                          -> map_value_compare<MK, pair<MK, MV>, MKC>
        // - set<SK, SKC, SA>       
        //  - bool compare(const SK&, const SK&);
        //                          -> SKC
        class ValueComparator = std::less<Value>,
        // Valueのアロケータ
        // - map<MK, MV, MKC, MA>   -> MA
        // - set<SK, SKC, SA>       -> SA
        class ValueAllocator = std::allocator<Value>
    >
    class tree {
        public:
            // インナークラス

            // [treeのノード]
            // ツリーの各要素(Value)のポインタ(Value*)を持つ。
            // 左子・右子・親へのポインタを持つ。
            // - 親がnull -> endノード
            // - 子がnull -> 子はNILノード
            // ノードはelementのライフタイムについて責を負わない。
            // (ので、コピーフリー)
            template<class TreeValue>
            class TreeNode {
                public:
                    typedef TreeValue               tree_value;
                    typedef tree_value*             tree_value_pointer;
                    typedef TreeNode<tree_value>    tree_node;
                    typedef tree_node*              tree_node_pointer;
                    typedef const tree_node*        tree_node_const_pointer;

                FT_PRIVATE:
                    // ツリーが保持する要素
                    tree_value_pointer  tree_value_;
                    // 左子へのポインタ
                    tree_node_pointer   left_child_node_;
                    // 右子へのポインタ
                    tree_node_pointer   right_child_node_;
                    // 親へのポインタ
                    tree_node_pointer   parent_node_;
                    // ノードが黒か
                    bool                is_black_;

                public:
                    // デフォルト構築
                    // endノードもこれで作る。
                    // endノードは黒。
                    explicit TreeNode()
                        :   tree_value_(NULL),
                            left_child_node_(NULL), right_child_node_(NULL), parent_node_(NULL),
                            is_black_(true) {}
                    // endでないノードは赤。
                    explicit TreeNode(tree_value& element)
                        :   tree_value_(&element),
                            left_child_node_(NULL), right_child_node_(NULL), parent_node_(NULL),
                            is_black_(false) {}
                    TreeNode(const TreeNode& other) {
                        *this = other;
                    }
                    ~TreeNode() {}

                    tree_node& operator=(const tree_node& rhs) {
                        if (this == &rhs) { return *this; }
                        tree_value_ = rhs.tree_value_;
                        left_child_node_ = rhs.left_child_node_;
                        right_child_node_ = rhs.right_child_node_;
                        parent_node_ = rhs.parent_node_;
                        is_black_ = rhs.is_black_;
                        return *this;
                    }

                public:

                    // [[getters]]

                    tree_value_pointer       value() { return tree_value_; }
                    const tree_value_pointer value() const { return tree_value_; }
                    tree_node_pointer&       left() { return left_child_node_; }
                    tree_node_pointer&       right() { return right_child_node_; }
                    tree_node_pointer&       parent() { return parent_node_; }
                    const tree_node_pointer& left() const { return left_child_node_; }
                    const tree_node_pointer& right() const { return right_child_node_; }
                    const tree_node_pointer& parent() const { return parent_node_; }

                    // [[predicates]]

                    bool is_black() const { return is_black_; }
                    // 自身が左子かどうか
                    // (rootに対してはfalseを返すことに注意！)
                    bool is_left_child() const { return has_parent_() && parent()->left() == this; }
                    // 自身が右子かどうか
                    bool is_right_child() const { return has_parent_() && parent()->right() == this; }
                    bool has_left_child() const { return left() != NULL; }
                    bool has_right_child() const { return right() != NULL; }

                FT_PRIVATE:

                    // [[private predicates]]
                    bool is_end_() const { return !has_parent_(); }
                    bool is_root_() const { return has_parent_() && parent()->is_end_(); }
                    bool has_parent_() const { return parent() != NULL; }

                    // 自身を根とする部分木の最小ノードを返す
                    // O(log2 |V|)
                    tree_node_pointer   min_node_() {
                        tree_node_pointer    next_ptr = this;
                        // 左子があるなら左子をたどり続ける
                        while (next_ptr->has_left_child()) {
                            next_ptr = next_ptr->left();
                        }
                        return next_ptr;
                    }

                    // 自身を根とする部分木の最大ノードを返す
                    // O(log2 |V|)
                    tree_node_pointer   max_node_() {
                        tree_node_pointer    next_ptr = this;
                        // 右子があるなら右子をたどり続ける
                        while (next_ptr->has_right_child()) {
                            next_ptr = next_ptr->right();
                        }
                        return next_ptr;
                    }

                public:

                    // 後隣接するノードを返す
                    // O(log2 |V|)
                    // ただし、tree.begin()からtree.end()までを辿った場合の総計算量は O(log2 |E|)となる。
                    // (|E|は辺数; すべての辺を1度ずつ通るため。辺は向きを区別する。)
                    tree_node_pointer   forward_neighbor() {
                        if (has_right_child()) {
                            // 右子がある場合
                            // -> 右子のminを返す
                            return right()->min_node_();
                        }
                        // 右子がない場合
                        // -> 自身が左子になるまで(=右子である間は)親に遡る
                        // -> 最後に親(「右親」)を返す。
                        // (endの場合はNULLが返る)
                        tree_node_pointer    ptr = this;
                        while (ptr->is_right_child()) {
                            ptr = ptr->parent();
                        }
                        return ptr->parent();
                    }
                    tree_node_const_pointer forward_neighbor() const {
                        if (has_right_child()) {
                            return right()->min_node_();
                        }
                        tree_node_const_pointer    ptr = this;
                        while (ptr->is_right_child()) {
                            ptr = ptr->parent();
                        }
                        return ptr->parent();
                    }

                    // 前隣接するノードを返す
                    // O(log2 n)
                    tree_node_pointer    backward_neighbor() {
                        if (has_left_child()) {
                            // 左子がある場合
                            // -> 左子のminを返す
                            return left()->max_node_();
                        }
                        // 左子がない場合
                        // -> 自身が右子になるまで(=左子である間は)親に遡る
                        // -> 最後に親(「左親」)を返す。
                        tree_node_pointer    ptr = this;
                        while (ptr->is_left_child()) {
                            ptr = ptr->parent();
                        }
                        return ptr->parent();
                    }
                    tree_node_const_pointer    backward_neighbor() const {
                        if (has_left_child()) {
                            return left()->max_node_();
                        }
                        tree_node_const_pointer    ptr = this;
                        while (ptr->is_left_child()) {
                            ptr = ptr->parent();
                        }
                        return ptr->parent();
                    }

                    // 「シス子」、つまり
                    // 自分が左子なら右子、自分が右子なら左子を返す。
                    tree_node_pointer    cis_child() const {
                        if (is_left_child()) { return right(); }
                        if (is_right_child()) { return left(); }
                        return NULL;
                    }

                    // 「トランス子」、つまり
                    // 自分が左子なら左子、自分が右子なら右子を返す。
                    tree_node_pointer    trans_child() const {
                        if (is_left_child()) { return left(); }
                        if (is_right_child()) { return right(); }
                        return NULL;
                    }

                    // childがthisの子である場合、childと逆の子を返す
                    // そうでない場合 NULL を返す
                    tree_node_pointer   counter_child(const tree_node_pointer& child) const {
                        if (left() == child) { return right(); }
                        if (right() == child) { return left(); }
                        return NULL;
                    }

                    // 自分がトランス子かどうかを返す。
                    bool                is_trans_child() const {
                        return !is_end_() && is_left_child() == parent()->is_left_child();
                    }

                    // 自分がシス子かどうかを返す。
                    bool                is_cis_child() const {
                        return !is_end_() && is_left_child() != parent()->is_left_child();
                    }

                    // valueだけをswapする。
                    void    swap_value(tree_node& other) {
                        ft::swap(tree_value_, other.tree_value_);
                    }

                    void    swap_color(tree_node& other) {
                        ft::swap(is_black_, other.is_black_);
                    }

                    // 色を反転する
                    void flip_color() { is_black_ = !is_black_; }

                    // 2つのノードの位置関係、つまり接続されているエッジを入れ替える.
                    // 入れ替え対象はどちらも通常のノード.
                    // (なぜこんな面倒なことをするかというと, ポインタと実体の対応関係を保つため.)
                    void    swap_position(tree_node& other) {
                        bool    this_was_left_child = is_root_() || is_left_child();
                        bool    other_was_left_child = other.is_root_() || other.is_left_child();
                        // 色をswap
                        swap_color(other);
                        // 出る方のswap
                        ft::swap(parent(), other.parent());
                        ft::swap(left(), other.left());
                        ft::swap(right(), other.right());
                        // 入る方
                        pull_in_(other, other_was_left_child);
                        other.pull_in_(*this, this_was_left_child);
                    }

                    // child を自身の左子として配置する
                    void    place_into_left(tree_node_pointer child) {
                        this->left() = child;
                        if (child != NULL) {
                            child->parent() = this;
                        }
                    }

                    // child を自身の右子として配置する
                    void    place_into_right(tree_node_pointer child) {
                        this->right() = child;
                        if (child != NULL) {
                            child->parent() = this;
                        }
                    }

                FT_PRIVATE:

                    // swap_positionの一部分
                    // 自分と隣接するノードを自分に向ける。
                    // ただし、自分とcounterが隣接している場合は特別扱いが必要。
                    void pull_in_(tree_node& counter, bool counter_was_left_child) {
                        if (left()) {
                            if (left() == this)
                                left() = &counter;
                            else
                                left()->parent() = this;
                        }
                        if (right()) {
                            if (right() == this)
                                right() = &counter;
                            else
                                right()->parent() = this;
                        }
                        if (parent() == this)
                            parent() = &counter;
                        else
                            (counter_was_left_child ? parent()->left() : parent()->right())
                                = this;
                    }

                /*
                // [[デバッグメソッド]]
                public:

                    // TODO: 提出時は消す
                    friend std::ostream&    operator<<(std::ostream& stream, const tree_node& rhs) {
                        stream << "[";
                        if (rhs.is_end_())
                            stream << "(end)";
                        else
                        {
                            if (rhs.is_root_())
                                stream << "(root)";
                            // stream << *(rhs.value());
                        }
                        stream << ":" << &rhs;
                        stream << ":" << (rhs.is_black() ? "B" : "r");
                        stream << ":";
                        if (rhs.has_left_child())
                            stream << "(" << &*(rhs.left()) << ")";
                        else
                            stream << "-";
                        if (rhs.has_parent_())
                            stream << "(" << &*(rhs.parent()) << ")";
                        else
                            stream << "-";
                        if (rhs.has_right_child())
                            stream << "(" << &*(rhs.right()) << ")";
                        else
                            stream << "-";
                        return stream << "]";
                    }

                    std::size_t debug_shortest_height() const {
                        return 1 + std::min(
                            left() ? left()->debug_shortest_height() : 0,
                            right() ? right()->debug_shortest_height() : 0
                        );
                    }

                    std::size_t debug_longest_height() const {
                        return 1 + std::max(
                            left() ? left()->debug_longest_height() : 0,
                            right() ? right()->debug_longest_height() : 0
                        );
                    }

                    std::size_t debug_shortest_black_height() const {
                        return (is_black() ? 1 : 0) + std::min(
                            left() ? left()->debug_shortest_black_height() : 0,
                            right() ? right()->debug_shortest_black_height() : 0
                        );
                    }

                    std::size_t debug_longest_black_height() const {
                        return (is_black() ? 1 : 0) + std::max(
                            left() ? left()->debug_longest_black_height() : 0,
                            right() ? right()->debug_longest_black_height() : 0
                        );
                    }
                */
            };

            // 要素のタイプ
            typedef Value                               value_type;
            // 要素のコンパレータ
            typedef ValueComparator                     value_comparator_type;
            // 要素のアロケータ
            typedef ValueAllocator                      value_allocator_type;
            // ノードクラス
            typedef TreeNode<value_type>                node_type;
            // ノードのアロケータ
            typedef typename value_allocator_type::template rebind<node_type>::other
                                                                    node_allocator_type;
            typedef typename node_allocator_type::reference         reference;
            typedef typename node_allocator_type::const_reference   const_reference;
            typedef typename node_allocator_type::pointer           pointer;
            typedef typename node_allocator_type::const_pointer     const_pointer;

            typedef tree<Value, ValueComparator, ValueAllocator>    self_type;
            typedef typename std::size_t                            size_type;
            typedef typename std::ptrdiff_t                         difference_type;
            typedef typename ValueAllocator::reference              value_reference;
            typedef typename ValueAllocator::const_reference        value_const_reference;
            typedef typename ValueAllocator::pointer                value_pointer;
            typedef typename ValueAllocator::const_pointer          value_const_pointer;

            // インナークラス

            // [ノードホルダー]
            // ノード構築時の例外安全性を担保するための一時オブジェクト
            class TreeNodeHolder {
                public:
                    typedef tree::value_type            value_type;
                    typedef value_type*                 value_pointer;
                    typedef tree::node_allocator_type   node_allocator_type;
                    typedef tree::value_allocator_type  value_allocator_type;
                    typedef tree::node_type             node_type;
                    typedef node_type*                  node_pointer;

                FT_PRIVATE:
                    bool                    value_constructed_;
                    bool                    node_constructed_;
                    node_allocator_type     node_alloc_;
                    value_allocator_type    value_alloc_;
                    node_pointer            node_ptr_;
                    value_pointer           value_ptr_;

                public:
                    TreeNodeHolder()
                        :   value_constructed_(false), node_constructed_(false),
                            node_alloc_(), value_alloc_(),
                            node_ptr_(NULL), value_ptr_(NULL) {}
                    TreeNodeHolder(
                        const node_allocator_type& node_alloc,
                        const value_allocator_type& value_alloc
                    ):      value_constructed_(false), node_constructed_(false),
                            node_alloc_(node_alloc), value_alloc_(value_alloc),
                            node_ptr_(NULL), value_ptr_(NULL) {}
                    TreeNodeHolder(const self_type& other) {
                        *this = other;
                    }
                    ~TreeNodeHolder() {
                        destroy_();
                    }

                    // このクラスが保持するオブジェクトは増殖して欲しくないので,
                    // 特殊な処理になっている
                    TreeNodeHolder&  operator=(const self_type &rhs) {
                        if (this != &rhs) {
                            swap(rhs);
                        }
                        return *this;
                    }

                    void    swap(self_type& other) {
                        ft::swap(value_constructed_, other.value_constructed_);
                        ft::swap(node_constructed_, other.node_constructed_);
                        ft::swap(node_alloc_, other.node_alloc_);
                        ft::swap(value_alloc_, other.value_alloc_);
                        ft::swap(node_ptr_, other.node_ptr_);
                        ft::swap(value_ptr_, other.value_ptr_);
                    }

                    // value構築済みなら、あらかじめ与えておいたアロケータを使い、ノードを構築する
                    void    construct_node() {
                        if (value_constructed_ && !node_constructed_ && node_ptr_ == NULL) {
                            node_ptr_ = node_alloc_.allocate(1);
                            if (node_ptr_ && !node_constructed_) {
                                node_alloc_.construct(node_ptr_, node_type(*value_ptr_));
                                // end 以外の新しいノードは赤とする
                                node_constructed_ = true;
                            }
                        }
                    }

                    // あらかじめ与えておいたアロケータを使い、valueを構築する
                    void    construct_value(const value_type& val) {
                        if (!node_constructed_ && value_ptr_ == NULL) {
                            value_ptr_ = value_alloc_.allocate(1);
                            if (value_ptr_ && !value_constructed_) {
                                value_alloc_.construct(value_ptr_, val);
                                value_constructed_ = true;
                            }
                        }
                    }

                    // valueの構築が必要ない場合、value構築フラグだけを立てる
                    void    construct_blank_value() {
                        if (node_constructed_) {
                            value_constructed_ = true;
                        }
                    }

                    // ノードとValueが確保・構築済みであればノードのポインタを返す。
                    // その際、自身の中身を空にして、所有権を放棄する。
                    // そうでなければNULLを返す。
                    // 所有権は保持したまま。
                    node_pointer    release() {
                        if (node_constructed_ && value_constructed_) {
                            node_pointer    rv = node_ptr_;
                            node_ptr_ = NULL;
                            value_ptr_ = NULL;
                            node_constructed_ = false;
                            value_constructed_ = false;
                            return (rv);
                        }
                        return NULL;
                    }

                FT_PRIVATE:
                    // ノード, value を保持している場合、それらを破壊する
                    void    destroy_() {
                        if (node_ptr_) {
                            if (node_constructed_) {
                                node_alloc_.destroy(node_ptr_);
                            }
                            node_alloc_.deallocate(node_ptr_, 1);
                        }
                        if (value_ptr_) {
                            if (value_constructed_) {
                                value_alloc_.destroy(value_ptr_);
                            }
                            value_alloc_.deallocate(value_ptr_, 1);
                        }
                    }
            };

            public:
                typedef TreeNodeHolder  node_holder_type;

            class const_iterator;

            // [(variable)イテレータ]
            // treeのイテレータはノードを返す。
            // ノードに乗ってる要素を返すのは利用者(map, set)の責任とする。
            class iterator {
                public:
                    typedef iterator                                    iterator_type;
                    typedef const_iterator                              const_iterator_type;
                    typedef typename tree::value_type                   value_type;
                    typedef typename tree::difference_type              difference_type;
                    typedef typename tree::pointer                      pointer;
                    typedef typename tree::reference                    reference;
                    typedef typename std::bidirectional_iterator_tag    iterator_category;

                FT_PRIVATE:
                    pointer    ptr_;

                public:
                    iterator(): ptr_(NULL) {}
                    explicit iterator(pointer ptr): ptr_(ptr) {}
                    iterator(const iterator_type& other)
                        : ptr_(other.ptr_) {}
                    iterator(const_iterator_type constant)
                        : ptr_(const_cast<pointer>(constant.operator->())) {}
                    // iterator_type&  operator=(const iterator_type &rhs) {
                    //     ptr_ = rhs.ptr_;
                    //     return *this;
                    // }
                    // ~iterator() {}

                FT_PRIVATE:
                    pointer  next() const { return ptr_->forward_neighbor(); }
                    pointer  prev() const { return ptr_->backward_neighbor(); }

                public:
                    reference    operator*() const { return *ptr_; }
                    pointer      operator->() const { return ptr_; }

                    iterator_type&      operator++() {
                        ptr_ = next();
                        return *this;
                    }
                    iterator_type       operator++(int) {
                        iterator_type   it = *this;
                        ++*this;
                        return it;
                    }

                    iterator_type&      operator--() {
                        ptr_ = prev();
                        return *this;
                    }
                    iterator_type       operator--(int) {
                        iterator_type   it = *this;
                        --*this;
                        return it;
                    }

                    bool         operator==(const iterator_type& rhs) const {
                        return ptr_ == rhs.ptr_;
                    }
                    bool         operator!=(const iterator_type& rhs) const {
                        return !(*this == rhs);
                    }
            };

            // [constイテレータ]
            class const_iterator {
                public:
                    typedef const_iterator                              iterator_type;
                    typedef iterator                                    variable_iterator_type;
                    typedef typename tree::value_type                   value_type;
                    typedef typename tree::difference_type              difference_type;
                    typedef typename tree::const_pointer                pointer;
                    typedef typename tree::const_reference              reference;
                    typedef typename std::bidirectional_iterator_tag    iterator_category;

                FT_PRIVATE:
                    pointer    ptr_;

                public:
                    const_iterator(): ptr_(NULL) {}
                    explicit const_iterator(pointer ptr): ptr_(ptr) {}
                    const_iterator(const iterator_type& other)
                        : ptr_(other.ptr_) {}
                    const_iterator(variable_iterator_type variable)
                        : ptr_(variable.operator->()) {}
                    iterator_type&  operator=(const iterator_type &rhs) {
                        ptr_ = rhs.ptr_;
                        return *this;
                    }
                    ~const_iterator() {}

                FT_PRIVATE:
                    pointer  next() const { return ptr_->forward_neighbor(); }
                    pointer  prev() const { return ptr_->backward_neighbor(); }

                public:
                    reference        operator*() const { return *ptr_; }
                    pointer          operator->() const { return ptr_; }

                    iterator_type&          operator++() {
                        ptr_ = next();
                        return *this;
                    }
                    iterator_type           operator++(int) {
                        iterator_type   it = *this;
                        ++*this;
                        return it;
                    }

                    iterator_type&          operator--() {
                        ptr_ = prev();
                        return *this;
                    }
                    iterator_type           operator--(int) {
                        iterator_type   it = *this;
                        --*this;
                        return it;
                    }

                    bool                    operator==(const iterator_type& rhs) const {
                        return ptr_ == rhs.ptr_;
                    }
                    bool                    operator!=(const iterator_type& rhs) const {
                        return !(*this == rhs);
                    }

            };

            typedef typename ft::reverse_iterator<iterator>         reverse_iterator;
            typedef typename ft::reverse_iterator<const_iterator>   const_reverse_iterator;

        FT_PRIVATE:
            // [[メンバ変数]]

            // endノード
            node_type               end_node_;
            // beginノード(キャッシュ)
            pointer                 begin_node_;
            // ノードアロケータ
            node_allocator_type     node_allocator_;
            // Valueのアロケータ
            value_allocator_type    value_allocator_;
            // ノード数
            size_type               size_;
            // Valueの比較関数
            value_comparator_type   value_compare_;

        public:
            // [[コンストラクタ群]]

            // デフォルト
            tree():
                end_node_(node_type()),
                node_allocator_(node_allocator_type()),
                size_(0),
                value_compare_(value_comparator_type()) {
                    begin_node_ = end_node();
                }
            // コンパレータ指定
            explicit tree(const value_comparator_type& comp):
                end_node_(node_type()),
                node_allocator_(node_allocator_type()),
                size_(0),
                value_compare_(comp) {
                    begin_node_ = end_node();
                }
            // アロケータ指定
            explicit tree(const value_allocator_type& allocator):
                end_node_(node_type()),
                value_allocator_(allocator),
                size_(0),
                value_compare_(value_comparator_type())
            {
                begin_node_ = end_node();
            }
            // コンパレータ・アロケータ指定
            explicit tree(const value_comparator_type& comp, const value_allocator_type& allocator):
                end_node_(node_type()),
                value_allocator_(allocator),
                size_(0),
                value_compare_(comp)
            {
                begin_node_ = end_node();
            }
            // コピーコンストラクタ
            tree(const self_type& other) {
                *this = other;
            }
            // デストラクタ
            ~tree() {
                clear();
            }

            self_type& operator=(const self_type& rhs) {
                if (this == &rhs) { return *this; }
                // copy and swap.
                // - temp の作成に           O(1)
                // - ranged-insertion に    O(N)
                // - swapに                 O(1)
                // - tempの破壊に            O(N)
                // 全体で O(N).
                self_type   temp(rhs.value_compare_, rhs.value_allocator_);
                temp.insert_node_(rhs.begin(), rhs.end());
                swap(temp);
                return *this;
            }

            value_allocator_type&        get_allocator() { return value_allocator_; }
            const value_allocator_type&  get_allocator() const { return value_allocator_; }
            node_allocator_type&         get_node_allocator() { return node_allocator_; }
            const node_allocator_type&   get_node_allocator() const { return node_allocator_; }
            value_comparator_type&       value_compare() { return value_compare_; }
            const value_comparator_type& value_compare() const { return value_compare_; }

            iterator                 begin()
                { return iterator(begin_node()); }
            const_iterator           begin() const
                { return const_iterator(begin_node()); }
            iterator                 end()
                { return iterator(end_node()); }
            const_iterator           end() const
                { return const_iterator(end_node()); }
            reverse_iterator         rbegin()
                { return reverse_iterator(end()); }
            const_reverse_iterator   rbegin() const
                { return reverse_iterator(end()); }
            reverse_iterator         rend()
                { return reverse_iterator(begin()); }
            const_reverse_iterator   rend() const
                { return reverse_iterator(begin()); }

            size_type    size() const { return size_; }
            bool         empty() const { return size() == 0; }
            size_type    max_size() const {
                return std::min<size_type>(
                    get_node_allocator().max_size(),
                    std::numeric_limits<difference_type >::max()
                );
            }

            // [[ 変更系 ]]

            // すべての要素を削除する(注意: endは破壊しない).
            // sizeが0になる.
            // beginはendと一致する.
            // endの左子はNULLになる.
            void    clear() {
                destroy_subtree_(root());
                size_ = 0;
                begin_node_ = end_node();
                end_node()->left() = NULL;
            }

            // [insert]
            // insertの例外安全性はSTRONG

            // 単一・ヒントなし挿入
            pair<iterator, bool>    insert(const value_type& key) {
                pair<pointer, pointer*> place = find_equal_(key);
                if (place.first == *(place.second)) {
                    // 挿入できない場合
                    return make_pair(iterator(place.first), false);
                }
                // 挿入できる場合
                // -> place.second の位置に挿入する。
                return make_pair(iterator(insert_at_(place, key)), true);
            }

            // 単一・ヒントあり挿入
            // > hint に可能な限り近い位置に key を挿入します。
            // > 挿入が hint の直後の位置に行われた場合、償却定数時間。
            // > そうでなければ、コンテナのサイズの対数。
            iterator    insert(iterator hint, const value_type& key) {
                pair<pointer, pointer*> place = find_equal_(&*hint, key);
                if (place.first == *(place.second)) {
                    // 挿入不可
                    return iterator(place.first);
                }
                // 挿入できる場合
                // -> place.second の位置に挿入する。
                pointer result = insert_at_(place, key);
                return iterator(result);
            }

            // 範囲挿入
            template <class InputIterator>
            // (使ってない)
            void    insert(InputIterator first, InputIterator last) {
                for (; first != last; ++first) {
                    // 常にendをヒントにしながら挿入するので,
                    //   - 挿入元ツリーが空
                    //   - [first, last)が挿入元ツリーのコンパレータにおいてsorted
                    // の2条件を満たすなら線形時間になる
                    // (例えばtreeをコピーする時にこの条件が満たされる).
                    // そうでなければO(nlogn)になる
                    // (もしかすると線形になることもあるかもしれないが).
                    insert(end(), *first);
                }
            }

        FT_PRIVATE:
            // [ライフサイクル]

            // value x を保持するノードを構築して、そのメモリアドレスを返す
            pointer create_node_(const value_type& x) {
                node_allocator_type&    na = get_node_allocator();
                value_allocator_type&   va = get_allocator();
                node_holder_type        holder(na, va);
                holder.construct_value(x);
                holder.construct_node();
                // releaseすると、holderは持っているノードを返す。
                // その際、holderはノードの所有権も手放す。
                // ---
                // release前に関数が終了した場合、ノードの所有権はholderにあるので、
                // holderのデストラクタでノードが破壊される。
                return holder.release();
            }

            // node_ptr が指し示すノードを破壊する
            void    destroy_node_(pointer node_ptr) {
                if (node_ptr == NULL) { return; }
                if (node_ptr != end_node() && node_ptr->value()) {
                    value_allocator_type&   va = get_allocator();
                    va.destroy(node_ptr->value());
                    va.deallocate(node_ptr->value(), 1);
                }
                node_allocator_type&    na = get_node_allocator();
                na.destroy(node_ptr);
                na.deallocate(node_ptr, 1);
            }

            // node_ptr を根とする部分木を再帰的に破壊する。
            void    destroy_subtree_(pointer node_ptr) {
                if (node_ptr == NULL) { return; }
                destroy_subtree_(node_ptr->left());
                destroy_subtree_(node_ptr->right());
                destroy_node_(node_ptr);
            }

        public:

            // [[検索系]]

            // [find]

            // keyと等しい要素があれば返す。
            // 言い換えると、 key <= element かつ element <= key が成り立つelementを返す。
            template <class Key>
            iterator         find(const Key& key) {
                iterator it = lower_bound(key);
                if (it == end() || value_compare()(key, *(it->value()))) {
                    return end();
                }
                return it;
            }
            template <class Key>
            const_iterator   find(const Key& key) const {
                const_iterator it = lower_bound(key);
                if (it == end() || value_compare()(key, *(it->value()))) {
                    return end();
                }
                return it;
            }

            // 「key より小さくない最小の要素」を返す
            // 言い換えると   min{ element | key <= element }
            // または        min{ element | !value_comp()(element, key) }
            template <class Key>
            iterator         lower_bound(const Key& key) {
                return iterator(lower_bound_ptr_(key));
            }
            template <class Key>
            const_iterator   lower_bound(const Key& key) const {
                return const_iterator(lower_bound_ptr_(key));
            }
            // 「key より大きい最小の要素」を返す
            template <class Key>
            iterator         upper_bound(const Key& key) {
                return iterator(upper_bound_ptr_(key));
            }
            template <class Key>
            const_iterator   upper_bound(const Key& key) const {
                return const_iterator(upper_bound_ptr_(key));
            }

            template <class Key>
            size_type        count(const Key& x) const {
                return find_ptr_(x) == end_node() ? 0 : 1;
            }

            // ツリーが key を保持しているなら、keyを含む最小の半開区間[lower_bound, upper_bound)を返す。
            // 保持していないなら、[upper_bound, upper_bound)を返す。
            template <class Key>
            pair<iterator, iterator>    equal_range(const Key& key) {
                pointer p = lower_bound_ptr_(key);
                if (p != end_node()) {
                    // p = min{ x | key <= x }, つまり key <= p
                    if (!value_compare()(key, *p->value())) {
                        // p <= key
                        // -> p <= key <= p
                        // つまり p == key
                        iterator    it2(p);
                        iterator    it1 = it2++;
                        return ft::make_pair(it1, it2);
                    }
                }
                return ft::make_pair(p, p);
            }
            template <class Key>
            pair<const_iterator, const_iterator>    equal_range(const Key& key) const {
                const_pointer p = lower_bound_ptr_(key);
                if (p != end_node()) {
                    // p = min{ x | key <= x }, つまり key <= p
                    if (!value_compare()(key, *p->value())) {
                        // p <= key
                        // -> p <= key <= p
                        // つまり p == key
                        const_iterator    it2(p);
                        const_iterator    it1 = it2++;
                        return ft::make_pair(it1, it2);
                    }
                }
                return ft::make_pair(p, p);
            }
            // pair<const_iterator, const_iterator> equal_range(const value_type& key) const {
            //     pair<iterator, iterator>    pp = equal_range(key);
            //     return pair<const_iterator, const_iterator>(
            //         static_cast<const_iterator>(pp.first),
            //         static_cast<const_iterator>(pp.second)
            //     );
            // }

        FT_PRIVATE:

            pointer          end_node() { return &end_node_; }
            const_pointer    end_node() const { return &end_node_; }
            pointer          root() { return end_node()->left(); }
            const_pointer    root() const { return end_node()->left(); }
            pointer          begin_node() { return begin_node_; }
            const_pointer    begin_node() const { return begin_node_; }

            // key と一致するノードがあれば、そのポインタを返す。
            // "key と一致"とはつまり、xが key <= x && x <= key を満たすこと。
            // なければendのポインタを返す。
            template <class Key>
            const_pointer   find_ptr_(const Key& key) const {
                const_pointer ptr = lower_bound_ptr_(key);
                if (ptr == end_node()) {
                    return ptr;
                }
                // key <= ptr
                if (value_compare()(key, *ptr->value())) {
                    // key < ptr -> ダメ
                    return end_node();
                }
                // key <= ptr かつ !(key < prt)
                // -> key == ptr であるはず
                return ptr;
            }

            // key以上の要素があれば、それらのうち最も小さいもののポインタを返す。
            // なければendのポインタを返す。
            template <class Key>
            pointer    lower_bound_ptr_(const Key& key) {
                pointer rv = end_node();
                pointer target = root();
                while (target != NULL) {
                    if (value_compare()(*target->value(), key)) {
                        // target < key
                        target = target->right();
                    } else {
                        // key <= target
                        // -> 返すべき値が存在する。
                        rv = target;
                        target = target->left();
                    }
                }
                return rv;
            }

            template <class Key>
            const_pointer   lower_bound_ptr_(const Key& key) const {
                const_pointer   rv = end_node();
                const_pointer   target = root();
                while (target != NULL) {
                    if (value_compare()(*target->value(), key)) {
                        // target < key
                        target = target->right();
                    } else {
                        // key <= target
                        // -> 返すべき値が存在する。
                        rv = target;
                        target = target->left();
                    }
                }
                return rv;
            }

            template <class Key>
            pointer    upper_bound_ptr_(const Key& key) {
                pointer rv = end_node();
                pointer target = root();
                while (target != NULL) {
                    if (value_compare()(key, *target->value())) {
                        // key < target
                        // -> 返すべき値が存在する。
                        rv = target;
                        target = target->left();
                    } else {
                        // target <= key
                        target = target->right();
                    }
                }
                return rv;
            }

            template <class Key>
            const_pointer    upper_bound_ptr_(const Key& key) const {
                const_pointer rv = end_node();
                const_pointer target = root();
                while (target != NULL) {
                    if (value_compare()(key, *target->value())) {
                        // key < target
                        // -> 返すべき値が存在する。
                        rv = target;
                        target = target->left();
                    } else {
                        // target <= key
                        target = target->right();
                    }
                }
                return rv;
            }

            // [[find_equal]]
            // insertionに使うため、insertion先候補とその親をpairで返すメソッド

            // RVは
            // - first: 挿入されるノードの親となるノードのポインタ
            // - second: 親(first)のleft / right のうち、挿入されるべき方のポインタ。
            // - *second がNULLでない場合は値があるということになる。
            pair<pointer, pointer*> find_equal_(const value_type& key) {
                pointer target = root();
                if (target == NULL) {
                    return pair<pointer, pointer*>(end_node(), &(end_node()->left()));
                }
                while (true) {
                    if (value_compare()(key, *target->value())) {
                        // DSOUT() << "key < target" << std::endl;
                        if (target->left() == NULL) {
                            return pair<pointer, pointer*>(target, &target->left());
                        }
                        target = target->left();
                    } else if (value_compare()(*target->value(), key)) {
                        // DSOUT() << "target < key" << std::endl;
                        if (target->right() == NULL) {
                            return pair<pointer, pointer*>(target, &target->right());
                        }
                        target = target->right();
                    } else {
                        // DSOUT() << "key == target" << std::endl;
                        break;
                    }
                }
                // keyと同じものが見つかった。
                pointer     target_parent = target->parent();
                pointer*    target_ptr = &(
                    target->is_left_child() ? target_parent->left() : target_parent->right()
                );
                return pair<pointer, pointer*>(target, target_ptr);
            }

            // ヒントありfind_equal
            pair<pointer, pointer*> find_equal_(pointer hint, const value_type& key) {
                // [ヒントとは]
                // -> ツリー内に一致するノードがあり、
                //    かつその直前または直後がvalueの入るべき位置であるようなノード
                if (hint == end_node() || value_compare()(key, *(hint->value()))) {
                    // (1) hint == end or key < *hint
                    // -> prev = hint - 1 として, prev == begin or *prev < key なら,
                    //    prevとhintの間にkeyがあるべき.
                    //    (prev < key < hint)
                    iterator    prev(hint);
                    if (&*prev != begin_node() && value_compare()(*((--prev)->value()), key)) {
                        // このとき、hintとprevの関係性は以下のいずれかとなり,
                        // hintとprevのうち少なくとも一方が空き子を持つ.
                        if (prev->right() == NULL) {
                            // 1. hintの左子がprev
                            //   -> prevは右子を持たない.(持つならそれがhintとprevの間に入る)
                            return pair<pointer, pointer*>(&*prev, &(prev->right()));
                        } else {
                            // 2. prevの右部分木の最小値がhint
                            //   -> hintは左子を持たない.(持つならそれがhintとprevの間に入る)
                            return pair<pointer, pointer*>(&*hint, &(hint->left()));
                        }
                    }
                    return find_equal_(key);
                } else if (value_compare()(*(hint->value()), key)) {
                    // (2) *hint < key
                    // -> next = hint + 1 として、 next == end or key < *next なら,
                    //    hintとnextの間にkeyがあるべき.
                    //    (hint < key < next)
                    iterator    next(hint);
                    if (&*++next != end_node() && value_compare()(key, *((next->value())))) {
                        // この時、hintとnextの関係性は以下のいずれかとなり,
                        // hintとnextのうち少なくとも一方が空き子を持つ.
                        if (next->left() == NULL) {
                            // 1. hintの右部分木の最小値がnext
                            //   -> nextは左子を持たない.(持つならそれがhintとnextの間に入る)
                            return pair<pointer, pointer*>(&*next, &(next->left()));
                        } else {
                            // 2. nextの左子がhint
                            //   -> hintは右子を持たない.(持つならそれがhintとnextの間に入る)
                            return pair<pointer, pointer*>(&*hint, &(hint->right()));
                        }
                    }
                    return find_equal_(key);
                }
                // (3) *hint == key
                return pair<pointer, pointer*>(end_node(), &(end_node()->left()));
            }

            pointer insert_at_(pair<pointer, pointer*>& place, const value_type& x) {
                *(place.second) = create_node_(x);
                pointer inserted = *(place.second);
                (*(place.second))->parent() = place.first;
                size_ += 1;
                // begin が変更されるのは:
                // 1. beginより小さい要素が挿入された時
                // 2. beginが削除された時
                // -> 挿入されたノードがbeginのleftである場合, 1.に該当するのでキャッシュを更新する。
                if (begin_node()->left() == *(place.second)) {
                    begin_node_ = *(place.second);
                }
                rebalance_after_insertion_(*(place.second));
                return inserted;
            }

            void    insert_node_(const_iterator first, const_iterator last) {
                for (; first != last; ++first) {
                    insert(end(), *(first->value()));
                }
            }


        public:
            // [[swap]]

            void    swap(self_type& other) {
                ft::swap(size_, other.size_);
                ft::swap(value_compare_, other.value_compare_);
                ft::swap(node_allocator_, other.node_allocator_);
                ft::swap(value_allocator_, other.value_allocator_);
                ft::swap(end_node_, other.end_node_);
                ft::swap(begin_node_, other.begin_node_);
                // end_node_ はポインタではなく実体として持っているので、
                // end_node_ 絡みのものを再設定する必要がある。
                // さらに、 this だけでなく other の分もやってあげる必要がある。
                after_swap_();
                other.after_swap_();
            }

        FT_PRIVATE:

            // swap 後のもろもろ再設定
            void    after_swap_() {
                if (size() == 0) {
                    // size() == 0 なら、beginをendに再設定する。
                    begin_node_ = end_node();
                } else {
                    // ルートノートがある(<=> size() > 0)なら、ルートの親を今のendに設定し直す。
                    root()->parent() = end_node();
                }
            }

        public:

            // [[erase]]

            // (1)
            // 単一削除
            void        erase(iterator position) {
                swap_down_(position);

                // ここまで来たということは、positionは子を持たない。
                pointer target = &(*position);
                pointer parent = target->parent();
                bool    target_is_black = target->is_black();
                bool    target_is_begin = target == begin_node();
                release_node_from_parent_(target);
                destroy_node_(target);
                // Case 0. is red -> 破壊するだけ
                if (target_is_black) {
                    rebalance_after_erasure_(parent, NULL);
                }
                // 必要なら begin_node_ を更新
                if (target_is_begin) {
                    begin_node_ = parent;
                }
            }

            // (2)
            void        erase(iterator first, iterator last) {
                for (iterator it = first; it != last;) {
                    erase(it++);
                }
            }

            // (3)
            // key に一致するものがあれば削除。
            // 削除した場合は1, しなかった場合は0を返す。
            // -> 単にfindでerase(iterator)に投げるだけ
            template <class Key>
            size_type   erase_by_key(const Key& key) {
                iterator it = find(key);
                if (it == end()) {
                    return 0;
                }
                erase(it);
                return 1;
            }

            // ある要素を消す時、その要素と「隣の要素」をスワップしてから
            // 「隣の要素」を消すことにしても、要素の順序は変化しない。
            // 「隣の要素」としてツリーを下る(=葉の方に向かう)方を選択し続ければ、
            // やがて子を持たないノードに行き当たる。
            // 「隣の要素」としては、以下のいずれかを採用する
            // - 対象要素が右部分木を持つ -> 右部分木の最小要素
            // - 対象要素が左部分木を持つ -> 左子
            // - 対象要素が子を持たない -> 打ち止め
            iterator    swap_down_(iterator& position) {
                while (true) {
                    iterator    dummy = position;
                    if (position->has_right_child()) {
                        ++position;
                        position->swap_position(*dummy);
                    } else if (position->has_left_child()) {
                        --position;
                        position->swap_position(*dummy);
                    } else {
                        break;
                    }
                    position = dummy;
                }
                return position;
            }

        FT_PRIVATE:

            // [[リバランシング]]

            // 挿入後リバランス
            void    rebalance_after_insertion_(pointer node) {
                // 0. Nが通常ノードでないか、黒ノードの場合
                // -> なにもしない
                if (node == NULL) {
                    // DOUT() << "do nothing; is NULL" << std::endl;
                    return;
                }
                if (node == end_node()) {
                    // DOUT() << "do nothing; is end" << std::endl;
                    return;
                }
                if (node->is_black()) {
                    // DOUT() << "do nothing; is black" << std::endl;
                    return;
                }
                // 1. Nが根ノードの場合
                // -> 色変して終わり
                if (node == root()) {
                    // DOUT() << "flip and exit; is root" << std::endl;
                    node->flip_color();
                    return;
                }
                // 2. Nの親が黒ノードの場合
                // -> なにもしなくてよい
                if (node->parent()->is_black()) {
                    // DOUT() << "do nothing; parent is black" << std::endl;
                    return;
                }
                // 3. Nの親が赤ノードの場合
                if (node->is_cis_child()) {
                    // 曲がっている場合(シス)
                    // -> Nの親の親とNの親で回転させる(トランスにする)
                    // https://ja.wikipedia.org/wiki/%E3%82%B7%E3%82%B9_(%E5%8C%96%E5%AD%A6)
                    // DOUT() << "cis -> trans" << std::endl;
                    pointer parent = node->parent();
                    rotate_(parent, node);
                    node = parent;
                }
                // 変数名については別図を参照のこと
                pointer p = node->parent();
                pointer q = p->parent();
                pointer u = q->counter_child(p);
                if (u == NULL || u->is_black()) {
                    // Uが黒ノードの場合
                    // P, Qを色変する
                    // さらにQ-Pに対して右回転を行う。
                    rotate_flip_(q, p);
                } else {
                    // Uが赤ノードの場合
                    // P, Q, Uの色を反転する。
                    // Q -> N として、再帰的に処理を繰り返すことにする。
                    p->flip_color();
                    q->flip_color();
                    u->flip_color();
                    rebalance_after_insertion_(q);
                }
                // DOUT() << "node -> " << *node << std::endl;
            }

            // 削除後リバランス
            void    rebalance_after_erasure_(pointer np, pointer nm) {
                if (nm == root()) {
                    // Mは根である
                    // DOUT() << "Case 1. do nothing" << std::endl;
                    return;
                }
                // nm は NULL である可能性がある。
                // 正しい赤黒木であれば、ns != NULL のはず。
                pointer ns = np->counter_child(nm);
                pointer nx = ns->cis_child();
                pointer ny = ns->trans_child();
                if (np->is_black() &&
                    ns->is_black() &&
                    (!nx || nx->is_black()) &&
                    (!ny || ny->is_black())
                ) {
                    // Case 3. BBBB
                    // PSXYすべて黒
                    // -> Sを赤に色変し。Pを基準にしてもう一度黒リバランスを行う
                    // DOUT() << "Case 3. flip ns and rebalance recursively" << std::endl;
                    ns->flip_color();
                    rebalance_after_erasure_(np->parent(), np);
                    return;
                }
                if (np->is_black() &&
                    !ns->is_black() &&
                    (!nx || nx->is_black()) &&
                    (!ny || ny->is_black())
                ) {
                    // Case 2. BRBB
                    // PXYが黒, Sが赤
                    // BSBB
                    // DOUT() << "Case 2." << std::endl;
                    rotate_flip_(np, ns);
                    // np = nm->parent();
                    ns = np->counter_child(nm);
                    nx = ns->cis_child();
                    ny = ns->trans_child();
                }
                if (!np->is_black() &&
                    ns->is_black() &&
                    (!nx || nx->is_black()) &&
                    (!ny || ny->is_black())
                ) {
                    // Case 4. RBBB
                    // SXYが黒, Pが赤
                    // RBBB
                    // DOUT() << "Case 4." << std::endl;
                    np->flip_color();
                    ns->flip_color();
                    return;
                }
                if (ns->is_black() &&
                    nx && !nx->is_black() &&
                    (!ny || ny->is_black())
                ) {
                    // SYが黒, Xが赤
                    // ?BRB
                    // -> Case 5.
                    // DOUT() << "Case 5." << std::endl;
                    rotate_flip_(ns, nx);
                    ns = np->counter_child(nm);
                    nx = ns->cis_child();
                    ny = ns->trans_child();
                }
                // Case 6. ?B?R
                // ?B?R
                // DOUT() << "Case 6." << std::endl;
                rotate_swap_(np, ns);
                ny->flip_color();
            }

            // node をツリーから切り離す
            void    release_node_from_parent_(pointer node) {
                pointer parent = node->parent();
                (node->is_left_child() ? parent->left() : parent->right()) = NULL;
                --size_;
            }

            // [[リバランス関連staticメンバ関数]]

            // ノードとその親について, 可能な回転を行う.
            // (可能な回転は, parent と node の関係性から自動的に定まる)
            static void rotate_(pointer parent, pointer node) {
                if (node == parent->left()) {
                    rotate_right_(node, parent);
                } else if (node == parent->right()) {
                    rotate_left_(node, parent);
                } else {
                    throw "invalid rotation";
                }
            }

            // ノードa, cについて左回転する.
            //    pa
            //    |
            //    a
            //   / \
            //  /   \
            // b     c
            //      / \
            //     d   e
            static void rotate_left_(pointer c, pointer a) {
                bool    a_was_left_child = a->is_left_child();
                pointer pa = a->parent();
                pointer d = c->left();
                // 1. Aの右をDに向ける
                a->place_into_right(d);
                // 2. Cの左をAに向ける
                c->place_into_left(a);
                // 3. Aに入っていたエッジをCに向ける
                if (a_was_left_child) {
                    pa->place_into_left(c);
                } else {
                    pa->place_into_right(c);
                }
            }

            // ノードa, cについて右回転する.
            //      pc
            //      |
            //      c
            //     / \
            //    /   \
            //   a     e
            //  / \
            // b   d
            static void rotate_right_(pointer a, pointer c) {
                bool      c_was_left_child = c->is_left_child();
                pointer   pc = c->parent();
                pointer   d = a->right();
                // 1. Cの左をDに向ける
                c->place_into_left(d);
                // 2. Aの右をCに向ける
                a->place_into_right(c);
                // 3. Cに入っていたエッジをAに向ける
                if (c_was_left_child) {
                    pc->place_into_left(a);
                } else {
                    pc->place_into_right(a);
                }
            }

            // 回転と色反転を同時に行う
            static void    rotate_flip_(pointer parent, pointer node) {
                node->flip_color();
                parent->flip_color();
                rotate_(parent, node);
            }

            // 回転と色交換を同時に行う
            static void    rotate_swap_(pointer parent, pointer node) {
                parent->swap_color(*node);
                rotate_(parent, node);
            }

        FT_PRIVATE:
            static void    node_out_(const std::string& name, pointer node) {
                std::cerr << name << "(" << node << ").(p, l, r) = (" << node->parent() << ", " << node->left() << ", " << node->right() << ")" << std::endl;
            }

        /*
        // [[デバッグメソッド]]
        public:

            // 葉ノードまでの長さのうち、最も短いものを返す
            size_type   debug_shortest_height() const {
                return root() ? root()->debug_shortest_height() : 0;
            }

            // 葉ノードまでの長さのうち、最も長いものを返す
            size_type   debug_longest_height() const {
                return root() ? root()->debug_longest_height() : 0;
            }

            size_type   debug_shortest_black_height() const {
                return root() ? root()->debug_shortest_black_height() : 0;
            }

            size_type   debug_longest_black_height() const {
                return root() ? root()->debug_longest_black_height() : 0;
            }
        */
    };
}

#endif
