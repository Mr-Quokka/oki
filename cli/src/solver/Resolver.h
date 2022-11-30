#pragma once

#include <unordered_map>

#include "../repository/Repository.h"
#include "../semver/Range.h"

namespace solver {
    /**
     * Contraint la version d'un paquet.
     */
    using Requirement = semver::Range;

    /**
     * Les dépendances directes, c'est-à-dire celles qui sont déclarées dans le paquet source.
     */
    using Summaries = std::unordered_map<std::string, Requirement>;

    /**
     * Une solution du graphe de dépendances déterminant les versions transitives à installer.
     */
    using Resolved = std::unordered_map<std::string, package::PackageVersion>;

    class Resolver {
    private:
        std::unordered_map<std::string, package::Package> registry;
        void fetchVersions(const solver::Summaries &summaries, repository::Repository &repository);

    public:
        Resolved resolve(const solver::Summaries &summaries, repository::Repository &repository);
    };

    Resolved resolve(const solver::Summaries &summaries, repository::Repository &repository);
}
