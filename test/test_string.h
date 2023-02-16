#ifndef TEST_STRING_H__
#define TEST_STRING_H__


#include "gtest/gtest.h"
#include "../tiny_stl/tstring.h"

namespace tiny_stl{
namespace test{
namespace string{

TEST(TinySTLTest, StringTest){

    tiny_stl::string s1("abcdef");
    tiny_stl::string s2(s1);
    tiny_stl::string s3(s1.begin(), s1.end());
    tiny_stl::string s4(10, 'a');
    tiny_stl::string s5 = s1 + s4;
    tiny_stl::string s6 = "abcdefg";
    tiny_stl::string s7 = "  abcdef wef  ";

    s3.push_back('a');


    EXPECT_EQ(s1.size(), 6);
    EXPECT_EQ(s1[0], 'a');
    EXPECT_EQ(s1[1], 'b');

    EXPECT_EQ(s1, s2);

    EXPECT_EQ(s3, "abcdefa");
    EXPECT_EQ(s3.size(), 7);
    EXPECT_EQ(s3[6], 'a');

    EXPECT_EQ(s4.size(), 10);
    EXPECT_EQ(s4, "aaaaaaaaaa");

    EXPECT_EQ(s5.size(), 16);
    EXPECT_EQ(s5, "abcdefaaaaaaaaaa");

    EXPECT_GT(s6, s1);
    EXPECT_GE(s2, s1);

    EXPECT_LT(s1, s6);
    EXPECT_LE(s1, s2);


    s1 += "+-*/Wzab";
    EXPECT_EQ(s1, "abcdef+-*/Wzab");
    EXPECT_EQ(s1.upper(), "ABCDEF+-*/WZAB");
    EXPECT_EQ(s1.lower(), "abcdef+-*/wzab");

    EXPECT_EQ(s7.lstrip(' '), "abcdef wef  ");
    EXPECT_EQ(s7.lstrip(' ').lstrip('a').rstrip(' '), "bcdef wef");
    EXPECT_EQ(s7.rstrip(' '), "  abcdef wef");
    EXPECT_EQ(s7.strip(' '), "abcdef wef");


    EXPECT_EQ(s7.find(' '), 0);
    EXPECT_EQ(s7.find('a'), 2);
    EXPECT_EQ(s7.find('k'), -1);

    EXPECT_EQ(s7.split(' ')[0], "abcdef");
    EXPECT_EQ(s7.split(' ')[1], "wef");
    EXPECT_EQ(s7.split('k')[0], "  abcdef wef  ");

    EXPECT_EQ(s7.replace(' ', '*'), "**abcdef*wef**");
    EXPECT_EQ(s7.replace(' ', "--"), "----abcdef--wef----");
    EXPECT_EQ(s7.replace("  ", '*'), "*abcdef wef*");
    EXPECT_EQ(s7.replace("  ", "--"), "--abcdef wef--");

}


} //string

} //test
} //tiny_stl

#endif 
