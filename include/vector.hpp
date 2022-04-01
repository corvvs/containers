#ifndef VECTOR_HPP
# define VECTOR_HPP

# include "ft_common.hpp"
# include "ft_meta_functions.hpp"
# include "ft_algorithm.hpp"
# include "ft_iterator.hpp"
# include "iterator_wrapper.hpp"
# include <memory>
# include <iostream>
# include <iterator>
# include <exception>
# include <stdexcept>

namespace ft {

    template<class T, class Allocator = std::allocator<T> >
    class vector {
        public:

            typedef T                                               value_type;
            typedef Allocator                                       allocator_type;
            typedef value_type&                                     reference;
            typedef const value_type&                               const_reference;
            typedef typename allocator_type::size_type              size_type;
            typedef typename allocator_type::difference_type        difference_type;
            typedef typename allocator_type::pointer                pointer;
            typedef typename allocator_type::const_pointer          const_pointer;

            typedef ft::iterator_wrapper<pointer>              iterator;
            typedef ft::iterator_wrapper<const_pointer>        const_iterator;

            typedef typename ft::reverse_iterator<iterator>         reverse_iterator;
            typedef typename ft::reverse_iterator<const_iterator>   const_reverse_iterator;

            // [コンストラクタ]
            // デフォルト
            vector()
                : capacity_(0), size_(0), allocator_(allocator_type()), storage_(NULL) {}

            // アロケータ指定
            explicit vector(const allocator_type& alloc)
                : capacity_(0), size_(0), allocator_(alloc), storage_(NULL) {}

            // 要素数
            explicit vector(size_type count)
                : capacity_(0), size_(0), allocator_(allocator_type()), storage_(NULL) {
                resize(count);
            }

            // 要素数, 初期値(コピーされる), (アロケータ)
            vector(
                size_type count,
                const_reference value,
                const allocator_type& alloc = allocator_type()
            ): capacity_(0), size_(0), allocator_(alloc), storage_(NULL) {
                resize(count, value);
            }

            // レンジイテレータ, (アロケータ)
            template <class InputIt>
            vector(
                InputIt first, InputIt last,
                const allocator_type& alloc = allocator_type(),
                typename ft::disable_if< ft::is_integral<InputIt>::value >::type* = NULL
            ): capacity_(0), size_(0), allocator_(alloc), storage_(NULL) {
                // InputIt のイテレータタグの識別はinsertがやる
                // 第４引数はdisable_ifのためのものなので、ノータッチ
                insert(end(), first, last);
            }
            // コピーコンストラクタ
            vector(const vector& other)
                : capacity_(0), size_(0), allocator_(other.allocator_), storage_(NULL) {
                *this = other;
            }

            // [デストラクタ]
            ~vector() {
                obliterate_();
            }

            // [代入]
            vector<T, Allocator>& operator=(const vector<T, Allocator> &rhs) {
                if (this == &rhs) { return *this; }
                this->assign(rhs.begin(), rhs.end());
                return *this;
            }

            // [assign]
            // !! INVALIDATE all signifiers !!
            // ※コンテナの要素へのイテレータ、ポインタ、参照はすべて無効化されます。 終端イテレータも無効化されます。 
            // (1) 内容を「valueのコピーcount個」に置き換える
            void assign(size_type count, const_reference value) {
                if (is_reallocation_needed_(count)) {
                    vector<value_type, allocator_type>  new_one(count, value, allocator_);
                    swap(new_one);
                } else {
                    clear();
                    insert(begin(), count, value);
                }
            }
            // (2) 内容をレンジイテレータが指すものに置き換える
            // first and/or lastがthis内を指している場合はundefined
            template< class InputIt >
            void assign(
                InputIt first, InputIt last,
                typename ft::disable_if< ft::is_integral<InputIt>::value >::type* = NULL
            ) {
                vector<value_type, allocator_type>  new_one(first, last, allocator_);
                if (is_reallocation_needed_(new_one.size())) {
                    swap(new_one);
                } else {
                    clear();
                    insert(begin(), new_one.begin(), new_one.end());
                }
            }

