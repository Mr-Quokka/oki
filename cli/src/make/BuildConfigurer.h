#pragma once

#include "../solver/DependencyGraph.h"
#include <filesystem>
#include <ostream>
#include <string>

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
    };
}
