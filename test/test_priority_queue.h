#ifndef TEST_PRIORITY_QUEUE_H__
#define TEST_PRIORITY_QUEUE_H__

#include "gtest/gtest.h"
#include "../tiny_stl/priority_queue.h"

namespace tiny_stl{
namespace test{
namespace priority_queue{

TEST(TinySTLTest, PriorityQueueTest) {

    tiny_stl::priority_queue<int> pq;
    tiny_stl::vector<int> v1;
    v1.push_back(1);
    v1.push_back(7);
    v1.push_back(8);
    v1.push_back(9);
    v1.push_back(10);
    v1.push_back(2);

    tiny_stl::priority_queue<int> pq2(v1.begin(), v1.end());

    pq.push(1);
    pq.push(6);
    pq.push(3);
    pq.push(5);
    pq.push(4);
    pq.push(2);

    EXPECT_EQ(pq.size(), 6);
    
    tiny_stl::vector<int> v;
    while(pq.size()) {
        v.push_back(pq.top());
        pq.pop();
    }

    while(pq2.size()) {
        v.push_back(pq2.top());
        pq2.pop();
    }

    EXPECT_EQ(v[0], 6);
    EXPECT_EQ(v[1], 5);
    EXPECT_EQ(v[2], 4);
    EXPECT_EQ(v[3], 3);
    EXPECT_EQ(v[4], 2);
    EXPECT_EQ(v[5], 1);

    EXPECT_EQ(v[6], 10);
    EXPECT_EQ(v[7], 9);
    EXPECT_EQ(v[8], 8);
    EXPECT_EQ(v[9], 7);
    EXPECT_EQ(v[10], 2);
    EXPECT_EQ(v[11], 1);
}
} // priority_queue
} //test
} // tiny_stl

#endif
