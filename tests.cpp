#include <gtest/gtest.h>
#include "main.cpp"

TEST(FourTest, DefaultConstructor) {
    Four four;
    EXPECT_TRUE(four.isEmpty());
    EXPECT_EQ(four.toString(), "0");
    EXPECT_EQ(four.getSize(), 0);
}

TEST(FourTest, ConstructorWithSize) {
    Four four(5, 3);
    EXPECT_FALSE(four.isEmpty());
    EXPECT_EQ(four.getSize(), 5);
    EXPECT_EQ(four.toString(), "33333");
}

TEST(FourTest, InitializerListConstructor) {
    Four four({1, 2, 3, 0, 2});
    EXPECT_EQ(four.getSize(), 5);
    EXPECT_EQ(four.toString(), "12302");
    
    Four zero({0});
    EXPECT_EQ(zero.toString(), "0");
}

TEST(FourTest, InitializerListConstructor_InvalidDigit) {
    EXPECT_THROW(Four({1, 2, 4, 3}), std::invalid_argument);
    EXPECT_THROW(Four({5}), std::invalid_argument);
    EXPECT_THROW(Four({1, 2, 8, 3}), std::invalid_argument);
}

TEST(FourTest, StringConstructor) {
    Four four("12302");
    EXPECT_EQ(four.getSize(), 5);
    EXPECT_EQ(four.toString(), "12302");
    
    Four zero("0");
    EXPECT_EQ(zero.toString(), "0");
}

TEST(FourTest, StringConstructor_InvalidDigit) {
    EXPECT_THROW(Four("12403"), std::invalid_argument);
    EXPECT_THROW(Four("5"), std::invalid_argument);
    EXPECT_THROW(Four("12a34"), std::invalid_argument);
}

TEST(FourTest, CopyConstructor) {
    Four original("12302");
    Four copy = original;
    
    EXPECT_EQ(original.toString(), copy.toString());
    EXPECT_EQ(original.getSize(), copy.getSize());
    
    EXPECT_EQ(copy.toString(), "12302");
}

TEST(FourTest, MoveConstructor) {
    Four original("12302");
    Four moved = std::move(original);
    
    EXPECT_EQ(moved.toString(), "12302");
    EXPECT_TRUE(original.isEmpty());
}

TEST(FourTest, AssignmentOperator) {
    Four a("123");
    Four b;
    
    b = a;
    EXPECT_EQ(a.toString(), b.toString());
    
    b = b;
    EXPECT_EQ(b.toString(), "123");
}

TEST(FourTest, AddOperation) {
    Four a("123");  
    Four b("201");  
    Four sum = a.add(b);
    
    EXPECT_EQ(sum.toString(), "330");
    
    Four zero;
    Four sum2 = a.add(zero);
    EXPECT_EQ(sum2.toString(), "123");
    
    Four sum3 = a.add(a);
    EXPECT_EQ(sum3.toString(), "312"); 
}

TEST(FourTest, SubtractOperation) {
    Four a("123");  ₀
    Four b("21");   
    Four diff = a.subtract(b);
    
    EXPECT_EQ(diff.toString(), "102");
    
    Four zero;
    Four diff2 = a.subtract(zero);
    EXPECT_EQ(diff2.toString(), "123");
    
    Four diff3 = a.subtract(a);
    EXPECT_EQ(diff3.toString(), "0");
}

TEST(FourTest, SubtractOperation_NegativeResult) {
    Four a("10"); 
    Four b("20"); 
    
    EXPECT_THROW(a.subtract(b), std::invalid_argument);
}

TEST(FourTest, ComparisonOperations) {
    Four a("123"); 
    Four b("21");  
    Four c("123"); 
    
    EXPECT_TRUE(a.greaterThan(b));
    EXPECT_TRUE(b.lessThan(a));
    EXPECT_TRUE(a.equals(c));
    EXPECT_FALSE(a.equals(b));
    
    Four d("00123");
    EXPECT_TRUE(a.equals(d));
}

TEST(FourTest, RemoveLeadingZeros) {
    Four withZeros({0, 0, 1, 2, 3});
    Four withoutZeros = withZeros.removeLeadingZeros();
    
    EXPECT_EQ(withoutZeros.toString(), "123");
    EXPECT_EQ(withoutZeros.getSize(), 3);
    
    Four allZeros({0, 0, 0});
    Four result = allZeros.removeLeadingZeros();
    EXPECT_EQ(result.toString(), "0");
    EXPECT_EQ(result.getSize(), 1);
}

TEST(FourTest, GetDigit) {
    Four four("12302");
    
    EXPECT_EQ(four.getDigit(0), 1);
    EXPECT_EQ(four.getDigit(2), 3);
    EXPECT_EQ(four.getDigit(4), 2);
    
    EXPECT_THROW(four.getDigit(5), std::out_of_range);
    EXPECT_THROW(four.getDigit(10), std::out_of_range);
}

TEST(FourTest, CopyMethod) {
    Four original("12302");
    Four copy = original.copy();
    
    EXPECT_EQ(original.toString(), copy.toString());
    EXPECT_NE(&original, &copy);
}

TEST(FourTest, ComplexOperations) {
    Four a("333"); 
    Four b("1");   
    Four c("1000");
    
    Four sum = a.add(b);
    EXPECT_TRUE(sum.equals(c));
    
    Four diff = c.subtract(b);
    EXPECT_TRUE(diff.equals(a));
}

TEST(FourTest, BoundaryDigits) {
    Four maxDigit("33333");
    Four minDigit("0");    
    Four mixed("1230");    
    
    EXPECT_EQ(maxDigit.toString(), "33333");
    EXPECT_EQ(minDigit.toString(), "0");
    EXPECT_EQ(mixed.toString(), "1230");
}

TEST(FourTest, LargeNumbers) {
    Four large1(100, 3);
    Four large2(100, 1);
    
    Four sum = large1.add(large2);
    EXPECT_EQ(sum.getSize(), 101);
    
    Four diff = large1.subtract(large2);
    EXPECT_EQ(diff.getSize(), 100);
}

TEST(FourTest, CarryOperations) {
    Four a("133");  
    Four b("1");    
    Four result = a.add(b);
    EXPECT_EQ(result.toString(), "200"); // 31 + 1 = 32₁₀ = 200₄
    
    Four c("100");
    Four d("1");  
    Four result2 = c.subtract(d);
    EXPECT_EQ(result2.toString(), "33"); // 16 - 1 = 15₁₀ = 33₄
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}