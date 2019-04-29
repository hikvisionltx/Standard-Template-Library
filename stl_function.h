#ifndef STL_FUNCTION_H
#define STL_FUNCTION_H

namespace ltx
{
    template <typename Arg, typename Result>
    struct unary_function
    {
        typedef Arg argument_type;
        typedef Result result_type;
    };


    template <typename T>
    struct negate : public unary_function<T, T>
    {
        T operator() (const T& x) { return -x; }
    };

    template <typename Arg1, typename Arg2, typename Result>
    struct binary_function
    {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };

    template <typename T>
    struct plus : public binary_function<T, T, T>
    {
        T operator() (const T&x, const T&y) const { return x + y; }
    };
    
}

#endif