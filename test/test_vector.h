#ifndef _TEST_VECTOR_H__
#define _TEST_VECTOR_H__

#include "gtest/gtest.h"
#include "vector.h"

namespace tiny_stl{
namespace test{

namespace vector{

TEST(STLTest, VectorTest){
    tiny_stl::vector<int> v1;
    tiny_stl::vector<int> v2(10, 1);
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);
    v1.push_back(5);
    v1.push_back(6);
    v1.pop_back();
    tiny_stl::vector<int> v3(v2);
    v3.clear();

    EXPECT_EQ(v1[2], 3);
    EXPECT_EQ(v1.size(), 5);

    EXPECT_EQ(v2.size(), 10);
    EXPECT_EQ(v2.capacity(), 10);

    EXPECT_EQ(v3.size(), 0);
    EXPECT_EQ(v3.capacity(), 10);
    
}

} //vector

} //test

} //tiny_stl

#endif
