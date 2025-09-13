#pragma once

#include <format>
#include <unordered_map>

namespace dispatcher {

enum class TaskPriority { High, Normal };

}  // namespace dispatcher

template <>
struct std::formatter<dispatcher::TaskPriority> : std::formatter<std::string> {
    auto format(dispatcher::TaskPriority priority, std::format_context &ctx) const {
        static const std::unordered_map<dispatcher::TaskPriority, std::string_view> strings = {
            {dispatcher::TaskPriority::High, "High"}, {dispatcher::TaskPriority::Normal, "Normal"}};

        return std::formatter<std::string>::format(std::format("{}", strings.at(priority)), ctx);
    }
};
