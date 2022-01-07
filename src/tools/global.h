#ifndef GLOBAL_H
#define GLOBAL_H



#include<cstdint>
#include<cstddef>
#include<cassert>

#include<type_traits>
#include<memory>
#include<limits>



#define DISABLE_COPY(CLASS) CLASS(CLASS&&other)=delete;
#define DISABLE_MOVE(CLASS) CLASS(const CLASS&other)=delete;
#define DISABLE_COPY_MOVE(CLASS) DISABLE_MOVE(CLASS) DISABLE_COPY(CLASS)

#define bytesof(TYPE) sizeof(TYPE)
#define bitsof(TYPE) bytesof(TYPE)*8

#if __cpp_constexpr>=201304L
#define __CXX14CONSTEXPR__ constexpr
#else
#define __CXX14CONSTEXPR__
#endif

#if __cpp_constexpr>=201603L
#define __CXX17CONSTEXPR__ constexpr
#else
#define __CXX17CONSTEXPR__
#endif

#if __cpp_constexpr>=201907L
#define __CXX20CONSTEXPR__ constexpr
#else
#define __CXX20CONSTEXPR__
#endif



#if __cplusplus>=201402L
#define SINCE_CXX14(code) code
#else
#define SINCE_CXX14(code)
#endif

#if __cplusplus>=201703L
#define SINCE_CXX17(code) code
#else
#define SINCE_CXX17(code)
#endif

#if __cplusplus>=202002L
#define SINCE_CXX20(code) code
#else
#define SINCE_CXX20(code)
#endif

#define SINCE(version,code) SINCE_#version (code)

#if __has_attribute(likey)
#define [[likey]] __likely__
#else
#define __likely__
#endif

#if __has_attribute(unlikey)
#define [[unlikey]] __unlikey__
#else
#define __unlikey__
#endif



namespace Tools {
enum SetValueOption{SetStoredValue=0x01,Update=0x02};
enum Direction{Forward=0x01,Backward=0x02};
enum StorageDuration{Automatic,Static,Thread,Dynamic};
enum FindChildrenOption{FindDirectChildrenOnly=0x00,FindChildrenRecursively=0x01};
enum TraversingMethod{PreorderTravsersal,InorderTranversal,PostorderTranversal};
enum SearchTreeType{UnbalancedTree,RedBlackTree,AVLTree};
enum TypeQualifier{Const=0x01,Ref=0x02,R_Ref=0x04,Pointer=0x08,Volatile=0x10,ConstVolatile=0x20};//,AllTypeQualifier=Const|Ref|R_Ref|Pointer|Volatile|ConstVolatile};

}

#endif // GLOBAL_H
