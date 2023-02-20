#ifndef DEQUE_H__
#define DEQUE_H__

#include "alloc.h"
#include "algorithm.h"
#include "construct.h"
#include "iterator.h"

namespace tiny_stl{

inline size_t __deque_buf_size(size_t buf_size, size_t val_size){
    return (buf_size != 0) ? buf_size:
                             ((val_size < 512) ? size_t(512 / val_size): size_t(1));
         
}
template<class T, size_t BufSize>
struct __deque_iterator{
 public:
     static size_t buffer_size() {return __deque_buf_size(BufSize, sizeof(T));}

     typedef __deque_iterator<T, BufSize>      iterator;
     typedef random_access_iterator_tag        iterator_category;
     typedef T                                 value_type;
     typedef T*                                pointer;
     typedef T&                                reference;
     typedef ptrdiff_t                         difference_type;
     typedef size_t                            size_type;
     typedef T**                               map_pointer; 
     typedef __deque_iterator                  self;

     T* cur;
     T* first;
     T* last;

     map_pointer node;

     void set_node(map_pointer new_node){
         node = new_node;
         first = *new_node;
         last = *new_node + buffer_size();
     }

     reference operator*(){return *cur;}
     reference operator->(){return &(operator*());}

     difference_type operator-(const self& rhs) {
         return (node - rhs.node - 1) * buffer_size() + (cur - first) + (rhs.last - rhs.cur);
     }

     self& operator++() {
         ++ cur;
         if(cur == last) {
             set_node(node + 1);
             cur = first;
         }

         return *this;
     }

     self operator++(int) {
         self tmp = *this;
         ++ *this;

         return tmp;
     }
     self& operator--() {
         if(cur == first){
             set_node(node - 1);
             cur = last;
         }
         -- cur;
         
         return *this;
     }
     self operator--(int) {
         self tmp = *this;
         -- *this;

         return tmp;
     }

     self& operator+=(difference_type n) {
         difference_type offset = n + (cur - first);
         if(offset >= 0 && offset < difference_type(buffer_size())){

             cur += n;
             return *this;
         }

         difference_type node_offset = offset > 0 ?  offset / difference_type(buffer_size()):
                                                     - difference_type((-offset - 1) / buffer_size()) - 1;
         set_node(node + node_offset);
         cur = first + (offset - node_offset * difference_type(buffer_size()));
                                            
         return *this;
     }

     self operator+(difference_type n)  const{
         self tmp = *this;
         tmp += n;

         return tmp;
     }

     self& operator-=(difference_type n) {
         return *this +=-n;
     }

     self operator-(difference_type n) {
         self tmp = *this;
         tmp -= n;
         return tmp;
     }

     reference operator[](difference_type n) const {return *(*this + n);}
     bool operator==(const self& x) const{ return cur == x.cur;}
     bool operator!=(const self& x) const{ return !(*this == x);}
     bool operator <(const self& x) const{
         return (node == x.node) ? (cur < x.cur) : (node < x.node);
     }
};

template<class T, class Alloc = alloc, size_t BufSize = 0>
class deque{
  public:
      typedef T                                                              value_type;
      typedef typename __deque_iterator<T, BufSize>::pointer                 pointer;
      typedef typename __deque_iterator<T, BufSize>::reference               reference;
      typedef typename __deque_iterator<T, BufSize>::iterator                iterator;
      typedef typename __deque_iterator<T, BufSize>::iterator_category       iterator_category;
      typedef typename __deque_iterator<T, BufSize>::size_type               size_type;
      typedef typename __deque_iterator<T, BufSize>::difference_type         difference_type;
      typedef typename __deque_iterator<T, BufSize>::map_pointer             map_pointer;

      size_t (*buffer_size)() = __deque_iterator<T, BufSize>::buffer_size;

 protected:
      map_pointer map_;
      iterator start_;
      iterator finish_;
      size_type map_size_;

      typedef simple_alloc<value_type, Alloc> data_allocator;
      typedef simple_alloc<pointer, Alloc> map_allocator;

