#include <gtest/gtest.h>
#include <limits.h>

//#include "stack.h"
//#include "calculator.h"

#define EXP_LEN 1024
#define MIN_VALUE -2000000000
#define MAX_VALUE 2000000000

extern "C" {
#include "../../src/calculator.h"
#include "../../src/stack.h"
}

// createStack test
TEST(StackTest, CreateStackTest)
{
    Stack* s = createStack();
    ASSERT_TRUE(s != NULL);
    ASSERT_TRUE(IsEmpty(s));
    freeStack(s);
}

// IsEmpty test
TEST(StackTest, IsEmptyTest)
{
    Stack* s = createStack();
    ASSERT_TRUE(IsEmpty(s));
    pushInt(s, 10);
    ASSERT_FALSE(IsEmpty(s));
    freeStack(s);
}

// pushInt popInt tests
TEST(StackTest, PushPopIntTest)
{
    Stack* s = createStack();
    pushInt(s, 10);
    pushInt(s, 20);
    ASSERT_EQ(popInt(s), 20);
    ASSERT_EQ(popInt(s), 10);
    ASSERT_TRUE(IsEmpty(s));
    freeStack(s);
}

// pushFloat popFloat tests
TEST(StackTest, PushPopFloatTest)
{
    Stack* s = createStack();
    pushFloat(s, 10.5);
    pushFloat(s, 20.5);
    ASSERT_DOUBLE_EQ(popFloat(s), 20.5);
    ASSERT_DOUBLE_EQ(popFloat(s), 10.5);
    ASSERT_TRUE(IsEmpty(s));
    freeStack(s);
}

// topOperator tests
TEST(StackTest, TopOperatorTest)
{
    Stack* s = createStack();
    pushInt(s, '+');
    EXPECT_EQ(topOperator(s), '+');
    pushInt(s, '*');
    EXPECT_EQ(topOperator(s), '*');
    freeStack(s);
}

TEST(RangeTest, IsInRangeTest)
{
    ASSERT_TRUE(isInRange(0));
    ASSERT_TRUE(isInRange(MAX_VALUE));
    ASSERT_TRUE(isInRange(MIN_VALUE));
    ASSERT_FALSE(isInRange(MAX_VALUE + 1));
    ASSERT_FALSE(isInRange(MIN_VALUE - 1));
}

TEST(RangeTest, IsInRangeFloatTest)
{
    ASSERT_TRUE(isInRangeFloat(0.0));
    ASSERT_TRUE(isInRangeFloat(MAX_VALUE));
    ASSERT_TRUE(isInRangeFloat(MIN_VALUE));
    ASSERT_FALSE(isInRangeFloat(MAX_VALUE + 1.0));
    ASSERT_FALSE(isInRangeFloat(MIN_VALUE - 1.0));
}

TEST(OperationTest, PrecedenceTest)
{
    ASSERT_EQ(precedence('+'), 1);
    ASSERT_EQ(precedence('-'), 1);
    ASSERT_EQ(precedence('*'), 2);
    ASSERT_EQ(precedence('/'), 2);
    ASSERT_EQ(precedence('('), 0);
}

TEST(EvalTest, EvaluateIntTest)
{
    ASSERT_EQ(evaluateInt(10, 5, '+'), 15);
    ASSERT_EQ(evaluateInt(10, 5, '-'), 5);
    ASSERT_EQ(evaluateInt(10, 5, '*'), 50);
    ASSERT_EQ(evaluateInt(10, 5, '/'), 2);
    // EXPECT_EXIT(evaluateInt(10, 0, '/'), ::testing::ExitedWithCode(-1), "Division by zero is forbidden");
}

TEST(EvalTest, EvaluateFloatTest)
{
    ASSERT_DOUBLE_EQ(evaluateFloat(10.5, 5.5, '+'), 16.0);
    ASSERT_DOUBLE_EQ(evaluateFloat(10.5, 5.5, '-'), 5.0);
    ASSERT_DOUBLE_EQ(evaluateFloat(10.5, 5.5, '*'), 57.75);
    ASSERT_DOUBLE_EQ(evaluateFloat(10.5, 5.5, '/'), 1.9090909090909092);
    // EXPECT_EXIT(evaluateFloat(10.5, 0.0, '/'), ::testing::ExitedWithCode(-1), "Division by a number less than 10^-4 is forbidden");
}

TEST(CountTest, CountIntTest)
{
    char expr1[] = "2+2";
    ASSERT_EQ(countInt(expr1), 4);
    char expr2[] = "(0 - 3) / 2";
    ASSERT_EQ(countInt(expr2), -1);
    char expr3[] = "81 + 63 / 8 * (52-37) - 188";
    ASSERT_EQ(countInt(expr3), -2);
    char expr4[] = "123 * 0";
    ASSERT_EQ(countInt(expr4), 0);
    char expr5[] = "(0 - 5) * ( 0 - 4)";
    ASSERT_EQ(countInt(expr5), 20);
}

TEST(CountTest, CountFloatTest)
{
    char expr1[] = "(54 - 12) * (0 - 1) / 41";
    ASSERT_DOUBLE_EQ(countFloat(expr1), -1.0244);
    char expr2[] = "(0 - 3) / 2";
    ASSERT_DOUBLE_EQ(countFloat(expr2), -1.5);
    char expr3[] = "81 + 63 / 8 * (52-37) - 188";
    ASSERT_DOUBLE_EQ(countFloat(expr3), 11.125);
    char expr4[] = "123.45 * 0";
    ASSERT_DOUBLE_EQ(countFloat(expr4), 0.0);
}
