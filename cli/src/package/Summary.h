#pragma once

#include <unordered_map>

#include "../semver/Range.h"

namespace package {
    /**
     * Contraint la version d'un paquet.
     */
    using Requirement = semver::Range;

    /**
     * Les dépendances directes, c'est-à-dire celles qui sont déclarées dans le paquet source.
     */
    using Summaries = std::unordered_map<std::string, Requirement>;

    class SummariesHolder {
    private:
        Summaries dependencies;

    public:
        SummariesHolder(Summaries &&dependencies);
        const package::Summaries &getDependencies() const;
    };
}
