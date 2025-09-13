#pragma once

#include "queue/priority_queue.hpp"
#include "queue/queue.hpp"
#include "thread_pool/thread_pool.hpp"

namespace dispatcher {

class TaskDispatcher {
public:
    TaskDispatcher(std::size_t threads_count,
                   queue::PriorityQueue::Config config = {{TaskPriority::High, {true, 1000}},
                                                          {TaskPriority::Normal, {false, std::nullopt}}})
        : queue_(std::make_shared<queue::PriorityQueue>(std::move(config))), pool_(queue_, threads_count) {}

    TaskDispatcher(std::size_t threads_count, std::shared_ptr<queue::PriorityQueue> queue)
        : queue_(std::move(queue)), pool_(queue_, threads_count) {}

    void schedule(TaskPriority priority, queue::QueueTask task) { queue_->push(priority, task); }

    void shutdown() { queue_->shutdown(); }

private:
    std::shared_ptr<queue::PriorityQueue> queue_;
    thread_pool::ThreadPool pool_;
};

}  // namespace dispatcher
