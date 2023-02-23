#ifndef HASHTABLE_H__
#define HASHTABLE_H__

#include "alloc.h"
#include "construct.h"
#include "iterator"
#include "vector.h"
#include <ratio>

namespace tiny_stl{
template<class Value>
struct __hashtable_node{
    Value val;
    __hashtable_node* next;
};

template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc = alloc>
struct hashtable;

template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc = alloc>
struct __hashtable_iterator{
 public:
     typedef forward_iterator_tag iterator_category;
     typedef Value                                                                      value_type;
     typedef Value&                                                                     reference;
     typedef Value*                                                                     pointer;
     typedef size_t                                                                     size_type;
     typedef ptrdiff_t                                                                  difference_type;
     typedef __hashtable_node<Value>                                                    node_type;
     typedef __hashtable_iterator<Value, Key, HashFunc, EqualKey, EqualKey, Alloc>      self;
     typedef __hashtable_iterator<Value, Key, HashFunc, EqualKey, EqualKey, Alloc>      iterator;
     typedef hashtable<Value, Key, HashFunc, EqualKey, EqualKey, Alloc>                 HashTable;

     node_type* cur;
     HashTable* htb;

 public:
     __hashtable_iterator(): cur(nullptr), htb(nullptr){}
     __hashtable_iterator(node_type* _cur, HashTable* _htb): cur(_cur), htb(_htb){}
     __hashtable_iterator(const self& rhs): cur(rhs.cur), htb(rhs.htb) {}

     reference operator*() const {return cur->val;}
     pointer operator->()  const {return &(operator*());}
 
     iterator& operator ++ (){
         const node_type* old = cur;
         cur = cur->next;

         if(cur == nullptr) {
             size_type bucket = htb->bucket_num(old->val);

             while(++ bucket < htb->buckets.size()) {
                 if(htb->buckets[bucket] != nullptr){
                     cur = htb->buckets[bucket];
                     break;
                 }
             }

         }

         return *this;
     }

     iterator operator ++ (int) {
         self tmp = *this;
         ++ *this;

         return tmp;
     }

     bool operator == (const iterator& rhs) {return (cur == rhs.cur);}
     bool operator != (const iterator& rhs) {return (cur != rhs.cur);}
}; 

static const int __stl_num_primes = 28;
static const unsigned long __stl_prime_list[__stl_num_primes] = {
    53,         97,           193,         389,       769,
    1543,       3079,         6151,        12289,     24593,
    49157,      98317,        196613,      393241,    786433,
    1572869,    3145739,      6291469,     12582917,  25165843,
    50331653,   100663319,    201326611,   402653189, 805306457,
    1610612741, 3221225473ul, 4294967291ul
};

inline unsigned long __stl_next_prime(unsigned long n) {
    const unsigned long* first = __stl_prime_list;
    const unsigned long* last = __stl_prime_list + __stl_num_primes;
    const unsigned long* pos = lower_bound(first, last, n);

    return pos == last ? *(last - 1) : *pos;
}


template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
struct hashtable{
 public:
     typedef typename __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey>::iterator iterator;
     typedef Value value_type;
     typedef Key key_type;
     typedef size_t size_type;
     typedef Value* pointer;
     typedef Value& reference;
     typedef ptrdiff_t difference_type;
 
     friend class __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>;
 private:
     typedef __hashtable_node<Value> node_type;
     typedef simple_alloc<node_type, Alloc> node_allocater;
     typedef hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc> self;

     vector<node_type*, Alloc> buckets;
     size_type num_elements;
     HashFunc hasher;
     EqualKey key_equals;
     ExtractKey get_key;

 protected:
     node_type* create_node(const value_type& val) {
         node_type* node = node_allocater::allocate();
         node->next = nullptr;

         construct(&node->val, val);

         return node;
     }

     void delete_node(node_type* node) {
         destory(&node->val);
         node_allocater::deallocate(node);
     }

     void resize(size_type num_elements_hint);

     std::pair<iterator, bool> insert_unique_noresize(const value_type& val);
     iterator insert_equal_noresize(const value_type& val);

     void __erase(const value_type& val);

     size_type next_size(size_type n) const {return __stl_next_prime(n);}

 public:
     size_type bucket_num(const value_type& val, size_t n) const {
         return bucket_num_key(get_key(val), n);
     }

     size_type bucket_num(const value_type& val) const {
         return bucket_num_key(get_key(val));
     }

     size_type bucket_num_key(const key_type& key) const {
         return bucket_num_key(key, buckets.size());
     }

     size_type bucket_num_key(const key_type& key, size_type n) const {
         return hasher(key) % n;
     }

     size_type bucket_count() const {return buckets.size();}

     int elemts_in_buckets(size_type n) {
         int  count = 0;
         node_type* first = buckets[n];

         while(first != nullptr) {
             ++ count;
             first = first->next;
         }

         return count;
     }

     HashFunc hash() const {
         return hasher;
     }

     EqualKey equals() const {
         return key_equals;
     }

     size_type max_bucket_count() const {
         return __stl_prime_list[__stl_num_primes - 1];
     }

 public:
     hashtable(size_type n, const HashFunc& hf, const EqualKey& eql)
         :buckets(next_size(n), nullptr) , hasher(hf), key_equals(eql), get_key(ExtractKey())
     {
         num_elements = 0;
     }

