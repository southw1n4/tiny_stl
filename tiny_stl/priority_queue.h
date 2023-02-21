#ifndef PRIORITY_QUEUE_H__
#define PRIORITY_QUEUE_H__

#include "vector.h"
#include "functional.h"
namespace tiny_stl {

template<class T, class Container = vector<T>, class Compare = less<T>>
class priority_queue{
 public:
     typedef T                  value_type;
     typedef size_t             size_type;
     typedef T&                 reference;
     typedef const T&           const_reference;

 private:
     Container container_;
     Compare   cmp_;

 private:
     void __push_up(size_type root){
         size_type tmp = root;
         if(root * 2 <= size())  tmp = cmp_(container_[tmp - 1], container_[root * 2 - 1]) ? root * 2: tmp;
         if(root * 2 + 1 <= size()) tmp = cmp_(container_[tmp - 1], container_[root * 2]) ? root * 2 + 1: tmp;

         if(tmp != root) {
             swap(container_[tmp - 1], container_[root - 1]);
             __push_down(tmp);
             __push_up(root / 2);
         }

     }

     void __push_down(size_type root){
         size_type tmp = root;
         if(root * 2 <= size()) tmp = cmp_(container_[tmp - 1], container_[root * 2 - 1]) ? root * 2: tmp;
         if(root * 2 + 1 <= size()) tmp = cmp_(container_[tmp - 1], container_[root * 2]) ? root * 2 + 1: tmp;

         if(tmp != root) {
             swap(container_[tmp - 1], container_[root - 1]);
             __push_down(tmp);
         }

     }


 public:
     priority_queue() {};
     priority_queue(const vector<T>& vec): container_(vec){
         make_heap(container_.begin(), container_.end(), cmp_);
     }

     template<class InputIterator>
     priority_queue(InputIterator first, InputIterator end): container_(first, end){
         make_heap(container_.begin(), container_.end(), cmp_);
     }

     size_type size(){return container_.size();}
     bool empty() {return container_.empty();}
     const_reference top() {return container_.front();}
     void pop(){
         container_.front() = container_.back();
         container_.pop_back();

         __push_down(1);
     }; 
     void push(const value_type& val){
         container_.push_back(val);

         __push_up(container_.size() / 2);
     }
};
} //tiny_stl
#endif //PRIORITY_QUEUE_H__
