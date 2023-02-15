#ifndef VECTOR_H__
#define VECTOR_H__
//

#include "alloc.h"
#include "construct.h"
#include "iterator.h"
#include "uninitialized.h"
#include "algorithm.h"


namespace tiny_stl{
template<class T, class Alloc = alloc>
class vector{
 public:
     typedef T                      value_type;
     typedef value_type*            pointer;
     typedef const value_type*      const_pointer;
     typedef value_type*            iterator;
     typedef const value_type*      cosnt_iterator;
     typedef value_type&            reference;
     typedef const value_type&      const_reference;
     typedef size_t                 size_type;
     typedef ptrdiff_t              difference_type;

 private:
     typedef simple_alloc<value_type, Alloc> data_allocator;
     iterator start;
     iterator finish;
     iterator end_of_storage;

 private:
     void deallocate(){
         if(start){
             data_allocator::deallocate(start, end_of_storage - start);
         }

         start = 0;
         finish = 0;
         end_of_storage = 0;
     }

     void fill_initialize(size_type n, const T& value){
         start = allocate_and_fill(n, value);
         finish = start + n;
         end_of_storage = finish;
     }

     iterator allocate_and_fill(size_type n, const T& x){
         iterator result = data_allocator::allocate(n);
         uninitialized_fill_n(result, n, x);
         return result;
     }

     void insert_aux(iterator pos, size_type n, const T& x);
 
 public:
     iterator begin() {return start;}
     iterator end() {return finish;}
     size_type size() const { return size_type(finish - start);}
     size_type capacity() const {return size_type(end_of_storage - start);}
     bool empty() const {return begin() == end();}
     reference operator[](size_type n) {return *(begin() + n);}

     vector():start(0), finish(0), end_of_storage(0){};
     explicit vector(size_type n){fill_initialize(n, T());}
     vector(int n, const T& value){fill_initialize(n, value);}
     vector(size_type n, const T& value){fill_initialize(n, value);}

     ~vector(){
         destory(start, finish);
         deallocate();
     }

     reference front(){ return *begin();}
     reference back() {return *(end() - 1);}
     void push_back(const T& x){
         if(finish != end_of_storage){
             construct(finish, x);
             ++ finish;
         }else {
             insert_aux(end(), 1, x);
         }
     }

     void pop_back() {
         -- finish;
         destory(finish);
     }

     void insert(iterator pos, size_type n, const T& x) {
         insert_aux(pos, n, x);
     }

     void insert(iterator pos, const T& x) {
         insert_aux(pos, 1, x);
     }

     iterator erase(iterator pos){
         if (pos + 1 != end()){
             copy(pos + 1, finish, pos);
         }
         -- finish;

         destory(finish);
         return pos;
     }
     
     iterator erase(iterator first, iterator last){
         iterator it = copy(last, finish, first);

         destory(it, finish);
         finish = it;
         return first;
     }

     void resize(size_type new_sz, const T& x){
         if(new_sz < size()) {
             erase(begin() + new_sz, end());
         }else {
             insert(end(), new_sz - size(), x);
         }
     }

     void resize(size_type new_sz) {resize(new_sz, T());}

     void clear(){erase(begin(), end());}

};
template<class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator pos, size_type n, const T& x){
    if(n <= 0) return ;

    if(size_type(end_of_storage - finish) >= n) {
        T x_copy = x;
        uninitialized_copy(pos, end(), pos + n);
        uninitialized_fill(pos, pos + n, x);

        return ;
    }

    // size_type(end_of_storage - finish) < n;
    const size_type old_capacity = capacity();
    const size_type new_capacity = 2 * max(old_capacity, n);

    iterator new_start = data_allocator::allocate(new_capacity);
    iterator new_finish = new_start;

    new_finish = uninitialized_copy(start, pos, new_start);
    new_finish = uninitialized_fill_n(new_finish, n, x);
    new_finish = uninitialized_copy(pos, finish, new_finish);

    destory(begin(), end());
    deallocate();

    start = new_start;
    finish = new_finish;
    end_of_storage = new_start + new_capacity;

}
}

#endif //vector