     iterator begin() {
         for(int i = 0; i < buckets.size(); ++ i) {
             if(buckets[i] != nullptr){
                 return iterator(buckets[i], this);
             }
         }

         return end();
     }

     iterator end() {
         return iterator(nullptr, this);
     }

     size_type size() const {return num_elements;}
     bool empty() {return num_elements == 0;}

     iterator find(const key_type& key) {
         size_type n = bucket_num_key(key);

         node_type* cur = buckets[n];
         while(cur) {
             if(key_equals(get_key(cur->val), key)) 
                 return iterator(cur, this);

             cur = cur->next;
         }

         return end();
     }

     size_type count(const key_type& key) {
         int result = 0;
         iterator it = find(key);

         if(it != end()) {
             node_type* cur = it.cur;

             while(cur) {
                 if(key_equals(get_key(cur->val), key))
                     ++ result;

                 cur = cur->next;
             }

         }

         return result;
     }

     std::pair<iterator, bool> insert_unique(const value_type& val) {
         resize(num_elements + 1);
         return insert_unique_noresize(val);
     }

     template<class InputIterator>
     void insert_unique(InputIterator first, InputIterator last) {
         for(; first != last; ++ first) 
             insert_unique(*first);
     }

     iterator insert_equal(const value_type& val) {
         resize(num_elements + 1);
         return insert_equal_noresize(val);
     }

     template<class InputIterator>
     void insert_equal(InputIterator first, InputIterator last) {
         for(; first != last; ++ first)
             insert_equal(*first);
     }

     void erase(iterator it) {__erase(*it);}
     void erase(node_type* node) {__erase(node->val);}
     template<class InputIterator>
     void erase(InputIterator first, InputIterator last) {
         while(first != last) {
             InputIterator tmp = first ++;
             __erase(*tmp);
         }
     }

     void clear();

     void copy_from(const hashtable& rhs);

     void swap(self& rhs) {
         buckets.swap(buckets);
         swap(num_elements, rhs.num_elements);
     }

};

template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::clear() {
    size_type sz = buckets.size();

    for(size_type i = 0; i < sz; ++ i) {
        node_type* cur = buckets[i];

        while(cur) {
            node_type* next = cur->next;
            delete_node(cur);
            cur = next;
        }

        buckets[i] = nullptr;
    }
}

template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable& rhs) {
    size_type rhs_size = rhs.buckets.size();

    clear();
    buckets.reserve(rhs_size);
    buckets.insert(buckets.end(), rhs_size, nullptr);

    for(size_type i = 0; i < rhs_size; ++ i) {
        node_type* cur = rhs.buckets[i];

        if(cur != nullptr){
            node_type* copy = create_node(cur->val);
            buckets[i] = copy;

            for(cur = cur->next; cur != nullptr; cur = cur->next) {
                copy->next = create_node(cur->val);
                copy = copy->next;
            }
        }
    }

    num_elements = rhs.num_elements;

}
template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint) {
    const size_type old_size = buckets.size();

    if(num_elements_hint > old_size) {
        const size_type new_size = next_size(num_elements_hint);
        vector<node_type*, Alloc> tmp_buckets(new_size, nullptr);

        for(int i = 0; i < old_size; ++ i) {
            node_type* first = buckets[i];

            while(first != nullptr) {
                size_type bucket = bucket_num(first->val, new_size);

                buckets[i] = first->next;
                first->next = tmp_buckets[bucket];
                tmp_buckets[bucket] = first;
                first = buckets[i];
            }
        }

        buckets.swap(tmp_buckets);
    }


}
template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
std::pair<typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::iterator, bool>
hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::insert_unique_noresize(const value_type& val){
    const size_type n = bucket_num(val);
    node_type* first = buckets[n];

    for(node_type* cur = first; cur != nullptr; cur = cur->next) {
        if(key_equals(get_key(cur->val), get_key(val)))
            return std::pair<iterator, bool>(iterator(cur, this), false);
    }

    node_type* new_node = create_node(val);
    new_node->next = first;
    buckets[n] = new_node;
    ++ num_elements;

    return std::pair<iterator, bool>(iterator(new_node, this), true);

}
template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::iterator 
hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type& val){
    const size_type n = buckets(val);
    node_type* first = buckets[n];

    for(node_type* cur = first; cur != nullptr; cur = cur->next) {
        if(key_equals(get_key(cur->val), get_key(val))) {
            node_type* new_node = create_node(val);
            new_node->next = cur->next;
            cur->next = new_node;
            ++num_elements;

            return iterator(new_node, this);
        }
    }

    node_type* new_node = create_node(val);
    new_node->next = first;
    buckets[n] = new_node;
    ++ num_elements;
    return iterator(new_node, this);
}


template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::__erase(const value_type& val){
    size_type n = bucket_num(val);
    node_type* cur = buckets[n];

    if(cur != nullptr) {
        if(key_equals(get_key(cur->val), get_key(val))) {
            buckets[n] = cur->next;
            delete_node(cur);
            -- num_elements;

            return ;
        }

        while(cur->next != nullptr) {
            if(key_equals(get_key(cur->next->val), get_key(val))) {
                node_type* tmp_node = cur->next;
                cur->next = tmp_node->next;
                delete_node(tmp_node);
                --num_elements;
                return ;
            }

            cur = cur->next;
        }
    }
}
}


#endif  //HASHTABLE_H__
