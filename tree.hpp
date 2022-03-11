#ifndef TREE_HPP
# define TREE_HPP

# include "ft_common.hpp"
# include "ft_meta_functions.hpp"
# include <memory>
# include <iostream>
# include <iterator>
# include <exception>
# include <stdexcept>



namespace ft {

    // tree (Red-Black Tree)
    template < class Value, class ValueComparator, class ValueAllocator >
    class tree {
        public:
            // インナークラス

            // [treeのノード]
            // 左子・右子・親へのポインタを持つ。
            // - 親がnull -> endノード
            // - 子がnull -> 子はNILノード
            // ノードはelementのライフタイムについて責を負わない。
            template<class TreeValue>
            class TreeNode {
                public:
                    typedef TreeValue               value_type;
                    typedef value_type*             value_pointer;
                    typedef TreeNode<value_pointer> tree_node;
                    typedef tree_node*              node_pointer;


                FT_PRIVATE:
                    // ツリーが保持する要素
                    value_pointer   element_;
                    // 左子へのポインタ
                    node_pointer    left_child_;
                    // 右子へのポインタ
                    node_pointer    right_child_;
                    // 親へのポインタ
                    node_pointer    parent_;
                    // ノードが黒か
                    bool            is_black_;
                
                public:
                    // デフォルト構築
                    // endノードもこれで作る。
                    TreeNode()
                        :   element_(NULL),
                            left_child_(NULL), right_child_(NULL), parent_(NULL),
                            is_black_(true) {}
                    TreeNode(value_type& element)
                        :   element_(&element),
                            left_child_(NULL), right_child_(NULL), parent_(NULL),
                            is_black_(true) {}
                    TreeNode(const TreeNode& other) {
                        *this = other;
                    }
                    ~TreeNode() {}

                    node_pointer&       left() { return left_child_; }
                    node_pointer&       right() { return right_child_; }
                    node_pointer&       parent() { return parent_; }
                    const node_pointer& left() const { return left(); }
                    const node_pointer& right() const { return right(); }
                    const node_pointer& parent() const { return parent(); }

                    bool    is_end() const {
                        return has_parent();
                    }

                    bool    is_root() const {
                        return has_parent() && parent()->is_end();
                    }

                    bool    is_left_child() const {
                        const value_pointer& pp = parent();
                        if (pp == NULL) { return false; }
                        return pp->left == this;
                    }

                    bool    is_right_child() const {
                        const value_pointer& pp = parent();
                        if (pp == NULL) { return false; }
                        return pp->right == this;
                    }

                    bool    has_left_child() const { return left() != NULL; }
                    bool    has_right_child() const { return right() != NULL; }
                    bool    has_parent() const { return parent() != NULL; }

                    // 自身を根とする部分木の最小ノードを返す
                    node_pointer    min_node() {
                        node_pointer    next_ptr = this;
                        // 左子があるなら左子をたどり続ける
                        while (next_ptr->has_left_child()) {
                            next_ptr = next_ptr->left();
                        }
                        return next_ptr;
                    }

                    // 自身を根とする部分木の最大ノードを返す
                    node_pointer    max_node() {
                        node_pointer    next_ptr = this;
                        // 右子があるなら右子をたどり続ける
                        while (next_ptr->has_right_child()) {
                            next_ptr = next_ptr->right();
                        }
                        return next_ptr;
                    }

                    // 後隣接するノードを返す
                    node_pointer    forward_neighbor() {
                        if (has_right_child()) {
                            // 右子がある場合
                            // -> 右子のminを返す
                            return right()->min_node();
                        }
                        // 右子がない場合
                        // -> 自身が左子になるまで(=右子である間は)親に遡る
                        // -> 最後に親(「右親」)を返す。
                        // (endの場合はNULLが返る)
                        node_pointer    ptr = this;
                        while (ptr->is_right_child()) {
                            ptr = ptr->parent();
                        }
                        return ptr->parent();
                    }

                    // 前隣接するノードを返す
                    node_pointer    backward_neighbor() {
                        if (has_left_child()) {
                            // 左子がある場合
                            // -> 左子のminを返す
                            return left()->max_node();
                        }
                        // 左子がない場合
                        // -> 自身が右子になるまで(=左子である間は)親に遡る
                        // -> 最後に親(「左親」)を返す。
                        node_pointer    ptr = this;
                        while (ptr->is_left_child()) {
                            ptr = ptr->parent();
                        }
                        return ptr->parent();
                    }