            // [get_allocator]
            // 関連付けられているアロケータを返します 
            inline allocator_type get_allocator() const {
                return allocator_type(allocator_);
            }

            // [at]
            // 境界チェック付きで、指定された位置 pos の要素への参照を返します。
            // pos がコンテナの範囲内でなければ std::out_of_range 型の例外が投げられます。
            // cf. [] は境界チェックをしない。
            reference       at(size_type pos) {
                if (size_ <= pos) {
                    throw std::out_of_range("vector");
                }
                return (*this)[pos];
            }
            const_reference at(size_type pos) const {
                if (size_ <= pos) {
                    throw std::out_of_range("vector");
                }
                return (*this)[pos];
            }

            // [[]]
            // 指定された位置 pos の要素を指す参照を返します。 境界チェックは行われません。 
            inline reference       operator[](size_type pos) {
                return storage_[pos];
            }
            inline const_reference operator[](size_type pos) const {
                return storage_[pos];
            }

            // [front]
            // コンテナ内の最初の要素を指す参照を返します。
            // 空のコンテナに対する front の呼び出しは未定義です。
            // cf. コンテナ c に対して、式 c.front() は *c.begin() と同等です。 
            inline reference front() {
                return *begin();
            }
            inline const_reference front() const {
                return *begin();
            }

            // [back]
            // コンテナの最後の要素への参照を返します。
            // 空のコンテナに対する back の呼び出しは未定義です。 
            // cf. コンテナ c に対して、式は return c.back(); は { auto tmp = c.end(); --tmp; return *tmp; } と同等です。 
            inline reference back() {
                if (empty()) {
                    // 空
                }
                return (*this)[size_ - 1];
            }
            inline const_reference back() const {
                if (empty()) {
                    // 空
                }
                return (*this)[size_ - 1];
            }

            // [begin]
            // コンテナの最初の要素を指すイテレータを返します。
            // コンテナが空の場合は、返されたイテレータは end() と等しくなります。 
            inline iterator begin() {
                if (size_ > 0) {
                    return iterator(storage_);
                } else {
                    return end();
                }
            }
            inline const_iterator begin() const {
                return const_iterator(storage_);
            }

            // [end]
            // コンテナの最後の要素の次の要素を指すイテレータを返します。
            // この要素はプレースホルダとしての役割を持ちます。この要素にアクセスを試みると未定義動作になります。
            inline iterator end() {
                if (size_ > 0) {
                    return begin() + size_;
                } else {
                    return iterator(storage_);
                }
            }
            inline const_iterator end() const {
                if (size_ > 0) {
                    return begin() + size_;
                } else {
                    return iterator(storage_);
                }
            }

            // [rbegin]
            // 逆順の vector の最初の要素を指す逆イテレータを返します。 これは非逆順の vector の最後の要素に対応します。
            // vector が空の場合、返されるイテレータは rend() と等しくなります。 
            inline reverse_iterator rbegin() {
                return reverse_iterator(end());
            }
            inline const_reverse_iterator rbegin() const {
                return reverse_iterator(end());
            }

            // [rend]
            // 逆順の vector の最後の要素の次の要素を指す逆イテレータを返します。
            // これは非逆順の vector の最初の要素の前の要素に対応します。
            // この要素はプレースホルダとして振る舞い、アクセスを試みると未定義動作になります。
            inline reverse_iterator rend() {
                return reverse_iterator(begin());
            }
            inline const_reverse_iterator rend() const {
                return reverse_iterator(begin());
            }

            // [empty]
            // コンテナの持っている要素が無い、つまり begin() == end() かどうかを調べます。 
            inline bool empty() const {
                return size_ == 0;
            }
            
            // [size]
            // コンテナ内の要素の数、すなわち std::distance(begin(), end()) を返します。 
            inline size_type size() const {
                return size_;
            }