      void fill_initialize(size_type n, const value_type& value);
      void create_map_and_nodes(size_type num_elem);
      void reallocate_map(size_type nodes_to_add, bool add_at_front);
      void push_back_aux(const value_type& value);
      void push_front_aux(const value_type& value);
      void reserve_map_at_back(size_type nodes_to_add = 1) {
          if(nodes_to_add + 1 > map_size_ - (finish_.node - map_))
              reallocate_map(nodes_to_add, false);
      }
      void reserve_map_at_front(size_type nodes_to_add = 1){
          if(nodes_to_add > start_.node - map_) {
              reallocate_map(nodes_to_add, true);
          }


      }
      iterator insert_aux(iterator pos, const value_type& value);


 public:
      deque(): start_(), finish_(), map_(0), map_size_(0) {
          create_map_and_nodes(0);
      }
      deque(size_type n, const value_type& value):start_(), finish_(), map_(0), map_size_(0){
          fill_initialize(n, value);
      }

      iterator begin() {return start_;}
      iterator end() {return finish_;}

      reference front(){return *start_;}
      reference back(){
          iterator tmp = finish_;
          -- tmp;

          return *tmp;
      }

      size_type size() const {return finish_ - start_;}
      bool empty() const {
          return start_ == finish_;
      }

      reference operator[](size_type n) {
          return start_[difference_type(n)];
      }

      void push_front(const value_type& value) {
          if(start_.cur != start_.first){
              construct(start_.cur - 1, value);
              -- start_.cur;
          }else {
              push_front_aux(value);
          }
      }

      void push_back(const value_type& value) {
          if(finish_.cur != finish_.last - 1) {
              construct(finish_.cur, value);
              ++ finish_.cur;
          }else {
              push_back_aux(value);
          }
      }

      void pop_back() {
          if(finish_.cur != finish_.first) {
              -- finish_.cur;
              destory(finish_.cur);
          }else {
              data_allocator::deallocate(finish_.first, buffer_size());
              finish_.set_node(finish_.node - 1);
              finish_.cur = finish_.last - 1;
              destory(finish_.cur);
          }

      }

      void pop_front() {
          if(start_.cur != start_.last - 1) {
              destory(start_.cur);
              ++ start_.cur;
          }else {
              destory(start_.cur);
              data_allocator::deallocate(start_.first, buffer_size());
              start_.set_node(start_.node + 1);
              start_.cur = start_.first;
          }
      }

      void clear();
      iterator erase(iterator pos);
      iterator erase(iterator first, iterator last);
      iterator insert(iterator pos, const value_type& val);

};
template<class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::fill_initialize(size_type n, const value_type& value) {
    create_map_and_nodes(n);
    map_pointer cur;

    for(cur = start_.node; cur < finish_.node; ++ cur) {
        uninitialized_fill(*cur, *cur + buffer_size(), value);
    }

    uninitialized_fill(finish_.first, finish_.cur, value);
}

template<class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elements) {
    size_type num_nodes = num_elements / buffer_size() + 1;

    map_size_ = num_nodes + 2;
    map_ = map_allocator::allocate(map_size_);

    map_pointer nstart = map_ + 1;
    map_pointer nfinish = nstart + num_nodes - 1;

    map_pointer cur;

    for(cur = nstart; cur <= nfinish; ++ cur) {
        *cur = data_allocator::allocate(buffer_size());
    }

    start_.set_node(nstart);
    start_.cur = start_.first;

    finish_.set_node(nfinish);
    finish_.cur = finish_.first +  num_elements % buffer_size();
}

template<class T, class Alloc, size_t BufSize> 
void deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add, bool add_at_front){
    size_type old_num_nodes = finish_.node - start_.node + 1;
    size_type new_num_nodes = old_num_nodes + nodes_to_add;

    map_pointer new_nstart;
    if(map_size_ > 2 * new_num_nodes){
        new_nstart = map_ + (map_size_ - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);

        if(new_nstart < start_.node) {
            copy(start_.node, finish_.node + 1, new_nstart);
        }else {
            copy_backward(start_.node, finish_.node + 1, new_nstart + old_num_nodes);
        }

    }else {

        size_type new_map_size = map_size_ + max(map_size_, nodes_to_add) + 2;
        map_pointer new_map = map_allocator::allocate(new_map_size);
        new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);

        copy(start_.node, finish_.node + 1, new_nstart);
        map_ = new_map;
        map_size_ = new_map_size;
    }

    start_.set_node(new_nstart);
    finish_.set_node(new_nstart + old_num_nodes - 1);
}

