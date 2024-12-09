#include <gtest/gtest.h>

#include "List/List.h"
#include "List/LNode.h"

class ListTest : public testing::Test {
protected:
    void SetUp() override
    {
        list0 = new List();
    }

    void TearDown() override
    {
        delete list0;
    }

    List *list0;
};

// Тест на начальное состояние пустого списка
TEST_F(ListTest, CreateEmptyList)
{
    EXPECT_TRUE(list0->empty());
    EXPECT_EQ(list0->size(), 0);
}

// Тест на добавление элемента в конец списка
TEST_F(ListTest, PushBackElement)
{
    op operation(op_type::INSERT, "text", 0, 4);
    list0->push_back(LNode(operation));

    EXPECT_FALSE(list0->empty());
    EXPECT_EQ(list0->size(), 1);
    EXPECT_EQ(list0->back(), operation);
}

// Тест на добавление элемента в начало списка
TEST_F(ListTest, PushFrontElement)
{
    op operation(op_type::DELETE, "text", 5, 9);
    list0->push_front(LNode(operation));

    EXPECT_FALSE(list0->empty());
    EXPECT_EQ(list0->size(), 1);
    EXPECT_EQ(list0->front(), operation);
}

// Тест на доступ к элементу по индексу
TEST_F(ListTest, AccessElementAtIndex)
{
    op op1(op_type::INSERT, "text1", 0, 5);
    op op2(op_type::DELETE, "text2", 6, 11);
    list0->push_back(LNode(op1));
    list0->push_back(LNode(op2));

    EXPECT_EQ(list0->at(0), op1);
    EXPECT_EQ(list0->at(1), op2);
}

// Тест на удаление элемента с конца списка
TEST_F(ListTest, PopBackElement)
{
    op operation(op_type::INSERT, "text", 0, 4);
    list0->push_back(LNode(operation));
    list0->pop_back();

    EXPECT_TRUE(list0->empty());
    EXPECT_EQ(list0->size(), 0);
}

// Тест на удаление элемента с начала списка
TEST_F(ListTest, PopFrontElement)
{
    op operation(op_type::DELETE, "text", 5, 9);
    list0->push_front(LNode(operation));
    list0->pop_front();

    EXPECT_TRUE(list0->empty());
    EXPECT_EQ(list0->size(), 0);
}

// Тест на очистку списка
TEST_F(ListTest, ClearList)
{
    op op1(op_type::INSERT, "text1", 0, 5);
    op op2(op_type::DELETE, "text2", 6, 11);
    list0->push_back(LNode(op1));
    list0->push_back(LNode(op2));

    list0->clear();
    EXPECT_TRUE(list0->empty());
    EXPECT_EQ(list0->size(), 0);
}

// Тест на преобразование в вектор
TEST_F(ListTest, ConvertToVector)
{
    op op1(op_type::INSERT, "text1", 0, 5);
    op op2(op_type::DELETE, "text2", 6, 11);
    list0->push_back(LNode(op1));
    list0->push_back(LNode(op2));

    std::vector<op> vec = list0->to_vector();
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], op1);
    EXPECT_EQ(vec[1], op2);
}

// Тест на отсечение части списка
TEST_F(ListTest, CutBackFromIndex)
{
    op op1(op_type::INSERT, "text1", 0, 5);
    op op2(op_type::DELETE, "text2", 6, 11);
    op op3(op_type::INSERT, "text3", 12, 15);
    list0->push_back(LNode(op1));
    list0->push_back(LNode(op2));
    list0->push_back(LNode(op3));

    list0->cut_back(1);
    EXPECT_EQ(list0->size(), 1);
    EXPECT_EQ(list0->back(), op1);
}

// Тест на корректность работы методов front() и back()
TEST_F(ListTest, FrontAndBack)
{
    op op1(op_type::INSERT, "text1", 0, 5);
    op op2(op_type::DELETE, "text2", 6, 11);
    list0->push_back(LNode(op1));
    list0->push_back(LNode(op2));

    EXPECT_EQ(list0->front(), op1);
    EXPECT_EQ(list0->back(), op2);
}
