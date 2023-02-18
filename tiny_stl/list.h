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
     bool operator!=(const self& x){return node != x.node;}

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
         construct(&p->data, x);

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
     }

     void __sort();


 private:
     node_pointer node_;
     bool(*cmp_)(const T&, const T&) = nullptr;

 public:
     list() {empty_initialize();}

     ~list(){
         clear();
         destory_node(node_);
     }

     iterator begin(){return (*node_).next;}
     iterator end(){return node_;}
     iterator begin()const {return (*node_).next;}
     iterator end()const{return node_;}
     bool empty(){return begin() == end();}
     size_type size(){
         size_type n = 0;
         iterator first = begin();
         iterator last = end();

         while(first != last){
             ++first;
             ++ n;
         }

         return n;
     }

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
     void transfer(iterator pos, iterator first, iterator last);
     iterator find(const T& x);

};

template<class T, class Alloc>
void list<T, Alloc>::clear() {
    while(node_->next != node_){
        node_pointer tmp = node_;
        node_ = node_->next;
        tmp->prev->next = node_;
        node_->prev = tmp->prev;


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

    return iterator(next);
}

template<class T, class Alloc>
void list<T, Alloc>::remove(const T& x){
    iterator first = begin();
    iterator last = end();

    while(first != last) {
        if(*first == x) {
            first = erase(first);
            continue;
        }

        ++ first;
    }
}

template<class T, class Alloc>
void list<T, Alloc>::unique(){
    iterator first = begin();

    while(first != end()){
        iterator next = first;
        ++ next;

        while(next != end() &&  *next == *first) {
            next = erase(next);
        }

        first = next;
    }

}

template<class T, class Alloc>
void list<T, Alloc>::splice(iterator pos, list<T, Alloc>& x){
    if(!x.empty()){
        transfer(pos, x.begin(), x.end());
    }

}

template<class T, class Alloc>
void list<T, Alloc>::merge(list<T, Alloc>& rhs){
    iterator first1 = begin();
    iterator last1  = begin();
    iterator first2 = rhs.begin();
    iterator last2 = rhs.end();


    while(first1 != last1 && first2 != last2){
        if(*first1 < *first2) {
            iterator next = first2;
            transfer(first1, first2, ++ next);

            first2 = next;
        }else {
            ++ first1;
        }

        if(first2 != last2) transfer(last1, first2, last2);
    }

}

template<class T, class Alloc>
void list<T, Alloc>::reverse(){

    iterator first = begin();
    ++ first;

    while(first != end()){
        iterator old = first;
        ++ first;

        transfer(begin(), old, first);
    }
}

template<class T, class Alloc>
void list<T, Alloc>::sort(bool(*cmp)(const T&, const T&)){
    if(cmp == nullptr) {
        cmp = [](const T& a, const T& b) {
            return a < b;
        };
    }

    cmp_ = cmp;

    __sort();

}

template<class T, class Alloc>
void list<T, Alloc>::transfer(iterator pos, iterator first, iterator last){
    if(pos == last) return ;
    node_pointer tmp =  last.node->prev;

    last.node->prev = first.node->prev;
    first.node->prev->next = last.node;

    pos.node->prev->next = first.node;
    tmp->next = pos.node;
    first.node->prev = pos.node->prev;
    pos.node->prev = tmp;

}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::find(const T& x){
    iterator first = begin();
    iterator last = end();

    while(first != last){
        if(*first == x) return first;

        ++ first;
    }

    return last;
}
template<class T, class Alloc>
void list<T, Alloc>::__sort(){

    iterator first = ++ begin(); 
    iterator last = end();

    while(first != last) {
        iterator tmp = first ++;
        iterator pos = begin();

        while(pos != tmp && !cmp_(*tmp, *pos)) ++ pos;

        if(pos != tmp)
            transfer(pos, tmp, first);
    }
}

template<class T, class Alloc>
std::ostream& operator<<(std::ostream& o, const list<T, Alloc>& s) {
    for(auto it = s.begin(); it != s.end(); ++ it) {
        o << *it << " ";
    }

    return o;
}

}

#endif // LIST_H__
