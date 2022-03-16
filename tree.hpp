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
                    TreeNode()
                        :   tree_value_(NULL),
                            left_child_node_(NULL), right_child_node_(NULL), parent_node_(NULL),
                            is_black_(true) {}
                    TreeNode(tree_value& element)
                        :   tree_value_(&element),
                            left_child_node_(NULL), right_child_node_(NULL), parent_node_(NULL),
                            is_black_(true) {}
                    TreeNode(const TreeNode& other) {
                        *this = other;
                    }
                    ~TreeNode() {
                        DOUT() << this << ", " << tree_value_ << std::endl;
                    }

                    tree_value_pointer       value() { return tree_value_; }
                    const tree_value_pointer value() const { return tree_value_; }

                    tree_node_pointer&          left() { return left_child_node_; }
                    tree_node_pointer&          right() { return right_child_node_; }
                    tree_node_pointer&          parent() { return parent_node_; }
                    const tree_node_pointer&    left() const { return left_child_node_; }
                    const tree_node_pointer&    right() const { return right_child_node_; }
                    const tree_node_pointer&    parent() const { return parent_node_; }

                    bool    is_end() const {
                        return has_parent();
                    }

                    bool    is_root() const {
                        return has_parent() && parent()->is_end();
                    }

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
                    tree_node_pointer    cis_child() {
                        if (is_left_child()) { return right(); }
                        if (is_right_child()) { return left(); }
                        return NULL;
                    }

                    // 「トランス子」、つまり
                    // 自分が左子なら左子、自分が右子なら右子を返す。
                    tree_node_pointer    trans_child() {
                        if (is_left_child()) { return left(); }
                        if (is_right_child()) { return right(); }
                        return NULL;
                    }

                    // childがthisの子である場合、childと逆の子を返す
                    // そうでない場合 NULL を返す
                    tree_node_pointer    counter_child(const tree_node_pointer& child) {
                        if (left() == child) { return right(); }
                        if (right() == child) { return left(); }
                        return NULL;
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

                    void    swap(const self_type& other) {
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
                    iterator(const iterator& other): ptr_(other.ptr_) {}
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
                    inline const reference  operator*() const { return *this; }

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
            // メンバ変数

            // endノード
            node_type               end_node_;
            // ノードアロケータ
            node_allocator_type     node_allocator_;
            // Valueのアロケータ
            value_allocator_type    value_allocator_;
            // ノード数
            size_type               size_;
            // Valueの比較関数
            value_comparator_type   value_compare_;

            pointer                 begin_node_;
        
        public:
            // [コンストラクタ群]

            // デフォルト
            tree():
                end_node_(node_type()),
                node_allocator_(node_allocator_type()),
                size_(0),
                value_compare_(value_comparator_type()) {

                }
            // コンパレータ指定
            tree(const value_comparator_type& comp):
                end_node_(node_type()),
                node_allocator_(node_allocator_type()),
                size_(0),
                value_compare_(comp) {}
            // アロケータ指定
            tree(const value_allocator_type& allocator):
                end_node_(node_type()),
                value_allocator_(allocator),
                size_(0),
                value_compare_(value_comparator_type()) {}
            // コンパレータ・アロケータ指定
            tree(const value_comparator_type& comp, const value_allocator_type& allocator):
                end_node_(node_type()),
                value_allocator_(allocator),
                size_(0),
                value_compare_(comp) {}
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
            inline iterator                 end() {
                return iterator(end_node());
            }
            inline const_iterator           end() const {
                return iterator(end_node());
            }
            // TODO: beginをキャッシュする
            inline iterator                 begin() {
                return iterator(end_node()->min_node());
            }
            inline const_iterator           begin() const {
                return iterator(end_node()->min_node());
            }
            inline reverse_iterator rbegin() {
                return reverse_iterator(end());
            }
            inline const_reverse_iterator rbegin() const {
                return reverse_iterator(end());
            }
            inline reverse_iterator rend() {
                return reverse_iterator(begin());
            }
            inline const_reverse_iterator rend() const {
                return reverse_iterator(begin());
            }

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

            pair<iterator, bool>    insert(const value_type& x) {
                pair<pointer, pointer*> place = find_equal_(x);
                if (place.second == NULL) {
                    // 挿入できない場合
                    std::cout << "failed to insert " << x << std::endl;
                    return pair<iterator, bool>(iterator(place.first), false);
                }
                // 挿入できる場合
                // -> place.second の位置に挿入する。
                *(place.second) = create_node_(x);
                std::cout << place.second << ", " << *(place.second) << std::endl;
                (*(place.second))->parent() = place.first;
                size_ += 1;
                std::cout << "inserted " << x << ", parent is " << place.first << std::endl;
                // TODO: リバランス
                return pair<iterator, bool>(iterator(*place.second), true);
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
            inline iterator         find(const value_type& key) {
                return iterator(const_cast<pointer>(find_ptr_(key)));
            }
            inline const_iterator   find(const value_type& key) const {
                return const_iterator(find_ptr_(key));
            }

            // 「key より小さくない最小の要素」を返す
            // 言い換えると   min{ element | key <= element }
            // または        min{ element | !value_comp()(element, key) }
            iterator    lower_bound(const value_type& key) {
                return iterator(lower_bound_ptr_(key));
            }
            const_iterator  lower_bound(const value_type& key) const {
                return const_iterator(lower_bound_ptr_(key));
            }
            // 「key より大きい最小の要素」を返す
            iterator    upper_bound(const value_type& key) {
                return iterator(upper_bound_ptr_(key));
            }
            const_iterator  upper_bound(const value_type& key) const {
                return const_iterator(upper_bound_ptr_(key));
            }

            size_type count(const value_type& x) const {
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
            inline pointer begin_node() { return begin_node_; }

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
                            return pair<pointer, pointer*>(target, NULL);
                        }
                    }
                }
                std::cout << "target is null -> returns end: " << end_node() << std::endl;
                std::cout << &(end_node_.left()) << std::endl;
                return pair<pointer, pointer*>(end_node(), &(end_node()->left()));
            }

    };


}

#endif
