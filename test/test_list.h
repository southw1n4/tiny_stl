#ifndef  TEST_LIST_H__
#define TEST_LIST_H__

#include "gtest/gtest.h"
#include "../tiny_stl/list.h"
#include "../tiny_stl/vector.h"

namespace tiny_stl{
namespace test{
namespace list{

TEST(TinySTLTest, ListTest){
    tiny_stl::list<int> l1;
    l1.push_back(5);
    l1.push_back(3);
    l1.pop_back();
    l1.push_back(4);
    l1.push_back(10);
    l1.pop_front();
    l1.push_back(7);
    l1.push_front(12);
    l1.push_back(33);
    l1.pop_front();
    l1.pop_back();
    l1.push_back(9);
     

    tiny_stl::list<int> l2;
    l2.push_back(4);
    l2.push_back(5);
    l1.transfer(l1.end(), l2.begin(), l2.end());

    tiny_stl::list<int> l3;
    l3.push_back(9);
    l3.push_back(7);
    l1.transfer(l1.begin(), l3.begin(), l3.end());

    tiny_stl::vector<int> v1;
    for(auto it = l1.begin(); it != l1.end(); ++ it) v1.push_back(*it);


    l1.sort();
    tiny_stl::vector<int> v2;
    for(auto it = l1.begin(); it != l1.end(); ++ it) v2.push_back(*it);

    l1.sort([](const int& a, const int& b){return a > b;});
    tiny_stl::vector<int> v3;
    for(auto it = l1.begin(); it != l1.end(); ++ it) v3.push_back(*it);
    v3.reverse();

    l1.unique();
    l1.reverse();
    tiny_stl::vector<int> v4;
    for(auto it = l1.begin(); it != l1.end(); ++ it) v4.push_back(*it);

    EXPECT_EQ(l1.size(), 5);

    EXPECT_EQ(v1[0], 9);
    EXPECT_EQ(v1[1], 7);
    EXPECT_EQ(v1[2], 4);
    EXPECT_EQ(v1[3], 10);
    EXPECT_EQ(v1[4], 7);
    EXPECT_EQ(v1[5], 9);
    EXPECT_EQ(v1[6], 4);
    EXPECT_EQ(v1[7], 5);


    EXPECT_EQ(v2[0], 4);
    EXPECT_EQ(v2[1], 4);
    EXPECT_EQ(v2[2], 5);
    EXPECT_EQ(v2[3], 7);
    EXPECT_EQ(v2[4], 7);
    EXPECT_EQ(v2[5], 9);
    EXPECT_EQ(v2[6], 9);
    EXPECT_EQ(v2[7], 10);

    EXPECT_EQ(v3[0], 4);
    EXPECT_EQ(v3[1], 4);
    EXPECT_EQ(v3[2], 5);
    EXPECT_EQ(v3[3], 7);
    EXPECT_EQ(v3[4], 7);
    EXPECT_EQ(v3[5], 9);
    EXPECT_EQ(v3[6], 9);
    EXPECT_EQ(v3[7], 10);


    EXPECT_EQ(v4[0], 4);
    EXPECT_EQ(v4[1], 5);
    EXPECT_EQ(v4[2], 7);
    EXPECT_EQ(v4[3], 9);
    EXPECT_EQ(v4[4], 10);
}


} //list
}//test
}//tiny_stl
#endif //test_list_h