            // [max_size]
            // システムまたはライブラリ実装の制限によるコンテナが保持できる最大要素数を返します。 
            // この値は一般的にはコンテナのサイズの理論上の制限を反映します
            // (多くとも std::numeric_limits<difference_type>::max())。
            // 実行時の利用可能な RAM の量により、コンテナのサイズは max_size() より小さな値に制限される場合があります。 
            inline size_type max_size() const {
                return difference_type(-1) / sizeof(value_type);
                // return allocator_.max_size();
            }

            // [reserve]
            // ベクタの容量を new_cap より大きいまたは等しい値に増加させます。
            //  new_cap が現在の capacity() より大きい場合は、新しい記憶域が確保されます。 そうでなければ、この関数は何もしません。
            // reserve() はベクタのサイズを変更しません。
            // new_cap が capacity() より大きい場合は、すべてのイテレータ (終端イテレータも含む) および要素へのすべての参照が無効化されます。
            // そうでなければ、どのイテレータも参照も無効化されません。 
            //  new_cap > max_size() の場合は std::length_error。 
            // !! 例外が投げられた場合、この関数は効果を持ちません (強い例外保証)。 
            // reserve() はコンテナの容量を減らすために使用することはできません。 そのためには shrink_to_fit() が提供されています。 <- C++11
            void reserve(size_type new_cap) {
                //  new_cap が現在の capacity() より大きくなければ、この関数は何もしません。
                if (new_cap <= capacity_) {
                    return;
                }
                if (max_size() < new_cap) {
                    throw std::length_error("allocator<T>::allocate(size_t n) 'n' exceeds maximum supported size");
                }
                if (!storage_) {
                    // ストレージがない場合の確保
                    // size_ == 0 のはず。
                    storage_ = allocator_.allocate(new_cap);
                    capacity_ = new_cap;
                } else {
                    // 再確保
                    vector<value_type, allocator_type>  reserved(allocator_);
                    reserved.reserve(new_cap);
                    reserved.append_within_capacity_(begin(), end());
                    swap(reserved);
                }
            }

            // [capacity]
            // コンテナが現在確保している空間に格納できる要素の数を返します。 
            inline size_type capacity() const {
                return capacity_;
            }

            // [clear]
            // コンテナからすべての要素を削除します。 この呼び出しの後、 size() はゼロを返します。
            // 格納されている要素を指すあらゆる参照、ポインタ、イテレータは無効化されます。 終端イテレータも無効化されます。  
            // vector の capacity() は変更されません (ノート: 標準の capacity 変更に対する制約は vector::reserve で規定されています。 [1] を参照してください)。
            inline void clear() {
                resize(0);
            }

            // [insert]
            // pos の前に value を挿入します。
            // returns: 挿入された value を指すイテレータ。
            iterator insert(iterator pos, const value_type& value) {
                size_type    n = distance_(begin(), pos);
                insert(pos, static_cast<size_type>(1), value);
                return begin() + n;
            }
            // os の前に value のコピーを count 個挿入します。
            // returns: 挿入された最初の要素を指すイテレータ、または count==0 の場合は pos。
            void insert(iterator pos, size_type count, const value_type& value) {
                size_type   recommended_cap = recommended_capacity_(size() + count);
                bool        needed_realloc = recommended_cap > capacity();
                bool        do_append = pos == end();
                if (!needed_realloc) {
                    if (do_append) {
                        // 再確保不要 & 末尾に追加
                        for (size_type i = 0; i < count; ++i) {
                            push_back_within_capacity_(value);
                        }
                    } else {
                        // 再確保不要 & 末尾でない
                        size_type   p = distance_(begin(), pos);
                        size_type   old_size = size();
                        // 既存要素の移動
                        mass_moveright_within_capacity_(pos, count);
                        // 新規要素の代入
                        for (size_type i = 0; i < count; ++i) {
                            substitute_to_(i + p, value, old_size);
                        }
                    }
                } else {
                    // 再確保が必要
                    vector<value_type, allocator_type> new_one(allocator_);
                    new_one.reserve(recommended_cap);
                    new_one.append_within_capacity_(begin(), pos);
                    for (size_type i = 0; i < count; ++i) {
                        new_one.push_back_within_capacity_(value);
                    }
                    new_one.append_within_capacity_(pos, end());
                    swap(new_one);
                }
            }

