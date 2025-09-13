#include "thread_pool/thread_pool.hpp"

#include <iostream>
#include <ranges>

namespace dispatcher::thread_pool {

ThreadPool::ThreadPool(std::shared_ptr<queue::PriorityQueue> queue, std::size_t threads_count)
    : do_work_(true), queue_(std::move(queue)) {
    threads_.reserve(threads_count);
    for (std::size_t idx : std::views::iota(std::size_t{}, threads_count)) {
        threads_.emplace_back(&ThreadPool::work, this);
    }
}

ThreadPool::~ThreadPool() {
    do_work_ = false;
    queue_->shutdown();
    for (auto &thread : threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ThreadPool::work() {
    while (do_work_) {
        while (auto task = queue_->pop()) {
            task.value()();
        }
        std::this_thread::yield();
    }
}

}  // namespace dispatcher::thread_pool
