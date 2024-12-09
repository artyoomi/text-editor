#include <gtest/gtest.h>

#include "Rope/Rope.h"

class RopeTest : public testing::Test {
protected:
    void SetUp() override
    {
        rope0 = new Rope();
        rope1 = new Rope("HELLO WORLD!");
    }

    void TearDown() override
    {
        delete rope0;
        delete rope1;
    }

    Rope *rope0;
    Rope *rope1;
};

TEST_F(RopeTest, CreateEmptyRope)
{
    EXPECT_EQ(rope0->to_string(), "");
    EXPECT_EQ(rope0->length(), 0);
}

TEST_F(RopeTest, CreateNonEmptyRope)
{
    EXPECT_EQ(rope1->to_string(), "HELLO WORLD!");
    EXPECT_EQ(rope1->length(), 12);
}

TEST_F(RopeTest, At)
{
    EXPECT_EQ(rope1->at(0), 'H');
    EXPECT_EQ(rope1->at(6), 'W');
    EXPECT_THROW(rope1->at(20), std::invalid_argument); // Проверяем выход за пределы
}

TEST_F(RopeTest, Substring)
{
    EXPECT_EQ(rope1->substr(0, 5), "HELLO");
    EXPECT_EQ(rope1->substr(6, 5), "WORLD");
    EXPECT_THROW(rope1->substr(12, 5), std::invalid_argument); // Проверяем выход за пределы
}

TEST_F(RopeTest, BalanceMethod)
{
    EXPECT_FALSE(rope1->is_balanced());
    rope1->balance();
    EXPECT_TRUE(rope1->is_balanced());
}

TEST_F(RopeTest, InsertString)
{
    rope1->insert(6, "BEAUTIFUL ");
    EXPECT_EQ(rope1->to_string(), "HELLO BEAUTIFUL WORLD!");
}

TEST_F(RopeTest, InsertRope)
{
    Rope rope2("INSERTED ");
    rope1->insert(6, rope2);
    EXPECT_EQ(rope1->to_string(), "HELLO INSERTED WORLD!");
}

TEST_F(RopeTest, AppendString)
{
    rope1->append(" APPENDED");
    EXPECT_EQ(rope1->to_string(), "HELLO WORLD! APPENDED");
}

TEST_F(RopeTest, AppendRope)
{
    Rope rope2(" END");
    rope1->append(rope2);
    EXPECT_EQ(rope1->to_string(), "HELLO WORLD! END");
}

TEST_F(RopeTest, DeleteSubstring)
{
    rope1->rdelete(6, 5);
    EXPECT_EQ(rope1->to_string(), "HELLO !");
}

TEST_F(RopeTest, EqualityOperators)
{
    Rope rope2("HELLO WORLD!");
    EXPECT_TRUE(*rope1 == rope2);
    rope2.append(" DIFFERENT");
    EXPECT_TRUE(*rope1 != rope2);
}

TEST_F(RopeTest, AssignmentOperator)
{
    Rope rope2;
    rope2 = *rope1;
    EXPECT_EQ(rope2.to_string(), "HELLO WORLD!");
    EXPECT_TRUE(*rope1 == rope2);
}

TEST_F(RopeTest, LengthMethod)
{
    EXPECT_EQ(rope1->length(), 12);
    rope1->append(" MORE TEXT");
    EXPECT_EQ(rope1->length(), 22);
}
