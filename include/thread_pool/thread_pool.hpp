#pragma once

#include "queue/priority_queue.hpp"

#include <thread>

namespace dispatcher::thread_pool {

class ThreadPool {
public:
    ThreadPool(std::shared_ptr<queue::PriorityQueue>, std::size_t threads_count = std::thread::hardware_concurrency());

    ~ThreadPool();

private:
    void work();

    bool do_work_ = true;
    std::vector<std::jthread> threads_;

    std::shared_ptr<queue::PriorityQueue> queue_;
};

}  // namespace dispatcher::thread_pool
