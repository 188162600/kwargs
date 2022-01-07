#ifndef ALGORITHM_H
#define ALGORITHM_H
#include"global.h"
#include"typetraits.h"
namespace Tools {
template <class T, class Tuple, std::size_t... I>
constexpr T makeFromTuple( Tuple&& t, std::index_sequence<I...>  )
{
    static_assert(std::is_constructible_v<T,
        decltype(std::get<I>(std::declval<Tuple>()))...>);
    return T(std::get<I>(std::forward<Tuple>(t))...);
}
template <class T, class Tuple>
constexpr T makeFromTuple( Tuple&& t )
{
    return makeFromTuple<T>(t,std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

template <class T, class Tuple, std::size_t... I>
constexpr T makeFromTuple( const Tuple& t, std::index_sequence<I...>  )
{
    static_assert(std::is_constructible_v<T,
        decltype(std::get<I>(std::declval<Tuple>()))...>);
    return T(std::get<I>(t)...);
}
template <class T, class Tuple>
constexpr T makeFromTuple(const Tuple& t )
{
    return makeFromTuple<T>(t,std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}


template <template<typename...>class Class, class Tuple, std::size_t... I>
constexpr Class<typename std::tuple_element<I,Tuple>::type...>  makeFromTuple(const Tuple& t, std::index_sequence<I...> )
{
    static_assert(std::is_constructible_v< Class<typename std::tuple_element<I,Tuple>::type...>,
        decltype(std::get<I>(std::declval<Tuple>()))...>);
    return  Class<typename std::tuple_element<I,Tuple>::type...>(std::get<I>(t)...);
}

template <template<typename...>class Class, class Tuple>
constexpr auto  makeFromTuple(const Tuple& t )
{
    return makeFromTuple<Class>(t,std::make_index_sequence<std::tuple_size_v<Tuple>>{});
}


}
#endif // ALGORITHM_H
