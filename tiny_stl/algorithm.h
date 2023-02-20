#ifndef ALGORITHM_H__
#define ALGORITHM_H__

namespace tiny_stl{

    template<class ForwardIterator, class T>
    inline void fill(ForwardIterator first, ForwardIterator last, const T& value){
        for(; first != last; ++ first){
            *first = value;
        }
    }

    template<class OutputIterator, class Size, class T>
    inline OutputIterator fill_n(OutputIterator first, Size n, const T& value){
        for(; n > 0; -- n, ++ first){
            *first = value;
        }

        return first;
    }

    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator copy(InputIterator first, InputIterator last, ForwardIterator result) {
        ForwardIterator cur = result;
        for(; first != last; ++ first, ++ cur) {
            *cur = *first;
        }

        return cur;
    }

    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator copy_backward(InputIterator first, InputIterator last, ForwardIterator result) {
        while(last != first) 
            *( -- result) = *(-- last);

        return result;
    }


    template<class T>
    inline const T& max(const T& a, const T& b){
        return (a < b) ? b : a;
    }

    template<class T>
    inline const T& min(const T& a, const T& b){
        return (a < b) ? a : b;
    }

    template<class T>
    inline void swap(T& lhs, T& rhs) {
        T tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    }
}

#endif
