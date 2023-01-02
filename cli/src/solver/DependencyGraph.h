#pragma once

#include "../config/ManifestLock.h"
#include <span>
#include <string>
#include <vector>

namespace solver {
    class DependencyGraph {
    private:
        const config::ManifestLock &lock;

    public:
        explicit DependencyGraph(const config::ManifestLock &lock);
        std::vector<std::string_view> getTransitiveDependencies(const std::string &package) const;
    };
}