            // pos の前に範囲 [first, last) から要素を挿入します。 
            template <class InputIt>
            // 範囲挿入(入力イテレータ)
            void insert(
                iterator pos, InputIt first, InputIt last,
                typename ft::enable_if<
                    !ft::is_integral<InputIt>::value
                    &&
                    ft::is_input_iterator<InputIt>::value
                    &&
                    !ft::is_forward_iterator<InputIt>::value
                >::type* = NULL
            ) {
                // サイズを知るために一旦新しいvectorで受ける
                vector<value_type, allocator_type>  receiver(allocator_);
                for (; first != last; ++first) {
                    receiver.push_back(*first);
                }
                // 受けたvectorを使って前方向バージョンに移譲
                // -> vectorのiteratorは
                // - 前方向イテレータなので, 自己呼び出しにはならない.
                // - ランダムアクセスイテレータなので, std::distanceが定数時間で終わる.
                insert(pos, receiver.begin(), receiver.end());
            }

            template <class ForwardIt>
            // 範囲挿入(前方向イテレータ)
            void insert(
                iterator pos, ForwardIt first, ForwardIt last,
                typename ft::enable_if<
                    !ft::is_integral<ForwardIt>::value
                    &&
                    ft::is_forward_iterator<ForwardIt>::value
                >::type* = NULL
            ) {
                size_type   count = std::distance(first, last);
                // サイズはわかるので, 再確保の有無を計算
                size_type   recommended_cap = recommended_capacity_(size() + count);
                bool        needed_realloc = recommended_cap > capacity();
                bool        do_append = pos == end();
                if (!needed_realloc) {
                    if (do_append) {
                        // 再確保不要 & 末尾に追加
                        size_type i = size();
                        for (; first != last; ++i, ++first) {
                            allocator_.construct(&storage_[i], *first);
                        }
                        size_ = i;
                    } else {
                        // 再確保不要 & 末尾でない
                        size_type   p = distance_(begin(), pos);
                        size_type   old_size = size();
                        // 既存要素の移動
                        mass_moveright_within_capacity_(pos, count);
                        // 新規要素の代入
                        for (size_type i = 0; i < count; ++i, ++first) {
                            substitute_to_(i + p, *first, old_size);
                        }
                    }
                } else {
                    // 再確保が必要
                    vector<value_type, allocator_type> new_one(allocator_);
                    new_one.reserve(recommended_cap);
                    new_one.append_within_capacity_(begin(), pos);
                    for (size_type i = 0; i < count; ++i, ++first) {
                        new_one.push_back_within_capacity_(*first);
                    }
                    new_one.append_within_capacity_(pos, end());
                    swap(new_one);
                }
            }

            // [erase]
            // コンテナから指定された要素を削除します。 
            // 1) pos の指す要素を削除します。
            // 削除された最後の要素の次を指すイテレータ。
            // pos が最後の要素を参照する場合は、 end() イテレータが返されます。
            inline iterator erase(iterator pos) {
                if (pos == end()) {
                    // pos を逆参照できない
                }
                return erase(pos, pos + 1);
            }

