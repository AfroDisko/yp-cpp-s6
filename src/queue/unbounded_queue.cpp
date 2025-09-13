#include "queue/unbounded_queue.hpp"

namespace dispatcher::queue {

void UnboundedQueue::push(QueueTask task) {
    std::lock_guard<std::mutex> lock(mutex_);

    queue_.push(std::move(task));
}

std::optional<QueueTask> UnboundedQueue::try_pop() {
    std::lock_guard<std::mutex> lock(mutex_);

    if (queue_.empty()) {
        return std::nullopt;
    } else {
        auto task = std::move(queue_.front());
        queue_.pop();
        return task;
    }
}

}  // namespace dispatcher::queue
