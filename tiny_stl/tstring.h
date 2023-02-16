#ifndef STRING_H__
#define STRING_H__

#include "alloc.h"
#include "construct.h"
#include "iterator.h"
#include "uninitialized.h"
#include "algorithm.h"
#include "vector.h"

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
     size_type npos = -1; // for not found


     string(): size_(0), capacity_(0), buffer_(0){}
     string(size_type n) {
         buffer_ = data_allocator::allocate(n);
         size_ = 0;
         capacity_ = n;
     }
     int compare(const string&) const ;
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

     string(const_reference value) {

         buffer_ = data_allocator::allocate(1);
         construct(buffer_, value);
         
         size_ = 1;
         capacity_ = 1;

     }
     string(const string& rhs) {
         size_type n = rhs.size();
         buffer_ = data_allocator::allocate(n);

         uninitialized_copy(rhs.buffer_, rhs.buffer_ + rhs.size_, buffer_);
         size_ = n;
         capacity_ = n;
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
         destory(buffer_, buffer_ + size());
         deallocate();

         size_type n = rhs.size();
         buffer_ = data_allocator::allocate(n);

         uninitialized_copy(rhs.buffer_, rhs.buffer_ + rhs.size_, buffer_);
         size_ = n;
         capacity_ = n;

         return *this;
     }

     string& operator=(const char* s){
         destory(buffer_, buffer_ + size());
         deallocate();

         size_type n = strlen(s);
         buffer_ = data_allocator::allocate(n);
         uninitialized_copy(s, s + n, buffer_);
         
         size_ = n;
         capacity_ = n;

         return *this;
     }

     ~string(){
         destory(buffer_, buffer_ + size_);
         deallocate();
     }

     size_type size() const {return size_;}
     iterator begin() {return buffer_;}
     iterator end() {return buffer_ + size_;}
     bool empty() {return size_ == 0;}
     void clear(){
         destory(buffer_, buffer_ + size_);
         size_ = 0;
     };

     reference front(){return *buffer_;}
     reference back(){return *(buffer_ + size_ - 1);}
     reference operator[](size_type n) {return *(buffer_ + n);}
     const_reference operator[](size_type n) const {return *(buffer_ + n);}
     pointer data() {return buffer_;}

     string& append(const_pointer c_str){
         *this += c_str;
         return *this;
     }

     string& append(const string& rhs){
         *this += rhs;
         return *this;
     }

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


     string operator+(const string& rhs);
     string& operator+=(const string& rhs);

     string upper(); 
     string lower();

     string substr(size_type pos,  size_type n);
     string substr(size_type pos);
     string lstrip(const value_type&);
     string rstrip(const value_type&);
     size_type find(size_type pos, const_reference value); 
     size_type find(const_reference value); 
     string strip(const value_type&);
     vector<string> split(const value_type&);
     string replace(const string&& src, const string& tgt);
 
};

void string::__expand_buffer() {
    size_type new_capacity = 2 * capacity_; 
    size_type old_size = size_;
    iterator new_buffer = data_allocator::allocate(new_capacity);

    uninitialized_copy(buffer_, buffer_ + size_, new_buffer);
    destory(buffer_, buffer_ + size_);
    deallocate();


    buffer_ = new_buffer;
    capacity_ = new_capacity;
    size_ = old_size;
}

int string::compare(const string& rhs) const {
    size_type n = size();
    size_type m = rhs.size(); 
    size_type i = 0, j = 0;

    for(; i < n && j < m; ++ i, ++ j) {
        if(buffer_[i] == rhs[j]) continue;
        if(buffer_[i] > rhs[j]){
            return 1;
        }else {
            return -1;
        }

    }
    if(i == n && j == m) return 0;
    else if(i != n){
        return 1;
    }

    return -1;
}

string string::operator+(const string& rhs) {
    size_type n = size(), m = rhs.size();
    string result(n + m);

    uninitialized_copy(begin(), end(), result.begin());
    uninitialized_copy(rhs.buffer_, rhs.buffer_ + rhs.size_, result.begin() + n);
    result.size_ = n + m;

    return result;
}

