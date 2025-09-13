#include "queue/priority_queue.hpp"

#include "queue/bounded_queue.hpp"
#include "queue/unbounded_queue.hpp"

#include "gtest/gtest.h"
#include <cassert>
#include <iterator>
#include <mutex>
#include <optional>
#include <stdexcept>

namespace dispatcher::queue {

PriorityQueue::PriorityQueue(Config config) {
    auto visitor = [](const QueueOptions &options) -> std::unique_ptr<IQueue> {
        if (options.bounded) {
            return std::make_unique<BoundedQueue>(options.capacity.value_or(100));
        } else {
            return std::make_unique<UnboundedQueue>();
        }
    };

    for (const auto &[priority, options] : config) {
        queues_[priority] = visitor(options);
    }
}

void PriorityQueue::push(TaskPriority priority, QueueTask task) {
    auto it = queues_.find(priority);
    if (it == queues_.end()) {
        throw std::runtime_error(std::format("queue with priority '{}' is not initialized", priority));
    }
    it->second->push(std::move(task));
    {
        std::lock_guard<std::mutex> lock(mutex_);
        empty_ = false;
    }
    cv_.notify_one();
}

std::optional<QueueTask> PriorityQueue::pop() {
    for (const auto &queue : queues_ | std::views::values) {
        if (auto task = queue->try_pop()) {
            return task;
        }
    }

    std::unique_lock<std::mutex> lock(mutex_);
    empty_ = true;
    if (sleep_on_pop_) {
        cv_.wait(lock, [this]() { return !sleep_on_pop_ || !empty_; });
    }

    return std::nullopt;
}

void PriorityQueue::shutdown() {
    std::lock_guard<std::mutex> lock(mutex_);
    sleep_on_pop_ = false;
}

}  // namespace dispatcher::queue