                    // 「シス子」、つまり
                    // 自分が左子なら右子、自分が右子なら左子を返す。
                    node_pointer    cis_child() {
                        if (is_left_child()) { return right(); }
                        if (is_right_child()) { return left(); }
                        return NULL;
                    }

                    // 「トランス子」、つまり
                    // 自分が左子なら左子、自分が右子なら右子を返す。
                    node_pointer    trans_child() {
                        if (is_left_child()) { return left(); }
                        if (is_right_child()) { return right(); }
                        return NULL;
                    }

                    // childがthisの子である場合、childと逆の子を返す
                    // そうでない場合 NULL を返す
                    node_pointer    counter_child(const node_pointer& child) {
                        if (left() == this) { return right(); }
                        if (right() == this) { return left(); }
                        return NULL;
                    }
            };

            // 要素のタイプ
            typedef Value                               value_type;
            // 要素のコンパレータ
            typedef ValueComparator                     value_comparator_type;
            // 要素のアロケータ
            typedef ValueAllocator                      allocator_type;
            // ノードクラス
            typedef TreeNode<value_type*>               node_type;
            typedef node_type*                          node_pointer;
            // ノードのアロケータ
            typedef typename allocator_type::template rebind<node_type> node_allocator_type;

            typedef tree<Value, ValueComparator, ValueAllocator>        self_type;
            typedef typename std::size_t                                size_type;
            typedef typename std::ptrdiff_t                             difference_type;
            typedef typename ValueAllocator::reference                  reference;
            typedef typename ValueAllocator::const_reference            const_reference;
            typedef typename ValueAllocator::pointer                    pointer;
            typedef typename ValueAllocator::const_pointer              const_pointer;

            // インナークラス
            // [イテレータ]
            // treeのイテレータはノードを返す。
            // ノードに乗ってる要素を返すのは利用者(map, set)の責任とする。
            class iterator {
                public:
                    typedef iterator                                    iterator_type;
                    typedef typename tree::value_type                   value_type;
                    typedef typename tree::difference_type              difference_type;
                    typedef typename tree::pointer                      pointer;
                    typedef typename tree::reference                    reference;
                    typedef typename tree::const_reference              const_reference;
                    typedef typename std::bidirectional_iterator_tag    iterator_category;

                FT_PRIVATE:
                    pointer    ptr_;

                public:
                    iterator(): ptr_(NULL) {}
                    iterator(pointer ptr): ptr_(ptr) {}
                    iterator(const iterator& other): ptr_(other.ptr_) {}
                    iterator_type&  operator=(const iterator_type &rhs) {
                        ptr_ = rhs.ptr_;
                        return *this;
                    }

                    virtual         ~iterator() {}

            };

        FT_PRIVATE:
            // メンバ変数

            // endノード
            node_type               end_node_;
            // ノードアロケータ
            node_allocator_type     node_allocator_;
            // ノード数
            size_type               size_;
            // Valueの比較関数
            value_comparator_type   value_compare_;

            node_pointer            begin_node_;
        
        public:
            // [コンストラクタ群]

            // デフォルト
            tree():
                end_node_(node_type()),
                node_allocator_(node_allocator_type()),
                size_(0),
                value_compare_(value_comparator_type()) {}
            // コンパレータ指定
            tree(const value_comparator_type& comp):
                end_node_(node_type()),
                node_allocator_(node_allocator_type()),
                size_(0),
                value_compare_(comp) {}
            // アロケータ指定
            tree(const node_allocator_type& allocator):
                end_node_(node_type()),
                node_allocator_(allocator),
                size_(0),
                value_compare_(value_comparator_type()) {}
            // コンパレータ・アロケータ指定
            tree(const value_comparator_type& comp, const node_allocator_type& allocator):
                end_node_(node_type()),
                node_allocator_(allocator),
                size_(0),
                value_compare_(comp) {}
            // コピーコンストラクタ
            tree(const self_type& other) {
                *this = other;
            }
            // デストラクタ
            ~tree() {

            }

            self_type& operator=(const self_type& rhs) {
                if (this == &rhs) { return *this; }
                return *this;
            }

            // [find]

        FT_PRIVATE:
            node_pointer    end_node() {
                return end_node_;
            }

            node_pointer    root() {
                return end_node()->left();
            }

            node_pointer    begin_node() {
                return begin_node_;
            }

    };
}

#endif
