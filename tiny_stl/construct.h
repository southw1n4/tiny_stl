#ifndef CONSTRUCT_H__
#define CONSTRUCT_H__

#include <new>
#include "type_traits.h"
#include "iterator.h"

namespace tiny_stl{

template <class T1, class T2>
inline void construct(T1* p, const T2& value) {
    new(p) T1(value);
}

template <class T>
inline void destory(T* pointer) {
    pointer->~T();
}

template <class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __false_type) {
    for(; first < last; ++ first){
        destory(&*first);
    }
} 

template <class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __true_type) {} 

template<class ForwardIterator, class T> 
inline void __desotry(ForwardIterator first, ForwardIterator last, T*) {
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destory_aux(first, last, trivial_destructor());
}

template<class ForwardIterator>
inline void destory(ForwardIterator first, ForwardIterator last) {
    __desotry(first, last, value_type(first));
}



inline void destory(char*, char*){}
inline void destory(wchar_t*, wchar_t*){}

}


#endif // construct

