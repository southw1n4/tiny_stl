#ifndef TEST_STACK_H__
#define TEST_STACK_H__

#include "gtest/gtest.h"
#include "stack.h"

namespace tiny_stl{
namespace test{
namespace stack{

TEST(TinySTLTest, StackTest) {

    tiny_stl::stack<int> stk;

    stk.push(1);
    stk.push(2);
    stk.push(3);
    stk.pop();
    stk.push(4);
    stk.push(5);
    stk.pop();
    stk.push(6);
    stk.pop();
    stk.pop();
    stk.push(7);
    stk.push(8);


    EXPECT_EQ(stk.size(), 4);

    EXPECT_EQ(stk.top(), 8);
    stk.pop();

    EXPECT_EQ(stk.top(), 7);
    stk.pop();

    EXPECT_EQ(stk.top(), 2);
    stk.pop();

    EXPECT_EQ(stk.top(), 1);
    stk.pop();
}



} // stack
} //test
} //tiny_stl


#endif  //test_stack
