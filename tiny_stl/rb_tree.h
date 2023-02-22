#ifndef RB_TREE_H__
#define RB_TREE_H__

#include "algorithm.h"
#include "iterator.h"
#include "construct.h"
#include "alloc.h"
#include "queue.h"

namespace tiny_stl {

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;

struct __rb_tree_node_base {
    typedef __rb_tree_color_type color_type;
    typedef __rb_tree_node_base* base_ptr;

    color_type color;
    base_ptr parent;
    base_ptr left;
    base_ptr right;

    static base_ptr minimum(base_ptr x) {
        while(x->left != nullptr)
            x = x->left;

        return x;
    };

    static base_ptr maximum(base_ptr x) {
        while(x->right != nullptr)
            x = x->right;

        return x;
    }

};

template<class Value>
struct __rb_tree_node: public __rb_tree_node_base{
    typedef __rb_tree_node<Value>*  node_pointer;
    Value value_field;
};

struct __rb_tree_iterator_base{
    typedef __rb_tree_node_base::base_ptr    base_ptr;
    typedef bidirectional_iterator_tag       iterator_category;
    typedef ptrdiff_t                        difference_type;

    base_ptr node;

    __rb_tree_iterator_base(base_ptr ptr): node(ptr) {}
    void increment() {
        if(node->right != nullptr) {
            node = node->right;
            node = node->minimum(node);

            return ;
        }
        
        //node->right == nullptr || node = node->parent->right;
        base_ptr parent = node->parent;
        while(node == parent->right) {
            node = parent;
            parent = parent->parent;
        }

        if(node->right != parent){
            node = parent;
        }
    }

    void decrement() {
        if(node->color == __rb_tree_red && node->parent->parent == node) {
            node = node->right;

            return ;
        }

        if(node->left != nullptr) {
            node = node->left;
            node = node->maximum(node);

            return ;
        }

        base_ptr parent = node->parent;
        while(node == parent->left) {
            node = parent;
            parent = parent->parent;
        }

        node = parent;
    }
};

template<class T> 
struct __rb_tree_iterator: public __rb_tree_iterator_base {
    typedef T                         value_type;
    typedef T&                        reference;
    typedef T*                        pointer;
    typedef __rb_tree_node<T>*        node_pointer;
    typedef __rb_tree_iterator<T>     self;
    typedef __rb_tree_iterator<T>     iterator;

    __rb_tree_iterator():__rb_tree_iterator_base(nullptr){}
    __rb_tree_iterator(node_pointer np): __rb_tree_iterator_base(np){}
    __rb_tree_iterator(const iterator& it):__rb_tree_iterator_base(it.node){};

    reference operator*(){return node_pointer(node)->value_field;}
    pointer  operator->(){return &(operator*());}


    self& operator++(){
        increment();

        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        increment();

        return *this;
    }

    self& operator--() {
        decrement();

        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        decrement();

        return *this;
    }

    bool operator==(const iterator& rhs) const {return node == rhs.node;}
    bool operator!=(const iterator& rhs) const {return node != rhs.node;}
};


template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
class rb_tree{
 protected:
     typedef __rb_tree_color_type color_type;
     typedef __rb_tree_node_base* base_ptr;
     typedef __rb_tree_node<Value> rb_tree_node;
     typedef rb_tree<Key, Value, KeyOfValue, Compare, Alloc> self;

     typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;

 public:
     typedef Key                                             key_type;
     typedef Value                                           value_type;
     typedef value_type*                                     pointer;
     typedef value_type&                                     reference;
     typedef rb_tree_node*                                   node_pointer;
     typedef size_t                                          size_type;
     typedef ptrdiff_t                                       difference_type;
     typedef typename __rb_tree_iterator<Value>::iterator    iterator;

 protected:
     node_pointer header_;
     size_type node_count_;
     Compare key_cmp_;

     node_pointer& root() const {return (node_pointer&) header_->parent;}
     node_pointer& left_most() const {return (node_pointer&) header_->left;}
     node_pointer right_most() const {return (node_pointer&) header_->right;}

     static node_pointer& left(node_pointer x) {return (node_pointer&)(x->left);}
     static node_pointer& right(node_pointer x) {return (node_pointer&)(x->right);}
     static node_pointer& parent(node_pointer x) {return (node_pointer&)(x->parent);}
     static reference value(node_pointer x) {return x->value_field;}
     static const Key& key(node_pointer x) {return KeyOfValue()(x->value_field);}
     static color_type& color(node_pointer x) {return (color_type&)(x->color);}

