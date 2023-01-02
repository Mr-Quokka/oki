#include "DependencyGraph.h"
#include "../io/oki.h"
#include <stack>
#include <unordered_set>

namespace solver {
    DependencyGraph::DependencyGraph(const config::ManifestLock &lock)
        : lock{lock} {}

    std::vector<std::string_view> DependencyGraph::getTransitiveDependencies(const std::string &package) const {
        const auto &dependencies = lock.at(package).getDependencies();
        std::vector<std::string_view> transitive{dependencies.begin(), dependencies.end()};
        std::unordered_set<std::string_view> seen{dependencies.begin(), dependencies.end()};
        std::stack<std::string> stack;
        while (!stack.empty()) {
            auto current = std::move(stack.top());
            stack.pop();
            for (const std::string &dependency : lock.at(package).getDependencies()) {
                if (!seen.insert(dependency).second) {
                    transitive.emplace_back(dependency);
                    stack.push(dependency);
                }
            }
        }
        return transitive;
    }

    std::vector<std::string> DependencyGraph::topologicalSort() const {
        std::vector<std::string> sorted;
        std::unordered_set<std::string_view> seen;
        std::stack<std::string> stack;
        for (const auto &[package, _] : lock.getLocks()) {
            stack.push(package);
        }
        while (!stack.empty()) {
            auto current = std::move(stack.top());
            stack.pop();
            for (const std::string &dependency : lock.at(current).getDependencies()) {
                if (!seen.insert(dependency).second) {
                    stack.push(dependency);
                }
            }
            sorted.emplace_back(std::move(current));
        }
        return sorted;
    }
}
