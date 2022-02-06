#ifndef FT_VECTOR_HPP
# define FT_VECTOR_HPP

# include "ft_common.hpp"
# include "ft_meta_functions.hpp"
# include <memory>
# include <iostream>
# include <iterator>
# include <exception>
# include <stdexcept>

namespace ft {
    template<class T, class Allocator = std::allocator<T> >
    class vector {
        public:

            typedef T                                                   value_type;
            typedef Allocator                                           allocator_type;
            typedef typename std::size_t                                size_type;
            typedef typename std::ptrdiff_t                             difference_type;
            typedef typename Allocator::reference                       reference;
            typedef typename Allocator::const_reference                 const_reference;
            typedef typename Allocator::pointer                         pointer;
            typedef typename Allocator::const_pointer                   const_pointer;

            class iterator {
                public:
                    typedef iterator                                    iterator_type;
                    typedef typename vector::value_type                 value_type;
                    typedef typename vector::difference_type            difference_type;
                    typedef typename vector::pointer                    pointer;
                    typedef typename vector::reference                  reference;
                    typedef typename vector::const_reference            const_reference;
                    typedef typename std::random_access_iterator_tag    iterator_category;

                    iterator(): ptr_(NULL) {}
                    iterator(pointer ptr): ptr_(ptr) {}
                    iterator(const iterator& other): ptr_(other.ptr_) {}
                    iterator_type&  operator=(const iterator_type &rhs) {
                        ptr_ = rhs.ptr_;
                        return *this;
                    }

                    virtual         ~iterator() {}

                    // for 入力イテレータ
                    reference       operator*() {
                        return *ptr_;
                    }

                    // for 出力イテレータ
                    const reference operator*() const {
                        return *ptr_;
                    }

                    iterator_type&  operator++() {
                        ptr_ += 1;
                        return *this;
                    }

                    iterator_type   operator++(int) {
                        iterator_type    it = *this;
                        ++*this;
                        return it;
                    }

                    iterator_type&  operator--() {
                        ptr_ -= 1;
                        return *this;
                    }

                    iterator_type   operator--(int) {
                        iterator_type    it = *this;
                        --*this;
                        return it;
                    }

                    iterator_type&  operator+=(difference_type n) {
                        ptr_ += n;
                        return *this;
                    }

                    iterator_type&    operator-=(difference_type n) {
                        ptr_ -= n;
                        return *this;
                    }

                    iterator_type    operator+(difference_type n) const {
                        return iterator_type(ptr_ + n);
                    }

                    iterator_type    operator-(difference_type n) const {
                        return iterator_type(ptr_ - n);
                    }

                    // アドレスの差を返す
                    difference_type operator-(const iterator_type& rhs) const {
                        return &**this - &*rhs;
                    }

                    // 比較: ポインタアドレスを見る
                    bool operator==(const iterator_type& rhs) const {
                        return &**this == &*rhs;
                    }

                    bool operator!=(const iterator_type& rhs) const {
                        return !(*this == rhs);
                    }

                    bool operator<(const iterator_type& rhs) const {
                        return &**this < &*rhs;
                    }

                    bool operator<=(const iterator_type& rhs) const {
                        return (*this < rhs) || (*this == rhs);
                    }

                    bool operator>(const iterator_type& rhs) const {
                        return &*rhs <= &**this;
                    }

                    bool operator>=(const iterator_type& rhs) const {
                        return &*rhs < &**this;
                    }


                FT_PRIVATE:
                    pointer    ptr_;
            };

            typedef const iterator                                  const_iterator;
            // ?? std::reverse_iteratorは実装する必要あり？
            typedef typename std::reverse_iterator<iterator>        reverse_iterator;
            typedef typename std::reverse_iterator<const_iterator>  const_reverse_iterator;

            // [コンストラクタ]
            // デフォルト
            vector()
                : capacity_(0), size_(0), allocator_(std::allocator<value_type>()), storage_(NULL) {
            }

            // アロケータ指定
            explicit vector(const allocator_type& alloc)
                : capacity_(0), size_(0), allocator_(alloc), storage_(NULL) {
            }

