#include <gtest/gtest.h>
#include <queue>

#include "thread_pool/thread_pool.hpp"

using namespace dispatcher;
using namespace dispatcher::queue;
using namespace dispatcher::thread_pool;

TEST(ThreadPool, ctor) {
    auto queue = std::make_shared<PriorityQueue>(PriorityQueue::Config{{TaskPriority::High, {true, 10}}});

    queue->shutdown();

    ASSERT_NO_FATAL_FAILURE(ThreadPool{queue});
}

TEST(ThreadPool, work) {
    auto queue = std::make_shared<PriorityQueue>(PriorityQueue::Config{{TaskPriority::High, {true, 10}}});

    auto work1 = []() { std::println(std::cout, "work1"); };
    auto work2 = []() { std::println(std::cout, "work2"); };

    queue->push(TaskPriority::High, work1);
    queue->push(TaskPriority::High, work2);

    queue->shutdown();

    ASSERT_NO_FATAL_FAILURE(ThreadPool(queue, 10));
}