            // 2) 範囲 [first, last) 内の要素を削除します。
            // 削除位置およびその後を指すイテレータおよび参照は無効化されます。 終端イテレータも無効化されます。
            // イテレータ pos は有効かつ逆参照可能でなければなりません。 そのため end() イテレータは (有効であるが逆参照可能でないため) pos の値として使用することはできません。
            // first==last の場合、イテレータ first は逆参照可能である必要はありません。 空範囲の削除は何もしません。 
            // 削除された最後の要素の次を指すイテレータ。
            // 削除前に last==end() であった場合は、更新後の end() イテレータが返されます。
            // [first, last) が空範囲の場合は、 last が返されます。 
            iterator erase(iterator first, iterator last) {
                if (is_interval_empty_(first, last)) {
                    return last;
                }
                size_type   i = distance_(begin(), first);
                size_type   count = distance_(first, last);
                mass_moveleft_within_capacity_(last, count);
                return begin() + i;
            }

            // [push_back]
            // 指定された要素 value をコンテナの終端に追加します。
            // (1) 新しい要素は value のコピーとして初期化されます。
            // 新しい size() が capacity() より大きい場合は、すべてのイテレータおよび参照 (終端イテレータも含む) が無効化されます。 
            // そうでなければ、終端イテレータのみが無効化されます。 
            // <時間計算量: 償却定数>
            // <例外安全性: STRONG>
            // 例外が投げられた場合 (Allocator::allocate() または要素のコピー/ムーブのコンストラクタ/代入によって発生する可能性があります)、この関数は効果を持ちません (強い例外保証)。 
            void push_back(const_reference value) {
                size_type   rec_cap = recommended_capacity_(size_ + 1);
                if (rec_cap > capacity()) {
                    // 再確保発生
                    // コピーしてpush_backしてswap
                    // 1回あたりO(size)かかるが、size回pushしてlog2(size)回しか起こらないので、
                    // 全体としては定数くらいになってくれるはず。
                    vector<value_type, allocator_type> cloned(allocator_);
                    cloned.reserve(rec_cap);
                    cloned.append_within_capacity_(begin(), end());
                    cloned.push_back_within_capacity_(value);
                    swap(cloned);
                } else {
                    // 再確保不要
                    push_back_within_capacity_(value);
                }
            }

            // [pop_back]
            // コンテナの最後の要素を削除します。
            // 空のコンテナに対する pop_back の呼び出しは未定義です。
            // 最後の要素を指すイテレータと参照および end() が無効化されます。 
            void pop_back() {
                if (empty()) {
                    // 空
                    return;
                }
                allocator_.destroy(&storage_[size_ - 1]);
                size_ -= 1;
            }

            // [resize]
            // count 個の要素を格納するようにコンテナのサイズを変更します。
            // 現在のサイズが count より大きい場合、最初の count 個の要素にコンテナが縮小されます。
            // 現在のサイズが count より小さい場合、 value のコピーで初期化された要素が追加されます。
            // より小さなサイズに変更しても vector の容量は縮小されません。 これは、 pop_back() を複数回呼び出すことで同等の効果を得た場合に無効化されるイテレータは削除されたもののみであるのに対し、容量の変更はすべてのイテレータを無効化するためです。 
            void resize(size_type count) {
                const size_type   current_size = size();
                if (current_size > count) {
                    // 現在のサイズが count より大きい場合、最初の count 個の要素にコンテナが縮小されます。
                    // If n is less than or equal to the size of the container,
                    // the function never throws exceptions (no-throw guarantee).
                    destroy_from_(begin() + count, end());
                    size_ = count;
                } else if (current_size < count) {
                    // -> value ありに移譲
                    resize(count, value_type());
                }
            }

