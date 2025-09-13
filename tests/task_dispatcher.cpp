#include <chrono>
#include <gtest/gtest.h>

#include "task_dispatcher.hpp"

#include <memory>
#include <optional>
#include <ranges>
#include <thread>

using namespace dispatcher;
using namespace dispatcher::queue;

using namespace std::chrono_literals;

TEST(TaskDispatcher, dispatch) {
    auto queue = std::make_shared<PriorityQueue>(PriorityQueue::Config{{TaskPriority::Normal, {false, std::nullopt}}});

    TaskDispatcher dispatcher(std::thread::hardware_concurrency(), queue);

    auto work = [&queue]() {
        for (std::size_t idx : std::views::iota(0, 10)) {
            queue->push(TaskPriority::Normal, [idx]() { std::println(std::cout, "executed {}", idx); });
            std::println(std::cout, "pushed {}", idx);
        }
    };
    std::jthread tw(work);

    auto shutdown = [&queue]() {
        std::println(std::cout, "shutdown 1");
        std::this_thread::sleep_for(2s);
        queue->shutdown();
        std::println(std::cout, "shutdown 2");
    };
    std::jthread ts(shutdown);
}
