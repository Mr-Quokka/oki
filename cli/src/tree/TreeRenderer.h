#pragma once

#include "../config/ManifestLock.h"
#include <ostream>

namespace tree {
    /**
     * Rend un graphe de dépendances comme un arbre au format textuel.
     */
    class TreeRenderer {
    private:
        const config::ManifestLock &graph;
        std::vector<std::string> directDependencies;

    public:
        /**
         * Crée un nouveau renderer à partir de toutes les dépendances, en précisant celles qui sont directes.
         */
        TreeRenderer(const config::ManifestLock &graph, std::vector<std::string> directDependencies);

        /**
         * Écrit le résultat dans un flux.
         *
         * @param out Le flux.
         * @return Le même flux.
         */
        friend std::ostream &operator<<(std::ostream &out, const TreeRenderer &);
    };

    /**
     * Écrit le résultat dans un flux.
     *
     * @param out Le flux.
     * @return Le même flux.
     */
    std::ostream &operator<<(std::ostream &out, const TreeRenderer &renderer);
}