template<class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::push_back_aux(const value_type& val){
    reserve_map_at_back();
    *(finish_.node + 1) = data_allocator::allocate(buffer_size());

    construct(finish_.cur, val);
    finish_.set_node(finish_.node + 1);
    finish_.cur = finish_.first;
}


template<class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::push_front_aux(const value_type& val){
    reserve_map_at_front();
    *(start_.node - 1) = data_allocator::allocate(buffer_size());

    start_.set_node(start_.node - 1);
    start_.cur = start_.last - 1;
    construct(start_.cur ,val);

}

template<class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::clear(){
    for(map_pointer node = start_.node + 1; node < finish_.node; ++ node) {
        destory(*node, *node + buffer_size());
        data_allocator::deallocate(*node, buffer_size());
    }

    if(start_.node != finish_.node){
        destory(start_.cur, start_.last);
        destory(finish_.first, finish_.cur);

        data_allocator::deallocate(finish_.first, buffer_size());
    }else {
        destory(start_.cur, finish_.cur);
    }

    start_.cur = start_.first;
    finish_ = start_;
}


template<class T, class Alloc, size_t BufSize>
typename deque<T, Alloc, BufSize>::iterator
deque<T, Alloc, BufSize>::erase(iterator pos){
    iterator next = pos;
    ++ next;

    difference_type elem_before = pos - start_;
    if(elem_before < (finish_ - next)) {
        copy_backward(start_, pos, next);
        pop_front();
    }else {
        copy(next, finish_, pos);
        pop_back();
    }

    return start_ + elem_before;
}

template<class T, class Alloc, size_t BufSize>
typename deque<T, Alloc, BufSize>::iterator
deque<T, Alloc, BufSize>::erase(iterator first, iterator last){
    if(first == start_ && last == finish_) {
        clear();
        return finish_;
    }
    else {
        difference_type len = last - first;
        difference_type elem_before = first - start_;

        if(elem_before < (finish_ - last)) {
            copy_backward(start_, first, last);
            iterator new_start = start_ + len;

            destory(start_, new_start);

            for(map_pointer cur = start_.node; cur < new_start.node; ++ cur) {
                data_allocator::deallocate(*cur, buffer_size());
            }
            start_ = new_start;
        }
        else {
            copy(last, finish_, first);
            iterator new_finish = finish_ - len;
            destory(new_finish, finish_);
            for(map_pointer cur = new_finish.node + 1; cur <= finish_.node; ++ cur) {
                data_allocator::deallocate(*cur, buffer_size());
            }
            finish_ = new_finish;
        }

        return start_ + elem_before;
    }
}

template<class T, class Alloc, size_t BufSize>
typename deque<T, Alloc, BufSize>::iterator
deque<T, Alloc, BufSize>::insert(iterator pos, const value_type& val) {
    if(pos.cur == start_.cur) {
        push_front(val);
        return start_;
    }else if(pos.cur == finish_.cur) {
        push_back(val);
        iterator tmp = finish_;
        -- tmp;
        return tmp;
    }else {
        return insert_aux(pos, val);
    }
}

template<class T, class Alloc, size_t BufSize>
typename deque<T, Alloc, BufSize>::iterator
deque<T, Alloc, BufSize>::insert_aux(iterator pos, const value_type& val) {
    difference_type elem_before = pos - start_;

    if( elem_before < (finish_ - start_)) {
        iterator front_old = start_;
        push_front(front());
        iterator move_front = front_old + 1;
        copy(move_front, pos, front_old);

        -- pos;
    }
    else {
        iterator back_old = finish_;
        push_back(back());
        iterator move_back = back_old - 1;
        copy_backward(pos, move_back, back_old);

    }
    *pos = val;
    return pos;
}


}

#endif

