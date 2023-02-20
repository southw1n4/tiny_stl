#ifndef TEST_DEQUE_H__
#define TEST_DEQUE_H__

#include "gtest/gtest.h"
#include "deque.h"

namespace tiny_stl {
namespace test{
namespace deque{

TEST(TinySTLTest, DequeTest) {

    tiny_stl::deque<int> dq;
    dq.push_back(0);
    dq.push_back(1);
    dq.push_back(2);
    dq.clear();

    dq.push_front(2);
    dq.push_front(3);
    dq.push_back(3);
    dq.push_back(4);
    dq.pop_front();
    dq.push_back(5);
    dq.pop_back();
    dq.push_back(6);
    dq.push_back(7);
    dq.push_back(8);
    dq.push_back(10);

    EXPECT_EQ(dq[0], 2);
    EXPECT_EQ(dq[1], 3);
    EXPECT_EQ(dq[2], 4);
    EXPECT_EQ(dq[3], 6);
    EXPECT_EQ(dq[4], 7);
    EXPECT_EQ(dq[5], 8);
    EXPECT_EQ(dq[6], 10);
}


}  //deque
} //test
} //tiny_stl

#endif //test_deque_;
