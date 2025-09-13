#pragma once

#include "queue.hpp"
#include "types.hpp"

#include <condition_variable>
#include <map>
#include <memory>
#include <optional>

namespace dispatcher::queue {

class PriorityQueue {
public:
    using Config = std::map<TaskPriority, QueueOptions>;

    explicit PriorityQueue(Config);

    void push(TaskPriority priority, QueueTask task);

    std::optional<QueueTask> pop();

    void shutdown();

private:
    bool empty_ = true;
    bool sleep_on_pop_ = true;

    std::mutex mutex_;
    std::condition_variable cv_;

    std::map<TaskPriority, std::unique_ptr<IQueue>> queues_;
};

}  // namespace dispatcher::queue
