#ifndef FT_VECTOR_H
# define FT_VECTOR_H

# include "ft_common.hpp"
# include <memory>

namespace ft {
	template<class T, class Allocator = std::allocator<T> >
	class vector {
		public:

			typedef typename T										value_type;
			typedef typename Allocator								allocator_type;
			typedef typename std::size_t							size_type;
			typedef typename std::ptrdiff_t							difference_type;
			typedef typename Allocator::reference					reference;
			typedef typename Allocator::const_reference				const_reference;
			typedef typename Allocator::pointer						pointer;
			typedef typename Allocator::const_pointer				const_pointer;
			typedef typename LegacyRandomAccessIterator				iterator;
			typedef typename const LegacyRandomAccessIterator		const_iterator;
			// ?? これらは実装する必要あり？
			typedef typename std::reverse_iterator<iterator>		reverse_iterator;
			typedef typename std::reverse_iterator<const_iterator>	const_reverse_iterator;

			// [コンストラクタ]
			// デフォルト
			vector();
			// アロケータ指定
			explicit vector(const Allocator& alloc);
			// 要素数, (初期値(コピーされる), アロケータ)
			explicit vector(
					size_type count,
					const T& value = T(),
					const Allocator& alloc = Allocator());
			// レンジイテレータ, (アロケータ)
			template< class InputIt >
			vector(InputIt first, InputIt last,
					const Allocator& alloc = Allocator());
			// コピーコンストラクタ
			vector( const vector& other );

			// [デストラクタ]
			virtual ~vector();

			// [代入]
			// ?? signifierは無効化される？
			vector<T, Allocator>& operator=(const vector<T, Allocator> &rhs);

			// [assign]
			// !! INVALIDATE all signifiers !!
			// ※コンテナの要素へのイテレータ、ポインタ、参照はすべて無効化されます。 終端イテレータも無効化されます。 
			// (1) 内容を「valueのコピーcount個」に置き換える
			void assign( size_type count, const T& value );
			// (2) 内容をレンジイテレータが指すものに置き換える
			// first and/or lastがthis内を指している場合はundefined
			template< class InputIt >
			void assign( InputIt first, InputIt last );

			// [get_allocator]
			// 関連付けられているアロケータを返します 
			allocator_type get_allocator() const;

			// [at]
			// 境界チェック付きで、指定された位置 pos の要素への参照を返します。
			// pos がコンテナの範囲内でなければ std::out_of_range 型の例外が投げられます。
			// cf. [] は境界チェックをしない。
			reference       at( size_type pos );
			const_reference at( size_type pos ) const;

			// [[]]
			// 指定された位置 pos の要素を指す参照を返します。 境界チェックは行われません。 
			reference       operator[]( size_type pos );
			const_reference operator[]( size_type pos ) const;

			// [front]
			// コンテナ内の最初の要素を指す参照を返します。
			// 空のコンテナに対する front の呼び出しは未定義です。
			// cf. コンテナ c に対して、式 c.front() は *c.begin() と同等です。 
			reference front();
			const_reference front() const;

			// [back]
			// コンテナの最後の要素への参照を返します。
			// 空のコンテナに対する back の呼び出しは未定義です。 
			// cf. コンテナ c に対して、式は return c.back(); は { auto tmp = c.end(); --tmp; return *tmp; } と同等です。 
			reference back();
			const_reference back() const;

			// [begin]
			// コンテナの最初の要素を指すイテレータを返します。
			// コンテナが空の場合は、返されたイテレータは end() と等しくなります。 
			iterator begin();
			const_iterator begin() const;

			// [end]
			// コンテナの最後の要素の次の要素を指すイテレータを返します。
			// この要素はプレースホルダとしての役割を持ちます。この要素にアクセスを試みると未定義動作になります。
			iterator end();
			const_iterator end() const;

			// [rbegin]
			// 逆順の vector の最初の要素を指す逆イテレータを返します。 これは非逆順の vector の最後の要素に対応します。
			// vector が空の場合、返されるイテレータは rend() と等しくなります。 
			reverse_iterator rbegin();
			const_reverse_iterator rbegin() const;

			// [rend]
			// 逆順の vector の最後の要素の次の要素を指す逆イテレータを返します。
			// これは非逆順の vector の最初の要素の前の要素に対応します。
			// この要素はプレースホルダとして振る舞い、アクセスを試みると未定義動作になります。
			reverse_iterator rend();
			const_reverse_iterator rend() const;

			// [empty]
			// コンテナの持っている要素が無い、つまり begin() == end() かどうかを調べます。 
			bool empty() const;
			
			// [size]
			// コンテナ内の要素の数、すなわち std::distance(begin(), end()) を返します。 
			size_type size() const;

			// [max_size]
			// システムまたはライブラリ実装の制限によるコンテナが保持できる最大要素数を返します。 
			// この値は一般的にはコンテナのサイズの理論上の制限を反映します
			// (多くとも std::numeric_limits<difference_type>::max())。
			// 実行時の利用可能な RAM の量により、コンテナのサイズは max_size() より小さな値に制限される場合があります。 
			size_type max_size() const;

