#ifndef STRING_H__
#define STRING_H__

#include "alloc.h"
#include "construct.h"
#include "iterator.h"
#include "uninitialized.h"
#include "algorithm.h"

namespace tiny_stl{
class string{
 public:
     typedef char                 value_type;
     typedef value_type*          pointer;
     typedef const value_type*    const_pointer;
     typedef value_type*          iterator;
     typedef const value_type*    const_iterator;
     typedef value_type&          reference;
     typedef const value_type&    const_reference;
     typedef size_t               size_type;
     typedef ptrdiff_t            difference_type;
 private:
     size_type size_;
     size_type capacity_;
     iterator buffer_;

     size_type strlen(const char* s) {
         size_type n = 0;
         while(*s != '\0') {
             ++ s; ++ n;
         }

         return n;
     }

     typedef simple_alloc<value_type, alloc> data_allocator;

     void deallocate() {
         if(buffer_){
             data_allocator::deallocate(buffer_, size_);
         }

         buffer_ = 0;
         size_ = 0;
     } 

     void fill_initialize(size_type n, const value_type& value) {
         buffer_ = allocate_and_fill(n, value);
         size_ = n;
         capacity_ = n; 
     }

     iterator allocate_and_fill(size_type n, const value_type& value) {
         iterator result = data_allocator::allocate(n);
         uninitialized_fill_n(result, n, value);

         return result;
     }

     void __expand_buffer();
 public:
     string(): size_(0), capacity_(0), buffer_(0){}
     string(size_type n) {
         buffer_ = data_allocator::allocate(n);
         size_ = 0;
         capacity_ = n;
     }
     string(size_type n, const value_type&& value){
         fill_initialize(n, value);
     }
     string(const char* str) {
         size_type n = strlen(str);

         buffer_ = data_allocator::allocate(n);
         uninitialized_copy(str, str + n, buffer_);
         
         size_ = n;
         capacity_ = n;

     }
     string(const string& rhs) {
         size_type n = rhs.size();
         buffer_ = data_allocator::allocate(n);

         uninitialized_copy(rhs.begin(), rhs.end(), buffer_);
         size_ = n;
     }

     template<class Iterator>
     string(Iterator first, Iterator last) {
         size_type n = distance(first, last);
         buffer_ = data_allocator::allocate(n);
         uninitialized_copy(first, last, buffer_);
         
         size_ = n;
         capacity_ = n;
     }

     string& operator=(const string& rhs){
         if(this == &rhs){
             return *this;
         }

         size_type n = rhs.size();
         buffer_ = data_allocator::allocate(n);

         uninitialized_copy(rhs.begin(), rhs.end(), buffer_);
         size_ = n;

         return *this;
     }

     ~string(){
         destory(buffer_, buffer_ + size_);
         deallocate();
     }

     size_type size() const {return size_;}
     iterator begin() const {return buffer_;}
     iterator end() const {return buffer_ + size_;}

     reference front(){return *buffer_;}
     reference back(){return *(buffer_ + size_ - 1);}
     reference operator[](size_type n) {return *(buffer_ + n - 1);}
     pointer data() {return buffer_;}

     void push_back(const char& value){
         if(size_ >= capacity_) {
             __expand_buffer();
         }

         construct(buffer_ + size_, value);
         ++ size_;

     }

     void pop_back() {
         -- size_; 
         destory(buffer_ + size_);
         
     }
 
};

void string::__expand_buffer() {
    size_type new_capacity = 2 * capacity_; 
    iterator new_buffer = data_allocator::allocate(new_capacity);

    uninitialized_copy(buffer_, buffer_ + size_, new_buffer);
    destory(buffer_, buffer_ + size_);
    deallocate();


    buffer_ = new_buffer;
    capacity_ = new_capacity;
}
}

#endif
