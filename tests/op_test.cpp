#include <gtest/gtest.h>

#include "op.h"

TEST(op_test, EqualOperator)
{
    op oper0;
    op oper1;
    op oper2(op_type::INSERT, "MIR", 10, 23);
    op oper3(op_type::INSERT, "MIR", 11, 23);

    EXPECT_TRUE (oper0 == oper1);
    EXPECT_FALSE(oper0 == oper2);
    EXPECT_FALSE(oper2 == oper3);
}

TEST(op_test, ToString)
{
    op oper0;
    op oper1(op_type::INSERT, "HELLO!", 1000, 1231);
    op oper2(op_type::DELETE, "I'M DELETE", 0, 1000);

    EXPECT_EQ(oper0.to_string(), "UNKNOWN  0 0");
    EXPECT_EQ(oper1.to_string(), "INSERT HELLO! 1000 1231");
    EXPECT_EQ(oper2.to_string(), "DELETE I'M DELETE 0 1000");
}