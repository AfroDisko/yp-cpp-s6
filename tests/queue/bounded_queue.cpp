#include <gtest/gtest.h>
#include <optional>

#include "queue/bounded_queue.hpp"

#include <ranges>

using namespace dispatcher::queue;

TEST(BoundedQueue, ctor) { ASSERT_NO_FATAL_FAILURE(BoundedQueue{10}); }

TEST(BoundedQueue, push_pop) {
    BoundedQueue queue(10);

    for (std::size_t idx : std::views::iota(0, 15)) {
        ASSERT_NO_FATAL_FAILURE(queue.push([]() {}));
    }
    for (std::size_t idx : std::views::iota(0, 10)) {
        ASSERT_TRUE(queue.try_pop().has_value());
    }
    for (std::size_t idx : std::views::iota(0, 5)) {
        ASSERT_FALSE(queue.try_pop().has_value());
    }
}
