#pragma once

#include "../config/ManifestLock.h"
#include <ostream>

namespace tree {
    class TreeRenderer {
    private:
        const config::ManifestLock &graph;
        std::vector<std::string> directDependencies;

    public:
        TreeRenderer(const config::ManifestLock &graph, std::vector<std::string> directDependencies);

        friend std::ostream &operator<<(std::ostream &out, const TreeRenderer &);
    };

    std::ostream &operator<<(std::ostream &out, const TreeRenderer &renderer);
}
