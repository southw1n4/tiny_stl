#ifndef LIST_H__
#define LIST_H__

#include "iterator.h"
#include "algorithm"
#include "alloc.h"
#include "construct.h"

namespace tiny_stl{


template<class T>
struct __node_type{
    typedef __node_type<T>* node_pointer;

    node_pointer prev;
    node_pointer next;

    T data;
};

template<class T, class Ref = T&, class Ptr = T*>
struct __list_iterator{
 public:
     typedef __list_iterator<T, T&, T*>   iterator;
     typedef __list_iterator<T, Ref, Ptr> self;
     typedef bidirectional_iterator_tag   iterator_category;
     typedef T                            value_type;
     typedef Ptr                          pointer;
     typedef Ref                          reference;
     typedef size_t                       size_type;
     typedef ptrdiff_t                    difference_type;
     typedef __node_type<T>*              node_pointer;

     node_pointer node;


     __list_iterator(node_pointer x):node(x){}
     __list_iterator(){};
     __list_iterator(const iterator& x):node(x.node){}

     bool operator==(const self& x){return node == x.node;}
     bool operator!=(const self& x){return node != x.ndoe;}

     reference operator*(){return (*node).data;}
     pointer  operator->(){return &(operator*());}

     self& operator++(){
         node = (*node).next;
         return *this;
     }

     self operator++(int){
         self tmp = *this;
         ++*this;
         return tmp;
     }

     self& operator--(){
         node = (*node).prev;
         return *this;
     }

     self operator--(int){
         self tmp = *this;
         --*this;
         return tmp;
     }
};

template<class T, class Alloc = alloc>
class list{
 public:
     typedef typename __list_iterator<T>::node_pointer        node_pointer;
     typedef typename __list_iterator<T>::iterator            iterator;
     typedef typename __list_iterator<T>::iterator_category   iterator_category;
     typedef typename __list_iterator<T>::value_type          value_type;
     typedef typename __list_iterator<T>::reference           reference;
     typedef typename __list_iterator<T>::difference_type     difference_type;
     typedef typename __list_iterator<T>::pointer             pointer;
     typedef typename __list_iterator<T>::size_type           size_type;
     typedef __node_type<T>                                   node_type;

 protected:
     typedef simple_alloc<node_type, alloc> list_node_allocate;

     node_pointer get_node() {
         return list_node_allocate::allocate();
     };

     void put_node(node_pointer p) {
         list_node_allocate::deallocate(p);
     }

     node_pointer create_node(const T& x) {
         node_pointer p = get_node();
         construct(p->data, x);

         return p;
     }

     void destory_node(node_pointer p){
         destory(&p->data);
         put_node(p);
     }

     void empty_initialize() {
         node_ = get_node();
         node_->next = node_;
         node_->prev = node_;
         size_ = 0;
     }

 private:
     size_type size_;
     node_pointer node_;

 public:
     list() {empty_initialize();}

     ~list(){
         clear();
         destory_node(node_);
     }

     iterator begin(){return (*node_).next;}
     iterator end(){return node_;}
     bool empty(){return size_ == 0;}
     size_type size(){return size_;}

     reference front(){return *begin();}
     reference back(){return *(--end());}

     void push_back(const T& x){insert(end(), x);}
     void push_front(const T& x){insert(begin(), x);}
     void pop_back() {erase(--end());}
     void pop_front() {erase(begin());}

     void clear();
     void insert(iterator pos, const T& x);
     iterator erase(iterator pos);

     void remove(const T& x);
     void unique();
     void splice(iterator pos, list<T, Alloc>& x);
     void merge(list<T, Alloc>& x);
     void reverse();
     void sort(bool(*)(const T& a, const T&) = nullptr);

};

template<class T, class Alloc>
void list<T, Alloc>::clear() {
    while(size_ --){
        node_pointer tmp = node_->next;
        node_->next = tmp->next;
        tmp->next->prev = node_;

        destory_node(tmp);
    }

}

template<class T, class Alloc>
void list<T, Alloc>::insert(iterator pos, const T& x) {
    node_pointer tmp = create_node(x);
    
    tmp->next = pos.node;
    tmp->prev = tmp->next->prev;

    tmp->prev->next = tmp;
    tmp->next->prev = tmp;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator pos){
    node_pointer tmp = pos.node;
    node_pointer next = tmp->next;

    tmp->next->prev = tmp->prev;
    tmp->prev->next = tmp->next;

    destory_node(tmp);
    size_ --;

    return next;
}

template<class T, class Alloc>
void list<T, Alloc>::remove(const T& x){
    for(iterator it = begin(); it != end(); ++ it) {
        if(*it == x)
            erase(it);
    }
}

template<class T, class Alloc>
void list<T, Alloc>::unique(){

}

template<class T, class Alloc>
void list<T, Alloc>::splice(iterator pos, list<T, Alloc>& x){

}

template<class T, class Alloc>
void list<T, Alloc>::merge(list<T, Alloc>& rhs){

}

template<class T, class Alloc>
void list<T, Alloc>::reverse(){

}

template<class T, class Alloc>
void list<T, Alloc>::sort(bool(*cmp)(const T&, const T&)){
}
}

#endif // LIST_H__
