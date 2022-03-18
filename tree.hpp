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
    template <
        class Value,
        class ValueComparator = std::less<Value>,
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
            template<class TreeValue>
            class TreeNode {
                public:
                    typedef TreeValue               tree_value;
                    typedef tree_value*             tree_value_pointer;
                    typedef TreeNode<tree_value>    tree_node;
                    typedef tree_node*              tree_node_pointer;


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
                    TreeNode()
                        :   tree_value_(NULL),
                            left_child_node_(NULL), right_child_node_(NULL), parent_node_(NULL),
                            is_black_(true) {}
                    // endでないノードは赤。
                    TreeNode(tree_value& element)
                        :   tree_value_(&element),
                            left_child_node_(NULL), right_child_node_(NULL), parent_node_(NULL),
                            is_black_(false) {}
                    TreeNode(const TreeNode& other) {
                        *this = other;
                    }
                    ~TreeNode() {
                        DOUT() << this << ", " << tree_value_ << std::endl;
                    }

                    friend std::ostream&    operator<<(std::ostream& stream, const tree_node& rhs) {
                        stream << "[";
                        if (rhs.is_end())
                            stream << "(end)";
                        else
                            stream << *(rhs.value());
                        stream << ":" << (rhs.is_black() ? "B" : "r");
                        stream << ":";
                        stream << (rhs.has_parent() ? "P" : "-");
                        stream << (rhs.has_left_child() ? "L" : "-");
                        stream << (rhs.has_right_child() ? "R" : "-");
                        return stream << "]";
                    }


                    tree_value_pointer       value() { return tree_value_; }
                    const tree_value_pointer value() const { return tree_value_; }

                    tree_node_pointer&          left() { return left_child_node_; }
                    tree_node_pointer&          right() { return right_child_node_; }
                    tree_node_pointer&          parent() { return parent_node_; }
                    const tree_node_pointer&    left() const { return left_child_node_; }
                    const tree_node_pointer&    right() const { return right_child_node_; }
                    const tree_node_pointer&    parent() const { return parent_node_; }

                    bool    is_black() const { return is_black_; }
                    bool    is_end() const { return !has_parent(); }
                    bool    is_root() const { return has_parent() && parent()->is_end(); }

                    bool    is_left_child() const {
                        const tree_node_pointer& pp = parent();
                        if (pp == NULL) { return false; }
                        return pp->left() == this;
                    }

                    bool    is_right_child() const {
                        const tree_node_pointer& pp = parent();
                        if (pp == NULL) { return false; }
                        return pp->right() == this;
                    }

                    bool    has_left_child() const { return left() != NULL; }
                    bool    has_right_child() const { return right() != NULL; }
                    bool    has_parent() const { return parent() != NULL; }

                    // 色を反転する
                    void    flip_color() { is_black_ = !is_black_; }

                    // 自身を根とする部分木の最小ノードを返す
                    tree_node_pointer   min_node() {
                        tree_node_pointer    next_ptr = this;
                        // 左子があるなら左子をたどり続ける
                        while (next_ptr->has_left_child()) {
                            next_ptr = next_ptr->left();
                        }
                        return next_ptr;
                    }

                    // 自身を根とする部分木の最大ノードを返す
                    tree_node_pointer   max_node() {
                        tree_node_pointer    next_ptr = this;
                        // 右子があるなら右子をたどり続ける
                        while (next_ptr->has_right_child()) {
                            next_ptr = next_ptr->right();
                        }
                        return next_ptr;
                    }

                    // 後隣接するノードを返す
                    tree_node_pointer   forward_neighbor() {
                        if (has_right_child()) {
                            // 右子がある場合
                            // -> 右子のminを返す
                            return right()->min_node();
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

                    // 前隣接するノードを返す
                    tree_node_pointer    backward_neighbor() {
                        if (has_left_child()) {
                            // 左子がある場合
                            // -> 左子のminを返す
                            return left()->max_node();
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
                        DOUT() << this << " , " << left() << " - " << right() << " ? " << child << std::endl;
                        if (left() == child) { return right(); }
                        if (right() == child) { return left(); }
                        return NULL;
                    }

                    // 自分がトランス子かどうかを返す。
                    bool                is_trans_child() const {
                        return !is_end() && is_left_child() == parent()->is_left_child();
                    }

                    // 自分がシス子かどうかを返す。
                    bool                is_cis_child() const {
                        return !is_end() && is_left_child() != parent()->is_left_child();
                    }

                    // valueだけをswapする。
                    void    swap_value(tree_node& other) {
                        std::swap(tree_value_, other.tree_value_);
                    }

                    void    swap_color(tree_node& other) {
                        std::swap(is_black_, other.is_black_);
                    }

                    // 自身とその親について、可能な回転を行う。
                    void    rotate() {
                        if (is_left_child()) {
                            rotate_right_();
                        } else {
                            rotate_left_();
                        }
                    }

                    void    rotate_flip() {
                        flip_color();
                        parent()->flip_color();
                        rotate();
                    }

                    void    rotate_swap() {
                        std::swap(is_black_, parent()->is_black_);
                        rotate();
                    }

                    // child を自身の左子として配置する
                    void    place_into_left_(tree_node_pointer child) {
                        this->left() = child;
                        if (child != NULL) {
                            child->parent() = this;
                        }
                    }

                    // child を自身の右子として配置する
                    void    place_into_right_(tree_node_pointer child) {
                        this->right() = child;
                        if (child != NULL) {
                            child->parent() = this;
                        }
                    }

                FT_PRIVATE:

                    // 自身とその親について左回転する
                    void    rotate_left_() {
                        DOUT() << "rot left" << std::endl;
                        tree_node_pointer   c = this;
                        tree_node_pointer   a = parent();
                        bool                a_was_left_child = a->is_left_child();
                        tree_node_pointer   pa = a->parent();
                        tree_node_pointer   d = left();
                        DOUT() << "(a, c, pa, d) = " << a << ", " << c << ", " << pa << ", " << d << std::endl;
                        // 1. Aの右をDに向ける
                        a->place_into_right_(d);
                        // 2. Cの左をAに向ける
                        c->place_into_left_(a);
                        // 3. Aに入っていたエッジをCに向ける
                        if (a_was_left_child) {
                            pa->place_into_left_(c);
                        } else {
                            pa->place_into_right_(c);
                        }
                    }

                    // 自身とその親について右回転する
                    void    rotate_right_() {
                        DOUT() << "rot left" << std::endl;
                        tree_node_pointer   a = this;
                        tree_node_pointer   c = parent();
                        bool                c_was_left_child = c->is_left_child();
                        tree_node_pointer   pc = c->parent();
                        tree_node_pointer   d = left();
                        // 1. Cの左をDに向ける
                        c->place_into_left_(d);
                        // 2. Aの右をCに向ける
                        a->place_into_right_(c);
                        // 3. Cに入っていたエッジをAに向ける
                        if (c_was_left_child) {
                            pc->place_into_left_(a);
                        } else {
                            pc->place_into_right_(a);
                        }
                    }


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
                        const value_allocator_type& value_alloc)
                        :   value_constructed_(false), node_constructed_(false),
                            node_alloc_(node_alloc), value_alloc_(value_alloc),
                            node_ptr_(NULL), value_ptr_(NULL) {}
                    TreeNodeHolder(const self_type& other) {
                        *this = other;
                    }
                    ~TreeNodeHolder() {
                        destroy();
                    }

                    TreeNodeHolder&  operator=(const self_type &rhs) {
                        if (this != &rhs) {
                            swap(rhs);
                        }
                        return *this;
                    }

                    void    swap(self_type& other) {
                        std::swap(value_constructed_, other.value_constructed_);
                        std::swap(node_constructed_, other.node_constructed_);
                        std::swap(node_alloc_, other.node_alloc_);
                        std::swap(value_alloc_, other.value_alloc_);
                        std::swap(node_ptr_, other.node_ptr_);
                        std::swap(value_ptr_, other.value_ptr_);
                    }

                    // value構築済みなら、あらかじめ与えておいたアロケータを使い、ノードを構築する
                    void    construct_node() {
                        DOUT() << node_constructed_ << ", " << value_constructed_ << std::endl;
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
                        DOUT() << node_constructed_ << ", " << value_constructed_ << std::endl;
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
                        DOUT() << node_constructed_ << ", " << value_constructed_ << std::endl;
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
                    void    destroy() {
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
                    iterator(const iterator_type& other): ptr_(other.ptr_) {}
                    iterator_type&  operator=(const iterator_type &rhs) {
                        ptr_ = rhs.ptr_;
                        return *this;
                    }
                    virtual         ~iterator() {}

                FT_PRIVATE:
                    inline pointer  next() const { return ptr_->forward_neighbor(); }
                    inline pointer  prev() const { return ptr_->backward_neighbor(); }

                public:
                    inline reference        operator*() { return *ptr_; }
                    inline const reference  operator*() const { return *ptr_; }

                    inline pointer          operator->() { return ptr_; }
                    inline const_pointer    operator->() const { return ptr_; }

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

            typedef const iterator                                  const_iterator;
            typedef typename std::reverse_iterator<iterator>        reverse_iterator;
            typedef typename std::reverse_iterator<const_iterator>  const_reverse_iterator;

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
            // [コンストラクタ群]

            // デフォルト
            tree():
                end_node_(node_type()),
                node_allocator_(node_allocator_type()),
                size_(0),
                value_compare_(value_comparator_type()) {
                    begin_node_ = end_node();
                }
            // コンパレータ指定
            tree(const value_comparator_type& comp):
                end_node_(node_type()),
                node_allocator_(node_allocator_type()),
                size_(0),
                value_compare_(comp) {
                    begin_node_ = end_node();
                }
            // アロケータ指定
            tree(const value_allocator_type& allocator):
                end_node_(node_type()),
                value_allocator_(allocator),
                size_(0),
                value_compare_(value_comparator_type()) {
                    begin_node_ = end_node();
                }
            // コンパレータ・アロケータ指定
            tree(const value_comparator_type& comp, const value_allocator_type& allocator):
                end_node_(node_type()),
                value_allocator_(allocator),
                size_(0),
                value_compare_(comp) {
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
                return *this;
            }

            inline value_allocator_type&        get_allocator() { return value_allocator_; }
            inline const value_allocator_type&  get_allocator() const { return value_allocator_; }
            inline node_allocator_type&         get_node_allocator() { return node_allocator_; }
            inline const node_allocator_type&   get_node_allocator() const { return node_allocator_; }
            inline value_comparator_type&       value_compare() { return value_compare_; }
            inline const value_comparator_type& value_compare() const { return value_compare_; }

            inline iterator                 end()
                { return iterator(end_node()); }
            inline const_iterator           end() const
                { return iterator(end_node()); }
            inline iterator                 begin()
                { return iterator(begin_node()); }
            inline const_iterator           begin() const
                { return iterator(begin_node()); }
            inline reverse_iterator         rbegin()
                { return reverse_iterator(end()); }
            inline const_reverse_iterator   rbegin() const
                { return reverse_iterator(end()); }
            inline reverse_iterator         rend()
                { return reverse_iterator(begin()); }
            inline const_reverse_iterator   rend() const
                { return reverse_iterator(begin()); }

            inline size_type    size() const { return size_; }
            inline bool         empty() const { return size() == 0; }
            inline size_type    max_size() const {
                return std::min<size_type>(
                    get_node_allocator().max_size(),
                    std::numeric_limits<difference_type >::max()
                );
            }

            // [[ 変更系 ]]

            // すべての要素を削除する(注意: endは破壊しない)。
            // sizeが0になる。
            // beginはendと一致する。
            void    clear() {
                destroy_subtree_(root());
                size_ = 0;
            }

            // [insert]
            // insertの例外安全性はSTRONG

            // 単一・ヒントなし挿入
            pair<iterator, bool>    insert(const value_type& key) {
                pair<pointer, pointer*> place = find_equal_(key);
                if (place.first == *place.second) {
                    // 挿入できない場合
                    DOUT() << "failed to insert " << key << std::endl;
                    return pair<iterator, bool>(iterator(place.first), false);
                }
                // 挿入できる場合
                // -> place.second の位置に挿入する。
                insert_at_(place, key);
                return pair<iterator, bool>(iterator(*place.second), true);
            }

            // 単一・ヒントあり挿入
            // > hint に可能な限り近い位置に key を挿入します。
            // > 挿入が hint の直後の位置に行われた場合、償却定数時間。
            // > そうでなければ、コンテナのサイズの対数。
            iterator    insert(iterator hint, const value_type& key) {
                pair<pointer, pointer*> place = find_equal_(&*hint, key);
                if (place.first == *place.second) {
                    // 挿入不可
                    DOUT() << "failed to insert " << key << std::endl;
                    return iterator(place.first);
                }
                // 挿入できる場合
                // -> place.second の位置に挿入する。
                insert_at_(place, key);
                return iterator(*place.second);
            }

            // 範囲挿入
            // TODO:
            // > InputIt は LegacyInputIterator の要件を満たさなければなりません。 
            template <class InputIterator>
            void    insert(InputIterator first, InputIterator last) {
                for (; first != last; ++first) {
                    insert(*first);
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
                DOUT() << *node_ptr << ": destroying node"  << std::endl;
                if (node_ptr != end_node() && node_ptr->value()) {
                    DOUT() << *node_ptr << ": destroying value" << std::endl;
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
            inline iterator         find(const value_type& key) {
                return iterator(const_cast<pointer>(find_ptr_(key)));
            }
            inline const_iterator   find(const value_type& key) const {
                return const_iterator(find_ptr_(key));
            }

            // 「key より小さくない最小の要素」を返す
            // 言い換えると   min{ element | key <= element }
            // または        min{ element | !value_comp()(element, key) }
            iterator        lower_bound(const value_type& key) {
                return iterator(lower_bound_ptr_(key));
            }
            const_iterator  lower_bound(const value_type& key) const {
                return const_iterator(lower_bound_ptr_(key));
            }
            // 「key より大きい最小の要素」を返す
            iterator        upper_bound(const value_type& key) {
                return iterator(upper_bound_ptr_(key));
            }
            const_iterator  upper_bound(const value_type& key) const {
                return const_iterator(upper_bound_ptr_(key));
            }

            size_type       count(const value_type& x) const {
                return find_ptr_(x) == end_node() ? 0 : 1;
            }

            // ツリーが key を保持しているなら、keyを含む最小の半開区間[lower_bound, upper_bound)を返す。
            // 保持していないなら、[upper_bound, upper_bound)を返す。
            pair<iterator, iterator> equal_range(const value_type& key) {
                pointer p = lower_bound_ptr_(key);
                if (p != end_node()) {
                    // p = min{ x | key <= x }, つまり key <= p
                    if (!value_compare()(key, *p->value())) {
                        // p <= key
                        // -> p <= key <= p
                        // つまり p == key
                        iterator    it2(p);
                        iterator    it1 = it2++;
                        return ft::pair<iterator, iterator>(it1, it2);
                    }
                }
                return ft::pair<iterator, iterator>(p, p);
            }
            pair<const_iterator, const_iterator> equal_range(const value_type& key) const {
                pair<iterator, iterator>    pp = equal_range(key);
                return pair<const_iterator, const_iterator>(
                    static_cast<const_iterator>(pp.first),
                    static_cast<const_iterator>(pp.second)
                );
            }

        FT_PRIVATE:

            inline pointer          end_node() { return &end_node_; }
            inline const_pointer    end_node() const { return &end_node_; }
            inline pointer          root() { return end_node()->left(); }
            inline const_pointer    root() const { return end_node()->left(); }
            inline pointer          begin_node() { return begin_node_; }

            // key と一致するノードがあれば、そのポインタを返す。
            // "key と一致"とはつまり、xが key <= x && x <= key を満たすこと。
            // なければendのポインタを返す。
            const_pointer   find_ptr_(const value_type& key) const {
                const_pointer ptr = lower_bound_ptr_(key);
                if (ptr != end_node()) {
                    // key <= ptr
                    if (value_compare()(key, *ptr->value())) {
                        // key < ptr -> ダメ
                        return end_node();
                    }
                }
                return ptr;
            }

            // key以上の要素があれば、それらのうち最も小さいもののポインタを返す。
            // なければendのポインタを返す。
            pointer    lower_bound_ptr_(const value_type& key) {
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

            const_pointer   lower_bound_ptr_(const value_type& key) const {
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

            pointer    upper_bound_ptr_(const value_type& key) const {
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

            // [[find_equal]]
            // insertionに使うため、insertion先候補とその親をpairで返すメソッド

            // RVは
            // - first: 挿入されるノードの親となるノードのポインタ
            // - second: 親(first)のleft / right のうち、挿入されるべき方のポインタ。
            // - *second がNULLでない場合は値があるということになる。
            pair<pointer, pointer*> find_equal_(const value_type& key) {
                pointer target = root();
                if (target != NULL) {
                    while (true) {
                        if (value_compare()(key, *target->value())) {
                            // key < target
                            if (target->left() == NULL) {
                                return pair<pointer, pointer*>(target, &target->left());
                            }
                            target = target->left();
                        } else if (value_compare()(*target->value(), key)) {
                            // target < key
                            if (target->right() == NULL) {
                                return pair<pointer, pointer*>(target, &target->right());
                            }
                            target = target->right();
                        } else {
                            // key == target
                            break;
                        }
                    }
                    return pair<pointer, pointer*>(target, &target);
                }
                DOUT() << "target is null -> returns end: " << end_node() << std::endl;
                DOUT() << &(end_node_.left()) << std::endl;
                return pair<pointer, pointer*>(end_node(), &(end_node()->left()));
            }

            // ヒントありfind_equal
            pair<pointer, pointer*> find_equal_(pointer hint, const value_type& key) {
                // [ヒントとは]
                // -> ツリー内に一致するノードがあり、
                //    かつその直前または直後がvalueの入るべき位置であるようなノード
                if (hint == end_node() || value_compare()(key, *(hint->value()))) {
                    // (1) hint == end or key < *hint
                    // -> prev = hint - 1 として、 prev == begin or *prev < key なら、
                    //    prevとhintの間にkeyがあるべき。
                    iterator    prev = hint;
                    if (prev != begin_node() && value_compare()(*((--prev)->value()), key)) {
                        // このとき、hintとprevの関係性は以下のいずれかとなり、
                        // hintとprevのうち少なくとも一方が空き子を持つ。
                        DOUT() << "found: prev < " << key << " < hint" << std::endl;
                        if (prev->right() == NULL) {
                            // 1. hintの左子がprev
                            //   -> prevは右子を持たない。(持つならそれがhintとprevの間に入る)
                            return pair<pointer, pointer*>(&*prev, &((*prev).right()));
                        } else {
                            // 2. prevの右部分木の最小値がhint
                            //   -> hintは左子を持たない。(持つならそれがhintとprevの間に入る)
                            return pair<pointer, pointer*>(&*hint, &((*hint).left()));
                        }
                    }
                    return find_equal_(key);
                } else if (value_compare()(*(hint->value()), key)) {
                    // (2) *hint < key
                    // -> next = hint + 1 として、 next == end or key < *next なら、
                    //    hintとnextの間にkeyがあるべき。
                    iterator    next = hint;
                    if (next != end_node() && value_compare()(key, *(((++next)->value())))) {
                        // この時、hintとnextの関係性は以下のいずれかとなり、
                        // hintとnextのうち少なくとも一方が空き子を持つ。
                        DOUT() << "found: hint < " << key << " < next" << std::endl;
                        if (next->left() == NULL) {
                            // 1. hintの右部分木の最小値がnext
                            //   -> nextは左子を持たない。(持つならそれがhintとnextの間に入る)
                            return pair<pointer, pointer*>(&*next, &((*next).left()));
                        } else {
                            // 2. nextの左子がhint
                            //   -> hintは右子を持たない。(持つならそれがhintとnextの間に入る)
                            return pair<pointer, pointer*>(&*hint, &((*hint).right()));
                        }
                    }
                    return find_equal_(key);
                }
                // (3) *hint == key
                DOUT() << "target is null -> returns end: " << end_node() << std::endl;
                DOUT() << &(end_node_.left()) << std::endl;
                return pair<pointer, pointer*>(end_node(), &(end_node()->left()));
            }

            void    insert_at_(pair<pointer, pointer*>& place, const value_type& x) {
                *(place.second) = create_node_(x);
                DOUT() << place.second << ", " << *(place.second) << std::endl;
                (*(place.second))->parent() = place.first;
                size_ += 1;
                DOUT() << "inserted " << x << ", parent is " << place.first << std::endl;
                // begin が変更されるのは:
                // 1. beginより小さい要素が挿入された時
                // 2. beginが削除された時
                // -> 挿入されたノードがbeginのleftである場合, 1.に該当するのでキャッシュを更新する。
                if (begin_node()->left() == *place.second) {
                    begin_node_ = *place.second;
                }
                rebalance_after_insertion_(*(place.second));
            }

        public:

            // [[erase]]

            // (1)
            // 単一削除
            void        erase(iterator position) {
                // ある要素を消す時、その要素と「隣の要素」をスワップしてから
                // 「隣の要素」を消すことにしても、要素の順序は変化しない。
                // 「隣の要素」としてツリーを下る(=葉の方に向かう)方を選択し続ければ、
                // やがて子を持たないノードに行き当たる。
                // 「隣の要素」としては、以下のいずれかを採用する
                // - 対象要素が右部分木を持つ -> 右部分木の最小要素
                // - 対象要素が左部分木を持つ -> 左子
                // - 対象要素が子を持たない -> 打ち止め
                while (true) {
                    iterator    dummy = position;
                    DOUT() << &*position << std::endl;
                    if (position->has_right_child()) {
                        ++position;
                        (*position).swap_value(*dummy);
                        DOUT() << "-> " << *(position->value()) << std::endl;
                    } else if (position->has_left_child()) {
                        --position;
                        DOUT() << "-> " << *(position->value()) << std::endl;
                        (*position).swap_value(*dummy);
                    } else {
                        break;
                    }
                }
                DOUT() << "erasing: " << *position << std::endl;

                // ここまで来たということは、positionは子を持たない。
                pointer target = &(*position);
                pointer parent = target->parent();
                bool    detroying_begin = target == begin_node();
                if (!position->is_black()) {
                    // Case 0. is red
                    // Tが赤である
                    // > Case 0 Tを葉ノードにする
                    release_node_from_parent_(target);
                    destroy_node_(target);
                } else {
                    // targetを破壊する
                    release_node_from_parent_(target);
                    destroy_node_(target);
                    rebalance_after_erasure_(parent, NULL);
                }
                // 必要なら begin_node_ を更新
                if (detroying_begin) {
                    DOUT() << "change begin: " << begin_node_ << " -> " << *parent << std::endl;
                    begin_node_ = parent;
                }
            }

            // (2)
            // TODO: 範囲削除
            void        erase(iterator first, iterator last) {
                for (iterator it = first; it != last; ++it) {
                    erase(it);
                }
            }

            // (3)
            // key に一致するものがあれば削除。
            // 削除した場合は1, しなかった場合は0を返す。
            // -> 単にfindでerase(iterator)に投げるだけ
            size_type   erase(const value_type& key) {
                iterator it = find(key);
                if (it == end()) {
                    return 0;
                }
                erase(it);
                return 1;
            }

            // [[swap]]

            void    swap(self_type& other) {
                std::swap(size_, other.size_);
                std::swap(value_compare_, other.value_compare_);
                std::swap(node_allocator_, other.node_allocator_);
                std::swap(value_allocator_, other.value_allocator_);
                std::swap(end_node_, other.end_node_);
                std::swap(begin_node_, other.begin_node_);
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
                    root()->parent = end_node();
                }
            }

        FT_PRIVATE:

            // [[リバランシング]]

            // 挿入後リバランス
            void    rebalance_after_insertion_(pointer node) {
                DOUT() << node << std::endl;
                // 0. Nが通常ノードでないか、黒ノードの場合
                // -> なにもしない
                if (node == NULL) {
                    DOUT() << "do nothing; is NULL" << std::endl;
                    return;
                }
                if (node == end_node()) {
                    DOUT() << "do nothing; is end" << std::endl;
                    return;
                }
                if (node->is_black()) {
                    DOUT() << "do nothing; is black" << std::endl;
                    return;
                }
                // 1. Nが根ノードの場合
                // -> 色変して終わり
                if (node == root()) {
                    DOUT() << "flip and exit; is root" << std::endl;
                    node->flip_color();
                    return;
                }
                // 2. Nの親が黒ノードの場合
                // -> なにもしなくてよい
                if (node->parent()->is_black()) {
                    DOUT() << "do nothing; parent is black" << std::endl;
                    return;
                }
                // 3. Nの親が赤ノードの場合
                if (node->is_cis_child()) {
                    // 曲がっている場合(シス)
                    // -> Nの親の親とNの親で回転させる(トランスにする)
                    // https://ja.wikipedia.org/wiki/%E3%82%B7%E3%82%B9_(%E5%8C%96%E5%AD%A6)
                    DOUT() << "cis -> trans" << std::endl;
                    pointer parent = node->parent();
                    node->rotate();
                    node = parent;
                }

                pointer p = node->parent();
                pointer q = p->parent();
                pointer u = q->counter_child(p);
                DOUT() << "p = " << p << std::endl;
                DOUT() << "q = " << q << std::endl;
                DOUT() << "u = " << u << std::endl;
                if (u == NULL || u->is_black()) {
                    // Uが黒ノードの場合
                    // P, Qを色変する
                    // さらにQ-Pに対して右回転を行う。
                    p->flip_color();
                    q->flip_color();
                    p->rotate();
                } else {
                    // Uが赤ノードの場合
                    // P, Q, Uの色を反転する。
                    // Q -> N として、再帰的に処理を繰り返すことにする。
                    p->flip_color();
                    q->flip_color();
                    u->flip_color();
                    rebalance_after_insertion_(q);
                }
            }

            // 削除後リバランス
            void    rebalance_after_erasure_(pointer np, pointer nm) {
                if (nm == root()) {
                    // Mは根である
                    // -> Case 1.
                    // なにもしない
                    DOUT() << "Case 1. do nothing" << std::endl;
                    return;
                }
                // nm は NULL である可能性がある。
                // 正しい赤黒木であれば、ns != NULL のはず。
                pointer ns = np->counter_child(nm);
                pointer nx = ns->cis_child();
                pointer ny = ns->trans_child();
                DOUT()
                    << "(nm, np, ns, nx, ny) = ("
                    << nm << ", "
                    << np << ", "
                    << ns << ", "
                    << nx << ", "
                    << ny
                    << ")" << std::endl;
                if (np->is_black() &&
                    ns->is_black() &&
                    (!nx || nx->is_black()) &&
                    (!ny || ny->is_black())
                ) {
                    // Case 3. BBBB
                    // PSXYすべて黒
                    // -> Sを赤に色変し。Pを基準にしてもう一度黒リバランスを行う
                    DOUT() << "Case 3. flip ns and rebalance recursively" << std::endl;
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
                    DOUT() << "Case 2." << std::endl;
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
                    DOUT() << "Case 4." << std::endl;
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
                    DOUT() << "Case 5." << std::endl;
                    rotate_flip_(ns, nx);
                    ns = np->counter_child(nm);
                    nx = ns->cis_child();
                    ny = ns->trans_child();
                }
                // Case 6. ?B?R
                // ?B?R
                DOUT() << "Case 6." << std::endl;
                ns->rotate_swap();
                ny->flip_color();
            }

            void    release_node_from_parent_(pointer node) {
                pointer parent = node->parent();
                DOUT() << "releasing: " << *node << " from parent: " << *parent << std::endl;
                // targetをツリーから切り離す
                (node->is_left_child() ? parent->left() : parent->right()) = NULL;
                --size_;
            }

            // [[リバランス関連staticメンバ関数]]

            // ノードとその親について、可能な回転を行う。
            static void rotate_(pointer parent, pointer node) {
                if (node == parent->left()) {
                    rotate_right_(node, parent);
                } else if (node == parent->right()) {
                    rotate_left_(node, parent);
                } else {
                    throw "invalid rotation";
                }
            }

            // 自身とその親について左回転する
            static void rotate_left_(pointer c, pointer a) {
                DOUT() << *c << " - " << *a << std::endl;
                bool    a_was_left_child = a->is_left_child();
                pointer pa = a->parent();
                pointer d = c->left();
                DOUT() << "(a, c, pa, d) = " << a << ", " << c << ", " << pa << ", " << d << std::endl;
                // 1. Aの右をDに向ける
                a->place_into_right_(d);
                // 2. Cの左をAに向ける
                c->place_into_left_(a);
                // 3. Aに入っていたエッジをCに向ける
                if (a_was_left_child) {
                    pa->place_into_left_(c);
                } else {
                    pa->place_into_right_(c);
                }
            }

            // 自身とその親について右回転する
            static void rotate_right_(pointer a, pointer c) {
                DOUT() << *a << " - " << *c << std::endl;
                bool      c_was_left_child = c->is_left_child();
                pointer   pc = c->parent();
                pointer   d = a->left();
                // 1. Cの左をDに向ける
                c->place_into_left_(d);
                // 2. Aの右をCに向ける
                a->place_into_right_(c);
                // 3. Cに入っていたエッジをAに向ける
                if (c_was_left_child) {
                    pc->place_into_left_(a);
                } else {
                    pc->place_into_right_(a);
                }
            }

            static void    rotate_flip_(pointer parent, pointer node) {
                node->flip_color();
                parent->flip_color();
                rotate_(parent, node);
            }

            static void    rotate_swap_(pointer parent, pointer node) {
                parent->swap_color(node);
                rotate_(parent, node);
            }
        };
}

#endif
