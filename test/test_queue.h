#ifndef TEST_QUEUE_H__
#define TEST_QUEUE_H__

#include "gtest/gtest.h"
#include "queue.h"

namespace  tiny_stl{
namespace test{
namespace queue{

TEST(TinySTLTest, QueueTest){

    tiny_stl::queue<int> que;

    que.push(0);
    que.push(1);
    que.push(2);
    que.pop();
    que.push(4);
    que.push(5);
    que.pop();
    que.push(6);
    que.pop();
    que.push(7);
    que.pop();
    que.push(8);


    EXPECT_EQ(que.size(), 4);

    EXPECT_EQ(que.front(), 5);
    que.pop();

    EXPECT_EQ(que.front(), 6);
    que.pop();

    EXPECT_EQ(que.front(), 7);
    que.pop();

    EXPECT_EQ(que.front(), 8);
    que.pop();

}

} //queue
} //test
} //tiny_stl


#endif //test_queue
