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

    template<class RandomAccessIterator, class Compare, class Distance>
    void __make_heap_aux(RandomAccessIterator first,
                         RandomAccessIterator last,
                         RandomAccessIterator root,
                         Compare cmp, 
                         Distance*) {
        typedef Distance difference_type;

        difference_type offset = root - first;

        RandomAccessIterator tmp = root;
        RandomAccessIterator l = first + offset * 2;
        RandomAccessIterator r = first + offset * 2 + 1;

        if(l < last && cmp(*tmp, *l)) tmp = l;
        if(r < last && cmp(*tmp, *r)) tmp = r;

        if(tmp != root) {
            swap(*tmp, *(first + offset));
            __make_heap_aux(first, last, tmp, cmp, distance_type(first));
        }
    }



    template<class RandomAccessIterator, class Compare, class Distance>
    void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp, Distance*) {
        typedef Distance difference_type;

        difference_type n = last - first;

        for(difference_type offset = n / 2; offset >= 0; -- offset) {
            RandomAccessIterator tmp = first + offset;
            RandomAccessIterator l = first + offset * 2;
            RandomAccessIterator r = first + offset * 2 + 1;

            if(l < last && cmp(*tmp, *l)) tmp = l;
            if(r < last && cmp(*tmp, *r)) tmp = r;

            if(tmp != offset + first) {
                swap(*tmp, *(first + offset));
                __make_heap_aux(first, last, tmp, cmp, distance_type(first));
            }
        }

    }

    template<class RandomAccessIterator, class Compare>
    void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp){
        __make_heap(first, last, cmp, distance_type(first));
    }



}

#endif