     static node_pointer& left(base_ptr x) {return (node_pointer&)(x->left);}
     static node_pointer& right(base_ptr x) {return (node_pointer&)(x->right);}
     static node_pointer& parent(base_ptr x) {return (node_pointer&)(x->parent);}
     static reference value(base_ptr x) {return ((node_pointer)x)->value_field;}
     static const Key& key(base_ptr x) {return KeyOfValue()(node_pointer(x)->value_field);}

     static node_pointer get_min_node(node_pointer x) {
         return (node_pointer)__rb_tree_node_base::minimum(x);
     }
     static node_pointer get_max_node(node_pointer x) {
         return (node_pointer)__rb_tree_node_base::maximum(x);
     }

 protected:
     node_pointer get_node() {return rb_tree_node_allocator::allocate();}
     node_pointer put_node(node_pointer p) {rb_tree_node_allocator::deallocate(p);}

     node_pointer create_node(const value_type& x) {
         node_pointer node = get_node();
         construct(&(node->value_field), x);

         return node;
     }

     node_pointer clone_node(const node_pointer x) {
         node_pointer node = create_node(x->value_field);
         node->color = x->color;
         node->left = nullptr;
         node->right = nullptr;

         return node;
     }

     void destory_node(node_pointer x) {
         destory(&x->value_field);
         put_node(x);
     }

 private:
     iterator __insert(base_ptr x, base_ptr y, const value_type& val);

     node_pointer __copy(node_pointer x);

     void __erase_node(node_pointer x);

     void __erase_node_case2_reblance(node_pointer x);

     void init() {
         header_ = get_node();
         color(header_) =  __rb_tree_red;
         root() = nullptr;
         left_most() = header_;
         right_most() = header_;
     }

 public:
     rb_tree(const Compare& cmp = Compare()): node_count_(0), key_cmp_(cmp){
         init();
     }
     rb_tree(const self& x){
         init();
         *this = x;
     }

     ~rb_tree() {
         clear();
         put_node(header_);
     }

     self& operator=(const self& x);
     void clear();
     void swap(self& x){
         node_pointer tmp = header_;
         *(&header_) = x.header;
         *(&(x.header)) = tmp;
     }

     const iterator find(const Key& k) const;

     iterator find(const Key& k) {
         const iterator it = static_cast<const self&>(*this).find(k);
         node_pointer p = static_cast<node_pointer>(it.node);

         return iterator(p);
     }

     size_type count(const key_type& x) const {
         size_type x_count = 0;
         const iterator it = find(x);

         if(it != end()) {
             ++ x_count;

             while(++ it != end()) {
                 if(!key_cmp_(x, key(it.node)) && !key_cmp_(key(it.node), x))
                     ++ x_count;
                 else 
                     break;
             }
         }

         return x_count;
     }

     Compare key_cmp() const {return key_cmp_;}
     iterator begin() {return left_most();}
     iterator end() {return header_;}
     bool empty() const {return (node_count_ == 0);}
     size_type size() const {return node_count_;}
     size_type max_size() const {return size_type(-1);}

     std::pair<iterator, bool> insert_unique(const value_type& val);
     template<class InputIterator>
     void insert_unique(InputIterator first, InputIterator last){
         while(first != last) {
             insert_unique(*first);

             ++first;
         }
     }

     iterator insert_equal(const value_type& val);
     template<class InputIterator>
     void insert_equal(InputIterator first, InputIterator last){
         while(first != last) {
             insert_equal(*first);

             ++first;
         }
     }


     void erase(node_pointer x) {__erase_node(x);}
     void erase(iterator x) {__erase_node(node_pointer(x.node));}

     size_type erase(const Key& x) {
         iterator it = find(x);
         size_type count = 0;

         while(it != end()) {
             __erase_node(node_pointer(it.node));
             it = find(x);
             ++ count;
         }

         return count;
     }

     void erase(iterator first, iterator last) {
         while(first != last) {
             iterator tmp = first ++;
             erase(tmp);
         }
     }

     iterator lower_bound(const key_type& x){return lower_bound(begin(), end(), x);}
     iterator upper_bound(const key_type& x) {return upper_bound(begin(), end(), x);}

