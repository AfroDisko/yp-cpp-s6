#include <gtest/gtest.h>

#include "queue/unbounded_queue.hpp"

#include <optional>
#include <thread>

using namespace dispatcher::queue;

TEST(UnboundedQueue, ctor) { ASSERT_NO_FATAL_FAILURE(UnboundedQueue{}); }

TEST(UnboundedQueue, push) {
    UnboundedQueue queue;

    auto push = [&queue](QueueTask task) { queue.push(std::move(task)); };

    auto task1 = []() {};
    auto task2 = []() {};

    std::jthread t1(push, task1);
    std::jthread t2(push, task2);

    ASSERT_NO_FATAL_FAILURE(t1.join());
    ASSERT_NO_FATAL_FAILURE(t2.join());
}

TEST(UnboundedQueue, try_pop) {
    UnboundedQueue queue;

    queue.push([]() {});
    queue.push([]() {});

    auto pop = [&queue]() {
        if (auto task = queue.try_pop()) {
            task.value()();
        }
    };

    std::jthread t1(pop);
    std::jthread t2(pop);

    ASSERT_NO_FATAL_FAILURE(t1.join());
    ASSERT_NO_FATAL_FAILURE(t2.join());

    ASSERT_TRUE(queue.try_pop() == std::nullopt);
}
