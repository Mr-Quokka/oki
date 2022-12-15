#include "TreeRenderer.h"
#include <iostream>
#include <stack>
#include <unordered_set>
#include <utility>

#define DOWN "│"
#define TEE "├"
#define ELL "└"
#define RIGHT "─"

namespace tree {
    TreeRenderer::TreeRenderer(const config::ManifestLock &graph, std::vector<std::string> directDependencies)
        : graph{graph}, directDependencies{std::move(directDependencies)} {}

    std::ostream &operator<<(std::ostream &out, const TreeRenderer &renderer) {
        std::stack<std::pair<std::string, unsigned int>> dependencyStack;
        std::vector<unsigned int> levelsContinue{static_cast<unsigned int>(renderer.directDependencies.size())};
        for (const auto &name : renderer.directDependencies) {
            dependencyStack.emplace(name, 0);
        }

        while (!dependencyStack.empty()) {
            auto [name, level] = dependencyStack.top();
            dependencyStack.pop();

            if (levelsContinue.size() <= level + 1) {
                levelsContinue.push_back(0);
            }
            levelsContinue[level]--;

            for (unsigned int i = 0; i < level; ++i) {
                if (levelsContinue[i] == 0) {
                    out << "   ";
                } else {
                    out << DOWN << "  ";
                }
            }
            if (dependencyStack.empty() || dependencyStack.top().second != level) {
                out << ELL;
            } else {
                out << TEE;
            }
            out << RIGHT << " " << name << " ";
            auto version = renderer.graph.find(name);
            if (version == renderer.graph.getLocks().cend()) {
                out << "?";
            } else {
                out << "v" << version->second;
                for (const auto &dependency : version->second.getDependencies()) {
                    dependencyStack.emplace(dependency, level + 1);
                    levelsContinue[level + 1]++;
                }
            }
            out << "\n";
        }
        return out;
    }
}
