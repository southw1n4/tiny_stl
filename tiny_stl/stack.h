#ifndef STACK_H__
#define STACK_H__

#include "deque.h"

namespace tiny_stl {

template<class T, class Sequence = deque<T>>
class stack{
 public:
     typedef typename Sequence::value_type           value_type;
     typedef typename Sequence::reference            reference;
     typedef typename Sequence::size_type            size_type;

 protected:
     Sequence container_;

 public:
     bool empty(){return container_.empty();}
     size_type size() {return container_.size();}
     reference top() {return container_.back();}
     void push(const value_type& val){ container_.push_back(val);}
     void pop(){container_.pop_back();}

};


}

#endif