            // 要素数, (初期値(コピーされる), アロケータ)
            explicit vector(
                    size_type count,
                    const_reference value = value_type(),
                    const allocator_type& alloc = allocator_type()
            ): capacity_(0), size_(0), allocator_(alloc), storage_(NULL) {
                resize(count, value);
            }

            // レンジイテレータ, (アロケータ)
            template< class InputIt >
            vector(InputIt first, InputIt last,
                    const allocator_type& alloc = allocator_type(),
                    typename ft::disable_if< ft::is_integral<InputIt>::value >::type* = NULL
            ): capacity_(0), size_(0), allocator_(alloc), storage_(NULL) {
                // 第４引数はdisable_ifのためのものなので、ノータッチ
                insert(end(), first, last);
            }
            // コピーコンストラクタ
            vector(const vector& other) {
                *this = other;
            }

            // [デストラクタ]
            virtual ~vector() {
                obliterate_();
            }

            // [代入]
            // ?? signifierは無効化される？
            vector<T, Allocator>& operator=(const vector<T, Allocator> &rhs) {
                DOUT() << this << " = " << &rhs << std::endl;
                if (this == &rhs) { return *this; }
                this->assign(rhs.begin(), rhs.end());
                return *this;
            }

            // [assign]
            // !! INVALIDATE all signifiers !!
            // ※コンテナの要素へのイテレータ、ポインタ、参照はすべて無効化されます。 終端イテレータも無効化されます。 
            // (1) 内容を「valueのコピーcount個」に置き換える
            void assign( size_type count, const_reference value ) {
                vector<value_type, allocator_type>  new_one(count, value, allocator_);
                swap(new_one);
            }
            // (2) 内容をレンジイテレータが指すものに置き換える
            // first and/or lastがthis内を指している場合はundefined
            template< class InputIt >
            void assign( InputIt first, InputIt last ) {
                vector<value_type, allocator_type>  new_one(first, last, allocator_);
                swap(new_one);
            }

            // [get_allocator]
            // 関連付けられているアロケータを返します 
            allocator_type get_allocator() const {
                return allocator_type(allocator_);
            }

            // [at]
            // 境界チェック付きで、指定された位置 pos の要素への参照を返します。
            // pos がコンテナの範囲内でなければ std::out_of_range 型の例外が投げられます。
            // cf. [] は境界チェックをしない。
            reference       at( size_type pos ) {
                if (size_ <= pos) {
                    throw std::out_of_range("oor");
                }
                return *this[pos];
            }
            const_reference at( size_type pos ) const {
                if (size_ <= pos) {
                    throw std::out_of_range("oor");
                }
                return *this[pos];
            }

            // [[]]
            // 指定された位置 pos の要素を指す参照を返します。 境界チェックは行われません。 
            reference       operator[]( size_type pos ) {
                return storage_[pos];
            }
            const_reference operator[]( size_type pos ) const {
                return storage_[pos];
            }

            // [front]
            // コンテナ内の最初の要素を指す参照を返します。
            // 空のコンテナに対する front の呼び出しは未定義です。
            // cf. コンテナ c に対して、式 c.front() は *c.begin() と同等です。 
            reference front() {
                return *this[0];
            }
            const_reference front() const {
                return *this[0];
            }

            // [back]
            // コンテナの最後の要素への参照を返します。
            // 空のコンテナに対する back の呼び出しは未定義です。 
            // cf. コンテナ c に対して、式は return c.back(); は { auto tmp = c.end(); --tmp; return *tmp; } と同等です。 
            reference back() {
                if (empty()) {
                    // 空
                }
                return *this[size_ - 1];
            }
            const_reference back() const {
                if (empty()) {
                    // 空
                }
                return *this[size_ - 1];
            }

            // [begin]
            // コンテナの最初の要素を指すイテレータを返します。
            // コンテナが空の場合は、返されたイテレータは end() と等しくなります。 
            iterator begin() {
                if (size_ > 0) {
                    return iterator(storage_);
                } else {
                    return end();
                }
            }
            const_iterator begin() const {
                return const_iterator(storage_);
            }

            // [end]
            // コンテナの最後の要素の次の要素を指すイテレータを返します。
            // この要素はプレースホルダとしての役割を持ちます。この要素にアクセスを試みると未定義動作になります。
            iterator end() {
                if (size_ > 0) {
                    return begin() + size_;
                } else {
                    return iterator(storage_);
                }
            }
            const_iterator end() const {
                if (size_ > 0) {
                    return begin() + size_;
                } else {
                    return iterator(storage_);
                }
            }

