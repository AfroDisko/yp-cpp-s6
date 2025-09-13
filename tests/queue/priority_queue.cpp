#include <gtest/gtest.h>

#include "queue/priority_queue.hpp"

#include <ranges>
#include <sstream>
#include <stdexcept>

using namespace dispatcher;
using namespace dispatcher::queue;

TEST(PriorityQueue, ctor) {
    PriorityQueue::Config config = {{TaskPriority::High, {true, 10}}, {TaskPriority::Normal, {true, 10}}};
    PriorityQueue queue(config);
}

TEST(PriorityQueue, push_pop) {
    PriorityQueue::Config config = {{TaskPriority::High, {true, 1}}, {TaskPriority::Normal, {true, 2}}};
    PriorityQueue queue(config);

    auto task_high = []() { std::println(std::cout, "{}", TaskPriority::High); };
    auto task_normal = []() { std::println(std::cout, "{}", TaskPriority::Normal); };

    for (std::size_t idx : std::views::iota(0, 5)) {
        queue.push(TaskPriority::High, task_high);
        queue.push(TaskPriority::Normal, task_normal);
    }

    ASSERT_NO_FATAL_FAILURE(queue.pop().value()());
    ASSERT_NO_FATAL_FAILURE(queue.pop().value()());
    ASSERT_NO_FATAL_FAILURE(queue.pop().value()());

    queue.shutdown();

    ASSERT_FALSE(queue.pop().has_value());
}

TEST(PriorityQueue, bad_queue) {
    PriorityQueue::Config config = {{TaskPriority::Normal, {true, 2}}};
    PriorityQueue queue(config);

    ASSERT_THROW(queue.push(TaskPriority::High, []() {}), std::runtime_error);
}
