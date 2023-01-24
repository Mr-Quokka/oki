#pragma once

#include <ostream>
#include <string>
#include <vector>

namespace make {
    /**
     * Représente les étapes de construction des dépendances.
     */
    class DependencyBuild {
    private:
        std::vector<std::string> staticLinks;
        std::vector<std::string> buildsStatic;

    public:
        void addBuildStep(std::string_view staticLink, std::string_view buildStatic);
        void asMakefile(std::ostream &out);
    };
}
