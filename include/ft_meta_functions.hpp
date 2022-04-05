#ifndef FT_META_FUNCTIONS_HPP
# define FT_META_FUNCTIONS_HPP

namespace ft {
    // [integral_constant]
    // <型, 値> という形式で、「「ある型のある値」という型」を表現する
    // たとえば
    // integral_constant<bool, true> は「bool型のtrue」を表す型で,
    // integral_constant<bool, true> は「bool型のtrue」を表す型.
    // 両者は型として異なる.
    // (なぜ"integral"なのかというと、テンプレート引数に与えられる値が整数限定だからだと思われる.)
    template <class T, T Val>
    struct integral_constant
    {
        static const T              value = Val;
        typedef T                   value_type;
        typedef integral_constant   type;
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
    // 浮動小数点数型であることを判定するpredicate
    template <class T>  struct is_floating_point                : public false_type {};
    template <>         struct is_floating_point<float>         : public true_type  {};
    template <>         struct is_floating_point<double>        : public true_type  {};
    template <>         struct is_floating_point<long double>   : public true_type  {};

    // [is_arithmetic]
    // 算術的な型であることを判定するpredicate
    template <class T>  struct is_arithmetic: public integral_constant<bool,
        is_integral<T>::value || is_floating_point<T>::value
    > {};
    
    // [enable_if]
    // 第１引数のpredicateがtrueになる時だけ, 第2引数の型をtypeとして持つ型が定義される
    template <bool, class T = void> struct enable_if            {};
    template <class T>              struct enable_if<true, T>   { typedef T type; };

    // [disable_if]
    // enable_if の逆
    template <bool, class T = void> struct disable_if           {};
    template <class T>              struct disable_if<false, T> { typedef T type; };

    // [iterator_tag]
    // これはstdのを使わないと意味がない.

    template <class>
    struct type_void { typedef void type; };

    // [type_identity]
    template<class T>
    struct type_identity { typedef T type; };

    // [remove_const]
    template <class T>
    struct remove_const {
        typedef T type;
    };
    template <class T>
    struct remove_const<const T> {
        typedef T type;
    };

    // [remove_volatile]
    template <class T>
    struct remove_volatile {
        typedef T type;
    };
    template <class T>
    struct remove_volatile<volatile T> {
        typedef T type;
    };

    // [remove_reference]
    template <class T>
    struct remove_reference {
        typedef T type;
    };
    template <class T>
    struct remove_reference<T&> {
        typedef T type;
    };

    template <class T>
    struct remove_cv {
        typedef typename remove_volatile<typename remove_const<T>::type>::type type;
    };

    // [is_same]
    template<typename S, typename T>
    struct is_same {
        static const bool value = false;
    };

    template<typename S>
    struct is_same<S, S> {
        static const bool value = true;
    };

    // [is_convertible]
    struct yes_type {
        char    m[8];
    };
    typedef char no_type;
    namespace is_convertible__ {
        template <class From, class To>
        struct is_convertible_base {
            private:
                static no_type      f(...);
                static yes_type     f(To);
                // declvalがないので値の箱だけ用意
                static From         v;
            public:
                static const bool   value = sizeof(f(v)) == sizeof(yes_type);
        };
    }

    template <typename From, typename To,
        bool = is_convertible__::is_convertible_base<From, To>::value
        // && に続く条件をつけると T -> T& の偽陽性がなくせるが, 以下の理由によりつけないことにした:
        // - T -> const T& を偽陰性にしてしまう.
        // - const char* -> const std::string& を偽陰性にしてしまう.
        // && (is_same<To, typename remove_reference<To>::type >::value || !is_convertible_base<From, typename remove_reference<To>::type >::value)
    >
    struct is_convertible: false_type {};

    template <typename From, typename To>
    struct is_convertible<From, To, true>: true_type {};
}


#endif
