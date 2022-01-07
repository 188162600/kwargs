#ifndef TYPETRAITS_H
#define TYPETRAITS_H
#include"global.h"
#include<tuple>
namespace Tools {
template<typename...Args>
struct type_count:std::tuple_size<std::tuple<Args...>>{};

//template<size_t Index,typename...Args>
//struct args_element;
//template<size_t Index>
//struct args_element<Index>{};

//template<size_t Index,typename First,typename ...Rest>
//struct args_element<Index,First,Rest...>
//{
//    using type=typename std::conditional<Index==0,First, typename args_element<Index-1,Rest...>::type>::type;
//};


struct placeholder;
//template<template<typename ...>class Class>
//struct transform
//{
//    template<typename...Args>
//    using type=Class<Args...>;
//};



template<typename Specialization, template<typename...> class Class>
struct is_specialization : std::false_type {};

template<template<typename...> class Class, typename... Args>
struct is_specialization<Class<Args...>, Class>: std::true_type {};

template<typename T>
struct unspecialize;

template<template<typename...> class Class, typename... Args>
struct unspecialize<Class<Args...>>
{
    template<typename...Args2>
    using type=Class<Args2...>;
};


template<template<typename...> class Class,typename...Args>
struct emplace;

template<template<typename...> class Class>
struct emplace<Class>
{
    template<typename...Args>
    using type=Class<Args...>;
};

template<template<typename...> class Class,typename First,typename...Rest>
struct emplace<Class,First,Rest...>
{
private:

    template<typename...Args>
    static auto helper(...)->typename emplace<Class,Rest...>::template type<First,Args...>;
    template<typename...Args>
    static auto helper(typename std::conditional<std::is_base_of<placeholder,First>::value,int,placeholder>::type)->typename emplace<Class,Rest...>::template type<Args...>;
public:
    template<typename ...Args>
    using type=decltype (helper<Args...>(0));


};
template<int Index,template<typename ...>class Predicate,typename ...Args>
struct find_type_if_helper;
template<int Index,template<typename ...>class Predicate>
struct find_type_if_helper<Index,Predicate>:std::integral_constant<int,-1>{};

template<int Index,template<typename ...>class Predicate,typename First,typename ...Rest>
struct find_type_if_helper<Index,Predicate,First,Rest...>:std::conditional<
        Predicate<First>::value,
        std::integral_constant<int,Index>,
        find_type_if_helper<Index+1,Predicate,Rest...>
        >::type{};

template<template<typename ...>class Predicate,typename ...Args>
using find_type_if=find_type_if_helper<0,Predicate,Args...>;



template<typename Base,typename ...Types>
using find_subclass=typename find_type_if< emplace<std::is_same,Base>::template type,Types...>::type;
template<typename Subclass,typename ...Types>
using find_base=typename find_type_if< emplace<std::is_same,placeholder,Subclass>::template type,Types...>::type;

template<template<typename ...>class Predicate,typename Class>
struct find_args_if;
template<template<typename ...>class Predicate,template<typename ...>class Class,typename...Args>
struct find_args_if<Predicate,Class<Args...>>:find_type_if<Predicate,Args...>{};


template<typename Base,typename Class>
using find_subclass_in_args=typename find_args_if< emplace<std::is_same,Base>::template type,Class>::type;
template<typename Subclass,typename Class>
using find_base_in_args=typename find_args_if< emplace<std::is_same,placeholder,Subclass>::template type,Class>::type;


template<size_t Index,typename...Args>
using type_at=std::tuple_element<Index,std::tuple<Args...>>;

template<typename Class,typename IndexSequence>
struct keep_args_at;

template<template<typename...> class Class,typename...Args,size_t... I>
struct keep_args_at<Class<Args...>,std::index_sequence<I...>>
{
    using type=Class<typename type_at<I>::type...>;

};

template<size_t Count,template<typename...>class Predicate,typename...Args>
struct count_type_if_helper;

template<size_t Count,template<typename...>class Predicate>
struct count_type_if_helper<Count,Predicate>:std::integral_constant<size_t,Count>{};

template<size_t Count,template<typename...>class Predicate,typename First,typename...Rest>
struct count_type_if_helper<Count,Predicate,First,Rest...>:
        std::conditional<Predicate<First>::value,
        count_type_if_helper<Count+1,Predicate,Rest...>,
        count_type_if_helper<Count,Predicate,Rest...>
        >{};
template<template<typename...>class Predicate,typename...Args>
struct count_type_if:count_type_if_helper<0,Predicate,Args...>{};

template<typename T,T...Args>
struct sum;
template<typename T,T First>
struct sum<T,First>:std::integral_constant<T,First>{};
template<typename T,T First,T...Args>
struct sum<T,First,Args...>:std:: integral_constant<T,First+sum<T,Args...>::value>{};

template<bool Test>
constexpr size_t test(){return Test?1:0;};

template<typename Class>
struct remove_last_arg;
template<template<typename...>class Class,typename...Args>
struct remove_last_arg<Class<Args...>>
{
    using type=typename keep_args_at<Class<Args...>,decltype(std::make_index_sequence<type_count<Args...>::value-1>())>::type;
};

template<typename T,TypeQualifier...Extends>
struct remove;

template<typename T>
struct remove<T>
{
    using type=T;
};


template<typename T,TypeQualifier...Extends>
struct remove<const T,TypeQualifier::Const,Extends...>
{
    using type=typename remove<T,Extends...>::type;
};

template<typename T,TypeQualifier...Extends>
struct remove<const volatile T,TypeQualifier::ConstVolatile,Extends...>
{
   using type=typename remove<T,Extends...>::type;
};


template<typename T,TypeQualifier...Extends>
struct remove<T*,TypeQualifier::Pointer,Extends...>
{
   using type=typename remove<T,Extends...>::type;
};
template<typename T,TypeQualifier...Extends>
struct remove<T&&,TypeQualifier::R_Ref,Extends...>
{
    using type=typename remove<T,Extends...>::type;
};

template<typename T,TypeQualifier...Extends>
struct remove<T&,TypeQualifier::Ref,Extends...>
{
    using type=T;
};
template<typename T,TypeQualifier...Extends>
struct remove<T&,TypeQualifier::Volatile,Extends...>
{
   using type=typename remove<T,Extends...>::type;
};
template<typename T,TypeQualifier First,TypeQualifier...Rest>
struct remove<T,First,Rest...>
{
    using type=typename remove<T,Rest...>::type;
};
template<typename T>
using normalize=remove<T,Const,Ref,R_Ref>;



}

#endif // TYPETRAITS_H
