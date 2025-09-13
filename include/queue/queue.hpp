#pragma once

#include <functional>
#include <optional>

namespace dispatcher::queue {

using QueueTask = std::function<void()>;

struct QueueOptions {
    bool bounded;
    std::optional<std::size_t> capacity;
};

class IQueue {
public:
    virtual ~IQueue() = default;
    virtual void push(QueueTask task) = 0;
    virtual std::optional<QueueTask> try_pop() = 0;
};

}  // namespace dispatcher::queue