     std::pair<iterator, iterator> equal_range(const key_type& x) {
         iterator first = find(x);
         iterator last = find(x);

         while(last != end()) {
             if(!key_cmp_(x, key(last.node)) && !key_cmp_(key(last.node), x))
                 ++ last;
             else
                 break;
         }
         return std::pair<iterator, iterator>(first, last);
     }

};


template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const self& x) {
    clear();

    key_cmp_ = x.key_cmp_;
    header_->parent = __copy(x.root());
    if(header_->parent != nullptr) {
        root()->parent = header_;
        node_count_ = x.node_count_;

        left_most() = get_max_node(root());
        right_most() = get_max_node(root());
    }

    return *this;
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear() {
    if(header_->parent != nullptr) {
        queue<base_ptr> node_queue;

        node_queue.push(header_->parent);

        while(!node_queue.empty()) {
            base_ptr cur = node_queue.front();
            node_queue.pop();

            if(cur->left != nullptr)
                node_queue.push(cur->left);
            if(cur->right != nullptr) 
                node_queue.push(cur->right);

            destory_node(node_pointer(cur));
        }

        node_count_ = 0;
        root() = nullptr;
        left_most() = header_;
        right_most() = header_;
    }
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
const typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) const{
    node_pointer x = root();

    while (x != nullptr) {
        if(key_cmp_(key(x), k))
            x = right(x);
        else if(key_cmp_(k, key(x)))
            x = left(x);
        else 
            return iterator(x);
    }

    return end();
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v) {
    node_pointer y = header_;
    node_pointer x = root();
    bool cmp = true;

    while(x != nullptr) {
        y = x;

        cmp = key_cmp_(KeyOfValue()(v), key(x));
        x = cmp ? left(x) : right(x);
    }

    iterator j = iterator(y);
    if(cmp) {
        if(j == begin())
            return std::pair<iterator, bool>(__insert(x, y, v), true);
        else
            -- j;
    }

    if(key_cmp_(key(j.node), KeyOfValue()(v))) {
        return std::pair<iterator, bool>(__insert(x, y, v), true);
    }

    return std::pair<iterator, bool>(j, false);
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& v) {
    node_pointer y = header_;
    node_pointer x = root();

    while(x != nullptr) {
        y = x;
        x = key_cmp_(KeyOfValue()(v), key(x)) ? left(x): right(x);
    }

    return __insert(x, y, v);
}


template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr _x, base_ptr _y, const value_type& val) {
    node_pointer x = (node_pointer)_x;
    node_pointer y = (node_pointer)_y;

    node_pointer new_node = create_node(val);
    if(y == header_ || x != 0 || key_cmp_(KeyOfValue()(val), key(y))) {
        left(y) = new_node;
        if(y == header_) {
            root() = new_node;
            right_most() = new_node;
        }else if(y == left_most()) {
            left_most() = new_node;
        }
    }else {
        right(y) = new_node();
        if(y == right_most()) {
            right_most() = new_node;
        }
    }

    parent(new_node) = y;
    left(new_node) = 0;
    right(new_node) = 0;

    __rb_tree_rebalance(new_node, header_->parent);
    ++ node_count_;

    return iterator(new_node);
}


template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__copy(node_pointer x) {
    if(x == nullptr)
        return nullptr;

    node_pointer x_copy = clone_node(x);
    x_copy->left = __copy(left(x));
    x_copy->right = __copy(right(x));

    if(x_copy->left != nullptr)
        x->copy->left->parent = x_copy;
    if(x_copy->right != nullptr)
        x->copy->right->parent = x_copy;

    return x_copy;
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase_node(node_pointer x) {
    /*TODO
     *
     */
}


template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase_node_case2_reblance(node_pointer x) {
    /*TODO

      */
}

inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
    __rb_tree_node_base* y = x->right;

    x->right = y->left;
    if(y->left != nullptr) {
        y->left->parent = x;
    }
    y->parent = x->parent;

    if(x == root){
        root = y;
        x->parent->parent = y;
    }else if(x == x->parent->left) 
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;


}

inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
    __rb_tree_node_base* y = x->left;

    x->left = y->right;
    if(y->right != nullptr) {
        y->right->parent = x;
    }
    y->parent = x->parent;

    if(x == root) {
        root = y;
        x->parent->parent = y;
    }
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;

    y->right = x;
    x->parent = y;

}

inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root){
    x->color = __rb_tree_red;

    while(x != root && x->parent->color == __rb_tree_red) {
        if(x->parent == x->parent->parent->left) {
            __rb_tree_node_base* y = x->parent->parent->right;

            if(y && y->color == __rb_tree_red){
                y->color = __rb_tree_black;
                x->parent->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;

                x = x->parent->parent;
            }else {
                if(x == x->parent->right) {
                    x = x->parent;
                    __rb_tree_rotate_left(x, root);
                }

                x->parent->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;
                __rb_tree_rotate_right(x->parent->parent, root);
            }
        }else {
            __rb_tree_node_base* y = x->parent->parent->left;

            if(y && y->color == __rb_tree_red) {
                y->color  = __rb_tree_black;
                x->parent->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;

                x = x->parent->parent;
            }else {
                if(x == x->parent->left) {
                    x = x->parent;
                    __rb_tree_rotate_right(x, root);
                }

                x->parent->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;

                __rb_tree_rotate_left(x->parent->parent, root);
            }
        }
    }

    root->color = __rb_tree_black;
}




}


#endif 
