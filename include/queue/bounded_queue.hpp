#pragma once

#include "queue.hpp"

#include <mutex>
#include <queue>

namespace dispatcher::queue {

class BoundedQueue : public IQueue {
public:
    explicit BoundedQueue(std::size_t capacity) : capacity_(capacity) {}

    void push(QueueTask) override;

    std::optional<QueueTask> try_pop() override;

private:
    std::mutex mutex_;

    const std::size_t capacity_ = 0;
    std::queue<QueueTask> queue_;
};

}  // namespace dispatcher::queue
