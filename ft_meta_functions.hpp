#ifndef FT_META_FUNCTIONS_HPP
# define FT_META_FUNCTIONS_HPP

namespace ft {
    template <class T, T Val>
    struct integral_constant
    {
        static const T              value = Val;
        typedef T                   value_type;
        typedef integral_constant    type;
    };

    typedef integral_constant<bool, true>   true_type;
    typedef integral_constant<bool, false>  false_type;

    // [is_integral]
    // 整数型であることを判定するpredicate
    template <class T>  struct is_integral                  : public false_type {};
    template <>         struct is_integral<bool>            : public true_type  {};
    template <>         struct is_integral<char>            : public true_type  {};
    template <>         struct is_integral<unsigned char>   : public true_type  {};
    template <>         struct is_integral<wchar_t>         : public true_type  {};
    template <>         struct is_integral<short>           : public true_type  {};
    template <>         struct is_integral<unsigned short>  : public true_type  {};
    template <>         struct is_integral<int>             : public true_type  {};
    template <>         struct is_integral<unsigned int>    : public true_type  {};
    template <>         struct is_integral<long>            : public true_type  {};
    template <>         struct is_integral<unsigned long>   : public true_type  {};

    // [is_floating_point]
    // 整数型であることを判定するpredicate
    template <class T>  struct is_floating_point                : public false_type {};
    template <>         struct is_floating_point<float>         : public true_type  {};
    template <>         struct is_floating_point<double>        : public true_type  {};
    template <>         struct is_floating_point<long double>   : public true_type  {};

    // [is_arithmetic]
    // 算術的な型であることを判定するpredicate
    template <class T>  struct is_aritmetic: public integral_constant<bool,
        is_integral<T>::value || is_floating_point<T>::value
    > {};
    
    // [enable_if]
    // 第１引数のpredicateがtrueになる時だけ、第2引数の型をtypeとして持つ型が定義される
    template <bool, class T = void> struct enable_if            {};
    template <class T>              struct enable_if<true, T>   { typedef T type; };

    // [disable_if]
    // enable_if の逆
    template <bool, class T = void> struct disable_if           {};
    template <class T>              struct disable_if<false, T> { typedef T type; };

    // [iterator_tag]
    // イテレータの階層構造を表現するための空のstruct
    // 継承してるかしてないかを問題にするので中身がいらない
    // struct input_iterator_tag                                               {};
    // struct output_iterator_tag                                              {};
    // struct forward_iterator_tag       : public input_iterator_tag           {};
    // struct bidirectional_iterator_tag : public forward_iterator_tag         {};
    // struct random_access_iterator_tag : public bidirectional_iterator_tag   {};

    template <class>
    struct type_void { typedef void type; };

    // [type_identity]
    template<class T>
    struct type_identity { typedef T type; };

    // template <class From, class To>
    // struct rebind_pointer {
    //     typedef typename pointer_traits<From>::template rebind<To>        type;
    // };




}

#endif
