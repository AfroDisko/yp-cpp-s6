#pragma once

#include "queue.hpp"

#include <mutex>
#include <queue>

namespace dispatcher::queue {

class UnboundedQueue : public IQueue {
public:
    explicit UnboundedQueue() = default;

    void push(QueueTask) override;

    std::optional<QueueTask> try_pop() override;

private:
    std::mutex mutex_;

    std::queue<QueueTask> queue_;
};

}  // namespace dispatcher::queue
