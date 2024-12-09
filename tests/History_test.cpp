#include <gtest/gtest.h>

#include "History.h"
#include "Rope/Rope.h"

class HistoryTest : public testing::Test {
protected:
    void SetUp() override {}

    void TearDown() override {}

    History history;
};

TEST_F(HistoryTest, InitialState)
{
    EXPECT_EQ(history.get_current_index(), -1);
    EXPECT_TRUE(history.get_list().empty());
}

TEST_F(HistoryTest, GetList)
{
    // EXPECT_EQ(history.get_list().to_vector(), std::vector());
}

TEST_F(HistoryTest, GetCurrentIndex)
{
    EXPECT_EQ(history.get_current_index(), -1);

    Rope rope("Initial text");
    op insert_op(op_type::INSERT, " added", 12, 18);
    history.add(insert_op, rope);

    EXPECT_EQ(history.get_current_index(), 0);
}

TEST_F(HistoryTest, AddOperation)
{
    Rope rope("Initial text");
    op operation(op_type::INSERT, " added", 12, 18);

    history.add(operation, rope);
    EXPECT_EQ(history.get_current_index(), 0);
    EXPECT_FALSE(history.get_list().empty());
    EXPECT_EQ(history.get_list().size(), 1);
}

TEST_F(HistoryTest, ExecuteOperation)
{
    Rope rope("Initial");
    op operation(op_type::INSERT, " text", 7, 12);

    history.execute(operation, rope);
    EXPECT_EQ(rope.to_string(), "Initial text");
}

TEST_F(HistoryTest, UndoOperation)
{
    Rope rope("Initial text");
    op insert_op(op_type::INSERT, " added", 12, 18);

    history.add(insert_op, rope);
    history.undo(rope);
    EXPECT_EQ(rope.to_string(), "Initial text"); // Операция добавления отменена
    EXPECT_EQ(history.get_current_index(), -1);
}

TEST_F(HistoryTest, RedoOperation)
{
    Rope rope("Initial text");
    op insert_op(op_type::INSERT, " added", 12, 18);

    history.add(insert_op, rope);
    history.undo(rope);
    history.redo(rope);
    EXPECT_EQ(rope.to_string(), "Initial text added"); // Операция добавления снова выполнена
    EXPECT_EQ(history.get_current_index(), 0);
}

TEST_F(HistoryTest, UndoRedoChain)
{
    Rope rope("Initial");
    op op1(op_type::INSERT, " text", 7, 12);
    op op2(op_type::DELETE, " text", 7, 12);

    history.add(op1, rope);
    history.add(op2, rope);

    history.undo(rope); // Удаление отменяется
    EXPECT_EQ(rope.to_string(), "Initial text");

    history.undo(rope); // Добавление отменяется
    EXPECT_EQ(rope.to_string(), "Initial");

    history.redo(rope); // Добавление снова выполняется
    EXPECT_EQ(rope.to_string(), "Initial text");

    history.redo(rope); // Удаление снова выполняется
    EXPECT_EQ(rope.to_string(), "Initial");
}

TEST_F(HistoryTest, HistoryLimit)
{
    Rope rope("Start");
    const std::size_t hist_size = History::hist_size;
    for (std::size_t i = 0; i < hist_size + 5; ++i) {
        history.add(op(op_type::INSERT, std::to_string(i), i, i + 1), rope);
    }
    EXPECT_EQ(history.get_list().size(), hist_size);
    EXPECT_EQ(history.get_current_index(), hist_size - 1);
}
