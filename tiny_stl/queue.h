#ifndef QUEUE_H__
#define QUEUE_H__

#include "deque.h"

namespace tiny_stl{

template<class T, class Sequence = deque<T>>
class queue{
 public:
     typedef typename Sequence::value_type    value_type;
     typedef typename Sequence::reference     reference;
     typedef typename Sequence::size_type     size_type;
 private:
     Sequence container_;

 public:
     bool empty() {return container_.empty();}
     size_type size(){ return container_.size();}
     reference front() {return container_.front();}

     void push(const value_type& val){ container_.push_back(val);}
     void pop() {container_.pop_front();}
};

}

#endif
