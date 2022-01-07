#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include"global.h"
#include"typetraits.h"
#include"algorithm.h"

#define DECL_KWARG(NAME)\
namespace kw { \
template<typename T,typename Test=void> \
struct kwarg_##NAME; \
struct keyword_##NAME \
{ \
    template<typename T> \
    using kwarg=kwarg_##NAME <T>; \
    template<typename T> \
    kwarg_##NAME<T>operator=(const T&value)const; \
    template<typename T> \
    kwarg_##NAME<T&>operator=(std::reference_wrapper<T>value)const; \
    template<typename T> \
    kwarg_##NAME<T>operator=(T&&value)const; \
}; \
template<typename T,typename> \
struct kwarg_##NAME:  keyword_##NAME\
{ \
    inline kwarg_##NAME(const T&v):value(v){};\
    inline kwarg_##NAME(T&&v):value(v){};\
    using type=T; \
    T value;\
};\
template<typename T> \
struct kwarg_##NAME<T,typename std::enable_if<std::is_reference<T>::value,void>::type>:  keyword_##NAME\
{ \
    inline kwarg_##NAME(T v):value(v){};\
    using type=T; \
    T value;\
};\
template<typename T> \
kwarg_##NAME<T> keyword_##NAME::operator=(const T&value)const{return kwarg_##NAME<T>{value};}; \
template<typename T> \
kwarg_##NAME<T&> keyword_##NAME::operator=(std::reference_wrapper<T>value)const{return kwarg_##NAME<T&>{value};}; \
template<typename T> \
kwarg_##NAME<T> keyword_##NAME::operator=(T&&value)const{return kwarg_##NAME<T>{value};}; \
const inline keyword_##NAME NAME;\
}

namespace Tools {

template<typename Keyword>
struct kwarg_index_helper
{
    template<typename Arg>
    using type=std::is_base_of<Keyword,typename normalize<Arg>::type>;
};

template<typename Keyword,typename Args>
struct kwarg_index:Tools::find_args_if<kwarg_index_helper<Keyword>::template type, Args>{};


template<typename AllArgs,typename...Keywords>
struct kwarg_count:sum<size_t, test<kwarg_index<Keywords,AllArgs>::value>=0>() ...>{};

template<typename Keyword, typename Args, typename Defaults,
    typename std::enable_if<kwarg_index<Keyword,Args>::value >= 0>::type* Test = nullptr>
    constexpr auto getKwarg(const Args& args, const Defaults&) { return std::get<kwarg_index<Keyword,Args>::value>(args); };

template<typename Keyword, typename Args, typename Defaults,
    typename std::enable_if<(kwarg_index<Keyword, Args>::value < 0) and( kwarg_index<Keyword, Defaults>::value >= 0)>::type* Test = nullptr>
    constexpr auto getKwarg(const Args&, const Defaults& defaultVals) { return std::get<kwarg_index<Keyword,Defaults>::value>(defaultVals); };

template<typename... Keyword,typename Args,typename Defaults>
auto getKwargs(const Args&args,const Defaults&defaults)
{
  return std::make_tuple(getKwarg<Keyword>(args,defaults)...);
};
 template<typename...Args>
 std::tuple<const Args...>allArgs(const Args&...args){return std::make_tuple(&args...);}

template<typename AllArgs,typename Args,typename Kwargs,typename Defaults>
struct Params
{
    AllArgs allArgs;
    Args args;
    Kwargs kwargs;
    Defaults defaults;
};


template<typename...Kwargs,typename AllArgs,typename Defaults>
auto getParams(AllArgs&&allArgs,Defaults&&defaults)
{
    using KwargsType=decltype(getKwargs<Kwargs...>(allArgs,defaults));
    using AllArgsType=AllArgs;
    const static size_t ArgsCount=std::tuple_size<AllArgsType>::value- kwarg_count<AllArgs,Kwargs...>::value;
    using ArgsType=decltype(makeFromTuple<std::tuple>(allArgs,std::make_index_sequence<ArgsCount>{}));
    using DefaultType=Defaults;
    return Params<AllArgsType,ArgsType,KwargsType,DefaultType>{
         allArgs,
         makeFromTuple<std::tuple>(allArgs,std::make_index_sequence<ArgsCount>{}),
         getKwargs<Kwargs...>(allArgs,defaults),
         defaults
    };

};


}

#endif // FUNCTIONAL_H
