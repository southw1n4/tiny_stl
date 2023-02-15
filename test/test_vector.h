#ifndef _TEST_VECTOR_H__
#define _TEST_VECTOR_H__

#include "gtest/gtest.h"
#include "vector.h"

namespace tiny_stl{
namespace test{

namespace vector{

TEST(TinySTLTest, VectorTest){

    int arr[] = {6, 7, 8, 9, 10};
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
    tiny_stl::vector<int> v4(v3);
    tiny_stl::vector<int> v5(v1);
    tiny_stl::vector<int> v6(arr, arr + 5);
    v3[0] = 3;
    v4.insert(v4.begin(), 100, 1);
    v5.erase(v5.begin() + 2, v5.begin() + 4);

    EXPECT_EQ(v1[2], 3);
    EXPECT_EQ(v1.size(), 5);

    EXPECT_EQ(v2.size(), 10);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2.capacity(), 10);

    EXPECT_EQ(v3.size(), 10);
    EXPECT_EQ(v3[0], 3);
    EXPECT_EQ(v3.capacity(), 10);
    

    EXPECT_EQ(v4.size(), 110);
    EXPECT_EQ(v4.capacity(), 200);
    
    EXPECT_EQ(v5.size(), 3);
    EXPECT_EQ(v5[0], 1);
    EXPECT_EQ(v5[1], 2);
    EXPECT_EQ(v5[2], 5);

    EXPECT_EQ(v6.size(), 5);
    EXPECT_EQ(v6.capacity(), 5);
    EXPECT_EQ(v6[0], 6);
    EXPECT_EQ(v6[1], 7);
    EXPECT_EQ(v6[2], 8);
    EXPECT_EQ(v6[3], 9);
    EXPECT_EQ(v6[4], 10);

}

} //vector

} //test

} //tiny_stl

#endif
