#include "DependencyGraph.h"
#include "../io/oki.h"
#include <stack>
#include <unordered_set>

namespace solver {
    DependencyGraph::DependencyGraph(const config::ManifestLock &lock)
        : lock{lock} {}

    std::vector<std::string_view> DependencyGraph::getTransitiveDependencies(const std::string &package) const {
        const auto &dependencies = lock.find(package)->second.getDependencies();
        std::vector<std::string_view> transitive{dependencies.begin(), dependencies.end()};
        std::unordered_set<std::string_view> seen{dependencies.begin(), dependencies.end()};
        std::stack<std::string> stack;
        while (!stack.empty()) {
            auto current = std::move(stack.top());
            stack.pop();
            for (const std::string &dependency : lock.find(package)->second.getDependencies()) {
                if (!seen.insert(dependency).second) {
                    transitive.emplace_back(dependency);
                    stack.push(dependency);
                }
            }
        }
        return transitive;
    }
}
