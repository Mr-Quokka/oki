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

        /**
         * Récupère les noms des dépendances transitives d'un paquet.
         *
         * @param package Le nom d'un paquet.
         * @return Toutes les dépendances sur lesquelles le paquet dépend.
         */
        std::vector<std::string_view> getTransitiveDependencies(const std::string &package) const;

        /**
         * Trie selon un ordre topologique les dépendances.
         *
         * @return Les dépendances triées, de la plus superficielle à la plus profonde.
         */
        std::vector<std::string> topologicalSort() const;
    };
}
