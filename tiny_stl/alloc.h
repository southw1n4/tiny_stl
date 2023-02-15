#ifndef __ALLOC_H__
#define __ALLOC_H__
//

#if 0
#   include<new>
#   define __THROW_BAD_ALLOC throw bad_alloc
#
#elif !defined(__THROW_BAD_ALLOC)
#   include <iostream>
#    define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl; exit(1);
#endif

#include <cstdlib>

namespace tiny_stl{

template <int inst>
class __malloc_alloc_template{
 private:
     static void* oom_malloc(size_t);
     static void* oom_realloc(void*, size_t);
     static void(*__malloc_alloc_oom_hander)();
 public:
     static void* allocate(size_t n) {
         void* result = ::malloc(n);

         if(result == nullptr)
            result = oom_malloc(n);

         return result;
     }

     static void deallocate(void* p, size_t /* n */){
         ::free(p);
     }

     static void* reallocate(void* p, size_t /* old_size */, size_t new_sz){
         void* result = ::realloc(p, new_sz);

         if(result == nullptr)
             result = oom_realloc(p, new_sz);
        
         return result;
     }

     static void (*set_malloc_handler( void (*f)() ))() {
         void (*old)() = __malloc_alloc_oom_hander;

         __malloc_alloc_oom_hander = f;

         return (old);
     }

};

template<int inst>
void (* __malloc_alloc_template<inst>::__malloc_alloc_oom_hander)() = 0;

template<int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t n){
    void (* my_malloc_hander)();
    void* result;

    for(; ;){
        my_malloc_hander = __malloc_alloc_oom_hander;
        if(my_malloc_hander == 0){
            __THROW_BAD_ALLOC;
        } 

        (*my_malloc_hander)();

        result = ::malloc(n);
        if(result) return (result);
    }
}

template<int inst>
void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n){
    void (* my_malloc_hander)();
    void* result;

    for(; ;){
        my_malloc_hander = __malloc_alloc_oom_hander;
        if(my_malloc_hander == 0){
            __THROW_BAD_ALLOC;
        } 

        (*my_malloc_hander)();

        result = ::realloc(p, n);
        if(result) return (result);
    }
}

typedef __malloc_alloc_template<0> malloc_alloc;

enum {__ALIGN = 8};
enum {__MAX_BYTES = 128};
enum {__NFREELISTS = __MAX_BYTES / __ALIGN};

template<bool threads, int inst> 
class __default_alloc_template{
 private:
     union obj{
         union obj* free_list_link;
         char client_data[1];
     };

     static obj* volatile free_lists[__NFREELISTS];
     static char* start_free;
     static char* end_free;
     static size_t heap_size;

 private:
     static size_t ROUND_UP(size_t bytes) {
         return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
     } 

     static size_t FREELIST_INDEX(size_t bytes) {
         return ((bytes + __ALIGN - 1) / __ALIGN  - 1);
     }

     static void* refill(size_t n);
     static char* chunk_alloc(size_t size, int& nobjs);

 public:
     static void* allocate(size_t n);
     static void deallocate(void* p, size_t n);
     static void* reallocate(void* p, size_t old_se, size_t new_sz);

};

template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::start_free = 0;

template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::end_free = 0;

template<bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

template<bool threads, int inst>
typename __default_alloc_template<threads, inst>::obj* volatile
__default_alloc_template<threads, inst>::free_lists[__NFREELISTS] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};


template<bool threads, int inst>
void* __default_alloc_template<threads, inst>::allocate(size_t n) {
    if(n > (size_t)__MAX_BYTES) {
        return (malloc_alloc::allocate(n));
    };

    obj* volatile* my_free_list;
    obj* result;

    my_free_list = free_lists + FREELIST_INDEX(n);
    result = *my_free_list;

    if(result == 0) {
        void* r = refill(ROUND_UP(n));
        return r;
    }

    *my_free_list = result->free_list_link;

    return (result);
}

template<bool threads, int inst>
void __default_alloc_template<threads, inst>::deallocate(void* p, size_t n){
    if(n > (size_t)__MAX_BYTES) {
        malloc_alloc::deallocate(p, n);
        return;
    }

    obj* q = (obj*)p;
    obj* volatile * my_free_list;

    my_free_list = free_lists + FREELIST_INDEX(n);
    q->free_list_link  = *my_free_list;
    *my_free_list = q;
} 

template<bool threads, int inst>
void* __default_alloc_template<threads, inst>::reallocate(void *p, size_t old_se, size_t new_sz){
    /*TODO
     *
     *
     */

}

template<bool threads, int inst>
void* __default_alloc_template<threads, inst>::refill(size_t n){
    int nobjs = 20;
    char* chunk = chunk_alloc(n, nobjs);

    if(1 == nobjs)
        return (chunk);

    obj* result = (obj*)chunk;
    obj* volatile * my_free_list = free_lists + FREELIST_INDEX(n);
    obj* current_obj;
    obj* next_obj;

    *my_free_list = next_obj = (obj*)(chunk + n);
    for(int i = 1; ; ++ i){
        current_obj = next_obj;
        next_obj = (obj*) ((char *)next_obj + n);

        if(nobjs - 1 == i) {
            current_obj->free_list_link = 0;
            break;
        }else{
            current_obj->free_list_link = next_obj;
        }
    }

    return (result);

}

template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs){
    char* result;
    size_t total_bytes = size * nobjs;
    size_t bytes_left = end_free - start_free;

    if(bytes_left >= total_bytes) {
        result = start_free;
        start_free += total_bytes;

        return result;
    }

    if(bytes_left >= size){
        result = start_free;
        nobjs = (int)(bytes_left / size);
        total_bytes = size * nobjs;
        start_free += total_bytes;

        return result;
    }

    size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 2);
    if(bytes_left > 0) {
        obj* volatile * my_free_list = free_lists + FREELIST_INDEX(bytes_left);
        ((obj*)start_free)->free_list_link = *my_free_list;
        *my_free_list = (obj*) start_free;
    }

    start_free = (char *)malloc(bytes_to_get);
    if(start_free == 0) {
        obj* volatile *my_free_list;

        for(int i = size; i <= __MAX_BYTES; i += __ALIGN){
            my_free_list = free_lists + FREELIST_INDEX(i);
            obj* p_obj = *my_free_list;

            if(0 != p_obj) {
                *my_free_list = p_obj->free_list_link;
                start_free = (char*)p_obj;
                end_free = start_free + i;

                return (chunk_alloc(size, nobjs));
            }
        }

        end_free = 0;
        start_free = (char *)malloc_alloc::allocate(bytes_to_get);
    }

    heap_size += bytes_to_get;
    end_free = start_free + bytes_to_get;

    return chunk_alloc(size, nobjs);

}

#define __NODE_ALLOCATER_THREADS false

typedef __default_alloc_template<__NODE_ALLOCATER_THREADS, 0> alloc;

template<typename T, class Alloc>
class simple_alloc{
 public:
     static T* allocate(size_t n) {
         return 0 == n ? 0: (T*) Alloc::allocate(n * sizeof(T));
     }

     static T* allocate(void) {
         return (T*)Alloc::allocate(sizeof(T));
     }

     static void deallocate(T* p, size_t n){
         if(0 != n)
             Alloc::deallocate(p, n * sizeof(T));
     }

     static void deallocate(T* p){
             Alloc::deallocate(p, sizeof(T));
     }

};

} // namespace tiny_stl


#endif // alloc