            void resize(size_type count, value_type value) {
                const size_type   current_size = size();
                if (current_size > count) {
                    // 現在のサイズが count より大きい場合、最初の count 個の要素にコンテナが縮小されます。
                    // If n is less than or equal to the size of the container,
                    // the function never throws exceptions (no-throw guarantee).
                    destroy_from_(begin() + count, end());
                    size_ = count;
                } else if (current_size < count) {
                    // 現在のサイズが count より小さい場合、 value のコピーで初期化された要素が追加されます。
                    // If n is greater and a reallocation happens, there are no changes in the container in case of exception (strong guarantee)
                    // if the type of the elements is either copyable or no-throw moveable.
                    // Otherwise, if an exception is thrown, the container is left with a valid state (basic guarantee).
                    const size_type   current_cap = capacity_;
                    if (count <= current_cap) {
                        // count が容量未満
                        // -> 再確保不要。サイズがcountになるまでvalueを追加する。
                        // -> 基本保証
                        insert(end(), count - current_size, value);
                    } else {
                        // 再確保が必要
                        // -> Copyableなら強い保証、そうでないなら基本保証
                        vector<value_type, allocator_type> cloned(allocator_);
                        cloned.reserve(recommended_capacity_(count));
                        cloned.insert(cloned.end(), begin(), end());
                        cloned.insert(cloned.end(), count - cloned.size(), value);
                        swap(cloned);
                    }
                }
            }

            // [swap]
            // コンテナの内容を other の内容と交換します。 個々の要素に対するいかなるムーブ、コピー、swap 操作も発生しません。
            // すべてのイテレータおよび参照は有効なまま残されます。 終端イテレータは無効化されます。
            void swap( vector& other ) {
                // allocator_ 以外はプリミティブ型なので、最初にallocator_をスワップする。
                // -> 少なくともSTRONG, 多分no-throw
                ft::swap(allocator_, other.allocator_);
                ft::swap(storage_, other.storage_);
                ft::swap(size_, other.size_);
                ft::swap(capacity_, other.capacity_);
            }

        FT_PRIVATE:
            size_type       capacity_;
            size_type       size_;
            allocator_type  allocator_;
            pointer         storage_;

            difference_type distance_(const_iterator from, const_iterator to) const {
                if (is_interval_empty_(from, to)) { return 0; }
                if (to == end()) {
                    return size() - (from - begin());
                }
                return to - from;
            }

            // サイズを「n以上に増やす」ために推奨されるcapacityの値を返す
            size_type    recommended_capacity_(size_type n) const {
                size_type    c = capacity();
                if (n <= c) { return c; }
                size_type    ms = max_size();
                if (c == 0 && n == 1) {
                    c = 1;
                } else {
                    if (c <= n / 2) {
                        c = n;
                        if (ms < c) {
                            // もう拡張できない
                            throw std::length_error("too long");
                        }
                    } else {
                        if (ms / 2 <= c) {
                            // もう拡張できない
                            throw std::length_error("too long");
                        }
                        c *= 2;
                    }
                }
                return c;
            }

            // [Predicates]
            // サイズをnにしたい場合、再確保が必要かどうかを返す。
            inline bool is_reallocation_needed_(size_type n) const {
                return recommended_capacity_(n) > capacity();
            }

            // 区間 [from, to) が空かどうかを返す
            inline bool is_interval_empty_(const_iterator from, const_iterator to) const {
                return from == to;
            }

            // [from, to) の要素を末尾に追加する。
            // capacityは十分にあるものとする。
            void    append_within_capacity_(const_iterator from, const_iterator to) {
                // size_type   n = to - from;
                // ASSERTION: size() + n <= capacity
                size_type i = size();
                for (const_iterator it = from; it != to; ++i, ++it) {
                    allocator_.construct(&storage_[i], *it);
                }
                size_ = i;
            }

            // 再確保不要な前提でpush_backを行う。
            void    push_back_within_capacity_(const_reference value) {
                // ASSERTION: capacity() >= size() + 1
                allocator_.construct(&storage_[size_], value);
                size_ += 1;
            }

            // 区間 [pos, end) の部分を d だけ右にずらし、
            // サイズを d だけ増やす。
            // 再確保が起きない前提。つまり、
            // ASSERTION: size_ + d <= capacity()
            // (pos == end() の状況でこの関数を使うことは想定されていないが、対応しておく)
            void    mass_moveright_within_capacity_(iterator pos, size_type d) {
                if (d == 0) { return; }
                if (!is_interval_empty_(pos, end())) {
                    // 区間が空でない場合
                    size_type   p = distance_(begin(), pos);
                    size_type   old_size = size();
                    size_type   border = p;
                    if (p + d <= old_size) {
                        border = old_size - d;
                    }
                    for (size_type i = border; i < old_size; ++i) {
                        allocator_.construct(&storage_[i + d], storage_[i]);
                    }
                    std::copy_backward(
                        pos, begin() + border,
                        begin() + border + d);
                }
                size_ += d;
            }