string& string::operator+=(const string& rhs) {
    size_type n = size(), m = rhs.size();
    iterator new_buffer = data_allocator::allocate(n + m);

    uninitialized_copy(begin(), end(), new_buffer);
    uninitialized_copy(rhs.buffer_, rhs.buffer_ + rhs.size_, new_buffer + n);

    destory(buffer_, buffer_ + n);
    deallocate();

    buffer_ = new_buffer;
    size_ = n + m;
    capacity_ = n + m;

    return *this;
}
std::ostream& operator<<(std::ostream& o, const string& s) {
    for(int i = 0; i < s.size(); ++ i) {
        o << s[i];
    }
    return o;
}



string string::upper() {
    string tmp(*this);
    for(int i = 0; i < size_; ++ i) {
        if(tmp[i] >= 'a' && tmp[i] <= 'z')
            tmp[i] = tmp[i] - 'a' + 'A';
    }

    return tmp;
}

string string::lower() {
    string tmp(*this);
    for(int i = 0; i < size_; ++ i) {
        if(tmp[i] >= 'A' && tmp[i] <= 'Z')
            tmp[i] = tmp[i] - 'A' + 'a';
    }

    return tmp;

}

string string::lstrip(const_reference value){
    size_type pos = 0;
    while(pos < size_ && buffer_[pos] == value) ++ pos;

    return substr(pos);

}

string string::rstrip(const_reference value){
    size_type pos = size_ - 1;
    while(pos >= 0 && buffer_[pos] == value) -- pos;

    return substr(0, pos + 1); 
}

string string::strip(const_reference value) {
    return this->lstrip(value).rstrip(value);
}

string string::substr(size_type pos){
    return substr(pos, size_ - pos);
}

string string::substr(size_type pos, size_type n) {
    string tmp(n);

    uninitialized_copy(buffer_ + pos, buffer_ + pos + n, tmp.buffer_);
    tmp.size_ = n;

    return tmp;
}

string::size_type string::find(size_type pos, const_reference value) {

    while(pos < size_ && buffer_[pos] != value) ++ pos;

    if(pos == size_) return npos;
    return pos;
}

string::size_type string::find(const_reference value) {
    return find(0, value);
}

vector<string> string::split(const_reference value) {
    vector<string> tmp;

    for(size_type i = 0, j = 0; i < size_; ++ i) {
        if(buffer_[i] == value) continue;

        j = i;
        while(j + 1 < size_ && buffer_[j + 1] != value) ++ j;
        tmp.push_back(substr(i, j - i + 1));

        i = j;
    }

    return tmp;

}

string string::replace(const string&& src, const string& tgt){
    string tmp(size_);

    for(size_type i = 0; i < size_; ++ i) {
        if(buffer_[i] != src[0]) {
            tmp.push_back(buffer_[i]);
            continue;
        }

        bool status = true;
        size_type t = 0;
        for(; t < src.size() && i + t < size_; ++ t) {
            if(buffer_[i + t] != src[t]) {
                status = false;
                break;
            }
        }

        if(!status){
            tmp.push_back(buffer_[i]);
            continue;
        }

        tmp += tgt;
        i += t - 1;


    }

    return tmp;
}



bool operator>=(const tiny_stl::string& lhs, const tiny_stl::string& rhs){return lhs.compare(rhs) >= 0;}
bool operator>(const tiny_stl::string& lhs,  const tiny_stl::string& rhs) {return lhs.compare(rhs) > 0;}
bool operator==(const tiny_stl::string& lhs, const tiny_stl::string& rhs){return lhs.compare(rhs) == 0;} 
bool operator==(const tiny_stl::string& lhs, const char* rhs){return lhs.compare(rhs) == 0;}
bool operator!=(const tiny_stl::string& lhs, const tiny_stl::string& rhs){return lhs.compare(rhs) != 0;}
bool operator<(const tiny_stl::string& lhs, const tiny_stl::string& rhs){return lhs.compare(rhs) < 0;}
bool operator<=(const tiny_stl::string& lhs, const tiny_stl::string& rhs){return lhs.compare(rhs) <= 0;}
}

#endif
