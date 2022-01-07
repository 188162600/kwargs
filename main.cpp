#include <iostream>

#include"src/tools/debug.h"

using namespace Tools;
int main()
{


    print(1,2,std::string("vghujh"),kw::sep=std::string(","),kw::end=std::string(" end\n"));
    print(1,2,std::string("vghujh"),kw::end=std::string(" end\n"),kw::sep=std::string("-"));
    print(1,2,std::string("vghujh"),kw::end=std::string(" end\n"));
     print(1,2,std::string("vghujh"),kw::sep=std::string("sep"));
    print(123.1234, std::string("12"));


}