            // [rbegin]
            // 逆順の vector の最初の要素を指す逆イテレータを返します。 これは非逆順の vector の最後の要素に対応します。
            // vector が空の場合、返されるイテレータは rend() と等しくなります。 
            reverse_iterator rbegin() {
                return reverse_iterator(end());
            }
            const_reverse_iterator rbegin() const {
                return reverse_iterator(end());
            }

            // [rend]
            // 逆順の vector の最後の要素の次の要素を指す逆イテレータを返します。
            // これは非逆順の vector の最初の要素の前の要素に対応します。
            // この要素はプレースホルダとして振る舞い、アクセスを試みると未定義動作になります。
            reverse_iterator rend() {
                return reverse_iterator(begin());
            }
            const_reverse_iterator rend() const {
                return reverse_iterator(begin());
            }

            // [empty]
            // コンテナの持っている要素が無い、つまり begin() == end() かどうかを調べます。 
            bool empty() const {
                return size_ == 0;
            }
            
            // [size]
            // コンテナ内の要素の数、すなわち std::distance(begin(), end()) を返します。 
            size_type size() const {
                return size_;
            }

            // [max_size]
            // システムまたはライブラリ実装の制限によるコンテナが保持できる最大要素数を返します。 
            // この値は一般的にはコンテナのサイズの理論上の制限を反映します
            // (多くとも std::numeric_limits<difference_type>::max())。
            // 実行時の利用可能な RAM の量により、コンテナのサイズは max_size() より小さな値に制限される場合があります。 
            size_type max_size() const {
                return allocator_.max_size();
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
            void reserve( size_type new_cap ) {
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
            size_type capacity() const {
                return capacity_;
            }

            // [clear]
            // コンテナからすべての要素を削除します。 この呼び出しの後、 size() はゼロを返します。
            // 格納されている要素を指すあらゆる参照、ポインタ、イテレータは無効化されます。 終端イテレータも無効化されます。  
            // vector の capacity() は変更されません (ノート: 標準の capacity 変更に対する制約は vector::reserve で規定されています。 [1] を参照してください)。
            void clear() {
                resize(0);
            }
            
            // [insert]
            // pos の前に value を挿入します。
            // returns: 挿入された value を指すイテレータ。
            iterator insert(iterator pos, const value_type& value) {
                size_type    n = distance_(begin(), pos);
                insert(pos, 1, value);
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
                        mass_move_within_capacity_(pos, count);
                        fill_within_capacity_(pos, count, value);
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
            template< class InputIt >
            void insert(
                iterator pos, InputIt first, InputIt last,
                typename ft::disable_if< ft::is_integral<InputIt>::value >::type* = NULL
            ) {
                // 一旦新しいvectorで受ける
                vector<value_type, allocator_type>  receiver(allocator_);
                for (; first != last; ++first) {
                    receiver.push_back(*first);
                }
                size_type   count = receiver.size();
                // サイズがわかるので、再確保の有無を計算
                size_type   recommended_cap = recommended_capacity_(size() + count);
                bool        needed_realloc = recommended_cap > capacity();
                bool        do_append = pos == end();
                if (!needed_realloc) {
                    if (do_append) {
                        // 再確保不要 & 末尾に追加
                        append_within_capacity_(receiver.begin(), receiver.end());
                    } else {
                        // 再確保不要 & 末尾でない
                        size_type   p = distance_(begin(), pos);
                        mass_move_within_capacity_(pos, count);
                        for (size_type i = 0; i < count; ++i) {
                            allocator_.construct(&storage_[i + p], receiver[i]);
                        }
                    }
                } else {
                    // 再確保が必要
                    vector<value_type, allocator_type> new_one(allocator_);
                    new_one.reserve(recommended_cap);
                    new_one.append_within_capacity_(begin(), pos);
                    for (size_type i = 0; i < count; ++i) {
                        new_one.push_back_within_capacity_(receiver[i]);
                    }
                    new_one.append_within_capacity_(pos, end());
                    swap(new_one);
                }
            }

            // [erase]
            // コンテナから指定された要素を削除します。 
            // 1) pos の指す要素を削除します。
            iterator erase(const_iterator pos) {
                if (pos == end()) {

                }
                size_type   i = distance_(begin(), pos);
                size_type   sz = size();
                allocator_.destroy(&storage_[i]);
                for (size_type j = i + 1; j < sz; ++j) {
                    storage_[j - 1] = storage_[j];
                }
                size_ -= 1;
                return begin() + i;
            }

            // 2) 範囲 [first, last) 内の要素を削除します。
            // iterator erase( iterator first, iterator last );
            // 削除位置およびその後を指すイテレータおよび参照は無効化されます。 終端イテレータも無効化されます。
            // イテレータ pos は有効かつ逆参照可能でなければなりません。 そのため end() イテレータは (有効であるが逆参照可能でないため) pos の値として使用することはできません。
            // first==last の場合、イテレータ first は逆参照可能である必要はありません。 空範囲の削除は何もしません。 
            // 削除された最後の要素の次を指すイテレータ。
            // pos が最後の要素を参照する場合は、 end() イテレータが返されます。
            // 削除前に last==end() であった場合は、更新後の end() イテレータが返されます。
            // [first, last) が空範囲の場合は、 last が返されます。 

            // [push_back]
            // 指定された要素 value をコンテナの終端に追加します。
            // 1) 新しい要素は value のコピーとして初期化されます。
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
            void resize( size_type count, value_type value = value_type() ) {
                const size_type   current_size = size();
                // DOUT() << "size_ = " << current_size << ", count = " << count << std::endl;
                if (current_size > count) {
                    // 現在のサイズが count より大きい場合、最初の count 個の要素にコンテナが縮小されます。
                    // If n is less than or equal to the size of the container,
                    // the function never throws exceptions (no-throw guarantee).
                    destroy_from_(begin() + count);
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
                ft::swap(storage_, other.storage_);
                ft::swap(allocator_, other.allocator_);
                ft::swap(size_, other.size_);
                ft::swap(capacity_, other.capacity_);
            }

        FT_PRIVATE:
            size_type       capacity_;
            size_type       size_;
            allocator_type  allocator_;
            pointer         storage_;

            difference_type distance_(iterator from, iterator to) const {
                if (from == to) {
                    return 0;
                }
                if (to == end()) {
                    return size();
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
                // DOUT() << c << " for " << n << std::endl;
                return c;
            }

            // サイズをnにしたい場合、再確保が必要かどうかを返す。
            bool    needed_reallocation_(size_type n) const {
                return recommended_capacity_(n) > capacity();
            }

            // [from, to) の要素を末尾に追加する。
            // capacityは十分にあるものとする。
            void    append_within_capacity_(iterator from, iterator to) {
                // size_type   n = to - from;
                // ASSERTION: size() + n <= capacity
                size_type i = size();
                for (iterator it = from; it != to; ++it) {
                    storage_[i] = *it;
                    ++i;
                }
                size_ = i;
            }

            // 再確保不要な前提でpush_backを行う。
            void    push_back_within_capacity_(const_reference value) {
                // ASSERTION: capacity() >= size() + 1
                allocator_.construct(&storage_[size_], value);
                size_ += 1;
            }

            // 区間 [pos, end) の部分を to_extend だけ右にずらし、
            // サイズを to_extend だけ増やす。
            // 再確保が起きない前提。つまり、c
            // ASSERTION: size_ + to_extend <= capacity()
            void    mass_move_within_capacity_(iterator pos, size_type to_extend) {
                if (pos == end() || to_extend == 0) { return; }
                size_type i_pos = distance_(begin(), pos);
                for (size_type i_from = size_; i_pos < i_from;) {
                    if (i_pos + 1 > i_from) { break; }
                    --i_from;
                    size_type   i_to = i_from + to_extend;
                    storage_[i_to] = storage_[i_from];
                }
                size_ += to_extend;
            }

            // pos から count 個の要素を value のコピーで塗りつぶす。
            // 再確保が起きない前提。つまり、c
            // ASSERTION: size_ + cou t <= capacity()
            void    fill_within_capacity_(iterator pos, size_type count, const value_type& value) {
                size_type   i_pos = distance_(begin(), pos);
                for (size_type i = 0; i < count; ++i) {
                    allocator_.construct(&storage_[i_pos + i], value);
                }
            }

            // [from, end) の要素を後ろからdestroyする
            // allocator.destroyがno-throwならno-throw
            void    destroy_from_(iterator from) {
                if (from == begin()) {
                    return;
                }
                iterator it = end() - 1;
                while (true) {
                    allocator_.destroy(&*it);
                    if (from == begin()) {
                        break;
                    }
                    --it;
                }
            }

            // 初期状態(storage_なし, capacity == size == 0)に戻す
            void    obliterate_() {
                clear();
                if (storage_) {
                    // DOUT() << "deallocating " << storage_ << " with cap: " << capacity_ << std::endl;
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
    bool operator==(
        const ft::vector<T,Alloc>& lhs,
        const ft::vector<T,Alloc>& rhs
    ) {
        if (lhs.size() != rhs.size()) { return false; }
        for (typename ft::vector<T,Alloc>::size_type i = 0; i < lhs.size(); i += 1) {
            if (lhs[i] != rhs[i]) { return false; }
        }
        return true;
    }

    template< class T, class Alloc >
    bool operator!=(
        const ft::vector<T,Alloc>& lhs,
        const ft::vector<T,Alloc>& rhs
    ) {
        return !(lhs == rhs);
    }

    // lhs と rhs の内容を辞書的に比較します。 比較は std::lexicographical_compare と同等の関数によって行われます。 
    // lhs の内容が rhs の内容より辞書的に小さい場合は true、そうでなければ false。
    template< class T, class Alloc >
    bool operator<(
        const ft::vector<T,Alloc>& lhs,
        const ft::vector<T,Alloc>& rhs
    ) {
        // lhs の内容が rhs の内容より辞書的に小さいまたは等しい場合は true、そうでなければ false。
        typename ft::vector<T,Alloc>::size_type ml = lhs.size();
        typename ft::vector<T,Alloc>::size_type mr = rhs.size();
        typename ft::vector<T,Alloc>::size_type m = ml;
        if (mr < m) { m = mr; }
        for (typename ft::vector<T,Alloc>::size_type i = 0; i < m; i += 1) {
            if (lhs[i] < rhs[i]) { return true; }
            if (lhs[i] > rhs[i]) { return false; }
        }
        return ml <= mr;
    }
    
    template< class T, class Alloc >
    bool operator<=(
        const ft::vector<T,Alloc>& lhs,
        const ft::vector<T,Alloc>& rhs
    ) {
        // lhs の内容が rhs の内容より辞書的に大きい場合は true、そうでなければ false。
        // lhs の内容が rhs の内容より辞書的に小さいまたは等しい場合は true、そうでなければ false。
        typename ft::vector<T,Alloc>::size_type ml = lhs.size();
        typename ft::vector<T,Alloc>::size_type mr = rhs.size();
        typename ft::vector<T,Alloc>::size_type m = ml;
        if (mr < m) { m = mr; }
        for (typename ft::vector<T,Alloc>::size_type i = 0; i < m; i += 1) {
            if (lhs[i] < rhs[i]) { return true; }
            if (lhs[i] > rhs[i]) { return false; }
        }
        return ml <= mr;
    }

    template< class T, class Alloc >
    bool operator>(
        const ft::vector<T,Alloc>& lhs,
        const ft::vector<T,Alloc>& rhs
    ) {
        // lhs の内容が rhs の内容より辞書的に大きいまたは等しい場合は true、そうでなければ false。
        return rhs <= lhs;
    }

    template< class T, class Alloc >
    bool operator>=(
        const ft::vector<T,Alloc>& lhs,
        const ft::vector<T,Alloc>& rhs
    ) {
        return rhs < lhs;
    }

    // [swap]
    // ft::vector に対する std::swap アルゴリズムの特殊化。 lhs と rhs の内容を入れ替えます。 lhs.swap(rhs) を呼びます。 
    template< class T, class Alloc >
    void swap( vector<T,Alloc>& lhs, vector<T,Alloc>& rhs ) {
        lhs.swap(rhs);
    }


}
#endif
