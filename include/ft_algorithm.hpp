#ifndef FT_ALGORITHM_HPP
# define FT_ALGORITHM_HPP
# include "ft_common.hpp"
# include "ft_iterator.hpp"
# include <algorithm>
# include <iterator>

namespace ft {

    // template <class T>
    // struct equal_to {
    //     bool    operator()(const T& x, const T& y) const {
    //         return x == y;
    //     }
    //     typedef T       first_argument_type;
    //     typedef T       second_argument_type;
    //     typedef bool    result_type;
    // };
    
    // [[equal, 内部関数]]

    template <class Iterator1, class Iterator2>
    // compあり, ランダムアクセスイテレータが与えられた場合のequal
    // -> compありに転送
    bool    equal_(
        Iterator1 first1, Iterator1 last1,
        Iterator2 first2, Iterator2 last2,
        std::random_access_iterator_tag r1,
        std::random_access_iterator_tag r2
    ) {
        return equal_(
            first1, last1,
            first2, last2,
            r1, r2,
            std::equal_to< typename ft::iterator_traits<Iterator1>::value_type >()
        );
    }

    template <class Iterator1, class Iterator2, class Compare>
    // compあり, ランダムアクセスイテレータが与えられた場合のequal
    // -> distanceでサイズを算出。サイスが異なるなら問答無用でfalseを返す。
    // -> last2なし版に転送
    bool    equal_(
        Iterator1 first1, Iterator1 last1,
        Iterator2 first2, Iterator2 last2,
        std::random_access_iterator_tag,
        std::random_access_iterator_tag,
        Compare comp
    ) {
        if (std::distance(first1, last1) != std::distance(first2, last2)) {
            return false;
        }
        return equal_(
            first1, last1,
            first2,
            comp
        );
    }

    template <class Iterator1, class Iterator2>
    // compなし, 一般イテレータが与えられた場合のequal
    // -> compありに転送
    bool    equal_(
        Iterator1 first1, Iterator1 last1,
        Iterator2 first2, Iterator2 last2,
        std::input_iterator_tag r1,
        std::input_iterator_tag r2
    ) {
        return equal_(
            first1, last1,
            first2, last2,
            r1, r2,
            std::equal_to< typename ft::iterator_traits<Iterator1>::value_type >()
        );
    }

    template <class Iterator1, class Iterator2, class Compare>
    // compあり, 一般イテレータが与えられた場合のequal
    bool    equal_(
        Iterator1 first1, Iterator1 last1,
        Iterator2 first2, Iterator2 last2,
        std::input_iterator_tag,
        std::input_iterator_tag,
        Compare comp
    ) {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
            if (!comp(*first1, *first2)) {
                return false;
            }
        }
        return first1 == last1 && first2 == last2;
    }    

    template <class Iterator1, class Iterator2, class Compare>
    // compあり, 一般イテレータが与えられた場合のequal
    bool    equal_(
        Iterator1 first1, Iterator1 last1,
        Iterator2 first2,
        std::input_iterator_tag,
        std::input_iterator_tag,
        Compare comp
    ) {
        for (; first1 != last1; ++first1, ++first2) {
            if (!comp(*first1, *first2)) {
                return false;
            }
        }
        return true;
    }

    // [[equal, インターフェース]]

    // last2あり
    template <class InputIterator1, class InputIterator2>
    bool    equal(
        InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, InputIterator2 last2
    ) {
        return equal_(
            first1, last1,
            first2, last2,
            typename ft::iterator_traits<InputIterator1>::iterator_category(),
            typename ft::iterator_traits<InputIterator2>::iterator_category()
        );
    }

    template <class InputIterator1, class InputIterator2, class Compare>
    bool    equal(
        InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, InputIterator2 last2,
        Compare comp
    ) {
        return equal_(
            first1, last1,
            first2, last2,
            typename ft::iterator_traits<InputIterator1>::iterator_category(),
            typename ft::iterator_traits<InputIterator2>::iterator_category(),
            comp
        );
    }

    // last2なし

    template <class InputIterator1, class InputIterator2>
    bool    equal(
        InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2
    ) {
        return equal_(
            first1, last1,
            first2,
            std::equal_to< typename ft::iterator_traits<InputIterator1>::value_type >()
        );
    }

    template <class InputIterator1, class InputIterator2, class Compare>
    bool    equal(
        InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2,
        Compare comp
    ) {
        return equal_(
            first1, last1,
            first2,
            comp
        );
    }

    // [[lexicographical_compare]]

    template <class InputIterator1, class InputIterator2>
    bool    lexicographical_compare(
        InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, InputIterator2 last2
    ) {
        return lexicographical_compare(
            first1, last1,
            first2, last2,
            std::less< typename ft::iterator_traits<InputIterator1>::value_type >()
        );
    }

    template <class InputIterator1, class InputIterator2, class Compare>
    bool    lexicographical_compare(
        InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, InputIterator2 last2,
        Compare comp
    ) {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
            if (comp(*first1, *first2)) {
                return true;
            }
            if (comp(*first2, *first1)) {
                return false;
            }
        }
        return first1 == last1 && first2 != last2;
    }
}



#endif
