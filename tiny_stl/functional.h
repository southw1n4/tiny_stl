#ifndef FUNCTIONAL_H__
#define FUNCTIONAL_H__

namespace tiny_stl{

template<class T>
class less{
 public:
     bool operator()(const T& a, const T& b){
         return a < b;
     };
};
}

#endif
