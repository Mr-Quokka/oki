#pragma once

#include "../solver/DependencyGraph.h"
#include "DependencyBuild.h"
#include <filesystem>
#include <ostream>
#include <string>
#include <vector>

namespace make {
    class BuildConfigurer {
    private:
        solver::DependencyGraph graph;
        std::filesystem::path shared;

        void linkLocalDependencies(const std::string &packageName);

    public:
        BuildConfigurer(solver::DependencyGraph graph, std::filesystem::path &shared);

        /**
         * Configure un paquet nouvellement installé.
         *
         * Cela consiste à créer un lien symbolique vers les dépendances locales du paquet.
         *
         * @param packageName Le nom du paquet à configurer.
         */
        void configure(const std::string &packageName);

        /**
         * Génère les informations de construction pour le paquet racine.
         *
         * @param[out] depBuild Les informations de construction.
         * @param[out] exports Les points d'entrée accessibles.
         * @param[out] staticLink Les dépendances statiques à lier.
         */
        void gatherBuildInfoForRoot(DependencyBuild &depBuild, std::vector<std::string> &exports, std::vector<std::string> &staticLink);
    };
}
