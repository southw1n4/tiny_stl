#ifndef TEST_RB_TREE_H__
#define TEST_RB_TREE_H__

#include "gtest/gtest.h"
#include "../tiny_stl/rb_tree.h"
#include "../tiny_stl/functional.h"

namespace  tiny_stl{
namespace test{
namespace rb_tree{

TEST(TinySTLTest, RBTreeTest){
    tiny_stl::rb_tree<int, int, Identity<int>, less<int>> rb;

}

} //rb_tree
} //test
} //tiny_stl


#endif //test_rb_tree