			// [reserve]
			// ベクタの容量を new_cap より大きいまたは等しい値に増加させます。
			//  new_cap が現在の capacity() より大きい場合は、新しい記憶域が確保されます。 そうでなければ、この関数は何もしません。
			// reserve() はベクタのサイズを変更しません。
			// new_cap が capacity() より大きい場合は、すべてのイテレータ (終端イテレータも含む) および要素へのすべての参照が無効化されます。
			// そうでなければ、どのイテレータも参照も無効化されません。 
			//  new_cap > max_size() の場合は std::length_error。 
			// !! 例外が投げられた場合、この関数は効果を持ちません (強い例外保証)。 
			// reserve() はコンテナの容量を減らすために使用することはできません。 そのためには shrink_to_fit() が提供されています。 <- C++11
			void reserve( size_type new_cap );

			// [capacity]
			// コンテナが現在確保している空間に格納できる要素の数を返します。 
			size_type capacity() const;

			// [clear]
			// コンテナからすべての要素を削除します。 この呼び出しの後、 size() はゼロを返します。
			// 格納されている要素を指すあらゆる参照、ポインタ、イテレータは無効化されます。 終端イテレータも無効化されます。  
			// vector の capacity() は変更されません (ノート: 標準の capacity 変更に対する制約は vector::reserve で規定されています。 [1] を参照してください)。
			void clear();
			
			// [insert]
			// pos の前に value を挿入します。
			// returns: 挿入された value を指すイテレータ。
			iterator insert( iterator pos, const T& value );
			// os の前に value のコピーを count 個挿入します。
			void insert( iterator pos, size_type count, const T& value );
			// pos の前に範囲 [first, last) から要素を挿入します。 
			void insert( iterator pos, InputIt first, InputIt last);

			// [erase]
			// コンテナから指定された要素を削除します。 
			// 1) pos の指す要素を削除します。
			iterator erase( iterator pos );
			// 2) 範囲 [first, last) 内の要素を削除します。
			iterator erase( iterator first, iterator last );
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
			// 例外が投げられた場合 (Allocator::allocate() または要素のコピー/ムーブのコンストラクタ/代入によって発生する可能性があります)、この関数は効果を持ちません (強い例外保証)。 
			void push_back( const T& value );

			// [pop_back]
			// コンテナの最後の要素を削除します。
			// 空のコンテナに対する pop_back の呼び出しは未定義です。
			// 最後の要素を指すイテレータと参照および end() が無効化されます。 
			void pop_back();

			// [resize]
			// count 個の要素を格納するようにコンテナのサイズを変更します。
			// 現在のサイズが count より大きい場合、最初の count 個の要素にコンテナが縮小されます。
			// 現在のサイズが count より小さい場合、 value のコピーで初期化された要素が追加されます。
			// より小さなサイズに変更しても vector の容量は縮小されません。 これは、 pop_back() を複数回呼び出すことで同等の効果を得た場合に無効化されるイテレータは削除されたもののみであるのに対し、容量の変更はすべてのイテレータを無効化するためです。 
			void resize( size_type count, T value = T() );

			// [swap]
			// コンテナの内容を other の内容と交換します。 個々の要素に対するいかなるムーブ、コピー、swap 操作も発生しません。
			// すべてのイテレータおよび参照は有効なまま残されます。 終端イテレータは無効化されます。
			void swap( vector& other );


		FT_PRIVATE:
	};

	// [比較演算子]
	// lhs と rhs の内容が等しいかどうか調べます。 つまり、それらが同じ個数の要素を持ち、 lhs 内のそれぞれの要素が rhs 内の同じ位置の要素と等しいかどうか比較します。
	template< class T, class Alloc >
	bool operator==(
		const std::vector<T,Alloc>& lhs,
		const std::vector<T,Alloc>& rhs );
	template< class T, class Alloc >
	bool operator!=(
		const std::vector<T,Alloc>& lhs,
		const std::vector<T,Alloc>& rhs );
	// lhs と rhs の内容を辞書的に比較します。 比較は std::lexicographical_compare と同等の関数によって行われます。 
	// lhs の内容が rhs の内容より辞書的に小さい場合は true、そうでなければ false。
	template< class T, class Alloc >
	bool operator<(
		const std::vector<T,Alloc>& lhs,
		const std::vector<T,Alloc>& rhs );
	// lhs の内容が rhs の内容より辞書的に小さいまたは等しい場合は true、そうでなければ false。
	template< class T, class Alloc >
	bool operator<=(
		const std::vector<T,Alloc>& lhs,
		const std::vector<T,Alloc>& rhs );
	// lhs の内容が rhs の内容より辞書的に大きい場合は true、そうでなければ false。
	template< class T, class Alloc >
	bool operator>(
		const std::vector<T,Alloc>& lhs,
		const std::vector<T,Alloc>& rhs );
	// lhs の内容が rhs の内容より辞書的に大きいまたは等しい場合は true、そうでなければ false。
	template< class T, class Alloc >
	bool operator>=(
		const std::vector<T,Alloc>& lhs,
		const std::vector<T,Alloc>& rhs );

	// [swap]
	// std::vector に対する std::swap アルゴリズムの特殊化。 lhs と rhs の内容を入れ替えます。 lhs.swap(rhs) を呼びます。 
	template< class T, class Alloc >
	void swap( vector<T,Alloc>& lhs, vector<T,Alloc>& rhs );

}
#endif
