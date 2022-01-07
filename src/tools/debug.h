#ifndef DEBUG_H
#define DEBUG_H

#include<iostream>
#include"functional.h"
DECL_KWARG(sep)
DECL_KWARG(end)
DECL_KWARG(file)

namespace Tools {
//    template<typename...Args>
//    void doNothing(const Args...)
template<typename Tuple,typename Sep,typename OStream,size_t ...I>
void printHelper(const Tuple&tuple,const Sep&sep,OStream&out,std::index_sequence<I...>)
{
    ((out << std::get<I>(tuple) << sep),...);

};



///print(*values, *, sep=',' ,end='\n' , file=std::cout)
template<typename ...Args>
void print(const Args&...args)
{
    static auto params=getParams<kw::keyword_sep,kw::keyword_end,kw::keyword_file>(
                std::tuple<const Args&...>(args...),//args
                std::make_tuple(kw::sep=std::string(" "),kw::end=std::string("\n"),kw::file=std::ref(std::cout)));//defaults
#define __sep__ std::get<0>(params.kwargs).value
#define __end__ std::get<1>(params.kwargs).value
#define __file__ std::get<2>(params.kwargs).value

    printHelper(params.args,__sep__,__file__,std::make_index_sequence<std::tuple_size<decltype (params.args)>::value-1>{});
    __file__<<std::get<std::tuple_size<decltype (params.args)>::value-1>(params.args) <<__end__;
#undef __sep__
#undef __end__
#undef __file__


}

}
#endif // DEBUG_H