            // 区間 [pos, end) の部分を d だけ左にずらし、
            // サイズを d だけ減らす。
            // 移動先の範囲にあるものは destroy されている前提、
            // また移動先は利用可能範囲を外れない前提。つまり、
            // ASSERTION: begin() + d <= pos
            // (pos == end() の状況でこの関数を使うことは想定されていないが、対応しておく)
            void    mass_moveleft_within_capacity_(iterator pos, size_type d) {
                if (d == 0) { return; }
                if (!is_interval_empty_(pos, end())) {
                    // 区間が空でない場合
                    std::copy(pos, end(), pos - d);
                }
                reverse_iterator to = rbegin() + d;
                for (reverse_iterator it = rbegin(); it != to; ++it) {
                    allocator_.destroy(&*it);
                }
                size_ -= d;
            }

            // インデックス pos に value を入れる。
            // ただし、入れ方が異なる:
            // - pos < border:
            //   - 代入
            // - border <= pos:
            //   - コピー構築
            inline void substitute_to_(size_type pos, const value_type& value, size_type border) {
                if (pos < border) {
                    // end() より前に来るものは代入
                    storage_[pos] = value;
                } else {
                    // end()以降に来るものはコピー構築
                    allocator_.construct(&storage_[pos], value);
                }
            }

            // [from, to) の要素を後ろからdestroyする
            // 削除した後は何もしないことに注意(たとえばsizeも変えない)
            // allocator.destroyがno-throwならno-throw
            void    destroy_from_(iterator from, iterator to) {
                if (is_interval_empty_(from, to)) { return; }
                iterator it = to - 1;
                while (true) {
                    allocator_.destroy(&*it);
                    if (from == it) {
                        break;
                    }
                    --it;
                }
            }

            // 初期状態(storage_なし, capacity == size == 0)に戻す
            void    obliterate_() {
                clear();
                if (storage_) {
                    allocator_.deallocate(storage_, capacity());
                    storage_ = NULL;
                    size_ = 0;
                    capacity_ = 0;
                }
            }


    };

    // [比較演算子]
    // lhs と rhs の内容が等しいかどうか調べます。 つまり、それらが同じ個数の要素を持ち、 lhs 内のそれぞれの要素が rhs 内の同じ位置の要素と等しいかどうか比較します。
    template< class T, class Alloc >
    inline bool operator==(
        const ft::vector<T,Alloc>& lhs,
        const ft::vector<T,Alloc>& rhs
    ) {
        return equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template< class T, class Alloc >
    inline bool operator!=(
        const ft::vector<T,Alloc>& lhs,
        const ft::vector<T,Alloc>& rhs
    ) {
        return !(lhs == rhs);
    }

    // lhs と rhs の内容を辞書的に比較します。 比較は std::lexicographical_compare と同等の関数によって行われます。 
    // lhs の内容が rhs の内容より辞書的に小さい場合は true、そうでなければ false。
    template< class T, class Alloc >
    inline bool operator<(
        const ft::vector<T,Alloc>& lhs,
        const ft::vector<T,Alloc>& rhs
    ) {
        // lhs の内容が rhs の内容より辞書的に小さいまたは等しい場合は true、そうでなければ false。
        return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template< class T, class Alloc >
    inline bool operator<=(
        const ft::vector<T,Alloc>& lhs,
        const ft::vector<T,Alloc>& rhs
    ) {
        return !(lhs > rhs);
    }

    template< class T, class Alloc >
    inline bool operator>(
        const ft::vector<T,Alloc>& lhs,
        const ft::vector<T,Alloc>& rhs
    ) {
        // lhs の内容が rhs の内容より辞書的に大きいまたは等しい場合は true、そうでなければ false。
        return rhs < lhs;
    }

    template< class T, class Alloc >
    inline bool operator>=(
        const ft::vector<T,Alloc>& lhs,
        const ft::vector<T,Alloc>& rhs
    ) {
        return !(lhs < rhs);
    }

    template <class Iter>
    inline bool operator==(
        const ft::iterator_wrapper<Iter>& x,
        const ft::iterator_wrapper<Iter>& y
    ) {
        return x.base() == y.base();
    }
    template <class Iter>
    inline bool operator!=(
        const ft::iterator_wrapper<Iter>& x,
        const ft::iterator_wrapper<Iter>& y
    ) {
        return !(x == y);
    }

    template <class Iter1, class Iter2>
    inline bool operator==(
        const ft::iterator_wrapper<Iter1>& x,
        const ft::iterator_wrapper<Iter2>& y
    ) {
        return x.base() == y.base();
    }
    template <class Iter1, class Iter2>
    inline bool operator!=(
        const ft::iterator_wrapper<Iter1>& x,
        const ft::iterator_wrapper<Iter2>& y
    ) {
        return !(x == y);
    }

    template <class Iter>
    inline bool operator<(
        const ft::iterator_wrapper<Iter>& x,
        const ft::iterator_wrapper<Iter>& y
    ) {
        return x.base() < y.base();
    }
    template <class Iter>
    inline bool operator>(
        const ft::iterator_wrapper<Iter>& x,
        const ft::iterator_wrapper<Iter>& y
    ) {
        return (y < x);
    }
    template <class Iter>
    inline bool operator>=(
        const ft::iterator_wrapper<Iter>& x,
        const ft::iterator_wrapper<Iter>& y
    ) {
        return !(x < y);
    }
    template <class Iter>
    inline bool operator<=(
        const ft::iterator_wrapper<Iter>& x,
        const ft::iterator_wrapper<Iter>& y
    ) {
        return !(y < x);
    }

    template <class Iter1, class Iter2>
    inline bool operator<(
        const ft::iterator_wrapper<Iter1>& x,
        const ft::iterator_wrapper<Iter2>& y
    ) {
        return x.base() < y.base();
    }
    template <class Iter1, class Iter2>
    inline bool operator>(
        const ft::iterator_wrapper<Iter1>& x,
        const ft::iterator_wrapper<Iter2>& y
    ) {
        return (y < x);
    }
    template <class Iter1, class Iter2>
    inline bool operator>=(
        const ft::iterator_wrapper<Iter1>& x,
        const ft::iterator_wrapper<Iter2>& y
    ) {
        return !(x < y);
    }
    template <class Iter1, class Iter2>
    inline bool operator<=(
        const ft::iterator_wrapper<Iter1>& x,
        const ft::iterator_wrapper<Iter2>& y
    ) {
        return !(y < x);
    }

    template <class Iter1, class Iter2>
    typename ft::iterator_wrapper<Iter1>::difference_type
    operator-(
        const ft::iterator_wrapper<Iter1>& x,
        const ft::iterator_wrapper<Iter2>& y
    ) {
        return x.operator->() - y.operator->();
    }

    template <class It>
    ft::iterator_wrapper<It>    operator+(
        typename ft::iterator_wrapper<It>::difference_type n,
        const ft::iterator_wrapper<It>& x
    ) {
        x += n;
        return x;
    }
}

// [swap]
// ft::vector に対する std::swap アルゴリズムの特殊化。
// lhs と rhs の内容を入れ替えます。 lhs.swap(rhs) を呼びます。 
template< class T, class Alloc >
void swap( ft::vector<T,Alloc>& lhs, ft::vector<T,Alloc>& rhs ) {
    lhs.swap(rhs);
}

#endif
